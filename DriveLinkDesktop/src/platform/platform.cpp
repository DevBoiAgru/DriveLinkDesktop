#include "platform/platform.hpp"

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
    // Linux version not implemented!!
}
} // namespace platform
} // namespace dl