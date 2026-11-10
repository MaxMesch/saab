#pragma once

#include <asio.hpp>
#include <chrono>

#include "ObjectStore.hpp"

namespace saab {

// 5/3 s
constexpr std::chrono::microseconds kReportInterval{1'666'667};

// Writes a report to stdout every kReportInterval. Stops the io_context if
// stdout breaks (client gone).
class StdoutReporter {
public:
    StdoutReporter(asio::io_context& io_context, ObjectStore& store);

private:
    void schedule_next();
    bool write_report();

    asio::io_context& io_context_;
    ObjectStore& store_;
    asio::steady_timer timer_;
};

} // namespace saab
