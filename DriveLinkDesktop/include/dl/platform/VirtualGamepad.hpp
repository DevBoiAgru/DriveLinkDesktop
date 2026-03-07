#pragma once
#include "dl/core/Constants.hpp"
#include <stdexcept>

namespace dl {

class VirtualGamepad {
public:
    virtual ~VirtualGamepad() = default;

    // Takes the value in as angle in radians, in range -pi/2 to pi/2
    virtual void setSteering(float val) = 0;

    // Takes the value as a float 0-1, 0 being not pressed, 1 being fully pressed
    virtual void setThrottle(float val) = 0;
    // Takes the value as a float 0-1, 0 being not pressed, 1 being fully pressed
    virtual void setBrake(float val) = 0;
    // Takes the value as a float 0-1, 0 being not pressed, 1 being fully pressed
    virtual void setClutch(float val) = 0;

    // Takes the buttons as a bitmask
    virtual void setButtons(uint32_t buttonMask) = 0;

    virtual void update() = 0;

protected:
    long m_steeringAxisMin {}, m_steeringAxisMax {};
    long m_throttleAxisMin {}, m_throttleAxisMax {};
    long m_brakeAxisMin {}, m_brakeAxisMax {};
    long m_clutchAxisMin {}, m_clutchAxisMax {};
};

} // namespace dl