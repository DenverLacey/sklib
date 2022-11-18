#include "../optional.h"

#include <stdlib.h>

namespace sk {
    void internal::panic_if_none(bool has_value, const char* message) noexcept {
        if (has_value) return;
        sk::eprintln("Error: {}", message);
        exit(EXIT_FAILURE);
    }

    void Formatter<internal::TNone>::format(const internal::TNone&, std::string_view, Writer& writer) {
        writer.write("None");
    }
}
