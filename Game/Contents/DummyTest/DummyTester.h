#pragma once

#include "DummyClient.h"

#include <memory>
#include <vector>

class DummyTester
{
public:
    DummyTester() = default;
    ~DummyTester() = default;

public:
    bool Start(
        int dummyCount,
        const std::wstring& serverIp,
        int serverPort);

    void Update();

    bool IsFinished() const;

private:
    std::vector<std::unique_ptr<DummyClient>> clients_;
};
