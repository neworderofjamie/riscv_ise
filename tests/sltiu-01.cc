// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V assembler includes
#include "assembler/xbyak_riscv.hpp"

// RISC-V ISE includes
#include "ise/riscv.h"

#define MASK_XLEN(val)  val&0xFFFFFFFF // shortens 64b values to 32b

#define BIT(addr, bit) (((addr)>>(bit))&1)
#define IMMSZ      12
#define IMMSGN            (IMMSZ -1)
#define IMMMSK     ( (1 << IMMSZ)-1)
#define SEXT_IMM(x)  ((x & IMMMSK) | (-BIT((x), IMMSGN)<< IMMSZ))

using namespace Xbyak_riscv;

namespace
{
class sltiu_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        // Generate word-aligned vector to initialise memory
        const std::vector<uint32_t> scalarWordInitData{
        3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,3735928559,};
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(generateCode().getCode(), scalarInitData);
        
        if(!riscV.run()) {
            throw std::runtime_error("Error running test code");
        }
        
        // Copy output data
        s_ScalarData = riscV.getScalarDataMemory().getData();
    }
    

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite() 
    {
        s_ScalarData.clear();
    }
    
    static uint32_t *getScalarWordData(){ return reinterpret_cast<uint32_t*>(s_ScalarData.data()); }

private:
    static CodeGenerator generateCode()
    {
        CodeGenerator c;
        
        c.li(Reg::X7, 0);
        // rs1 != rd, rs1==x23, rd==x28, imm_val == 0, rs1_val == 1024
        // opcode: sltiu ; op1:x23; dest:x28; op1val:0x400;  immval:0x0
        c.li(Reg::X23, MASK_XLEN(0x400));
        c.sltiu(Reg::X28, Reg::X23, SEXT_IMM(0x0));
        c.sw(Reg::X28, Reg::X7, 0);

        // rs1 == rd, rs1==x2, rd==x2, imm_val == (2**(12)-1), rs1_val == 2048, rs1_val != imm_val and rs1_val > 0 and imm_val > 0
        // opcode: sltiu ; op1:x2; dest:x2; op1val:0x800;  immval:0xfff
        c.li(Reg::X2, MASK_XLEN(0x800));
        c.sltiu(Reg::X2, Reg::X2, SEXT_IMM(0xfff));
        c.sw(Reg::X2, Reg::X7, 4);

        // rs1==x3, rd==x25, imm_val == 1, rs1_val == 4, rs1_val==4 and imm_val==1
        // opcode: sltiu ; op1:x3; dest:x25; op1val:0x4;  immval:0x1
        c.li(Reg::X3, MASK_XLEN(0x4));
        c.sltiu(Reg::X25, Reg::X3, SEXT_IMM(0x1));
        c.sw(Reg::X25, Reg::X7, 8);

        // rs1==x19, rd==x11, rs1_val == 0, rs1_val==0 and imm_val==6
        // opcode: sltiu ; op1:x19; dest:x11; op1val:0x0;  immval:0x6
        c.li(Reg::X19, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X19, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X7, 12);

        // rs1==x14, rd==x15, rs1_val == (2**(xlen)-1), 
        // opcode: sltiu ; op1:x14; dest:x15; op1val:0xffffffff;  immval:0x2c
        c.li(Reg::X14, MASK_XLEN(0xffffffff));
        c.sltiu(Reg::X15, Reg::X14, SEXT_IMM(0x2c));
        c.sw(Reg::X15, Reg::X7, 16);

        // rs1==x13, rd==x4, rs1_val == 1, rs1_val==1 and imm_val==0
        // opcode: sltiu ; op1:x13; dest:x4; op1val:0x1;  immval:0x0
        c.li(Reg::X13, MASK_XLEN(0x1));
        c.sltiu(Reg::X4, Reg::X13, SEXT_IMM(0x0));
        c.sw(Reg::X4, Reg::X7, 20);

        // rs1==x26, rd==x3, rs1_val == imm_val and rs1_val > 0 and imm_val > 0, 
        // opcode: sltiu ; op1:x26; dest:x3; op1val:0xd;  immval:0xd
        c.li(Reg::X26, MASK_XLEN(0xd));
        c.sltiu(Reg::X3, Reg::X26, SEXT_IMM(0xd));
        c.sw(Reg::X3, Reg::X7, 24);

        // rs1==x20, rd==x29, imm_val == 2, rs1_val == 2863311530, rs1_val==2863311530 and imm_val==2
        // opcode: sltiu ; op1:x20; dest:x29; op1val:0xaaaaaaaa;  immval:0x2
        c.li(Reg::X20, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X29, Reg::X20, SEXT_IMM(0x2));
        c.sw(Reg::X29, Reg::X7, 28);

        // rs1==x27, rd==x16, imm_val == 4, rs1_val == 2147483647
        // opcode: sltiu ; op1:x27; dest:x16; op1val:0x7fffffff;  immval:0x4
        c.li(Reg::X27, MASK_XLEN(0x7fffffff));
        c.sltiu(Reg::X16, Reg::X27, SEXT_IMM(0x4));
        c.sw(Reg::X16, Reg::X7, 32);

        // rs1==x17, rd==x20, imm_val == 8, rs1_val == 4278190079
        // opcode: sltiu ; op1:x17; dest:x20; op1val:0xfeffffff;  immval:0x8
        c.li(Reg::X17, MASK_XLEN(0xfeffffff));
        c.sltiu(Reg::X20, Reg::X17, SEXT_IMM(0x8));
        c.sw(Reg::X20, Reg::X7, 36);

        // rs1==x31, rd==x8, imm_val == 16, 
        // opcode: sltiu ; op1:x31; dest:x8; op1val:0x800;  immval:0x10
        c.li(Reg::X31, MASK_XLEN(0x800));
        c.sltiu(Reg::X8, Reg::X31, SEXT_IMM(0x10));
        c.sw(Reg::X8, Reg::X7, 40);

        // rs1==x24, rd==x23, imm_val == 32, 
        // opcode: sltiu ; op1:x24; dest:x23; op1val:0xc;  immval:0x20
        c.li(Reg::X24, MASK_XLEN(0xc));
        c.sltiu(Reg::X23, Reg::X24, SEXT_IMM(0x20));
        c.sw(Reg::X23, Reg::X7, 44);

        // rs1==x25, rd==x26, imm_val == 64, rs1_val == 1431655765, rs1_val==1431655765 and imm_val==64
        // opcode: sltiu ; op1:x25; dest:x26; op1val:0x55555555;  immval:0x40
        c.li(Reg::X25, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X26, Reg::X25, SEXT_IMM(0x40));
        c.sw(Reg::X26, Reg::X7, 48);

        // rs1==x22, rd==x6, imm_val == 128, rs1_val == 524288
        // opcode: sltiu ; op1:x22; dest:x6; op1val:0x80000;  immval:0x80
        c.li(Reg::X22, MASK_XLEN(0x80000));
        c.sltiu(Reg::X6, Reg::X22, SEXT_IMM(0x80));
        c.sw(Reg::X6, Reg::X7, 52);

        // rs1==x12, rd==x5, imm_val == 256, rs1_val == 4294967287
        // opcode: sltiu ; op1:x12; dest:x5; op1val:0xfffffff7;  immval:0x100
        c.li(Reg::X12, MASK_XLEN(0xfffffff7));
        c.sltiu(Reg::X5, Reg::X12, SEXT_IMM(0x100));
        c.sw(Reg::X5, Reg::X7, 56);

        // rs1==x9, rd==x1, imm_val == 512, rs1_val == 2147483648
        // opcode: sltiu ; op1:x9; dest:x1; op1val:0x80000000;  immval:0x200
        c.li(Reg::X9, MASK_XLEN(0x80000000));
        c.sltiu(Reg::X1, Reg::X9, SEXT_IMM(0x200));
        c.sw(Reg::X1, Reg::X7, 60);

        // rs1==x28, rd==x10, imm_val == 1024, rs1_val == 4294705151
        // opcode: sltiu ; op1:x28; dest:x10; op1val:0xfffbffff;  immval:0x400
        c.li(Reg::X28, MASK_XLEN(0xfffbffff));
        c.sltiu(Reg::X10, Reg::X28, SEXT_IMM(0x400));
        c.sw(Reg::X10, Reg::X7, 64);

        // rs1==x21, rd==x31, imm_val == 2048, 
        // opcode: sltiu ; op1:x21; dest:x31; op1val:0x0;  immval:0x800
        c.li(Reg::X21, MASK_XLEN(0x0));
        c.sltiu(Reg::X31, Reg::X21, SEXT_IMM(0x800));
        c.sw(Reg::X31, Reg::X7, 68);

        c.li(Reg::X2, 72);
        // rs1==x0, rd==x21, imm_val == 4094, rs1_val == 16
        // opcode: sltiu ; op1:x0; dest:x21; op1val:0x0;  immval:0xffe
        c.li(Reg::X0, MASK_XLEN(0x0));
        c.sltiu(Reg::X21, Reg::X0, SEXT_IMM(0xffe));
        c.sw(Reg::X21, Reg::X2, 0);

        // rs1==x11, rd==x14, imm_val == 4093, 
        // opcode: sltiu ; op1:x11; dest:x14; op1val:0x12;  immval:0xffd
        c.li(Reg::X11, MASK_XLEN(0x12));
        c.sltiu(Reg::X14, Reg::X11, SEXT_IMM(0xffd));
        c.sw(Reg::X14, Reg::X2, 4);

        // rs1==x30, rd==x13, imm_val == 4091, 
        // opcode: sltiu ; op1:x30; dest:x13; op1val:0x6;  immval:0xffb
        c.li(Reg::X30, MASK_XLEN(0x6));
        c.sltiu(Reg::X13, Reg::X30, SEXT_IMM(0xffb));
        c.sw(Reg::X13, Reg::X2, 8);

        // rs1==x29, rd==x17, imm_val == 4087, rs1_val == 1073741824
        // opcode: sltiu ; op1:x29; dest:x17; op1val:0x40000000;  immval:0xff7
        c.li(Reg::X29, MASK_XLEN(0x40000000));
        c.sltiu(Reg::X17, Reg::X29, SEXT_IMM(0xff7));
        c.sw(Reg::X17, Reg::X2, 12);

        // rs1==x8, rd==x24, imm_val == 4079, rs1_val == 4294967231
        // opcode: sltiu ; op1:x8; dest:x24; op1val:0xffffffbf;  immval:0xfef
        c.li(Reg::X8, MASK_XLEN(0xffffffbf));
        c.sltiu(Reg::X24, Reg::X8, SEXT_IMM(0xfef));
        c.sw(Reg::X24, Reg::X2, 16);

        // rs1==x1, rd==x27, imm_val == 4063, 
        // opcode: sltiu ; op1:x1; dest:x27; op1val:0x5;  immval:0xfdf
        c.li(Reg::X1, MASK_XLEN(0x5));
        c.sltiu(Reg::X27, Reg::X1, SEXT_IMM(0xfdf));
        c.sw(Reg::X27, Reg::X2, 20);

        // rs1==x7, rd==x30, imm_val == 4031, 
        // opcode: sltiu ; op1:x7; dest:x30; op1val:0x3;  immval:0xfbf
        c.li(Reg::X7, MASK_XLEN(0x3));
        c.sltiu(Reg::X30, Reg::X7, SEXT_IMM(0xfbf));
        c.sw(Reg::X30, Reg::X2, 24);

        // rs1==x5, rd==x22, imm_val == 3967, rs1_val == 262144
        // opcode: sltiu ; op1:x5; dest:x22; op1val:0x40000;  immval:0xf7f
        c.li(Reg::X5, MASK_XLEN(0x40000));
        c.sltiu(Reg::X22, Reg::X5, SEXT_IMM(0xf7f));
        c.sw(Reg::X22, Reg::X2, 28);

        // rs1==x15, rd==x0, imm_val == 3839, 
        // opcode: sltiu ; op1:x15; dest:x0; op1val:0xe;  immval:0xeff
        c.li(Reg::X15, MASK_XLEN(0xe));
        c.sltiu(Reg::X0, Reg::X15, SEXT_IMM(0xeff));
        c.sw(Reg::X0, Reg::X2, 32);

        // rs1==x6, rd==x7, imm_val == 3583, 
        // opcode: sltiu ; op1:x6; dest:x7; op1val:0x3;  immval:0xdff
        c.li(Reg::X6, MASK_XLEN(0x3));
        c.sltiu(Reg::X7, Reg::X6, SEXT_IMM(0xdff));
        c.sw(Reg::X7, Reg::X2, 36);

        // rs1==x18, rd==x12, imm_val == 3071, 
        // opcode: sltiu ; op1:x18; dest:x12; op1val:0x7fffffff;  immval:0xbff
        c.li(Reg::X18, MASK_XLEN(0x7fffffff));
        c.sltiu(Reg::X12, Reg::X18, SEXT_IMM(0xbff));
        c.sw(Reg::X12, Reg::X2, 40);

        // rs1==x16, rd==x18, imm_val == 2047, rs1_val == 16777216
        // opcode: sltiu ; op1:x16; dest:x18; op1val:0x1000000;  immval:0x7ff
        c.li(Reg::X16, MASK_XLEN(0x1000000));
        c.sltiu(Reg::X18, Reg::X16, SEXT_IMM(0x7ff));
        c.sw(Reg::X18, Reg::X2, 44);

        // rs1==x4, rd==x9, imm_val == 1365, rs1_val == 4227858431
        // opcode: sltiu ; op1:x4; dest:x9; op1val:0xfbffffff;  immval:0x555
        c.li(Reg::X4, MASK_XLEN(0xfbffffff));
        c.sltiu(Reg::X9, Reg::X4, SEXT_IMM(0x555));
        c.sw(Reg::X9, Reg::X2, 48);

        // rs1==x10, rd==x19, imm_val == 2730, rs1_val == 4294965247
        // opcode: sltiu ; op1:x10; dest:x19; op1val:0xfffff7ff;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0xfffff7ff));
        c.sltiu(Reg::X19, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X19, Reg::X2, 52);

        // rs1_val == 2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x20
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x20));
        c.sw(Reg::X11, Reg::X2, 56);

        // rs1_val == 8, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x8;  immval:0x80
        c.li(Reg::X10, MASK_XLEN(0x8));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x80));
        c.sw(Reg::X11, Reg::X2, 60);

        // rs1_val == 32, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x20;  immval:0x800
        c.li(Reg::X10, MASK_XLEN(0x20));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x800));
        c.sw(Reg::X11, Reg::X2, 64);

        // rs1_val == 64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x40;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x40));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 68);

        // rs1_val == 128, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x80;  immval:0xff7
        c.li(Reg::X10, MASK_XLEN(0x80));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xff7));
        c.sw(Reg::X11, Reg::X2, 72);

        // rs1_val == 256, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x100;  immval:0xfdf
        c.li(Reg::X10, MASK_XLEN(0x100));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xfdf));
        c.sw(Reg::X11, Reg::X2, 76);

        // rs1_val == 512, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x200;  immval:0xff7
        c.li(Reg::X10, MASK_XLEN(0x200));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xff7));
        c.sw(Reg::X11, Reg::X2, 80);

        // rs1_val == 4096, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1000;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x1000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 84);

        // rs1_val == 8192, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2000;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x2000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 88);

        // rs1_val == 16384, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4000;  immval:0x7
        c.li(Reg::X10, MASK_XLEN(0x4000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x7));
        c.sw(Reg::X11, Reg::X2, 92);

        // rs1_val == 32768, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x8000;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x8000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 96);

        // rs1_val == 65536, rs1_val==65536 and imm_val==5
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 100);

        // rs1_val == 131072, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x20000;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x20000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 104);

        // rs1_val == 1048576, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x100000;  immval:0x10
        c.li(Reg::X10, MASK_XLEN(0x100000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x10));
        c.sw(Reg::X11, Reg::X2, 108);

        // rs1_val == 2097152, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x200000;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x200000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 112);

        // rs1_val == 4194304, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x400000;  immval:0xfdf
        c.li(Reg::X10, MASK_XLEN(0x400000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xfdf));
        c.sw(Reg::X11, Reg::X2, 116);

        // rs1_val == 8388608, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x800000;  immval:0x800
        c.li(Reg::X10, MASK_XLEN(0x800000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x800));
        c.sw(Reg::X11, Reg::X2, 120);

        // rs1_val == 33554432, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2000000;  immval:0x8
        c.li(Reg::X10, MASK_XLEN(0x2000000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x8));
        c.sw(Reg::X11, Reg::X2, 124);

        // rs1_val == 67108864, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4000000;  immval:0x800
        c.li(Reg::X10, MASK_XLEN(0x4000000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x800));
        c.sw(Reg::X11, Reg::X2, 128);

        // rs1_val == 134217728, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x8000000;  immval:0xc
        c.li(Reg::X10, MASK_XLEN(0x8000000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xc));
        c.sw(Reg::X11, Reg::X2, 132);

        // rs1_val == 268435456, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000000;  immval:0xfbf
        c.li(Reg::X10, MASK_XLEN(0x10000000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xfbf));
        c.sw(Reg::X11, Reg::X2, 136);

        // rs1_val == 536870912, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x20000000;  immval:0x10
        c.li(Reg::X10, MASK_XLEN(0x20000000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x10));
        c.sw(Reg::X11, Reg::X2, 140);

        // rs1_val == 4294967294, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffffffe;  immval:0xfef
        c.li(Reg::X10, MASK_XLEN(0xfffffffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xfef));
        c.sw(Reg::X11, Reg::X2, 144);

        // rs1_val == 4294967293, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffffffd;  immval:0xfff
        c.li(Reg::X10, MASK_XLEN(0xfffffffd));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xfff));
        c.sw(Reg::X11, Reg::X2, 148);

        // rs1_val == 4294967291, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffffffb;  immval:0x20
        c.li(Reg::X10, MASK_XLEN(0xfffffffb));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x20));
        c.sw(Reg::X11, Reg::X2, 152);

        // rs1_val == 4294967279, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffffffef;  immval:0xfff
        c.li(Reg::X10, MASK_XLEN(0xffffffef));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xfff));
        c.sw(Reg::X11, Reg::X2, 156);

        // rs1_val == 4294967263, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffffffdf;  immval:0xeff
        c.li(Reg::X10, MASK_XLEN(0xffffffdf));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xeff));
        c.sw(Reg::X11, Reg::X2, 160);

        // rs1_val == 4294967167, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffffff7f;  immval:0x10
        c.li(Reg::X10, MASK_XLEN(0xffffff7f));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x10));
        c.sw(Reg::X11, Reg::X2, 164);

        // rs1_val == 4294967039, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffffeff;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xfffffeff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 168);

        // rs1_val == 4294966783, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffffdff;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xfffffdff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 172);

        // rs1_val == 4294966271, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffffbff;  immval:0xf
        c.li(Reg::X10, MASK_XLEN(0xfffffbff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xf));
        c.sw(Reg::X11, Reg::X2, 176);

        // rs1_val == 4294963199, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffffefff;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xffffefff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 180);

        // rs1_val == 4294959103, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffffdfff;  immval:0xd
        c.li(Reg::X10, MASK_XLEN(0xffffdfff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xd));
        c.sw(Reg::X11, Reg::X2, 184);

        // rs1_val == 4294950911, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffffbfff;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xffffbfff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 188);

        // rs1_val == 4294934527, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff7fff;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0xffff7fff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 192);

        // rs1_val == 4294901759, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffeffff;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0xfffeffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 196);

        // rs1_val == 4294836223, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffdffff;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xfffdffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 200);

        // rs1_val == 4294443007, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfff7ffff;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xfff7ffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 204);

        // rs1_val == 4293918719, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffefffff;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xffefffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 208);

        // rs1_val == 4292870143, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffdfffff;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xffdfffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 212);

        // rs1_val == 4290772991, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffbfffff;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xffbfffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 216);

        // rs1_val == 4286578687, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xff7fffff;  immval:0xffe
        c.li(Reg::X10, MASK_XLEN(0xff7fffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xffe));
        c.sw(Reg::X11, Reg::X2, 220);

        // rs1_val == 4261412863, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfdffffff;  immval:0xd
        c.li(Reg::X10, MASK_XLEN(0xfdffffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xd));
        c.sw(Reg::X11, Reg::X2, 224);

        // rs1_val == 4160749567, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xf7ffffff;  immval:0x800
        c.li(Reg::X10, MASK_XLEN(0xf7ffffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x800));
        c.sw(Reg::X11, Reg::X2, 228);

        // rs1_val == 4026531839, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xefffffff;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xefffffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 232);

        // rs1_val == 3758096383, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xdfffffff;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0xdfffffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 236);

        // rs1_val == 3221225471, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xbfffffff;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xbfffffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 240);

        // rs1_val==3 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 244);

        // rs1_val==3 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 248);

        // rs1_val==3 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 252);

        // rs1_val==3 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 256);

        // rs1_val==3 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 260);

        // rs1_val==3 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 264);

        // rs1_val==3 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 268);

        // rs1_val==3 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 272);

        // rs1_val==3 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 276);

        // rs1_val==3 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 280);

        // rs1_val==3 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 284);

        // rs1_val==3 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 288);

        // rs1_val==3 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 292);

        // rs1_val==3 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 296);

        // rs1_val==3 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 300);

        // rs1_val==3 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 304);

        // rs1_val==3 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 308);

        // rs1_val==3 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 312);

        // rs1_val==3 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 316);

        // rs1_val==3 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 320);

        // rs1_val==3 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 324);

        // rs1_val==3 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 328);

        // rs1_val==3 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 332);

        // rs1_val==3 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 336);

        // rs1_val==3 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x3;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x3));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 340);

        // rs1_val==1431655765 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 344);

        // rs1_val==1431655765 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 348);

        // rs1_val==1431655765 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 352);

        // rs1_val==1431655765 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 356);

        // rs1_val==1431655765 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 360);

        // rs1_val==1431655765 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 364);

        // rs1_val==1431655765 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 368);

        // rs1_val==1431655765 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 372);

        // rs1_val==1431655765 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 376);

        // rs1_val==1431655765 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 380);

        // rs1_val==1431655765 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 384);

        // rs1_val==1431655765 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 388);

        // rs1_val==1431655765 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 392);

        // rs1_val==1431655765 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 396);

        // rs1_val==1431655765 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 400);

        // rs1_val==1431655765 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 404);

        // rs1_val==1431655765 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 408);

        // rs1_val==1431655765 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 412);

        // rs1_val==1431655765 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 416);

        // rs1_val==1431655765 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 420);

        // rs1_val==1431655765 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 424);

        // rs1_val==1431655765 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 428);

        // rs1_val==1431655765 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 432);

        // rs1_val==1431655765 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555555;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x55555555));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 436);

        // rs1_val==2863311530 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 440);

        // rs1_val==2863311530 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 444);

        // rs1_val==2863311530 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 448);

        // rs1_val==2863311530 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 452);

        // rs1_val==2863311530 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 456);

        // rs1_val==2863311530 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 460);

        // rs1_val==2863311530 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 464);

        // rs1_val==2863311530 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 468);

        // rs1_val==2863311530 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 472);

        // rs1_val==2863311530 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 476);

        // rs1_val==2863311530 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 480);

        // rs1_val==2863311530 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 484);

        // rs1_val==2863311530 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 488);

        // rs1_val==2863311530 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 492);

        // rs1_val==2863311530 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 496);

        // rs1_val==2863311530 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 500);

        // rs1_val==2863311530 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 504);

        // rs1_val==2863311530 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 508);

        // rs1_val==2863311530 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 512);

        // rs1_val==2863311530 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 516);

        // rs1_val==2863311530 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 520);

        // rs1_val==2863311530 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 524);

        // rs1_val==2863311530 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 528);

        // rs1_val==2863311530 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaaa;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaaa));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 532);

        // rs1_val==5 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 536);

        // rs1_val==5 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 540);

        // rs1_val==5 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 544);

        // rs1_val==5 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 548);

        // rs1_val==5 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 552);

        // rs1_val==5 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 556);

        // rs1_val==5 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 560);

        // rs1_val==5 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 564);

        // rs1_val==5 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 568);

        // rs1_val==5 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 572);

        // rs1_val==5 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 576);

        // rs1_val==5 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 580);

        // rs1_val==5 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 584);

        // rs1_val==5 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 588);

        // rs1_val==5 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 592);

        // rs1_val==5 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 596);

        // rs1_val==5 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 600);

        // rs1_val==5 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 604);

        // rs1_val==5 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 608);

        // rs1_val==5 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 612);

        // rs1_val==5 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 616);

        // rs1_val==5 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 620);

        // rs1_val==5 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 624);

        // rs1_val==5 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 628);

        // rs1_val==5 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x5;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x5));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 632);

        // rs1_val==858993459 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 636);

        // rs1_val==858993459 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 640);

        // rs1_val==858993459 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 644);

        // rs1_val==858993459 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 648);

        // rs1_val==858993459 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 652);

        // rs1_val==858993459 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 656);

        // rs1_val==858993459 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 660);

        // rs1_val==858993459 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 664);

        // rs1_val==858993459 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 668);

        // rs1_val==858993459 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 672);

        // rs1_val==858993459 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 676);

        // rs1_val==858993459 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 680);

        // rs1_val==858993459 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 684);

        // rs1_val==858993459 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 688);

        // rs1_val==858993459 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 692);

        // rs1_val==858993459 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 696);

        // rs1_val==858993459 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 700);

        // rs1_val==858993459 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 704);

        // rs1_val==858993459 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 708);

        // rs1_val==858993459 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 712);

        // rs1_val==858993459 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 716);

        // rs1_val==858993459 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 720);

        // rs1_val==858993459 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 724);

        // rs1_val==858993459 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 728);

        // rs1_val==858993459 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333333;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x33333333));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 732);

        // rs1_val==1717986918 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 736);

        // rs1_val==1717986918 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 740);

        // rs1_val==1717986918 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 744);

        // rs1_val==1717986918 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 748);

        // rs1_val==1717986918 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 752);

        // rs1_val==1717986918 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 756);

        // rs1_val==1717986918 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 760);

        // rs1_val==1717986918 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 764);

        // rs1_val==1717986918 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 768);

        // rs1_val==1717986918 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 772);

        // rs1_val==1717986918 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 776);

        // rs1_val==1717986918 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 780);

        // rs1_val==1717986918 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 784);

        // rs1_val==1717986918 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 788);

        // rs1_val==1717986918 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 792);

        // rs1_val==1717986918 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 796);

        // rs1_val==1717986918 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 800);

        // rs1_val==1717986918 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 804);

        // rs1_val==1717986918 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 808);

        // rs1_val==1717986918 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 812);

        // rs1_val==1717986918 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 816);

        // rs1_val==1717986918 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 820);

        // rs1_val==1717986918 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 824);

        // rs1_val==1717986918 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 828);

        // rs1_val==1717986918 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666666;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x66666666));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 832);

        // rs1_val==46340 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 836);

        // rs1_val==46340 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 840);

        // rs1_val==46340 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 844);

        // rs1_val==46340 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 848);

        // rs1_val==46340 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 852);

        // rs1_val==46340 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 856);

        // rs1_val==46340 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 860);

        // rs1_val==46340 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 864);

        // rs1_val==46340 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 868);

        // rs1_val==46340 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 872);

        // rs1_val==46340 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 876);

        // rs1_val==46340 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 880);

        // rs1_val==46340 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 884);

        // rs1_val==46340 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 888);

        // rs1_val==46340 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 892);

        // rs1_val==46340 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 896);

        // rs1_val==46340 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 900);

        // rs1_val==46340 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 904);

        // rs1_val==46340 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 908);

        // rs1_val==46340 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 912);

        // rs1_val==46340 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 916);

        // rs1_val==46340 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 920);

        // rs1_val==46340 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 924);

        // rs1_val==46340 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 928);

        // rs1_val==46340 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb504;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0xb504));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 932);

        // rs1_val==0 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 936);

        // rs1_val==0 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 940);

        // rs1_val==0 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 944);

        // rs1_val==0 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 948);

        // rs1_val==0 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 952);

        // rs1_val==0 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 956);

        // rs1_val==0 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 960);

        // rs1_val==0 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 964);

        // rs1_val==0 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 968);

        // rs1_val==0 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 972);

        // rs1_val==0 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 976);

        // rs1_val==0 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 980);

        // rs1_val==0 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 984);

        // rs1_val==0 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 988);

        // rs1_val==0 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 992);

        // rs1_val==0 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 996);

        // rs1_val==0 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1000);

        // rs1_val==0 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1004);

        // rs1_val==0 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 1008);

        // rs1_val==0 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1012);

        // rs1_val==0 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1016);

        // rs1_val==0 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1020);

        // rs1_val==0 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 1024);

        // rs1_val==0 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x0;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x0));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 1028);

        // rs1_val==65535 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1032);

        // rs1_val==65535 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1036);

        // rs1_val==65535 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 1040);

        // rs1_val==65535 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1044);

        // rs1_val==65535 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1048);

        // rs1_val==65535 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1052);

        // rs1_val==65535 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1056);

        // rs1_val==65535 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1060);

        // rs1_val==65535 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 1064);

        // rs1_val==65535 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1068);

        // rs1_val==65535 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1072);

        // rs1_val==65535 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 1076);

        // rs1_val==65535 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1080);

        // rs1_val==65535 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1084);

        // rs1_val==65535 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1088);

        // rs1_val==65535 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1092);

        // rs1_val==65535 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1096);

        // rs1_val==65535 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1100);

        // rs1_val==65535 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 1104);

        // rs1_val==65535 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1108);

        // rs1_val==65535 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1112);

        // rs1_val==65535 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1116);

        // rs1_val==65535 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1120);

        // rs1_val==65535 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 1124);

        // rs1_val==65535 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xffff;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0xffff));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 1128);

        // rs1_val==2 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1132);

        // rs1_val==2 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1136);

        // rs1_val==2 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 1140);

        // rs1_val==2 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1144);

        // rs1_val==2 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1148);

        // rs1_val==2 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1152);

        // rs1_val==2 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1156);

        // rs1_val==2 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1160);

        // rs1_val==2 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 1164);

        // rs1_val==2 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1168);

        // rs1_val==2 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1172);

        // rs1_val==2 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 1176);

        // rs1_val==2 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1180);

        // rs1_val==2 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1184);

        // rs1_val==2 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1188);

        // rs1_val==2 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1192);

        // rs1_val==2 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1196);

        // rs1_val==2 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1200);

        // rs1_val==2 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 1204);

        // rs1_val==2 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1208);

        // rs1_val==2 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1212);

        // rs1_val==2 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1216);

        // rs1_val==2 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1220);

        // rs1_val==2 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 1224);

        // rs1_val==2 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x2;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x2));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 1228);

        // rs1_val==1431655764 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1232);

        // rs1_val==1431655764 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1236);

        // rs1_val==1431655764 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 1240);

        // rs1_val==1431655764 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1244);

        // rs1_val==1431655764 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1248);

        // rs1_val==1431655764 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1252);

        // rs1_val==1431655764 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1256);

        // rs1_val==1431655764 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1260);

        // rs1_val==1431655764 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 1264);

        // rs1_val==1431655764 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1268);

        // rs1_val==1431655764 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1272);

        // rs1_val==1431655764 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 1276);

        // rs1_val==1431655764 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1280);

        // rs1_val==1431655764 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1284);

        // rs1_val==1431655764 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1288);

        // rs1_val==1431655764 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1292);

        // rs1_val==1431655764 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1296);

        // rs1_val==1431655764 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1300);

        // rs1_val==1431655764 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 1304);

        // rs1_val==1431655764 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1308);

        // rs1_val==1431655764 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1312);

        // rs1_val==1431655764 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1316);

        // rs1_val==1431655764 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1320);

        // rs1_val==1431655764 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 1324);

        // rs1_val==1431655764 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555554;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x55555554));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 1328);

        // rs1_val==2863311529 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1332);

        // rs1_val==2863311529 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1336);

        // rs1_val==2863311529 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 1340);

        // rs1_val==2863311529 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1344);

        // rs1_val==2863311529 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1348);

        // rs1_val==2863311529 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1352);

        // rs1_val==2863311529 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1356);

        // rs1_val==2863311529 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1360);

        // rs1_val==2863311529 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 1364);

        // rs1_val==2863311529 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1368);

        // rs1_val==2863311529 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1372);

        // rs1_val==2863311529 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 1376);

        // rs1_val==2863311529 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1380);

        // rs1_val==2863311529 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1384);

        // rs1_val==2863311529 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1388);

        // rs1_val==2863311529 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1392);

        // rs1_val==2863311529 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1396);

        // rs1_val==2863311529 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1400);

        // rs1_val==2863311529 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 1404);

        // rs1_val==2863311529 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1408);

        // rs1_val==2863311529 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1412);

        // rs1_val==2863311529 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1416);

        // rs1_val==2863311529 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1420);

        // rs1_val==2863311529 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 1424);

        // rs1_val==2863311529 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaa9;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaa9));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 1428);

        // rs1_val==4 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1432);

        // rs1_val==4 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1436);

        // rs1_val==4 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 1440);

        // rs1_val==4 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1444);

        // rs1_val==4 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1448);

        // rs1_val==4 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1452);

        // rs1_val==4 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1456);

        // rs1_val==4 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1460);

        // rs1_val==4 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 1464);

        // rs1_val==4 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1468);

        // rs1_val==4 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1472);

        // rs1_val==4 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 1476);

        // rs1_val==4 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1480);

        // rs1_val==4 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1484);

        // rs1_val==4 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1488);

        // rs1_val==4 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1492);

        // rs1_val==4 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1496);

        // rs1_val==4 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1500);

        // rs1_val==4 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 1504);

        // rs1_val==4 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1508);

        // rs1_val==4 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1512);

        // rs1_val==4 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1516);

        // rs1_val==4 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1520);

        // rs1_val==4 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x4;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x4));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 1524);

        // rs1_val==858993458 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1528);

        // rs1_val==858993458 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1532);

        // rs1_val==858993458 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 1536);

        // rs1_val==858993458 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1540);

        // rs1_val==858993458 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1544);

        // rs1_val==858993458 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1548);

        // rs1_val==858993458 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1552);

        // rs1_val==858993458 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1556);

        // rs1_val==858993458 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 1560);

        // rs1_val==858993458 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1564);

        // rs1_val==858993458 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1568);

        // rs1_val==858993458 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 1572);

        // rs1_val==858993458 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1576);

        // rs1_val==858993458 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1580);

        // rs1_val==858993458 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1584);

        // rs1_val==858993458 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1588);

        // rs1_val==858993458 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1592);

        // rs1_val==858993458 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1596);

        // rs1_val==858993458 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 1600);

        // rs1_val==858993458 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1604);

        // rs1_val==858993458 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1608);

        // rs1_val==858993458 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1612);

        // rs1_val==858993458 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1616);

        // rs1_val==858993458 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 1620);

        // rs1_val==858993458 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333332;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x33333332));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 1624);

        // rs1_val==1717986917 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1628);

        // rs1_val==1717986917 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1632);

        // rs1_val==1717986917 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 1636);

        // rs1_val==1717986917 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1640);

        // rs1_val==1717986917 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1644);

        // rs1_val==1717986917 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1648);

        // rs1_val==1717986917 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1652);

        // rs1_val==1717986917 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1656);

        // rs1_val==1717986917 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 1660);

        // rs1_val==1717986917 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1664);

        // rs1_val==1717986917 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1668);

        // rs1_val==1717986917 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 1672);

        // rs1_val==1717986917 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1676);

        // rs1_val==1717986917 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1680);

        // rs1_val==1717986917 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1684);

        // rs1_val==1717986917 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1688);

        // rs1_val==1717986917 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1692);

        // rs1_val==1717986917 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1696);

        // rs1_val==65534 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1700);

        // rs1_val==65534 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1704);

        // rs1_val==65534 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1708);

        // rs1_val==65534 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1712);

        // rs1_val==65534 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 1716);

        // rs1_val==65534 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1720);

        // rs1_val==65534 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1724);

        // rs1_val==65534 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1728);

        // rs1_val==65534 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1732);

        // rs1_val==65534 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 1736);

        // rs1_val==65534 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 1740);

        // rs1_val==1431655766 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1744);

        // rs1_val==1431655766 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1748);

        // rs1_val==1431655766 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 1752);

        // rs1_val==1431655766 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1756);

        // rs1_val==1431655766 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1760);

        // rs1_val==1431655766 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1764);

        // rs1_val==1431655766 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1768);

        // rs1_val==1431655766 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1772);

        // rs1_val==1431655766 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 1776);

        // rs1_val==1431655766 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1780);

        // rs1_val==1431655766 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1784);

        // rs1_val==1431655766 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 1788);

        // rs1_val==1431655766 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1792);

        // rs1_val==1431655766 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1796);

        // rs1_val==1431655766 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1800);

        // rs1_val==1431655766 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1804);

        // rs1_val==1431655766 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1808);

        // rs1_val==1431655766 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1812);

        // rs1_val==1431655766 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 1816);

        // rs1_val==1431655766 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1820);

        // rs1_val==1431655766 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1824);

        // rs1_val==1431655766 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1828);

        // rs1_val==1431655766 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1832);

        // rs1_val==1431655766 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 1836);

        // rs1_val==1431655766 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x55555556;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x55555556));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 1840);

        // rs1_val==2863311531 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1844);

        // rs1_val==2863311531 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1848);

        // rs1_val==2863311531 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 1852);

        // rs1_val==2863311531 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1856);

        // rs1_val==2863311531 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1860);

        // rs1_val==2863311531 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1864);

        // rs1_val==2863311531 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1868);

        // rs1_val==2863311531 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1872);

        // rs1_val==2863311531 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 1876);

        // rs1_val==2863311531 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1880);

        // rs1_val==2863311531 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1884);

        // rs1_val==2863311531 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 1888);

        // rs1_val==2863311531 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1892);

        // rs1_val==2863311531 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1896);

        // rs1_val==2863311531 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 1900);

        // rs1_val==2863311531 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 1904);

        // rs1_val==2863311531 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 1908);

        // rs1_val==2863311531 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 1912);

        // rs1_val==2863311531 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 1916);

        // rs1_val==2863311531 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 1920);

        // rs1_val==2863311531 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 1924);

        // rs1_val==2863311531 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 1928);

        // rs1_val==2863311531 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 1932);

        // rs1_val==2863311531 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 1936);

        // rs1_val==2863311531 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xaaaaaaab;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0xaaaaaaab));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 1940);

        // rs1_val==6 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 1944);

        // rs1_val==6 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 1948);

        // rs1_val==6 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 1952);

        // rs1_val==6 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 1956);

        // rs1_val==6 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 1960);

        // rs1_val==6 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 1964);

        // rs1_val==6 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 1968);

        // rs1_val==6 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 1972);

        // rs1_val==6 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 1976);

        // rs1_val==6 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 1980);

        // rs1_val==6 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 1984);

        // rs1_val==6 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 1988);

        // rs1_val==6 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 1992);

        // rs1_val==6 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 1996);

        // rs1_val==6 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 2000);

        // rs1_val==6 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 2004);

        // rs1_val==6 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 2008);

        // rs1_val==6 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 2012);

        // rs1_val==6 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 2016);

        // rs1_val==6 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 2020);

        // rs1_val==6 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 2024);

        // rs1_val==6 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 2028);

        // rs1_val==6 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 2032);

        // rs1_val==6 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 2036);

        // rs1_val==6 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x6;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x6));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 2040);

        // rs1_val==858993460 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 2044);

        c.li(Reg::X2, 2120);
        // rs1_val==858993460 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 0);

        // rs1_val==858993460 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 4);

        // rs1_val==858993460 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 8);

        // rs1_val==858993460 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 12);

        // rs1_val==858993460 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 16);

        // rs1_val==858993460 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 20);

        // rs1_val==858993460 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 24);

        // rs1_val==858993460 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 28);

        // rs1_val==858993460 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 32);

        // rs1_val==858993460 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 36);

        // rs1_val==858993460 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 40);

        // rs1_val==858993460 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 44);

        // rs1_val==858993460 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 48);

        // rs1_val==858993460 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 52);

        // rs1_val==858993460 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 56);

        // rs1_val==858993460 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 60);

        // rs1_val==858993460 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 64);

        // rs1_val==858993460 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 68);

        // rs1_val==858993460 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 72);

        // rs1_val==858993460 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 76);

        // rs1_val==858993460 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 80);

        // rs1_val==858993460 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 84);

        // rs1_val==858993460 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 88);

        // rs1_val==858993460 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x33333334;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x33333334));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 92);

        // rs1_val==1717986919 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 96);

        // rs1_val==1717986919 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 100);

        // rs1_val==1717986919 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 104);

        // rs1_val==1717986919 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 108);

        // rs1_val==1717986919 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 112);

        // rs1_val==1717986919 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 116);

        // rs1_val==1717986919 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 120);

        // rs1_val==1717986919 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 124);

        // rs1_val==1717986919 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 128);

        // rs1_val==1717986919 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 132);

        // rs1_val==1717986919 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 136);

        // rs1_val==1717986919 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 140);

        // rs1_val==1717986919 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 144);

        // rs1_val==1717986919 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 148);

        // rs1_val==1717986919 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 152);

        // rs1_val==1717986919 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 156);

        // rs1_val==1717986919 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 160);

        // rs1_val==65534 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 164);

        // rs1_val==1717986919 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 168);

        // rs1_val==1717986919 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 172);

        // rs1_val==1717986919 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 176);

        // rs1_val==1717986919 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 180);

        // rs1_val==1717986919 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 184);

        // rs1_val==1717986919 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 188);

        // rs1_val==1717986919 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 192);

        // rs1_val==1717986919 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666667;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x66666667));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 196);

        // rs1_val==46341 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 200);

        // rs1_val==46341 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 204);

        // rs1_val==46341 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 208);

        // rs1_val==46341 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 212);

        // rs1_val==46341 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 216);

        // rs1_val==46341 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 220);

        // rs1_val==46341 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 224);

        // rs1_val==46341 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 228);

        // rs1_val==46341 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 232);

        // rs1_val==46341 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 236);

        // rs1_val==46341 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 240);

        // rs1_val==46341 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 244);

        // rs1_val==46341 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 248);

        // rs1_val==46341 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 252);

        // rs1_val==46341 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 256);

        // rs1_val==46341 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 260);

        // rs1_val==46341 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 264);

        // rs1_val==46341 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 268);

        // rs1_val==46341 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 272);

        // rs1_val==46341 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 276);

        // rs1_val==46341 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 280);

        // rs1_val==46341 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 284);

        // rs1_val==46341 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 288);

        // rs1_val==46341 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 292);

        // rs1_val==46341 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb505;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0xb505));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 296);

        // rs1_val==1 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 300);

        // rs1_val==1 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 304);

        // rs1_val==1 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 308);

        // rs1_val==1 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 312);

        // rs1_val==1 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 316);

        // rs1_val==1 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 320);

        // rs1_val==1 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 324);

        // rs1_val==1 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 328);

        // rs1_val==1 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 332);

        // rs1_val==1 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 336);

        // rs1_val==1 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 340);

        // rs1_val==1 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 344);

        // rs1_val==1 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 348);

        // rs1_val==1 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 352);

        // rs1_val==1 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 356);

        // rs1_val==1 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 360);

        // rs1_val==1 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 364);

        // rs1_val==1 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 368);

        // rs1_val==1 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 372);

        // rs1_val==1 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 376);

        // rs1_val==1 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 380);

        // rs1_val==1 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 384);

        // rs1_val==1 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 388);

        // rs1_val==1 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x1;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x1));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 392);

        // rs1_val==65536 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 396);

        // rs1_val==65536 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 400);

        // rs1_val==65536 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 404);

        // rs1_val==65536 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 408);

        // rs1_val==65536 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 412);

        // rs1_val==65536 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 416);

        // rs1_val==65536 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 420);

        // rs1_val==65536 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 424);

        // rs1_val==65536 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 428);

        // rs1_val==65536 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 432);

        // rs1_val==65536 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 436);

        // rs1_val==65536 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 440);

        // rs1_val==65536 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 444);

        // rs1_val==65536 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 448);

        // rs1_val==65536 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 452);

        // rs1_val==65536 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 456);

        // rs1_val==65536 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 460);

        // rs1_val==65536 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 464);

        // rs1_val==65536 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 468);

        // rs1_val==65536 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 472);

        // rs1_val==65536 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 476);

        // rs1_val==65536 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 480);

        // rs1_val==65536 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 484);

        // rs1_val==65536 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10000;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x10000));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 488);

        // rs1_val==1717986917 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 492);

        // rs1_val==1717986917 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 496);

        // rs1_val==1717986917 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 500);

        // rs1_val==1717986917 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 504);

        // rs1_val==1717986917 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 508);

        // rs1_val==1717986917 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 512);

        // rs1_val==1717986917 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x66666665;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0x66666665));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 516);

        // rs1_val==46339 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 520);

        // rs1_val==46339 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 524);

        // rs1_val==46339 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 528);

        // rs1_val==46339 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 532);

        // rs1_val==46339 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 536);

        // rs1_val==46339 and imm_val==1638, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x666
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x666));
        c.sw(Reg::X11, Reg::X2, 540);

        // rs1_val==46339 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 544);

        // rs1_val==46339 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 548);

        // rs1_val==46339 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 552);

        // rs1_val==46339 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 556);

        // rs1_val==46339 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 560);

        // rs1_val==46339 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 564);

        // rs1_val==46339 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 568);

        // rs1_val==46339 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 572);

        // rs1_val==46339 and imm_val==1637, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x665
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x665));
        c.sw(Reg::X11, Reg::X2, 576);

        // rs1_val==46339 and imm_val==44, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2c
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2c));
        c.sw(Reg::X11, Reg::X2, 580);

        // rs1_val==46339 and imm_val==62, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x3e
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3e));
        c.sw(Reg::X11, Reg::X2, 584);

        // rs1_val==46339 and imm_val==1366, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x556
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x556));
        c.sw(Reg::X11, Reg::X2, 588);

        // rs1_val==46339 and imm_val==2731, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0xaab
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaab));
        c.sw(Reg::X11, Reg::X2, 592);

        // rs1_val==46339 and imm_val==6, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x6
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x6));
        c.sw(Reg::X11, Reg::X2, 596);

        // rs1_val==46339 and imm_val==820, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x334
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x334));
        c.sw(Reg::X11, Reg::X2, 600);

        // rs1_val==46339 and imm_val==1639, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x667
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x667));
        c.sw(Reg::X11, Reg::X2, 604);

        // rs1_val==46339 and imm_val==46, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x2e
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2e));
        c.sw(Reg::X11, Reg::X2, 608);

        // rs1_val==46339 and imm_val==1, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x1
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x1));
        c.sw(Reg::X11, Reg::X2, 612);

        // rs1_val==46339 and imm_val==64, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xb503;  immval:0x40
        c.li(Reg::X10, MASK_XLEN(0xb503));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x40));
        c.sw(Reg::X11, Reg::X2, 616);

        // rs1_val==65534 and imm_val==3, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x3
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3));
        c.sw(Reg::X11, Reg::X2, 620);

        // rs1_val==65534 and imm_val==1365, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x555
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x555));
        c.sw(Reg::X11, Reg::X2, 624);

        // rs1_val==65534 and imm_val==2730, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0xaaa
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaaa));
        c.sw(Reg::X11, Reg::X2, 628);

        // rs1_val==65534 and imm_val==5, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x5
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x5));
        c.sw(Reg::X11, Reg::X2, 632);

        // rs1_val==65534 and imm_val==819, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x333
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x333));
        c.sw(Reg::X11, Reg::X2, 636);

        // rs1_val==65534 and imm_val==45, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x2d
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2d));
        c.sw(Reg::X11, Reg::X2, 640);

        // rs1_val==65534 and imm_val==0, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x0
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x0));
        c.sw(Reg::X11, Reg::X2, 644);

        // rs1_val==65534 and imm_val==63, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x3f
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x3f));
        c.sw(Reg::X11, Reg::X2, 648);

        // rs1_val==65534 and imm_val==2, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x2
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x2));
        c.sw(Reg::X11, Reg::X2, 652);

        // rs1_val==65534 and imm_val==1364, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x554
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x554));
        c.sw(Reg::X11, Reg::X2, 656);

        // rs1_val==65534 and imm_val==2729, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0xaa9
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xaa9));
        c.sw(Reg::X11, Reg::X2, 660);

        // rs1_val==65534 and imm_val==4, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x4
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x4));
        c.sw(Reg::X11, Reg::X2, 664);

        // rs1_val==65534 and imm_val==818, 
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0xfffe;  immval:0x332
        c.li(Reg::X10, MASK_XLEN(0xfffe));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0x332));
        c.sw(Reg::X11, Reg::X2, 668);

        // imm_val == 4094, rs1_val == 16
        // opcode: sltiu ; op1:x10; dest:x11; op1val:0x10;  immval:0xffe
        c.li(Reg::X10, MASK_XLEN(0x10));
        c.sltiu(Reg::X11, Reg::X10, SEXT_IMM(0xffe));
        c.sw(Reg::X11, Reg::X2, 672);


                
        // End
        c.ecall();
        return c;
    }

    static std::vector<uint8_t> s_ScalarData;
};

std::vector<uint8_t> sltiu_01::s_ScalarData;
}


// Generate google tests

TEST_F(sltiu_01, opcode__sltiu___op1_x23__dest_x28__op1val_0x400___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[0], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x2__dest_x2__op1val_0x800___immval_0xfff) {
  ASSERT_EQ(getScalarWordData()[1], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x3__dest_x25__op1val_0x4___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[2], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x19__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[3], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x14__dest_x15__op1val_0xffffffff___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[4], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x13__dest_x4__op1val_0x1___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[5], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x26__dest_x3__op1val_0xd___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x20__dest_x29__op1val_0xaaaaaaaa___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x27__dest_x16__op1val_0x7fffffff___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[8], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x17__dest_x20__op1val_0xfeffffff___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x31__dest_x8__op1val_0x800___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x24__dest_x23__op1val_0xc___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[11], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x25__dest_x26__op1val_0x55555555___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x22__dest_x6__op1val_0x80000___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x12__dest_x5__op1val_0xfffffff7___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[14], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x9__dest_x1__op1val_0x80000000___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x28__dest_x10__op1val_0xfffbffff___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x21__dest_x31__op1val_0x0___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[17], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x0__dest_x21__op1val_0x0___immval_0xffe) {
  ASSERT_EQ(getScalarWordData()[18], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x11__dest_x14__op1val_0x12___immval_0xffd) {
  ASSERT_EQ(getScalarWordData()[19], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x30__dest_x13__op1val_0x6___immval_0xffb) {
  ASSERT_EQ(getScalarWordData()[20], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x29__dest_x17__op1val_0x40000000___immval_0xff7) {
  ASSERT_EQ(getScalarWordData()[21], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x8__dest_x24__op1val_0xffffffbf___immval_0xfef) {
  ASSERT_EQ(getScalarWordData()[22], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x1__dest_x27__op1val_0x5___immval_0xfdf) {
  ASSERT_EQ(getScalarWordData()[23], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x7__dest_x30__op1val_0x3___immval_0xfbf) {
  ASSERT_EQ(getScalarWordData()[24], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x5__dest_x22__op1val_0x40000___immval_0xf7f) {
  ASSERT_EQ(getScalarWordData()[25], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x15__dest_x0__op1val_0xe___immval_0xeff) {
  ASSERT_EQ(getScalarWordData()[26], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x6__dest_x7__op1val_0x3___immval_0xdff) {
  ASSERT_EQ(getScalarWordData()[27], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x18__dest_x12__op1val_0x7fffffff___immval_0xbff) {
  ASSERT_EQ(getScalarWordData()[28], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x16__dest_x18__op1val_0x1000000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x4__dest_x9__op1val_0xfbffffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[30], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x19__op1val_0xfffff7ff___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[31], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[32], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x8___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[33], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x20___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[34], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x40___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x80___immval_0xff7) {
  ASSERT_EQ(getScalarWordData()[36], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x100___immval_0xfdf) {
  ASSERT_EQ(getScalarWordData()[37], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x200___immval_0xff7) {
  ASSERT_EQ(getScalarWordData()[38], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[40], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4000___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[41], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x8000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[42], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[43], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x20000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[44], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x100000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[45], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x200000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[46], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x400000___immval_0xfdf) {
  ASSERT_EQ(getScalarWordData()[47], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x800000___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[48], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2000000___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[49], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4000000___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[50], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x8000000___immval_0xc) {
  ASSERT_EQ(getScalarWordData()[51], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000000___immval_0xfbf) {
  ASSERT_EQ(getScalarWordData()[52], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x20000000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[53], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffffe___immval_0xfef) {
  ASSERT_EQ(getScalarWordData()[54], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffffd___immval_0xfff) {
  ASSERT_EQ(getScalarWordData()[55], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffffb___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[56], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffffef___immval_0xfff) {
  ASSERT_EQ(getScalarWordData()[57], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffffdf___immval_0xeff) {
  ASSERT_EQ(getScalarWordData()[58], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffff7f___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[59], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffeff___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[60], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffdff___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[61], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffffbff___immval_0xf) {
  ASSERT_EQ(getScalarWordData()[62], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffefff___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[63], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffdfff___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[64], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffffbfff___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[65], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff7fff___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[66], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffeffff___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[67], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffdffff___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[68], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfff7ffff___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[69], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffefffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[70], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffdfffff___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[71], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffbfffff___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[72], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xff7fffff___immval_0xffe) {
  ASSERT_EQ(getScalarWordData()[73], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfdffffff___immval_0xd) {
  ASSERT_EQ(getScalarWordData()[74], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xf7ffffff___immval_0x800) {
  ASSERT_EQ(getScalarWordData()[75], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xefffffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[76], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xdfffffff___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[77], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xbfffffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[78], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[80], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[81], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[82], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[83], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[84], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[85], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[86], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[87], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[88], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[89], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[90], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[91], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[92], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[93], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[94], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[95], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[96], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[97], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[98], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[99], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[100], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[101], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[102], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x3___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[103], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[104], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[105], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[106], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[107], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[108], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[109], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[110], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[111], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[112], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[113], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[114], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[115], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[116], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[117], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[118], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[119], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[120], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[121], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[122], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[123], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[124], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[125], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[126], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555555___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[127], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[128], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[129], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[130], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[131], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[132], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[133], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[134], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[135], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[136], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[137], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[138], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[139], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[140], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[141], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[142], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[143], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[144], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[145], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[146], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[147], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[148], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[149], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[150], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaaa___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[151], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[152], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[153], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[154], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[155], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[156], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[157], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[158], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[159], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[160], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[161], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[162], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[163], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[164], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[165], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[166], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[167], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[168], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[169], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[170], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[171], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[172], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[173], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[174], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[175], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x5___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[176], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[177], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[178], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[179], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[180], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[181], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[182], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[183], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[184], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[185], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[186], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[187], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[188], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[189], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[190], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[191], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[192], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[193], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[194], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[195], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[196], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[197], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[198], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[199], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[200], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333333___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[201], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[202], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[203], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[204], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[205], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[206], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[207], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[208], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[209], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[210], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[211], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[212], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[213], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[214], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[215], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[216], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[217], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[218], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[219], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[220], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[221], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[222], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[223], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[224], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[225], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666666___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[226], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[227], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[228], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[229], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[230], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[231], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[232], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[233], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[234], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[235], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[236], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[237], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[238], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[239], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[240], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[241], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[242], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[243], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[244], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[245], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[246], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[247], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[248], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[249], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[250], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb504___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[251], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[252], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[253], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[254], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[255], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[256], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[257], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[258], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[259], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[260], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[261], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[262], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[263], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[264], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[265], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[266], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[267], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[268], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[269], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[270], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[271], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[272], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[273], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[274], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x0___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[275], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[276], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[277], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[278], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[279], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[280], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[281], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[282], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[283], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[284], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[285], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[286], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[287], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[288], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[289], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[290], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[291], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[292], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[293], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[294], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[295], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[296], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[297], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[298], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[299], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xffff___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[300], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[301], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[302], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[303], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[304], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[305], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[306], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[307], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[308], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[309], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[310], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[311], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[312], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[313], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[314], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[315], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[316], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[317], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[318], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[319], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[320], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[321], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[322], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[323], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[324], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x2___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[325], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[326], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[327], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[328], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[329], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[330], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[331], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[332], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[333], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[334], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[335], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[336], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[337], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[338], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[339], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[340], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[341], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[342], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[343], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[344], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[345], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[346], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[347], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[348], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[349], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555554___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[350], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[351], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[352], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[353], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[354], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[355], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[356], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[357], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[358], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[359], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[360], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[361], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[362], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[363], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[364], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[365], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[366], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[367], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[368], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[369], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[370], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[371], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[372], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[373], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[374], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaa9___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[375], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[376], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[377], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[378], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[379], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[380], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[381], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[382], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[383], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[384], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[385], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[386], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[387], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[388], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[389], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[390], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[391], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[392], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[393], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[394], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[395], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[396], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[397], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[398], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x4___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[399], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[400], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[401], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[402], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[403], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[404], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[405], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[406], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[407], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[408], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[409], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[410], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[411], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[412], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[413], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[414], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[415], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[416], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[417], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[418], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[419], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[420], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[421], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[422], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[423], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333332___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[424], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[425], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[426], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[427], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[428], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[429], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[430], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[431], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[432], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[433], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[434], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[435], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[436], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[437], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[438], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[439], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[440], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[441], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[442], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[443], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[444], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[445], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[446], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[447], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[448], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[449], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[450], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[451], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[452], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[453], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[454], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[455], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[456], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[457], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[458], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[459], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[460], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[461], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[462], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[463], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[464], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[465], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[466], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[467], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[468], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[469], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[470], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[471], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[472], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[473], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[474], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[475], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[476], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[477], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x55555556___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[478], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[479], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[480], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[481], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[482], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[483], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[484], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[485], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[486], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[487], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[488], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[489], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[490], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[491], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[492], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[493], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[494], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[495], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[496], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[497], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[498], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[499], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[500], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[501], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[502], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xaaaaaaab___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[503], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[504], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[505], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[506], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[507], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[508], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[509], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[510], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[511], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[512], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[513], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[514], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[515], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[516], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[517], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[518], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[519], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[520], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[521], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[522], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[523], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[524], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[525], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[526], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[527], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x6___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[528], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[529], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[530], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[531], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[532], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[533], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[534], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[535], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[536], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[537], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[538], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[539], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[540], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[541], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[542], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[543], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[544], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[545], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[546], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[547], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[548], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[549], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[550], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[551], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[552], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x33333334___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[553], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[554], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[555], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[556], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[557], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[558], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[559], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[560], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[561], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[562], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[563], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[564], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[565], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[566], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[567], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[568], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[569], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[570], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[571], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[572], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[573], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[574], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[575], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[576], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[577], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[578], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666667___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[579], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[580], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[581], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[582], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[583], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[584], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[585], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[586], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[587], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[588], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[589], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[590], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[591], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[592], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[593], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[594], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[595], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[596], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[597], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[598], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[599], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[600], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[601], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[602], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[603], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb505___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[604], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[605], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[606], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[607], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[608], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[609], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[610], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[611], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[612], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[613], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[614], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[615], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[616], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[617], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[618], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[619], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[620], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[621], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[622], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[623], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[624], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[625], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[626], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[627], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x1___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[628], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[629], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[630], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[631], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[632], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[633], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[634], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[635], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[636], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[637], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[638], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[639], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[640], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[641], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[642], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[643], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[644], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[645], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[646], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[647], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[648], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[649], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[650], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[651], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[652], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[653], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[654], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[655], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[656], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[657], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[658], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x66666665___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[659], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[660], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[661], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[662], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[663], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[664], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[665], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[666], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[667], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[668], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[669], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[670], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[671], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[672], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[673], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[674], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[675], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x3e) {
  ASSERT_EQ(getScalarWordData()[676], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[677], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0xaab) {
  ASSERT_EQ(getScalarWordData()[678], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[679], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[680], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[681], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[682], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[683], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xb503___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[684], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[685], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[686], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0xaaa) {
  ASSERT_EQ(getScalarWordData()[687], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[688], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[689], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[690], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[691], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x3f) {
  ASSERT_EQ(getScalarWordData()[692], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[693], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[694], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0xaa9) {
  ASSERT_EQ(getScalarWordData()[695], 1);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[696], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0xfffe___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[697], 0);
}


TEST_F(sltiu_01, opcode__sltiu___op1_x10__dest_x11__op1val_0x10___immval_0xffe) {
  ASSERT_EQ(getScalarWordData()[698], 1);
}

