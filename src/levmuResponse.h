#ifndef _LEVMU_RESPONSE_H_
#define _LEVMU_RESPONSE_H_

#include <muduo/net/Callbacks.h>

#include <string>

class levmuResponse {
    const muduo::net::TcpConnectionPtr conn_;
    std::string write_buffer;

    public:
    explicit 
    levmuResponse(const muduo::net::TcpConnectionPtr& conn)
        : conn_(conn), write_buffer("") {}

    ~levmuResponse();

    void write_nil();
    void write_error(const char* msg);
    void write_status(const char* msg);
    void write_integer(const char *out, size_t out_size);
    void write_bulk(const char *out, size_t out_size);
    void write_bulk(const std::string &out);
    void write_mbulk_header(int n);
    private:
    //None copyable
    levmuResponse(const levmuResponse&);
    void operator=(const levmuResponse&);
};

#endif // _LEVMU_RESPONSE_H_
