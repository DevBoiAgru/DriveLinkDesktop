#include <dl/app/DriveLink.hpp>
#include <dl/input/GamepadFeeder.hpp>
#include <dl/utility/Logger.hpp>

#ifdef _WIN32
#include <dl/platform/windows/vJoyGamepad.hpp>
#else
#include <dl/platform/linux/linuxGamepad.hpp>
#endif // _WIN32

#if defined(_WIN32) && !defined(_DEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main() {
#endif

    // Start main

    // Initialise logger
    dl::utils::Logger& logger = dl::utils::Logger::GetLogger();

    // Initialise resource manager
    logger.info("Initialising resource manager");
    dl::ResourceManager resources;

    // Windows only vJoy gamepad initialisation
#ifdef _WIN32
    logger.info("Creating Windows vJoy Gamepad");
    dl::GamepadFeeder<long> feeder(std::make_unique<dl::VJoyGamepad>());
#else
    logger.info("Creating Linux Gamepad");
    dl::GamepadFeeder<__s32> feeder(std::make_unique<dl::LinuxGamepad>());
#endif

    try {
        dl::DriveLinkApp app(resources);
        logger.info("App running");
        return app.run();
    } catch (const std::exception& e) {
        logger.flushLog();
        dl::platform::showMessageBox(nullptr, dl::platform::MessageBoxType::Error, "Error", e.what());
        return -1;
    } catch (...) {
        logger.flushLog();
        dl::platform::showMessageBox(nullptr, dl::platform::MessageBoxType::Error, "Error", "Unknown error");
        return -1;
    }
}