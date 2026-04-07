#include <dl/platform/linux/linuxGamepad.hpp>
#include <dl/utility/Logger.hpp>

namespace dl {

LinuxGamepad::LinuxGamepad() {
    utils::Logger& logger = utils::Logger::GetLogger();

}

LinuxGamepad::~LinuxGamepad() {
    
}

void LinuxGamepad::setThrottle(float val) {
}

void LinuxGamepad::setBrake(float val) {
}

void LinuxGamepad::setClutch(float val) {
}

void LinuxGamepad::setSteering(float val) {
}

void LinuxGamepad::setButtons(uint32_t buttonMask) {
}

void LinuxGamepad::update() {
}

} // namespace dl
