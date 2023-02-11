#include "../canvas.h"

#define SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define ABS(T, x) (SIGN(T, x)*(x))

namespace sk {
    Optional<NormalizedRect> NormalizedRect::make(int x, int y, int w, int h, size_t canvas_width, size_t canvas_height) {
        NormalizedRect rect;

        // no need to to render empty rectangle
        if (w == 0) return None;
        if (h == 0) return None;

        int cw = static_cast<int>(canvas_width);
        int ch = static_cast<int>(canvas_height);

        rect.ox1 = x;
        rect.oy1 = y;

        // TODO: convert the rectangle to 2-points representation
        rect.ox2 = rect.ox1 + SIGN(int, w)*(ABS(int, w) - 1);
        if (rect.ox1 > rect.ox2) SWAP(int, rect.ox1, rect.ox2);
        rect.oy2 = rect.oy1 + SIGN(int, h)*(ABS(int, h) - 1);
        if (rect.oy1 > rect.oy2) SWAP(int, rect.oy1, rect.oy2);  

        // cull out invisible rectangle
        if (rect.ox1 >= cw) return None;
        if (rect.ox2 < 0) return None;
        if (rect.oy1 >= ch) return None;
        if (rect.oy2 < 0) return None;

        rect.x1 = rect.ox1;
        rect.y1 = rect.oy1;
        rect.x2 = rect.ox2;
        rect.y2 = rect.oy2;

        // clamp the rectangle to the boundaries
        if (rect.x1 < 0) rect.x1 = 0;
        if (rect.x2 >= cw) rect.x2 = cw - 1;
        if (rect.y1 < 0) rect.y1 = 0;
        if (rect.y2 >= ch) rect.y2 = ch - 1;

        return Some(rect);
    }

    Canvas::Canvas(size_t width, size_t height, size_t stride, Array<Pixel> pixels) noexcept :
        width(width),
        height(height),
        stride(stride),
        pixels(pixels)
    {
    }

    Canvas Canvas::make(Allocator& ator, size_t width, size_t height) noexcept {
        auto pixels = ator.alloc<Pixel>(width * height);
        return Canvas{ width, height, width, pixels };
    }

    void Canvas::destroy(Allocator& ator) noexcept {
        ator.free(this->pixels);
    }

    Optional<Canvas> Canvas::subcanvas(ssize_t x, ssize_t y, size_t width, size_t height) noexcept {
        auto onr = NormalizedRect::make(x, y, width, height, this->width, this->height);
        if (onr.is_none()) {
            return None;
        }

        auto nr = onr.unwrap();

        Canvas c;
        c.width = nr.x2 - nr.x1 + 1;
        c.height = nr.y2 - nr.y1 + 1;
        c.stride = this->stride;

        auto start_idx = this->pixel_index(nr.x1, nr.y1);
        auto end_idx = this->pixel_index(nr.x2, nr.y2);
        c.pixels = this->pixels.slice(start_idx, end_idx - start_idx);

        return Some(c);
    }

    Pixel& Canvas::pixel(size_t x, size_t y) noexcept {
        auto idx = this->pixel_index(x, y);
        return this->pixels[idx];
    }
    
    size_t Canvas::pixel_index(size_t x, size_t y) noexcept {
        assert(x >= 0 && x < this->width && y >= 0 && y < this->height);
        return x + y * this->stride;
    }
}

#undef SWAP
#undef SIGN
#undef ABS
