#include <cstdio>
#include "sim/SimClock.h"

int main() {
    SimClock clock;
    const uint64_t dtMicros = 2000;   // 2 ms per cycle - 500 Hz

    for (int i = 0; i < 1000; ++i) {
        clock.advance(dtMicros);
    }

    std::printf("t=%.3fs\n", clock.seconds());
    return 0;
}