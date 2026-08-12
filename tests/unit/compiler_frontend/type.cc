// Google test includes
#include "gtest/gtest.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

using namespace CompilerFrontend;

//--------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------
TEST(Type, writeNumeric)
{
    // Float to float
    // Float to double
    // Float to int
    // Int to float
    // Int to int
    // Int to unsigned int

    EXPECT_EQ(Type::writeNumeric(12.8, Type::Int32), "12");
    EXPECT_EQ(Type::writeNumeric(-12.8, Type::Int32), "-12");
    EXPECT_EQ(Type::writeNumeric(13.1, Type::Uint32), "13u");
    EXPECT_EQ(Type::writeNumeric(0xFFFFFFFFu, Type::Uint32), "4294967295u");

    // Too small
    EXPECT_THROW(
        {
            Type::writeNumeric(-12, Type::Uint32);
        },
        std::runtime_error);
   

    // Too big
    EXPECT_THROW(
        {
            Type::writeNumeric(0xFFFFFFFFu, Type::Int32);
        },
        std::runtime_error);
}
//--------------------------------------------------------------------------
TEST(Type, pythonFormamt)
{
    EXPECT_EQ(Type::Bool.getNumeric().pythonFormat, "?");

    EXPECT_EQ(Type::Int8.getNumeric().pythonFormat, "b");
    EXPECT_EQ(Type::Int16.getNumeric().pythonFormat, "h");
    EXPECT_EQ(Type::Int32.getNumeric().pythonFormat, "i");
    EXPECT_EQ(Type::Int64.getNumeric().pythonFormat, "q");

    EXPECT_EQ(Type::Uint8.getNumeric().pythonFormat, "B");
    EXPECT_EQ(Type::Uint16.getNumeric().pythonFormat, "H");
    EXPECT_EQ(Type::Uint32.getNumeric().pythonFormat, "I");
    EXPECT_EQ(Type::Uint64.getNumeric().pythonFormat, "Q");

    EXPECT_EQ(Type::S0_15.getNumeric().pythonFormat, "h");
    EXPECT_EQ(Type::S14_1Sat.getNumeric().pythonFormat, "h");

    EXPECT_EQ(Type::Float.getNumeric().pythonFormat, "f");
    EXPECT_EQ(Type::Double.getNumeric().pythonFormat, "d");
}