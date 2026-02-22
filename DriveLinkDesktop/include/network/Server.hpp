#pragma once

#include "InputState.hpp"
#include "Protocols.hpp"

#include <atomic>
#include <SFML/Network/UdpSocket.hpp>
#include <thread>
#include <variant>

namespace dl {
namespace network {

class InputListener {
public:
    InputListener(int port)
        : m_port(port) { };

    /*
        Run the listener binded on the address provided.
        *STARTS A NEW THREAD*
    */
    void Start();
    void Stop();

private:
    int m_port;
    std::thread m_thr;
    sf::UdpSocket m_socket;
    std::atomic<bool> m_running;

    void listen();
};

} // namespace network
} // namespace dl