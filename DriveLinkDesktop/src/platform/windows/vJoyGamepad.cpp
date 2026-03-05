#include "dl/platform/windows/vJoyGamepad.hpp"

namespace dl {
namespace {
static LONG mapAngleToAxis(float angle, LONG axisMin, LONG axisMax) {
    float t = (angle + static_cast<float>(dl::consts::M_PI_2)) / static_cast<float>(dl::consts::M_PI);

    // clamp just in case
    if (t < 0.f)
        t = 0.f;
    if (t > 1.f)
        t = 1.f;

    return axisMin + static_cast<LONG>((axisMax - axisMin) * t);
}
} // namespace

VJoyGamepad::VJoyGamepad() {

    if (!vJoyEnabled()) {
        throw std::runtime_error("vJoy driver is required for this application to work. Install it if it isn't installed yet.");
    }

    VjdStat deviceStatus = GetVJDStatus(m_devId);

    if (deviceStatus != VJD_STAT_FREE) {
        throw std::runtime_error("vJoy device is in use. Make sure there are no other instances of the application running.");
    }

    if (!AcquireVJD(m_devId)) {
        throw std::runtime_error("Failed to acquire vJoy device");
    }
    m_report.bDevice = m_devId;

    GetVJDAxisMax(m_devId, HID_USAGE_X, &m_steeringAxisMax);
    GetVJDAxisMin(m_devId, HID_USAGE_X, &m_steeringAxisMin);
    GetVJDAxisMax(m_devId, HID_USAGE_Y, &m_throttleAxisMax);
    GetVJDAxisMin(m_devId, HID_USAGE_Y, &m_throttleAxisMin);
    GetVJDAxisMax(m_devId, HID_USAGE_Z, &m_brakeAxisMax);
    GetVJDAxisMin(m_devId, HID_USAGE_Z, &m_brakeAxisMin);
    GetVJDAxisMax(m_devId, HID_USAGE_RX, &m_clutchAxisMax);
    GetVJDAxisMin(m_devId, HID_USAGE_RX, &m_clutchAxisMin);
}

VJoyGamepad::~VJoyGamepad() {
    RelinquishVJD(m_devId);
}

void VJoyGamepad::setThrottle(float val) {
    m_report.wAxisY = m_throttleAxisMin + static_cast<LONG>((m_throttleAxisMax - m_throttleAxisMin) * val);
}

void VJoyGamepad::setBrake(float val) {
    m_report.wAxisZ = m_brakeAxisMin + static_cast<LONG>((m_brakeAxisMax - m_brakeAxisMin) * val);
}

void VJoyGamepad::setClutch(float val) {
    m_report.wAxisXRot = m_clutchAxisMin + static_cast<LONG>((m_clutchAxisMax - m_clutchAxisMin) * val);
}

void VJoyGamepad::setSteering(float val) {
    m_report.wAxisX = mapAngleToAxis(val, m_steeringAxisMin, m_steeringAxisMax);
}

void VJoyGamepad::update() {
    UpdateVJD(m_devId, &m_report);
}

} // namespace dl
