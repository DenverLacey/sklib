#include "../fmt.h"

namespace sk { namespace internal {
    void ensure(bool condition, const char* message) {
        if (condition) return;
        sk::eprintln("Error: {}", message);
        __builtin_trap();
    }
}}