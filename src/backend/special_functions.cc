#include "backend/special_function.h"

// Standard C includes
#include <cmath>
#include <cstdint>

// GeNN includes
#include "type.h"

// Common includes
#include "common/utils.h"

// Assembler includes
#include "assembler/assembler.h"
#include "assembler/register_allocator.h"

// Backend includes
#include "backend/environment.h"

using namespace GeNN;

//----------------------------------------------------------------------------
// SpecialFunctions::Exp
//----------------------------------------------------------------------------
namespace SpecialFunctions
{
// Constants
constexpr size_t numBits = 15;
constexpr size_t tableBits = (numBits - 3) / 2;
constexpr size_t fracBits = tableBits + 3;

size_t getLUTCount()
{
    return ((1 << tableBits) + 1);
}

//----------------------------------------------------------------------------
// SpecialFunctions::Exp
//----------------------------------------------------------------------------
namespace Exp
{
// Constants
const double log2 = std::log(2.0);
const double expMax = 0.5 * log2;

void add(CodeGenerator &codeGenerator, ScalarRegisterAllocator &scalarRegisterAllocator, 
         VectorRegisterAllocator &vectorRegisterAllocator, EnvironmentExternal &env,
         EnvironmentLibrary::Library &library, uint32_t lutField)
{
    auto &c = codeGenerator;

    // Allocate registers for constants that can be shared 
    // between all exp invocations, regardless of fixed-point
    ALLOCATE_VECTOR(VFracMask);
    ALLOCATE_VECTOR(VLog2);
    ALLOCATE_VECTOR(VInvLog);
    ALLOCATE_VECTOR(VHalf);
    ALLOCATE_VECTOR(VLUTBaseAddress);
    
    // Load constants
    c.vlui(*VFracMask, (1 << fracBits) - 1);
    c.vlui(*VLog2, convertFixedPoint(log2, 15));
    c.vlui(*VInvLog, convertFixedPoint(1.0 / log2, 14));
    c.vlui(*VHalf, convertFixedPoint(0.5, 15));

    // Generate code to load address from field
    {
        ALLOCATE_SCALAR(STmp);
        c.lw(*STmp, Reg::X0, lutField);
        c.vfill(*VLUTBaseAddress, *STmp);
    }
    
    // Add to environment
    env.add(Type::Void, "_exp_frac_mask", VFracMask);        
    env.add(Type::Void, "_exp_log_2", VLog2);        
    env.add(Type::Void, "_exp_inv_2", VInvLog);        
    env.add(Type::Void, "_exp_half", VHalf);
    
    // Loop through possible number of integer bits for operand a
    for(int aInt = 0; aInt < 16; aInt++) {
        // Create saturating and non-saturating fixed-point types 
        // **NOTE** we only specify saturating as non-saturating types will be promoted
        // **YUCK** these should go in GeNN::Type
        const int aFrac = 15 - aInt;
        const auto aType = Type::ResolvedType::createFixedPointNumeric<int16_t>(
            "s" + std::to_string(aInt) + "_" + std::to_string(aFrac) + "_sat_t", 
            50 + aInt, true, aFrac, &ffi_type_sint16, "");

        // Loop through possible number of integer bits for result
        for(int rInt = 0; rInt < 16; rInt++) {
            // Create saturating and non-saturating fixed-point types
            // **NOTE** we only specify saturating as non-saturating types will be promoted
            // **YUCK** these should go in GeNN::Type
            const int rFrac = 15 - rInt;
            const auto rType = Type::ResolvedType::createFixedPointNumeric<int16_t>(
                "s" + std::to_string(rInt) + "_" + std::to_string(rFrac) + "_sat_t", 
                50 + rInt, true, rFrac, &ffi_type_sint16, "");

            // Exp functions should be called "exp" if operand and result type are  
            // the same, otherwise, it's suffixed with number of fractional bits
            const std::string name = (aInt == rInt) ? "exp" : ("exp_" + std::to_string(rFrac));

            library.emplace(
                name,
                std::make_pair(Type::ResolvedType::createFunction(rType, {aType}),
                               [VLUTBaseAddress, aFrac, rFrac](auto &env, auto &vectorRegisterAllocator, auto &scalarRegisterAllocator, auto, const auto &args)
                               {
                                   auto &c = env.getCodeGenerator();
                               
                                   ALLOCATE_VECTOR(VK);
                                   ALLOCATE_VECTOR(VR);
                                   ALLOCATE_VECTOR(VOutput);

                                   // Get registers from environment
                                   auto VFracMask = env.getVectorRegister("_exp_frac_mask");
                                   auto VLog2 = env.getVectorRegister("_exp_log_2");
                                   auto VInvLog = env.getVectorRegister("_exp_inv_2");
                                   auto VHalf = env.getVectorRegister("_exp_half");

                                   // START RANGE-REDUCTION
                                   // VK = floor((VX * VInvLog) + 0.5) [aType with one extra integer bit]
                                   // **NOTE** extra integer bit required because VInvLog is 1.44 ish so multiplying by it can cause overflow
                                   c.vmul(15, *VK, *std::get<VectorRegisterAllocator::RegisterPtr>(args[0]), *VInvLog);
                                   c.vsrai_rn(aFrac - 1, *VK, *VK);

                                   // VR = VX - (VK * VLog2) [aType]
                                   c.vmul_rn(15 - aFrac, *VR, *VK, *VLog2);
                                   c.vsub(*VR, *std::get<VectorRegisterAllocator::RegisterPtr>(args[0]), *VR);

                                    // VR = (VR - -0.5Ln2) / (0.5Ln2 - -0.5Ln2) [s0.15]
                                    // VR = (VR + 0.5Ln2) / Ln2
                                    // **NOTE** VR = (VR * (1/Ln2)) + 0.5 is more precise than (VR + 0.5Ln2) * (1 / Ln2)
                                    c.vmul_rn(aFrac - 1, *VR, *VR, *VInvLog);
                                    c.vadd(*VR, *VR, *VHalf);
  
                                   {
                                       ALLOCATE_VECTOR(VLUTAddress);
                                       ALLOCATE_VECTOR(VLUTLower);
                                       ALLOCATE_VECTOR(VLUTDiff);

                                       // START FAITHFUL INTERPOLATION
                                       // VLUTAddress = VX >> fracBits
                                       c.vsrai(fracBits, *VLUTAddress, *VR);

                                       // VLUTAddress *= 2 (to convert to bytes)
                                       // **THINK** could just subtract 1 from fracBits
                                       // Won't result in aligned address but that gets ignored on HW
                                       c.vslli(1, *VLUTAddress, *VLUTAddress);
                            
                                       // Add base address
                                       c.vadd(*VLUTAddress, *VLUTAddress, *VLUTBaseAddress);

                                       // Load lower LUT entry [s1.14]
                                       c.vloadl(*VLUTLower, *VLUTAddress, 0);
                        
                                       // Load higher LUT value [s1.14]
                                       c.vloadl(*VLUTDiff, *VLUTAddress, 2);

                                       // VOutput = VX & VFracMask
                                       c.vand(*VOutput, *VR, *VFracMask);

                                       // Calculate difference [s1.14]
                                       c.vsub(*VLUTDiff, *VLUTDiff, *VLUTLower);
                        
                                       // VOutput *= VLUTDiff [s1.14]
                                       c.vmul_rn(fracBits, *VOutput, *VOutput, *VLUTDiff);

                                       c.vadd(*VOutput, *VOutput, *VLUTLower);
                                   }

                                
                                   // **TODO** Single VS instruction which shifts left or right based on signs would save a lot of expense here
                                   {
                                       ALLOCATE_SCALAR(SShiftScaleLessK);
                                       ALLOCATE_VECTOR(VKLeft);
                                       ALLOCATE_VECTOR(VOutputLeft);
                                       ALLOCATE_VECTOR(VShiftScale);

                                       // Load shift-scale
                                       // **TODO** could be generated lazily by multi-pass assembler
                                       c.vlui(*VShiftScale, 14 - rFrac);

                                       // Determine which VK are less than 
                                       c.vtlt(*SShiftScaleLessK, *VShiftScale, *VK);

                                       // Shift left by (VK - ShiftScale) to 
                                       // convert from S1.14 to output form [rType]
                                       c.vsub(*VKLeft, *VK, *VShiftScale);
                                       c.vsll(*VOutputLeft, *VOutput, *VKLeft);

                                       // Shift right by (ShiftScale - VK) to 
                                       // convert from S1.14 to output form [rType]
                                       c.vsub(*VK, *VShiftScale, *VK);
                                       c.vsra(*VOutput, *VOutput, *VK);

                                       // Select between two results depending on whether shift scale is less than K
                                       c.vsel(*VOutput, *SShiftScaleLessK, *VOutputLeft);
                                   }
                            
                                   return std::make_pair(VOutput, true);
                               }));
        }
    }
}
}   // namespace Exp
}   // namespace SpecialFunctions