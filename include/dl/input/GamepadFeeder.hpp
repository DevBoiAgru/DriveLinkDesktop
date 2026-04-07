#pragma once
#include <dl/core/InputState.hpp>
#include <dl/platform/VirtualGamepad.hpp>
#include <thread>

namespace dl {
template <typename AxisResT>
class GamepadFeeder {
public:
    GamepadFeeder(std::unique_ptr<VirtualGamepad<AxisResT>> gamepad);
    ~GamepadFeeder();

private:
    void updateLoop();
    std::unique_ptr<VirtualGamepad<AxisResT>> m_gamepad;
    std::thread m_thr;
    std::atomic<bool> m_running;
};
} // namespace dl
