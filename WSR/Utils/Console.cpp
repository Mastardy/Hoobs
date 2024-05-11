#include "Console.hpp"
#include <Windows.h>

namespace WSR
{
    Console::Console()
    {
        AllocConsole();
        (void)freopen_s(&m_Console, "CONOUT$", "w", stdout);
        SetConsoleTitle(L"WSR Console");
    }

    Console::~Console()
    {
        FreeConsole();
        (void)fclose(m_Console);
    }
}