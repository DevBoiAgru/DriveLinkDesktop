#pragma once
#include <array>
#include <atomic>

namespace dl {

struct {
    float steering;
    float throttle;
    float brake;
    float clutch;
    long long int timestamp;
    unsigned buttons;
} InputData;

class InputState {
public:
    static InputState& GetInstance() {
        static InputState inst;
        return inst;
    }

    // Setters
    void setSteering(float val) {
        m_steering.store(val, std::memory_order_relaxed);
    }

    void setThrottle(float val) {
        m_throttle.store(val, std::memory_order_relaxed);
    }

    void setBrake(float val) {
        m_brake.store(val, std::memory_order_relaxed);
    }

    void setClutch(float val) {
        m_clutch.store(val, std::memory_order_relaxed);
    }

    void setTimestamp(long long int val) {
        m_timestamp.store(val, std::memory_order_relaxed);
    }

    void setButtons(uint32_t val) {
        m_buttons.store(val, std::memory_order_relaxed);
    }

    // Getters
    float getSteering() {
        return m_steering.load(std::memory_order_relaxed);
    }

    float getThrottle() {
        return m_throttle.load(std::memory_order_relaxed);
    }

    float getBrake() {
        return m_brake.load(std::memory_order_relaxed);
    }

    float getClutch() {
        return m_clutch.load(std::memory_order_relaxed);
    }

    long long int getTimestamp() {
        return m_timestamp.load(std::memory_order_relaxed);
    }

    uint32_t getButtons() {
        return m_buttons.load(std::memory_order_relaxed);
    }

private:
    InputState() = default; // no external construction
    InputState(const InputState&) = delete;
    InputState& operator=(const InputState&) = delete;

    std::atomic<float> m_steering;
    std::atomic<float> m_throttle;
    std::atomic<float> m_brake;
    std::atomic<float> m_clutch;
    std::atomic<uint32_t> m_buttons;
    std::atomic<long long int> m_timestamp;
};

} // namespace dl