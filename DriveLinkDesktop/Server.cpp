#include "Server.hpp"

namespace dl {
namespace network {

void InputListener::Start() {
    if (!m_sock.bind(m_addr)) {
        std::cerr << "Error binding the UDP socket." << std::endl;
        return;
    }
    m_running = true;

    m_thr = std::thread([this]() {
        listen();
    });
    m_thr.detach();
}

void InputListener::listen() {
    char buff[30];
    unsigned char version;
    float val_steering;
    float val_throttle;
    float val_brake;
    float val_clutch;
    long long int val_timestamp;
    uint32_t val_buttons;

    while (m_running) {
        std::cout << "Listening";
        size_t read = m_sock.recv(buff, sizeof(buff));

        if (read < 1) {
            // Packet doesn't even have the version? Try the next one
            std::cerr << "Error recving packet." << std::endl;
            continue;
        }

        // Extract version
        memcpy(&version, buff, sizeof(char));

        // Verify payload sizes based on versions
        if (version == 1 && read != 29) {
            // Skip packet, enough data didn't arrive
            continue;
        }

        // Important! We recieve data in Little Endian format
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
            dl::InputState::GetInstance().setButtons(val_buttons);
        }
    }
}

void InputListener::Stop() {
    m_running = false;
    m_thr.join();
}

} // namespace network
} // namespace dl