#include "dl/platform/windows/vJoyGamepad.hpp"
#include "dl/utility/Logger.hpp"

namespace dl {
namespace {

// Maps [-1,1] range to the min and max provided. Doesn't clamp the input.
static LONG mapAngleToAxis(float value, LONG axisMin, LONG axisMax) {
    // map [-1,1] → [0,1]
    float t = (value + 1.f) * 0.5f;

    return axisMin + static_cast<LONG>((axisMax - axisMin) * t);
}
} // namespace

VJoyGamepad::VJoyGamepad() {
    utils::Logger& logger = utils::Logger::GetLogger();

    if (!vJoyEnabled()) {
        logger.error("vJoy not enabled. Perhaps vJoy is not installed?");
        throw std::runtime_error("vJoy driver is required for this application to work. Install it if it isn't installed yet.");
    }

    VjdStat deviceStatus = GetVJDStatus(m_devId);

    if (deviceStatus != VJD_STAT_FREE) {
        logger.error("vJoy device already in use. Make sure there are no other instances of the app running in the background.");
        throw std::runtime_error("vJoy device is in use. Make sure there are no other instances of the application running.");
    }

    if (!AcquireVJD(m_devId)) {
        logger.error("Failed to acquire vJoy device.");
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

void VJoyGamepad::setButtons(uint32_t buttonMask) {
    m_report.lButtons = buttonMask;
}

void VJoyGamepad::update() {
    UpdateVJD(m_devId, &m_report);
}

} // namespace dl
