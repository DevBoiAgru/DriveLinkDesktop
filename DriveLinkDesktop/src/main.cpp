#include "dl/app/DriveLink.hpp"

int main() {

    dl::ResourceManager resources;

    try {
        dl::DriveLinkApp app(resources);
        return app.run();
    } catch (const std::exception& e) {
        dl::platform::showMessageBox(nullptr, dl::platform::MessageBoxType::Error, "Error", e.what());
        return -1;
    }
}