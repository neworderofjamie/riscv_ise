// Standard C++ includes
#include <functional>
#include <string_view>
#include <unordered_map>

// Common include
#include "common/isa.h"

namespace
{
std::string disassembleOP(uint32_t inst, std::string_view name)
{
    auto [funct7, rs2, rs1, funct3, rd] = decodeRType(inst);


}
const std::unordered_map<uint32_t, std::function<std::string(uint32_t)>> standardDisassembler{
    {static_cast<uint32_t>(StandardOpCode::AUIPC), [](uint32_t inst){ }}
};
}

std::string disassemble(uint32_t instruction)
{

}