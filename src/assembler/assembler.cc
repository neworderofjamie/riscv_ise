#include "assembler/assembler.h"

// Standard C++ includes
#include <tuple>

namespace
{
// split x to hi20bits and low12bits
// return false if x in 12-bit signed integer
std::optional<std::tuple<int, int>> split32bit(int x)
{
    if (inSBit(x, 12)) {
        return std::nullopt;
    }
    int H = (x >> 12) & mask(20);
    int L = x & mask(12);
    if (x & (1 << 11)) {
        H++;
        L = L | (mask(20) << 12);
    }

    return std::make_tuple(H, L);
}

inline uint32_t get20_10to1_11_19to12_z12(uint32_t v) { return ((v & (1<<20)) << 11)| ((v & (1023<<1)) << 20)| ((v & (1<<11)) << 9)| (v & (255<<12)); }
inline uint32_t get12_10to5_z13_4to1_11_z7(uint32_t v) { return ((v & (1<<12)) << 19)| ((v & (63<<5)) << 20)| ((v & (15<<1)) << 7)| ((v & (1<<11)) >> 4); }



}

//----------------------------------------------------------------------------
// Error
//----------------------------------------------------------------------------
Error::Error(AssemblerError err) : m_Err(err)
{
    std::cout << "Error:" << err._to_string() << std::endl;
}
//----------------------------------------------------------------------------
const char *Error::what() const noexcept
{
    return m_Err._to_string();
}

//----------------------------------------------------------------------------
// CodeGenerator
//----------------------------------------------------------------------------
std::vector<uint32_t> CodeGenerator::getCode() const
{
    // Copy code
    auto code = m_Code;

    // Loop through labels and jumps
    for (auto l : m_LabelJumps) {
        // Get address of the target label
        auto addressIter = m_LabelAddresses.find(l.first);
        if (addressIter == m_LabelAddresses.cend()) {
            throw Error(AssemblerError::LABEL_IS_NOT_FOUND);
        }

        // Ensure from is aligned and update jump instruction
        const uint32_t from = l.second.getFrom();
        assert((from & 3) == 0);
        code.at(from / 4) = l.second.encode(addressIter->second);
    }

    return code;
}
//----------------------------------------------------------------------------
void CodeGenerator::li(Reg rd, int imm)
{
    auto highLow = split32bit(imm);
    if (!highLow) {
        addi(rd, Reg::X0, imm);
    }
    else {
        lui(rd, std::get<0>(highLow.value()));
        addi(rd, rd, std::get<1>(highLow.value()));
    }
    
}

//----------------------------------------------------------------------------
// CodeGenerator::Jmp
//----------------------------------------------------------------------------
uint32_t CodeGenerator::Jmp::encode(uint32_t addr) const
{
        const int imm = addr - m_From;
        if (m_Type == Type::JAL) {
            if (!isValidImm(imm, 20)) {
                throw Error(AssemblerError::INVALID_IMM_OF_JAL);
            }
            return get20_10to1_11_19to12_z12(imm) | m_Encoded;
        }
        else {
            if (!isValidImm(imm, 12)) {
                throw Error(AssemblerError::INVALID_IMM_OF_JAL);
            }
            return get12_10to5_z13_4to1_11_z7(imm) | m_Encoded;
        }
}