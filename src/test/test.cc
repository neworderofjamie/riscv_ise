#include "assembler/xbyak_riscv.hpp"

using namespace Xbyak_riscv;



int main()
{
    CodeGenerator c;
    
    // alpha = e^(-1/20)
    c.vlui(VReg::V0, 7792);
    
    // v = 0
    c.vlui(VReg::V1, 0);
    
    // v_thresh = 1
    c.vlui(VReg::V2, 8192);

    // v_reset = 0
    c.vlui(VReg::V3, 0);
    
    // i = vmem[0..32]
    c.vloadv(VReg::V4, Reg::X0, 0);
    
    // t = 6400
    c.addi(Reg::X1, Reg::X0, 25);
    c.slli(Reg::X1, Reg::X1, 8);
    
    // a = 64 (2 bytes * 32 lanes)
    c.addi(Reg::X2, Reg::X0, 64);
    
    Label loop;
    c.L(loop);
    
    // v *= alpha
    c.vmul(13, VReg::V1, VReg::V1, VReg::V0);
    
    // v += i
    c.vadd(VReg::V1, VReg::V1, VReg::V4);
    
    // spk = v > 1.0
    c.addi(Reg::X3, Reg::X0, 0);
    c.vslt(Reg::X3, VReg::V2, VReg::V1);
    
    // v = spk ? v_reset : v
    c.vsel(VReg::V1, Reg::X3, VReg::V3);
    
    //vmem[a...a+32] = v
    c.vstore(VReg::V1, Reg::X2);
    
    // a += 64 (2 bytes * 32 lanes)
    c.addi(Reg::X2, Reg::X2, 64);
    
    // While x2 (address) < x1 (count), goto loop
    c.blt(Reg::X2, Reg::X1, loop);
}
