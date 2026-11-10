#pragma once

#include <asio.hpp>
#include <string>

#include "ObjectStore.hpp"

namespace saab {

// Reads "ID=..;X=..;Y=..;TYPE=.." lines from the SAAB server into the store.
// Bad lines are logged and skipped.
class SaabServerClient {
public:
    SaabServerClient(asio::io_context& io_context, ObjectStore& store,
                      std::string host, std::string port);

private:
    void connect();
    void read_line();
    void handle_line(const std::string& line);

    ObjectStore& store_;
    std::string host_;
    std::string port_;
    asio::ip::tcp::resolver resolver_;
    asio::ip::tcp::socket socket_;
    asio::streambuf buffer_;
};

} // namespace saab
