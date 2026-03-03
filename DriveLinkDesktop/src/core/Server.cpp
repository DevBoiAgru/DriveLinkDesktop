#include "dl/core/Server.hpp"

#include "dl/platform/Platform.hpp"

#include <iostream>

namespace dl {
namespace network {

InputListener::~InputListener() {
    Stop();
}

InputListener::InputListener(int port) : m_port(port), m_running(false) {}

void InputListener::Start() {
    if (m_running.load()) return;

    auto res = m_socket.bind(
        m_port,
        sf::IpAddress::resolve("0.0.0.0").value()
    );

    switch (res) {
    case sf::Socket::Status::Error:
        std::cerr << "Error binding the UDP socket." << std::endl;
    }

    m_running.store(true);

    m_thr = std::thread([this]() {
        listen();
    });
}

void InputListener::listen() {
    char buff[100];
    std::size_t recieved;
    std::optional<sf::IpAddress> sender;
    unsigned short remotePort;

    unsigned char version;
    float val_steering;
    float val_throttle;
    float val_brake;
    float val_clutch;
    long long int val_timestamp;
    uint32_t val_buttons;

    while (m_running.load()) {

        if (m_socket.receive(buff, sizeof(buff), recieved, sender, remotePort) != sf::Socket::Status::Done) {
            // Try again on next packet
            continue;
        }

        if (recieved < 1) {
            // Packet doesn't even have the version? Skip
            continue;
        }

        // Extract version
        memcpy(&version, buff, sizeof(char));

        // Verify payload sizes based on versions
        if (version == 1 && recieved != 29) {
            // Skip packet, enough data didn't arrive
            continue;
        }

        // We recieve data in Little Endian format, here, we assume the system uses little endian
        switch (version) {
        case 1:
            // We recieve 29 bytes.
            // 1 byte version + 4 * 4 byte floats + 8 byte long long int + 4 byte int
            memcpy(&val_steering, buff + sizeof(char), sizeof(float));
            memcpy(&val_throttle, buff + sizeof(char) + sizeof(float), sizeof(float));
            memcpy(&val_brake, buff + sizeof(char) + 2 * sizeof(float), sizeof(float));
            memcpy(&val_clutch, buff + sizeof(char) + 3 * sizeof(float), sizeof(float));
            memcpy(&val_timestamp, buff + sizeof(char) + 4 * sizeof(float), sizeof(long long int));
            memcpy(&val_buttons, buff + sizeof(char) + 4 * sizeof(float) + sizeof(long long int), sizeof(uint32_t));

            // Update values in the input state
            dl::InputState::GetInstance().setSteering(val_steering);
            dl::InputState::GetInstance().setThrottle(val_throttle);
            dl::InputState::GetInstance().setBrake(val_brake);
            dl::InputState::GetInstance().setClutch(val_clutch);
            dl::InputState::GetInstance().setTimestamp(val_timestamp);
            dl::InputState::GetInstance().setButtons(val_buttons);
        }
    }
}

void InputListener::Stop() {
    m_socket.unbind();
    m_running.store(false);
    if (m_thr.joinable()) {
        m_thr.join();
    }
}

} // namespace network
} // namespace dl