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
