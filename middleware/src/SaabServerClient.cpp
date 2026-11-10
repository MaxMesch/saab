#include "SaabServerClient.hpp"

#include <charconv>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

namespace saab {

namespace {

// Value after "<key>=" up to the next ';'.
std::optional<std::string_view> extract_field(std::string_view line, std::string_view key) {
    const std::string prefix = std::string(key) + "=";
    const auto pos = line.find(prefix);
    if (pos == std::string_view::npos) {
        return std::nullopt;
    }
    const auto value_start = pos + prefix.size();
    const auto end = line.find(';', value_start);
    return line.substr(value_start, end == std::string_view::npos ? std::string_view::npos
                                                                    : end - value_start);
}

template <typename T>
bool parse_integer(std::string_view text, T& out) {
    const auto result = std::from_chars(text.data(), text.data() + text.size(), out);
    return result.ec == std::errc() && result.ptr == text.data() + text.size();
}

std::optional<ObjectState> parse_object_line(std::string_view line, std::string& error) {
    const auto id_text = extract_field(line, "ID");
    const auto x_text = extract_field(line, "X");
    const auto y_text = extract_field(line, "Y");
    const auto type_text = extract_field(line, "TYPE");

    if (!id_text || !x_text || !y_text || !type_text) {
        error = "missing required field(s)";
        return std::nullopt;
    }

    ObjectState obj;
    if (!parse_integer(*id_text, obj.id) || !parse_integer(*x_text, obj.x) ||
        !parse_integer(*y_text, obj.y) || !parse_integer(*type_text, obj.type)) {
        error = "unparsable or out-of-range field value";
        return std::nullopt;
    }

    // The specification says type is unsigned 8bit integer, but not that the type has a valid category.
    if (!category_of(obj.type)) {
        error = "unknown TYPE " + std::to_string(obj.type) + " (no category)";
        return std::nullopt;
    }

    return obj;
}

} // namespace

SaabServerClient::SaabServerClient(asio::io_context& io_context, ObjectStore& store,
                                    std::string host, std::string port)
    : store_(store),
      host_(std::move(host)),
      port_(std::move(port)),
      resolver_(io_context),
      socket_(io_context) {
    connect();
}

void SaabServerClient::connect() {
    resolver_.async_resolve(
        host_, port_,
        [this](const asio::error_code& ec, const asio::ip::tcp::resolver::results_type& endpoints) {
            if (ec) {
                std::clog << "Error: failed to resolve SAAB server " << host_ << ":" << port_
                          << ": " << ec.message() << '\n';
                return;
            }
            asio::async_connect(
                socket_, endpoints,
                [this](const asio::error_code& ec, const asio::ip::tcp::endpoint&) {
                    if (ec) {
                        std::clog << "Error: failed to connect to SAAB server " << host_ << ":"
                                  << port_ << ": " << ec.message() << '\n';
                        return;
                    }
                    read_line();
                });
        });
}

void SaabServerClient::read_line() {
    asio::async_read_until(
        socket_, buffer_, '\n', [this](const asio::error_code& ec, std::size_t) {
            if (ec) {
                std::clog << "Error: lost connection to SAAB server: " << ec.message() << '\n';
                return;
            }

            std::istream stream(&buffer_);
            std::string line;
            std::getline(stream, line);

            handle_line(line);
            read_line();
        });
}

void SaabServerClient::handle_line(const std::string& line) {
    std::string error;
    if (auto obj = parse_object_line(line, error)) {
        store_.upsert(*obj);
    } else {
        std::clog << "Error: malformed line from SAAB server (" << error << "): \"" << line
                  << "\"\n";
    }
}

} // namespace saab
