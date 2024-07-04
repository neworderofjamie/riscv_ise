#pragma once

#include <cassert>
#include <cstdint>

inline constexpr uint32_t mask(uint32_t n)
{
	assert (n <= 32);
	return n == 32 ? 0xffffffff : (1u << n) - 1;
}
// is x <= mask(n) ?
inline constexpr bool inBit(uint32_t x, uint32_t n)
{
	return x <= mask(n);
}

// is x a signed n-bit integer?
inline constexpr bool inSBit(int x, int n)
{
	return -(1 << (n-1)) <= x && x < (1 << (n-1));
}

// Opcode in the standard encoding quadrant (11)
enum class StandardOpCode : uint32_t
{
	LOAD		= 0b00000,
	STORE		= 0b01000,
	BRANCH		= 0b11000,
	JALR		= 0b11001,
	JAL			= 0b11011,
	OP_IMM		= 0b00100,
	OP			= 0b01100,
	LUI			= 0b01101,
	AUIPC		= 0b00101,
	OP_IMM_32	= 0b00110,
	OP			= 0b01110,
};

template<size_t n>
class Bit {
public:
	Bit(uint32_t v)
		: v(v)
	{
		XBYAK_RISCV_ASSERT(inBit(v, n));
	}
	Bit(const IReg& r)
		: v(r.getIdx())
	{
	}
	Bit(VM vm)
		: v(static_cast<uint32_t>(vm))
	{
	}
	Bit(CSR csr)
		: v(static_cast<uint32_t>(csr))
	{
	}
	Bit(RM rm)
		: v(static_cast<uint32_t>(rm))
	{
	}

	operator uint32_t() const{ return v; }
private:
	uint32_t v;
};

uint32_t encodeRType(