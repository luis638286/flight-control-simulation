#pragma once
#include <cstdint>
#include "hal/IClock.h"

// simulated time source, advanced manually by the simulation loop,
// so a run is deterministic and not tied toa wall clock speed 
class SimClock : public IClock {
public:
    uint64_t nowMicros() const override { return now_; }

    void advance(uint64_t micros) { now_ += micros; }

    float seconds() const { return static_cast<float>(now_) / 1e6f; }

private:
    uint64_t now_ = 0;
};