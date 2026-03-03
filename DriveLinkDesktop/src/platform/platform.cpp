#include "dl/platform/Platform.hpp"
// Linux versions not implemented!!

#ifdef _WIN32
#include <windows.h>
#endif

namespace dl {
namespace platform {
void minimiseWindow(sf::Window& window) {
    auto windowHandle = window.getNativeHandle();

#ifdef _WIN32
    ::ShowWindow(windowHandle, SW_MINIMIZE);
#endif
}

MessageBoxAction showMessageBox(sf::Window* window, MessageBoxType type, sf::String title, sf::String message) {

    auto windowHandle = window != nullptr ? window->getNativeHandle() : NULL;

#ifdef _WIN32
    UINT flags = MB_OK;

    switch (type) {
    case Info:
        flags |= MB_ICONINFORMATION;
        break;
    case Warning:
        flags |= MB_ICONWARNING;
        break;
    case Error:
        flags |= MB_ICONERROR;
        break;
    case YesNo:
        flags |= MB_ICONWARNING;
        flags |= MB_YESNO;
        break;
    }

    switch (::MessageBoxW(windowHandle, message.toWideString().c_str(), title.toWideString().c_str(), flags)) {
    case IDOK:
        return Ok;
    case IDYES:
        return Yes;
    case IDNO:
        return No;
    case IDCANCEL:
        return Cancel;
    }

#endif

    return Ok;
}

} // namespace platform
} // namespace dl