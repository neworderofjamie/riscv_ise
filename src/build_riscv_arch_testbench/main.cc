// RISC-V utils include
#include "common/app_utils.h"

// RISC-V architecture test includes
#include "add-01.h"
#include "slli-01.h"
#include "slt-01.h"
#include "slti-01.h"
#include "sltiu-01.h"
#include "sltu-01.h"
#include "sra-01.h"
#include "srai-01.h"
#include "srl-01.h"
#include "srli-01.h"
#include "sub-01.h"
#include "xor-01.h"
#include "xori-01.h"
#include "addi-01.h"
#include "and-01.h"
#include "andi-01.h"
#include "lui-01.h"
#include "lw-align-01.h"
#include "or-01.h"
#include "ori-01.h"
#include "sll-01.h"


int main()
{
    //------------------------------------------------------------------------
    // add-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("add-01_data.coe",
                          ArchTest::add_01::getScalarInitData());
        AppUtils::dumpCOE("add-01_instructions.coe",
                          ArchTest::add_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // slli-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("slli-01_data.coe",
                          ArchTest::slli_01::getScalarInitData());
        AppUtils::dumpCOE("slli-01_instructions.coe",
                          ArchTest::slli_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // slt-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("slt-01_data.coe",
                          ArchTest::slt_01::getScalarInitData());
        AppUtils::dumpCOE("slt-01_instructions.coe",
                          ArchTest::slt_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // slti-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("slti-01_data.coe",
                          ArchTest::slti_01::getScalarInitData());
        AppUtils::dumpCOE("slti-01_instructions.coe",
                          ArchTest::slti_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // sltiu-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("sltiu-01_data.coe",
                          ArchTest::sltiu_01::getScalarInitData());
        AppUtils::dumpCOE("sltiu-01_instructions.coe",
                          ArchTest::sltiu_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // sltu-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("sltu-01_data.coe",
                          ArchTest::sltu_01::getScalarInitData());
        AppUtils::dumpCOE("sltu-01_instructions.coe",
                          ArchTest::sltu_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // sra-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("sra-01_data.coe",
                          ArchTest::sra_01::getScalarInitData());
        AppUtils::dumpCOE("sra-01_instructions.coe",
                          ArchTest::sra_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // srai-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("srai-01_data.coe",
                          ArchTest::srai_01::getScalarInitData());
        AppUtils::dumpCOE("srai-01_instructions.coe",
                          ArchTest::srai_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // srl-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("srl-01_data.coe",
                          ArchTest::srl_01::getScalarInitData());
        AppUtils::dumpCOE("srl-01_instructions.coe",
                          ArchTest::srl_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // srli-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("srli-01_data.coe",
                          ArchTest::srli_01::getScalarInitData());
        AppUtils::dumpCOE("srli-01_instructions.coe",
                          ArchTest::srli_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // sub-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("sub-01_data.coe",
                          ArchTest::sub_01::getScalarInitData());
        AppUtils::dumpCOE("sub-01_instructions.coe",
                          ArchTest::sub_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // xor-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("xor-01_data.coe",
                          ArchTest::xor_01::getScalarInitData());
        AppUtils::dumpCOE("xor-01_instructions.coe",
                          ArchTest::xor_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // xori-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("xori-01_data.coe",
                          ArchTest::xori_01::getScalarInitData());
        AppUtils::dumpCOE("xori-01_instructions.coe",
                          ArchTest::xori_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // addi-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("addi-01_data.coe",
                          ArchTest::addi_01::getScalarInitData());
        AppUtils::dumpCOE("addi-01_instructions.coe",
                          ArchTest::addi_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // and-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("and-01_data.coe",
                          ArchTest::and_01::getScalarInitData());
        AppUtils::dumpCOE("and-01_instructions.coe",
                          ArchTest::and_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // andi-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("andi-01_data.coe",
                          ArchTest::andi_01::getScalarInitData());
        AppUtils::dumpCOE("andi-01_instructions.coe",
                          ArchTest::andi_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // lui-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("lui-01_data.coe",
                          ArchTest::lui_01::getScalarInitData());
        AppUtils::dumpCOE("lui-01_instructions.coe",
                          ArchTest::lui_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // lw-align-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("lw-align-01_data.coe",
                          ArchTest::lw_align_01::getScalarInitData());
        AppUtils::dumpCOE("lw-align-01_instructions.coe",
                          ArchTest::lw_align_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // or-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("or-01_data.coe",
                          ArchTest::or_01::getScalarInitData());
        AppUtils::dumpCOE("or-01_instructions.coe",
                          ArchTest::or_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // ori-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("ori-01_data.coe",
                          ArchTest::ori_01::getScalarInitData());
        AppUtils::dumpCOE("ori-01_instructions.coe",
                          ArchTest::ori_01::generateCode().getCode());
    }
    //------------------------------------------------------------------------
    // sll-01
    //------------------------------------------------------------------------
    {
        AppUtils::dumpCOE("sll-01_data.coe",
                          ArchTest::sll_01::getScalarInitData());
        AppUtils::dumpCOE("sll-01_instructions.coe",
                          ArchTest::sll_01::generateCode().getCode());
    }
    
    return 0;
}