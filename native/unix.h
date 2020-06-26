#ifndef UNIX_H
#define UNIX_H

#include <ostream>
#if defined(__unix__) || defined(__unix)
    enum class TextColor {
        Default         = 39,
        black           = 30,
        darkBlue        = 34,
        darkGreen       = 32,
        darkCyan        = 36,
        darkRed         = 31,
        darkMagenta     = 35,
        darkYellow      = 33,
        darkGrey        = 90,
        grey            = 37,
        blue            = 94,
        green           = 92,
        cyan            = 96,
        red             = 91,
        magenta         = 95,
        yellow          = 33,
        white           = 97
    };
    std::ostream& operator<< (std::ostream& stream, TextColor tc) {
        stream << "\e[" + std::to_string((int)tc) + "m";
        return stream;
    }
#endif // __unix__ || __unix
#endif // UNIX_H