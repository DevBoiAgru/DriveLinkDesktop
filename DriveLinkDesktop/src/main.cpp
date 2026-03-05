#include "dl/app/DriveLink.hpp"
#include "dl/input/GamepadFeeder.hpp"

#ifdef _WIN32
#include "dl/platform/windows/vJoyGamepad.hpp"
#endif // _WIN32

int main() {
    dl::ResourceManager resources;

    // Windows only vJoy gamepad iniialisation
#ifdef _WIN32
    dl::GamepadFeeder feeder(std::make_unique<dl::VJoyGamepad>());
#endif

    try {
        dl::DriveLinkApp app(resources);
        return app.run();
    } catch (const std::exception& e) {
        dl::platform::showMessageBox(nullptr, dl::platform::MessageBoxType::Error, "Error", e.what());
        return -1;
    }
}