#include "../optional.h"

#include <stdlib.h>

namespace sk {
    void Formatter<internal::TNone>::format(const internal::TNone&, std::string_view, Writer& writer) {
        writer.write_string("None");
    }
}
