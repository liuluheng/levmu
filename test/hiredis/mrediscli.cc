#include "Hiredis.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

#include <string>

using namespace muduo;
using namespace muduo::net;

string toString(long long value)
{
  char buf[32];
  snprintf(buf, sizeof buf, "%lld", value);
  return buf;
}

string redisReplyToString(const redisReply *reply)
{
  static const char *const types[] = { "",
                                       "REDIS_REPLY_STRING", "REDIS_REPLY_ARRAY",
                                       "REDIS_REPLY_INTEGER", "REDIS_REPLY_NIL",
                                       "REDIS_REPLY_STATUS", "REDIS_REPLY_ERROR"
                                     };
  string str;

  if (!reply) {
    return str;
  }

  str += types[reply->type] + string("(") + toString(reply->type) + ") ";

  str += "{ ";

  if (reply->type == REDIS_REPLY_STRING ||
      reply->type == REDIS_REPLY_STATUS ||
      reply->type == REDIS_REPLY_ERROR) {
    str += '"' + string(reply->str, reply->len) + '"';
  } else if (reply->type == REDIS_REPLY_INTEGER) {
    str += toString(reply->integer);
  } else if (reply->type == REDIS_REPLY_ARRAY) {
    str += toString(reply->elements) + " ";

    for (size_t i = 0; i < reply->elements; i++) {
      str += " " + redisReplyToString(reply->element[i]);
    }
  }

  str += " }";

  return str;
}

void SETCallback(hiredis::Hiredis *c, redisReply *reply, string *key)
{
  LOG_INFO << "SET " << *key << " " << redisReplyToString(reply);
}

void connectCallback(hiredis::Hiredis *c, int status)
{
  if (status != REDIS_OK) {
    LOG_ERROR << "connectCallback Error:" << c->errstr();
  } else {
    LOG_INFO << "Connected...";
    string key_hiredis("key_hiredis");
    // shit here...
    c->command(boost::bind(SETCallback, _1, _2, &key_hiredis),
            "SET %s %d", key_hiredis.c_str(), 0);
  }
}

void disconnectCallback(hiredis::Hiredis *c, int status)
{
  if (status != REDIS_OK) {
    LOG_ERROR << "disconnectCallback Error:" << c->errstr();
  } else {
    LOG_INFO << "Disconnected...";
  }
}

void echoCallback(hiredis::Hiredis *c, redisReply *reply, string *echo)
{
  LOG_INFO << *echo << " " << redisReplyToString(reply);
  c->disconnect();
}

void echo(hiredis::Hiredis *c, string *s)
{
  c->command(boost::bind(echoCallback, _1, _2, s), "echo %s", s->c_str());
}

void set(hiredis::Hiredis *c, string *key)
{
  c->command(boost::bind(SETCallback, _1, _2, key), "SET %s 0", key->c_str());
}

void GETCallback(hiredis::Hiredis *c, redisReply *reply, string *key)
{
  LOG_INFO << "GET " << *key << " " << redisReplyToString(reply);
}

void get(hiredis::Hiredis *c, string *key)
{
  c->command(boost::bind(GETCallback, _1, _2, key), "GET %s", key->c_str());
}

void INCRCallback(hiredis::Hiredis *c, redisReply *reply, string *key)
{
  LOG_INFO << "INCR " << *key << " " << redisReplyToString(reply);
}

void incr(hiredis::Hiredis *c, string *key)
{
  c->command(boost::bind(INCRCallback, _1, _2, key), "INCR %s", key->c_str());
}

int main(int argc, char **argv)
{
  Logger::setLogLevel(Logger::DEBUG);

  EventLoop loop;

  InetAddress serverAddr("127.0.0.1", 6379);
  hiredis::Hiredis hiredis(&loop, serverAddr);

  hiredis.setConnectCallback(connectCallback);
  hiredis.setDisconnectCallback(disconnectCallback);
  hiredis.connect();

  //hiredis.ping();
  // loop.runEvery(1.0, boost::bind(&hiredis::Hiredis::ping, &hiredis));

  // string hi = "hi";
  // hiredis.command(boost::bind(echoCallback, _1, _2, &hi), "echo %s", hi.c_str());
  // loop.runEvery(2.0, boost::bind(echo, &hiredis, &hi));


  string key_hiredis("key_hiredis");
  loop.runEvery(2.0, boost::bind(incr, &hiredis, &key_hiredis));

  // hiredis.command(boost::bind(GETCallback, _1, _2, &key_hiredis),
  //                "GET %s", key_hiredis.c_str());
  loop.runEvery(2.0, boost::bind(get, &hiredis, &key_hiredis));

  loop.loop();

  return 0;
}
