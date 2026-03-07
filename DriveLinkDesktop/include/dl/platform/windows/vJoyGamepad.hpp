#pragma once

#include "dl/platform/VirtualGamepad.hpp"
#include <Windows.h>

#include "vjoyinterface.h"

namespace dl {

class VJoyGamepad : public VirtualGamepad {
public:
    VJoyGamepad();
    ~VJoyGamepad();

    void setSteering(float val) override;
    void setThrottle(float val) override;
    void setBrake(float val) override;
    void setClutch(float val) override;
    void setButtons(uint32_t buttonMask) override;
    void update() override;

private:
    UINT m_devId = 1;
    JOYSTICK_POSITION_V3 m_report {};
};

} // namespace dl