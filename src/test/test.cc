#include "assembler/xbyak_riscv.hpp"

using namespace Xbyak_riscv;



int main()
{
    CodeGenerator c;
    
    // alpha = e^(-1/20)
    c.vlui(v0, 7792);
    
    // v = 0
    c.vlui(v1, 0);
    
    // v_thresh = 1
    c.vlui(v2, 8192);

    // v_reset = 0
    c.vlui(v3, 0);
    
    // i = vmem[0..32]
    c.vloadv(v4, zero, 0);
    
    // t = 6400
    c.addi(x1, zero, 25);
    c.slli(x1, x1, 8);
    
    // a = 64 (2 bytes * 32 lanes)
    c.addi(x2, zero, 64);
    
    Label loop;
    L(loop);
    
    // v *= alpha
    c.vmul(13, v1, v1, v0);
    
    // v += i
    c.vadd(v1, v1, v4);
    
    // spk = v > 1.0
    c.addi(x3, zero, 0);
    c.vslt(x3, v2, v1);
    
    // v = spk ? v_reset : v
    c.vsel(v1, x3, v3);
    
    //vmem[a...a+32] = v
    c.vstore(v1, x2);
    
    // a += 64 (2 bytes * 32 lanes)
    c.addi(x2, x2, 64);
    
    // While x2 (address) < x1 (count), goto loop
    c.blt(x2, x1, loop);
}
