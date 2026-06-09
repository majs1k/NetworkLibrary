#pragma once
#include <random>
#include "Singleton.h"

#define RAND(minValue, maxValue)          RandomMT::getInstance().rand(minValue, maxValue)

class RandomMT : public Singleton<RandomMT>
{
public:
    RandomMT()
        : engine_(rd()) { }

    int rand(int minValue, int maxValue)
    {
        std::uniform_int_distribution<> dist(minValue, maxValue);
        return dist(engine_);
    }

private:
    std::random_device rd;
    std::mt19937 engine_;
};
