#pragma once

#include "formatter.h"
#include "writer.h"

namespace sk {
    template<typename T>
    struct Formatter<std::vector<T>> {
        static void format(const std::vector<T>& ts, std::string_view fmt, Writer& writer) {
            auto format = Format::from(fmt);

            if (format.alternate) {
                writer.write("{\n");
            } else {
                writer.write("{ ");
            }

            for (size_t i = 0; i < ts.size(); i++) {
                auto& t = ts[i];

                if (format.alternate) {
                    writer.print("\t{:#}", t);
                    writer.write(",\n");
                } else {
                    writer.print("{}", t);
                    if (i + 1 < ts.size()) writer.write(", ");
                }
            }

            if (format.alternate) {
                writer.write('}');
            } else {
                writer.write(" }");
            }
        }
    };
}
