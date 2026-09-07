#include "DummyTester.h"

#include <iostream>
#include <Windows.h>

int main()
{
    int dummyCount;

    std::cout
        << "Dummy Count : ";

    std::cin
        >> dummyCount;

    DummyTester tester;

    if (!tester.Start(
        dummyCount,
        L"127.0.0.1",
        7777))
    {
        std::cout
            << "Failed to start tester."
            << std::endl;

        return 1;
    }

    while (true)
    {
        tester.Update();

        Sleep(1);
    }

    return 0;
}