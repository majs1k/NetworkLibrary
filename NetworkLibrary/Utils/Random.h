#pragma once
#include <random>
#include "Singleton.h"

#define RAND(minValue, maxValue)          RandomMT::Instance().RandomRange(minValue, maxValue)

class RandomMT : public Singleton<RandomMT>
{
public:
    RandomMT()
        : engine_(rd()) { }

    int RandomRange(int minValue, int maxValue)
    {
        std::uniform_int_distribution<> dist(minValue, maxValue);
        return dist(engine_);
    }

private:
    std::random_device rd;
    std::mt19937 engine_;
};
