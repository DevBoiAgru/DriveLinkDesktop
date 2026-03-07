# DriveLink Desktop - Technical Summary

## Core Tech Stack
- **Languages**: C++20 
- **UI Framework**: SFML 3.0
- **Virtual Gamepad**: vJoy SDK (Windows-only).
- **Build System**: Visual Studio 2022.
- **Dependency Management**: vcpkg (`sfml`).

## Networking (UDP)
- **Protocol**: UDP.
- **Default Port**: `7001`.
- **Packet Structure (V1)**:
    - `Offset 0 [1 byte]`: Version (currently `1`).
    - `Offset 1 [4 bytes]`: Steering (`float`, radians, -π/2 to π/2).
    - `Offset 5 [4 bytes]`: Throttle (`float`, 0.0 to 1.0).
    - `Offset 9 [4 bytes]`: Brake (`float`, 0.0 to 1.0).
    - `Offset 13 [4 bytes]`: Clutch (`float`, 0.0 to 1.0).
    - `Offset 17 [4 bytes]`: Buttons (`uint32_t` bitmask).
    - `Offset 21 [8 bytes]`: Timestamp (`long long`).
    - **Total Size**: 29 bytes.

## Input Mapping (Phone -> Gamepad)
| Input Type | Source (UDP) | Virtual Gamepad Attribute | Scaling / Range |
| :--- | :--- | :--- | :--- |
| Steering | `float` (radians) | `wAxisX` | `-π/2` to `π/2` -> Virtual Gamepad X Axis Range |
| Throttle | `float` (0-1) | `wAxisY` | `0.0` to `1.0` -> Virtual Gamepad Y Axis Range |
| Brake | `float` (0-1) | `wAxisZ` | `0.0` to `1.0` -> Virtual Gamepad Z Axis Range |
| Clutch | `float` (0-1) | `wAxisXRot` | `0.0` to `1.0` -> Virtual Gamepad RX Axis Range |
| Buttons | `uint32_t` (mask) | `lButtons` | Direct bitmask mapped to 32 buttons |

## Build Requirements
- **Compiler**: MSVC (Visual Studio 2022).
- **Dependencies**: 
    - SFML 3.0 (via vcpkg).
    - vJoy SDK (included in `thirdparty/vJoy` or linked via system).
- **Runtime**: Visual C++ Redistributable 2022, vJoy Driver.

## Code Structure
- `dl::network::InputListener`: Handles UDP reception in a dedicated thread.
- `dl::InputState`: Thread-safe Singleton storing current controller state.
- `dl::GamepadFeeder`: Bridges `InputState` to `VirtualGamepad` interface at ~100Hz (10ms sleep).
- `dl::VJoyGamepad`: Windows implementation of `VirtualGamepad` using vJoy SDK.
- `dl::DriveLinkApp`: Main SFML application for UI and status monitoring.


**AI Disclaimer**: This technical summary was made with the help of Google Antigravity, which was used for analysing the entire codebase at once. The output was manually heavily modified to be more concise and readable.