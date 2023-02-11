#pragma once

#include <stddef.h>

#include "../array.h"
#include "../optional.h"
#include "../mem/allocator.h"

namespace sk {
    using Pixel = uint32_t;

    struct NormalizedRect {
        // Safe ranges to iterate over.
        int x1, x2;
        int y1, y2;

        // Original uncut ranges some parts of which may be outside of the canvas boundaries.
        int ox1, ox2;
        int oy1, oy2;

        // === Constructors ===
        static Optional<NormalizedRect> make(int x, int y, int w, int h, size_t canvas_width, size_t canvas_height);
    };

    struct Canvas {
        // === Data ===
        size_t width, height, stride;
        Array<Pixel> pixels;

        // === Constructors ===
        Canvas() = default;
        Canvas(const Canvas&) = default;
        Canvas(Canvas&&) = default;

        Canvas(size_t width, size_t height, size_t stride, Array<Pixel> pixels) noexcept;

        static Canvas make(Allocator& ator, size_t width, size_t height) noexcept;

        // === Destructors ===
        void destroy(Allocator& ator) noexcept;

        // === Associated Functions ===
        Optional<Canvas> subcanvas(ssize_t x, ssize_t y, size_t width, size_t height) noexcept;
        Pixel& pixel(size_t x, size_t y) noexcept;
        size_t pixel_index(size_t x, size_t y) noexcept;
    };
}
