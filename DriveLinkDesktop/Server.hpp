#pragma once

#include "InputState.hpp"
#include "Protocols.hpp"

#include <atomic>
#include <sockpp/udp_socket.h>
#include <thread>
#include <variant>

namespace dl {
namespace network {

class InputListener {
public:
    InputListener(sockpp::inet_address addr)
        : m_addr(addr) { };

    /*
        Run the listener binded on the address provided.
        *STARTS A NEW THREAD*
    */
    void Start();
    void Stop();

private:
    std::thread m_thr;
    sockpp::inet_address m_addr;
    sockpp::udp_socket m_sock;
    std::atomic<bool> m_running;

    void listen();
};

} // namespace network
} // namespace dl