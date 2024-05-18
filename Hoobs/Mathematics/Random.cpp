#include "Random.hpp"

namespace WSR
{
    std::map<int, std::mt19937_64> Random::generators{{0, std::mt19937_64()}};
    int Random::currentSeed = 0;

    void Random::Seed(int seed)
    {
        currentSeed = seed;
        if(!generators.contains(seed)) generators[seed] = std::mt19937_64(seed);
    }

    float Random::GetValue(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generators[currentSeed]);
    }

    float Random::GetValue(float min, int max)
    {
        std::uniform_real_distribution<float> distribution(min, static_cast<float>(max));
        return distribution(generators[currentSeed]);
    }

    float Random::GetValue(int min, float max)
    {
        std::uniform_real_distribution<float> distribution(static_cast<float>(min), max);
        return distribution(generators[currentSeed]);
    }

    int Random::GetValue(int min, int max)
    {
        std::uniform_int_distribution distribution(min, max);
        return distribution(generators[currentSeed]);
    }
}