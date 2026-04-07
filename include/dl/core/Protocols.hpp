#pragma once
#include <cstdint>

/*
    This file contains all the definitions and the structures of the payloads the client
    sends to the server based on the protocol version, this way we can switch to different
    types of payloads. For example if in the future there is another pedal, more buttons
    etc, we can easily switch to that type of input payload.

    All the data recieved from the client MUST have the first byte as the version.
*/

namespace dl {
namespace network {
namespace protocols {

typedef struct {
    float steering;
    float throttle;
    float brake;
    float clutch;
    long long timestamp;
    uint32_t buttons; // Bit map representing buttons 0: Unpressed, 1: Pressed
} V1;

} // namespace protocols
} // namespace network
} // namespace dl