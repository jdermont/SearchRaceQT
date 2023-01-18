#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>
#include <cstdlib>

using namespace std;

static const double NORM_DOUBLE = 1.0 / (1ULL << 53);
static const double NORM_FLOAT = 1.0 / (1ULL << 24);

class Random {
public:
    Random() {
        seed = 0xdeadbeefULL * (uint64_t)this + 0xbabef00d;
        seed ^= 0xdeadfacedeadULL * rand() + 0xfbadbeef;
        if (seed == 0ULL) {
            seed = 0xdeadbeefULL;
        }
    }

    Random(uint64_t seed) {
        if (seed == 0ULL) {
            seed = 0xdeadbeefULL;
        }
        this->seed = seed;
    }

    uint64_t nextLong() {
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;
        return seed;
    }

    uint32_t nextInt(int n) {
        return (nextLong() >> 32) % n;
    }

    double nextDouble() {
        return (nextLong() >> 11) * NORM_DOUBLE;
    }

    double nextDouble(double mi, double ma) {
        return mi + nextDouble() * (ma - mi);
    }

    float nextFloat() {
        return (float)((nextLong() >> 40) * NORM_FLOAT);
    }

    float nextFloat(float mi, float ma) {
        return mi + nextFloat() * (ma - mi);
    }

private:
    uint64_t seed;
};

#endif // RANDOM_H
