#include "../../fmt.h"

namespace sk {
    void print(const char* fmt, Args args) {
        out.print(fmt, args);
    }

    void println(const char* fmt, Args args) {
        out.println(fmt, args);
    }

    void eprint(const char* fmt, Args args) {
        err.print(fmt, args);
    }

    void eprintln(const char* fmt, Args args) {
        err.println(fmt, args);
    }

    std::string format(const char* fmt, Args args) {
        std::stringstream ss;
        auto writer = Writer{ ss };
        writer.print(fmt, args);
        return ss.str();
    }
}
