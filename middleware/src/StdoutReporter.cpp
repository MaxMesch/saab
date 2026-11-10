#include "StdoutReporter.hpp"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "Protocol.hpp"

namespace saab {

StdoutReporter::StdoutReporter(asio::io_context& io_context, ObjectStore& store)
    : io_context_(io_context), store_(store), timer_(io_context) {
    timer_.expires_after(kReportInterval);
    schedule_next();
}

void StdoutReporter::schedule_next() {
    timer_.async_wait([this](const asio::error_code&) {
        if (!write_report()) {
            io_context_.stop();
            return;
        }
        // From the last deadline, not now, to avoid drift.
        timer_.expires_at(timer_.expiry() + kReportInterval);
        schedule_next();
    });
}

bool StdoutReporter::write_report() {
    const auto payload = encode_report(store_.snapshot());

    const std::size_t written = std::fwrite(payload.data(), 1, payload.size(), stdout);
    if (written != payload.size() || std::fflush(stdout) != 0) {
        std::clog << "Error: failed to write report to stdout: " << std::strerror(errno)
                  << '\n';
        return false;
    }
    return true;
}

} // namespace saab
