#pragma once
#include <cstdint>

// monotonic time source; 
// in a simulation: a counter advanced manually by the control loop
// in real hardware: a hardware timer 
//
// contract: returns microseconds since start, monotonic means it never decreases
class IClock {
public:
    virtual ~IClock() = default;

    virtual uint64_t nowMicros() const = 0;
};