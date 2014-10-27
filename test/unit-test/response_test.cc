#include "../../src/response.h"
#include "testharness.h"

#include <muduo/base/Logging.h>

class ResponseTest { };

TEST(ResponseTest, Empty) {
  LOG_INFO << "Empty levmuResponse Test";
  //Response r;
}


int main(int argc, char** argv) {
  return RunAllTests();
}
