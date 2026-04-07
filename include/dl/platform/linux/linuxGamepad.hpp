#pragma once

#include <dl/platform/VirtualGamepad.hpp>


namespace dl {

class LinuxGamepad : public VirtualGamepad {
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
};

} // namespace dl