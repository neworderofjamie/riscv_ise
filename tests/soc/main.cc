// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// Google test includes
#include "gtest/gtest.h"

int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    return RUN_ALL_TESTS();
}
