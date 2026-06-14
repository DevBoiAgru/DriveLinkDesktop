#pragma once

#include <dl/platform/VirtualGamepad.hpp>
#include <linux/uinput.h>

namespace dl {

class LinuxGamepad : public VirtualGamepad<__s32> {
public:
    LinuxGamepad();
    ~LinuxGamepad();

    void setSteering(float val) override;
    void setThrottle(float val) override;
    void setBrake(float val) override;
    void setClutch(float val) override;
    void setButtons(uint32_t buttonMask) override;
    void update() override;

private:
    int m_fd;

    unsigned short m_axes[4] = { ABS_X /* Steering */ , ABS_RZ /* Throttle */, ABS_Z /* Brake */ , ABS_RX /* Clutch */ };
    unsigned short m_buttons[8] = { BTN_TR /* Shift Up */, BTN_TL /* Shift Down */, BTN_A, BTN_B, BTN_X, BTN_Y, BTN_THUMBL, BTN_THUMBR };

    struct input_event m_ev[8 + 4 + 1] = {};     // 8 buttons + 4 axes + 1 sync event

    // Local copies of input values
    float m_steering = 0;
    float m_throttle = 0;
    float m_brake = 0;
    float m_clutch = 0;
    uint32_t m_buttonMask = 0;
};

} // namespace dl
