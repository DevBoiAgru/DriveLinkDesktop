#include "dl/app/DriveLink.hpp"
#include "dl/input/GamepadFeeder.hpp"

#ifdef _WIN32
#include "dl/platform/windows/vJoyGamepad.hpp"
#endif // _WIN32

#if defined(_WIN32) && !defined(_DEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main() {
#endif

    dl::ResourceManager resources;

    // Windows only vJoy gamepad initialisation
#ifdef _WIN32
    dl::GamepadFeeder feeder(std::make_unique<dl::VJoyGamepad>());
#endif

    try {
        dl::DriveLinkApp app(resources);
        return app.run();
    } catch (const std::exception& e) {
        dl::platform::showMessageBox(nullptr, dl::platform::MessageBoxType::Error, "Error", e.what());
        return -1;
    } catch (...) {
        dl::platform::showMessageBox(nullptr, dl::platform::MessageBoxType::Error, "Error", "Unknown error");
        return -1;
    }
}