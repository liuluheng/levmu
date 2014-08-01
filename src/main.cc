// Use of this source code is governed by a MIT-style license
// that can be found in the License file.
//
// Author: Liu Lu Heng

#include "server.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <stdlib.h> //for atoi();

int main(int argc, char *argv[])
{
  LOG_INFO << "pid = " << getpid();

  muduo::net::EventLoop loop;
  muduo::net::InetAddress listenAddr(8323);
  levmu::Server server(&loop, listenAddr);

  if (argc > 1) {
      server.setThreadNum(atoi(argv[1]));
  }

  server.start();
  loop.loop();

  return 0;
}

