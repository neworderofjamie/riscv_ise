// Google test includes
#include "gtest/gtest.h"

// Common includes
#include "common/utils.h"

using namespace Common;

//--------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------
TEST(Utils, StringHashing)
{
    // Hash "hello" followed by empty string
    boost::uuids::detail::sha1 hash1;
    Utils::updateHash("hello", hash1);
    Utils::updateHash("", hash1);

    // Hash empty string followed by "hello"
    boost::uuids::detail::sha1 hash2;
    Utils::updateHash("", hash2);
    Utils::updateHash("hello", hash2);

    ASSERT_NE(hash1.get_digest(), hash2.get_digest());
}
//--------------------------------------------------------------------------
TEST(Utils, VectorHashing)
{
    const std::vector<float> vector1{ 1.0f, 0.0f };
    const std::vector<float> vector2;

    // Hash "hello" followed by empty string
    boost::uuids::detail::sha1 hash1;
    Utils::updateHash(vector1, hash1);
    Utils::updateHash(vector2, hash1);

    // Hash empty string followed by "hello"
    boost::uuids::detail::sha1 hash2;
    Utils::updateHash(vector2, hash2);
    Utils::updateHash(vector1, hash2);

    ASSERT_NE(hash1.get_digest(), hash2.get_digest());
}
