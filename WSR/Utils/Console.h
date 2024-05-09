#pragma once

#include <cstdio>

namespace WSR
{
    class Console
    {
    public:
        Console();
        ~Console();
        
        Console(const Console&) = delete;
        Console(Console&&) = delete;
        Console operator=(const Console&) = delete;
        Console operator=(Console&&) = delete;

    private:
        FILE* m_Console;
    };
}