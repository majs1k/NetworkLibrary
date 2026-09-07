#include "DummyTester.h"

#include <iostream>

bool DummyTester::Start(
    int dummyCount,
    const std::wstring& serverIp,
    int serverPort)
{
    if (dummyCount <= 0)
        return false;

    clients_.reserve(dummyCount);

    for (int i = 0;
        i < dummyCount;
        ++i)
    {
        auto client =
            std::make_unique<DummyClient>(i);

        if (!client->Start(
            serverIp,
            serverPort))
        {
            std::cout
                << "[Tester] Dummy "
                << i
                << " start failed"
                << std::endl;

            continue;
        }

        clients_.push_back(
            std::move(client));
    }

    return true;
}

void DummyTester::Update()
{
    for (auto& client : clients_)
    {
        client->Update();
    }

    Sleep(500);
}

bool DummyTester::IsFinished() const
{
    for (const auto& client : clients_)
    {
        if (!client->IsFinished())
            return false;
    }

    return true;
}
