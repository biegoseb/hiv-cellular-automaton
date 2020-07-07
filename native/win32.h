#ifndef WIN32_H
#define WIN32_H

#include <ostream>

#ifdef _WIN32
    #include "windows.h"
    enum class TextColor {
        Default           = -1,
        Black             = 0,
        DarkBlue          = FOREGROUND_BLUE,
        DarkGreen         = FOREGROUND_GREEN,
        DarkCyan          = FOREGROUND_GREEN     | FOREGROUND_BLUE,
        DarkRed           = FOREGROUND_RED,
        DarkMagenta       = FOREGROUND_RED       | FOREGROUND_BLUE,
        DarkYellow        = FOREGROUND_RED       | FOREGROUND_GREEN,
        DarkGrey          = FOREGROUND_RED       | FOREGROUND_GREEN  | FOREGROUND_BLUE,
        Grey              = FOREGROUND_INTENSITY,
        Blue              = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
        Green             = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
        Cyan              = FOREGROUND_INTENSITY | FOREGROUND_GREEN  | FOREGROUND_BLUE,
        Red               = FOREGROUND_INTENSITY | FOREGROUND_RED,
        Magenta           = FOREGROUND_INTENSITY | FOREGROUND_RED    | FOREGROUND_BLUE,
        Yellow            = FOREGROUND_INTENSITY | FOREGROUND_RED    | FOREGROUND_GREEN,
        White             = FOREGROUND_INTENSITY | FOREGROUND_RED    | FOREGROUND_GREEN | FOREGROUND_BLUE,
    };
    namespace {
        WORD wdefault;
        bool first = true;
    }
    std::ostream& operator<< (std::ostream& stream, TextColor tc) {
        if (first) {
            first = false;
            CONSOLE_SCREEN_BUFFER_INFO Info;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
            wdefault = Info.wAttributes;
        }
        if (tc == TextColor::Default)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wdefault);
        else
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)tc);
        return stream;
    }
#endif // _WIN32
#endif // WIN32_H