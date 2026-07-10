// Google test includes
#include "gtest/gtest.h"

// Frontend includes
#include "frontend/shape.h"

using namespace Frontend;

//--------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------
TEST(Shape, Split)
{
    // 1D
    {
        const Shape shape(180);
        
        // Don't split
        {
            const auto split = shape.getSplit(0, 0, 1, 32);

            EXPECT_EQ(split, Shape(180));
        }

        // Splitting two ways without granularity
        {
            const auto split0 = shape.getSplit(0, 0, 2, 1);
            const auto split1 = shape.getSplit(1, 0, 2, 1);

            EXPECT_EQ(split0, Shape(90));
            EXPECT_EQ(split1, Shape(90));
        }

        // Splitting two ways with granulatity
        {
            const auto split0 = shape.getSplit(0, 0, 2, 32);
            const auto split1 = shape.getSplit(1, 0, 2, 32);

            EXPECT_EQ(split0, Shape(96));
            EXPECT_EQ(split1, Shape(84));
        }
    }

    // 2D highest dimension
    {
        const Shape shape({5, 272});

        // Splitting two ways without granularity
        {
            const auto split0 = shape.getSplit(0, 0, 2, 1);
            const auto split1 = shape.getSplit(1, 0, 2, 1);

            EXPECT_EQ(split0, Shape({3, 272}));
            EXPECT_EQ(split1, Shape({2, 272}));
        }

        // Splitting two ways with granularity 32
        {
            const auto split0 = shape.getSplit(0, 0, 2, 32);
            const auto split1 = shape.getSplit(1, 0, 2, 32);

            // Order is different because
            EXPECT_EQ(split0, Shape({2, 272}));
            EXPECT_EQ(split1, Shape({3, 272}));
        }
    }

    // 2D lowest dimension
    {
        const Shape shape({1000, 400});

        // Splitting two ways without granularity
        {
            const auto split0 = shape.getSplit(0, 1, 2, 1);
            const auto split1 = shape.getSplit(1, 1, 2, 1);

            EXPECT_EQ(split0, Shape({1000, 200}));
            EXPECT_EQ(split1, Shape({1000, 200}));
        }

        // Splitting two ways with granularity 32
        {
            const auto split0 = shape.getSplit(0, 1, 2, 32);
            const auto split1 = shape.getSplit(1, 1, 2, 32);

            // Order is different because
            EXPECT_EQ(split0, Shape({1000, 192}));
            EXPECT_EQ(split1, Shape({1000, 208}));
        }
    }


}