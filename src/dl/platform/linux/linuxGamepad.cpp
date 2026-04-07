#include <dl/platform/linux/linuxGamepad.hpp>
#include <dl/utility/Logger.hpp>
#include <fcntl.h>
#include <iostream>
#include <bitset>
#include <cmath>


namespace dl {

namespace {
__s32 mapAxis(__s32 min, __s32 max, float t) {
    // Maps 0 to 1 to axisMin-axisMax
    return static_cast<__s32>(
        std::lround(
            static_cast<float>(min) +
            (static_cast<float>(max - min) * t)
        )
    );
}

__s32 mapAngleToAxis(float value, __s32 axisMin, __s32 axisMax) {
    // Maps -1 to 1 to axisMin-axisMax

    float halfRange = static_cast<float>(axisMax - axisMin) * 0.5f;
    float center    = static_cast<float>(axisMax + axisMin) * 0.5f;

    return static_cast<__s32>(
        std::lround(center + value * halfRange)
    );
}
}

LinuxGamepad::LinuxGamepad() {
    utils::Logger& logger = utils::Logger::GetLogger();

    logger.info("Opening /dev/uinput file descriptor.");
    m_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    logger.info("/dev/uinput file descriptor opened successfully.");

    if (m_fd < 0) {
        logger.error("Unable to open /dev/uinput. Try running the program as sudo.");
        throw std::runtime_error("Unable to open /dev/uinput. Try running the program as sudo.");
    }

    // Set up the buttons
    ioctl(m_fd, UI_SET_EVBIT, EV_KEY);
    for (const auto button: m_buttons) {
        ioctl(m_fd, UI_SET_KEYBIT, button);
    }

    // Set up the axes
    ioctl(m_fd, UI_SET_EVBIT, EV_ABS);
    for (const auto axis: m_axes) {
        // Enable the axis
        ioctl(m_fd, UI_SET_ABSBIT, axis);

        // Tell linux stuff about the axis
        struct uinput_abs_setup abs = { };
        abs.code = axis;
        abs.absinfo.flat = 0;
        abs.absinfo.fuzz = 0;

        switch (axis) {
        case ABS_X:
            abs.absinfo.minimum = m_throttleAxisMin;
            abs.absinfo.maximum = m_throttleAxisMax;
            break;
        case ABS_Y:
            abs.absinfo.minimum = m_brakeAxisMin;
            abs.absinfo.maximum = m_brakeAxisMax;
            break;
        case ABS_Z:
            abs.absinfo.minimum = m_clutchAxisMin;
            abs.absinfo.maximum = m_clutchAxisMax;
            break;
        case ABS_RX:
            abs.absinfo.minimum = m_steeringAxisMin;
            abs.absinfo.maximum = m_steeringAxisMax;
            break;
        default:
            break;
        }

        ioctl(m_fd, UI_ABS_SETUP, &abs);
    }

    m_throttleAxisMax = 32767;
    m_brakeAxisMax = 32767;
    m_clutchAxisMax = 32767;
    m_steeringAxisMax = 32767;

    m_throttleAxisMin = -32768;
    m_brakeAxisMin = -32768;
    m_clutchAxisMin = -32768;
    m_steeringAxisMin = -32768;


    struct uinput_setup device_setup =
    {
        .id = {
            .bustype = BUS_USB,
            .vendor = 0x3,
            .product = 0x3,
            .version = 2,
        },
        .name = "Drivelink Userspace Joystick"
    };

    if (ioctl(m_fd, UI_DEV_SETUP, &device_setup)) {
        logger.error("Error while setting up the linux joystick device.");
        throw std::runtime_error("Error while setting up the linux joystick device.");
    }

    if (ioctl(m_fd, UI_DEV_CREATE)) {
        logger.error("Error while creating the linux joystick device.");
        throw std::runtime_error("Error while creating the linux joystick device.");
    }
}

LinuxGamepad::~LinuxGamepad() {
    close(m_fd);
}

void LinuxGamepad::setThrottle(float val) {
    m_throttle = val;
}

void LinuxGamepad::setBrake(float val) {
    m_brake = val;
}

void LinuxGamepad::setClutch(float val) {
    m_clutch = val;
}

void LinuxGamepad::setSteering(float val) {
    m_steering = val;
}

void LinuxGamepad::setButtons(uint32_t buttonMask) {
    m_buttonMask = buttonMask;
}

void LinuxGamepad::update() {
    utils::Logger& logger = utils::Logger::GetLogger();

    // Update all the buttons
    for (int i = 0; i < 8; i++) {
        m_ev[i].type = EV_KEY;
        m_ev[i].code = m_buttons[i];
        m_ev[i].value = static_cast<__s32>((m_buttonMask >> i) & 1u);
    }

    // Update all the axes
    for (int i = 8; i < 12; i++) {
        m_ev[i].type = EV_ABS;
        m_ev[i].code = m_axes[i - 8];
    }

    m_ev[8].value = mapAxis(m_throttleAxisMin, m_throttleAxisMax, m_throttle);
    m_ev[9].value = mapAxis(m_brakeAxisMin, m_brakeAxisMax, m_brake);
    m_ev[10].value = mapAxis(m_clutchAxisMin, m_clutchAxisMax, m_clutch);
    m_ev[11].value = mapAngleToAxis(m_steering, m_steeringAxisMin, m_steeringAxisMax);

    m_ev[12].type = EV_SYN;
    m_ev[12].code = SYN_REPORT;
    m_ev[12].value = 0;

    // Send the state
    if(write(m_fd, &m_ev, sizeof(m_ev)) < 0) {
        logger.error("Failed to write input state");
        throw std::runtime_error("Failed to write input state");
    }

}

} // namespace dl
