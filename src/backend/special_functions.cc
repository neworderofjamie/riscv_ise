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
    ALLOCATE_VECTOR(VMaxScale);
    ALLOCATE_VECTOR(VLUTBaseAddress);
    
    // Load constants
    c.vlui(*VFracMask, (1 << fracBits) - 1);
    c.vlui(*VLog2, convertFixedPoint(log2, 15));
    c.vlui(*VInvLog, convertFixedPoint(1.0 / log2, 14));
    c.vlui(*VMaxScale, convertFixedPoint(1.0 / (2.0 * expMax), 14));

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
    env.add(Type::Void, "_exp_max_scale", VMaxScale);
    
    // Loop through possible number of integer bits for operand a
    for(int numInt = 0; numInt < 16; numInt++) {
        // Fixed-point types 
        // **NOTE** we only specify saturating as non-saturating types will be promoted
        // **YUCK** this should go in GeNN::Type
        const int numFrac = 15 - numInt;
        const auto aType = Type::ResolvedType::createFixedPointNumeric<int16_t>(
            "s" + std::to_string(numInt) + "_" + std::to_string(numFrac) + "_sat_t", 
            50 + numInt, true, numFrac, &ffi_type_sint16, "");

        library.emplace(
            "exp",
            std::make_pair(Type::ResolvedType::createFunction(aType, {aType}),
                           [VLUTBaseAddress, numFrac](auto &env, auto &vectorRegisterAllocator, auto &, auto, const auto &args)
                           {
                               auto &c = env.getCodeGenerator();
                               
                               ALLOCATE_VECTOR(VLUTAddress);
                               ALLOCATE_VECTOR(VLUTLower);
                               ALLOCATE_VECTOR(VLUTDiff);
                               ALLOCATE_VECTOR(VK);
                               ALLOCATE_VECTOR(VR);
                               ALLOCATE_VECTOR(VShiftScale);
                               ALLOCATE_VECTOR(VExpMax);
                               ALLOCATE_VECTOR(VOutput);

                               // Load constants that 
                               // **TODO** could be generated lazily by multi-pass assembler
                               c.vlui(*VShiftScale, 14 - fracBits);
                               c.vlui(*VExpMax, convertFixedPoint(expMax, fracBits));
                                
                               // Get registers from environment
                               auto VFracMask = env.getVectorRegister("_exp_frac_mask");
                               auto VLog2 = env.getVectorRegister("_exp_log_2");
                               auto VInvLog = env.getVectorRegister("_exp_inv_2");
                               auto VMaxScale = env.getVectorRegister("_exp_max_scale");

                               // START RANGE-REDUCTION
                               // VK = floor((VX * VInvLog) + 0.5).
                               c.vmul(14, *VK, *std::get<VectorRegisterAllocator::RegisterPtr>(args[0]), *VInvLog);
                               c.vsrai_rn(numFrac, *VK, *VK);

                               // VR = VX - (VK * VLog2)
                               c.vmul_rn(15 - numFrac, *VR, *VK, *VLog2);
                               c.vsub(*VR, *std::get<VectorRegisterAllocator::RegisterPtr>(args[0]), *VR);

                               // VR = (VR - VExpMax) / (VExpMax - -VExpMax)
                               c.vadd(*VR, *VR, *VExpMax);
                               c.vmul_rn(numFrac - 1, *VR, *VR, *VMaxScale);

                               // START FAITHFUL INTERPOLATION
                               // VLUTAddress = VX >> fracBits
                               c.vsrai(fracBits, *VLUTAddress, *VR);

                               // VLUTAddress *= 2 (to convert to bytes)
                               // **THINK** could just subtract 1 from fracBits
                               // Won't result in aligned address but that gets ignored on HW
                               c.vslli(1, *VLUTAddress, *VLUTAddress);
                            
                               // Add bas address
                               c.vadd(*VLUTAddress, *VLUTAddress, *VLUTBaseAddress);

                               // Load lower LUT entry
                               c.vloadl(*VLUTLower, *VLUTAddress, 0);
                        
                               // Load higher LUT value
                               c.vloadl(*VLUTDiff, *VLUTAddress, 2);

                               // VOutput = VX & VFracMask
                               c.vand(*VOutput, *VR, *VFracMask);

                               // Calculate difference
                               c.vsub(*VLUTDiff, *VLUTDiff, *VLUTLower);
                        
                               // VOutput *= 
                               c.vmul_rn(fracBits, *VOutput, *VOutput, *VLUTDiff);

                               c.vadd(*VOutput, *VOutput, *VLUTLower);

                               // K = shiftScale - K to include shift to 
                               // convert from S1.14 to output forma
                               c.vsub(*VK, *VShiftScale, *VK);

                               // END FAITHFUL INTERPOLATION
                               c.vsra(*VOutput, *VOutput, *VK);
                                
                               return std::make_pair(VOutput, true);
                           }));
    }
}
}   // namespace Exp
}   // namespace SpecialFunctions