#include <asio.hpp>
#include <csignal>
#include <cstdlib>
#include <iostream>

#include "ObjectStore.hpp"
#include "SaabServerClient.hpp"
#include "StdoutReporter.hpp"

int main() {
    // Get EPIPE instead of dying when the client exits.
    std::signal(SIGPIPE, SIG_IGN);

    try {
        asio::io_context io_context;
        saab::ObjectStore store;
        saab::SaabServerClient server_client(io_context, store, "localhost", "5463");
        saab::StdoutReporter reporter(io_context, store);

        std::clog << "Middleware connecting to SAAB server at localhost:5463, writing reports to stdout\n";
        io_context.run();
    } catch (const std::exception& e) {
        std::clog << "Error: fatal exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
