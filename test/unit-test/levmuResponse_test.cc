#include "../../src/levmuResponse.h"
#include "testharness.h"

#include <muduo/base/Logging.h>


class levmuResponseTest { };

TEST(levmuResponseTest, Empty) {
    LOG_INFO << "Empty levmuResponse Test";
    //levmuResponse lr;
}


int main(int argc, char** argv) {
    return RunAllTests();
}
