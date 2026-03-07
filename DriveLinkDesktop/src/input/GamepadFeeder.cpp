#include "dl/input/GamepadFeeder.hpp"

namespace dl {
using namespace std::chrono_literals;

GamepadFeeder::GamepadFeeder(std::unique_ptr<VirtualGamepad> gamepad)
    : m_gamepad(std::move(gamepad)) {

    if (m_running.load())
        return;

    m_running.store(true);
    m_thr = std::thread([this]() { updateLoop(); });
}

GamepadFeeder::~GamepadFeeder() {
    m_running.store(false);
    if (m_thr.joinable()) {
        m_thr.join();
    }
}

void GamepadFeeder::updateLoop() {
    dl::InputState& inputState = dl::InputState::GetInstance();

    while (m_running.load()) {
        m_gamepad->setBrake(inputState.getBrake());
        m_gamepad->setClutch(inputState.getClutch());
        m_gamepad->setThrottle(inputState.getThrottle());
        m_gamepad->setSteering(inputState.getSteering());
        m_gamepad->setButtons(inputState.getButtons());

        m_gamepad->update();
        std::this_thread::sleep_for(10ms);
    }
}

} // namespace dl