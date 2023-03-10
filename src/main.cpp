#include "sk/fmt.h"
#include "sk/string.h"
#include "sk/optional.h"
#include "sk/result.h"
#include "sk/array.h"
#include "sk/defer.h"
#include "sk/list.h"
#include "sk/mem/c-allocator.h"
#include "sk/mem/arena-allocator.h"
#include "sk/ptr/nonnull.h"
#include "sk/ptr/owned.h"
#include "sk/gfx/canvas.h"

#define FILE __FILE__
#define LINE __LINE__

void string_example() {
    sk::String s = "Hello";

    sk::println("s = \"{}\"", s);

    for (char c : s) {
        sk::println("c = {}", c);
    }
}

void string_builder_example() {
    sk::StringBuilder builder{ 24, 43 };

    builder.append("Hello there.");
    builder.append(" You are looking rather dashing, aren't you?");

    sk::println("builder = {}", builder);
}

void optional_example() {
    auto some = sk::Some(5);
    sk::Optional<int> none = sk::None;

    auto from_some = some.unwrap_or(0);
    auto from_none = none.unwrap_or(0);

    auto mapped_some = some.map([](int n){ return static_cast<char>('A' + n); });
    auto mapped_none = none.map([](int n){ return static_cast<char>('A' + n); });

    auto mapped_or_some = some.map_or(0, [](int n){ return n / 2; });
    auto mapped_or_none = none.map_or(0, [](int n){ return n / 2; });

    sk::println("some = {}\nnone = {}", some, none);
    sk::println("from_some = {}\nfrom_none = {}", from_some, from_none);
    sk::println("mapped_some = {}\nmapped_none = {}", mapped_some, mapped_none);
    sk::println("mapped_or_some = {}\nmapped_or_none = {}", mapped_or_some, mapped_or_none);

#if 0
    none.unwrap();
#endif
}

void optional_reference_example() {
    sk::println("sizeof(Optional<T&>) = {}", sizeof(sk::Optional<int&>));
    sk::println("sizeof(T*)           = {}", sizeof(int*));

    sk::Optional<int&> none = sk::None;
    sk::println("none = {}", none);

    int d = 5;
    sk::Optional<int&> some = d;
    sk::println("some = {}", some);

    if (none.is_none()) {
        sk::println("none successfully detected as is_none()");
    } else {
        sk::println("none failed to be detected as is_none()");
    }

    if (some.is_some()) {
        sk::println("some successfully detected as is_some()");
    } else {
        sk::println("some failed to be detected as is_some()");
    }
}

void result_example() {
    auto ok = sk::Ok<const char*>(69);
    auto err = sk::Err<void>((const char*)"This is an error");

    if (ok.is_ok()) {
        sk::println("ok successfully detected as is_ok()");
    } else {
        sk::println("ok failed to be detected as is_ok()");
    }

    if (err.is_err()) {
        sk::println("err successfully detected as is_err()");
    } else {
        sk::println("err failed to be detected as is_err()");
    }

    sk::println("ok  = {}", ok);
    sk::println("err = {}", err);
}

void result_reference_example() {

}

void array_example() {
    int ns[] = { 1, 2, 3, 4, 5 };
    auto arr = sk::Array{ sizeof(ns) / sizeof(ns[0]), ns };

    auto _3rd = arr[2];
    auto first = arr.first();
    auto last = arr.last();
    auto tail = arr.tail();

    sk::println("arr = {}", arr);
    sk::println("_3rd = {}", _3rd);
    sk::println("first = {}", first);
    sk::println("last = {}", last);
    sk::println("tail = {}", tail);
}

void list_example() {
    sk::List<int> list;
    defer { 
        sk::println("Destroying List");
        list.destroy(sk::c_allocator);
    };

    list.append(sk::c_allocator, 1);
    list.append(sk::c_allocator, 2);
    list.append(sk::c_allocator, 3);

    auto _2nd = list[1];
    auto first = list.first();
    auto last = list.last();

    sk::println("list = {}", list);
    sk::println("_2nd = {}", _2nd);
    sk::println("first = {}", first);
    sk::println("last = {}", last);
}

int sum_array(sk::Array<int> ns) {
    int total = 0;
    for (int n : ns) {
        total += n;
    }
    return total;
}

void sum_example() {
    auto list = sk::OwnedList<int>{ sk::c_allocator };

    list.append(1);
    list.append(2);
    list.append(3);
    list.append(4);
    list.append(5);

    int sum = sum_array(list);
    sk::println("list = {}", list);
    sk::println("sum  = {}", sum);
}

void defer_example() {
    defer { 
        sk::println("First Deferred");
    };

    defer { 
        sk::println("Second Deferred");
    };
}

void nonnull_example() {
    int d = 5;
    int e = 10;

    auto p = sk::NonNull<int>::make(&d).unwrap();
    auto q = sk::NonNull<int>::make_unchecked(&e);

    sk::println("p = {}", p);
    sk::println("q = {}", q);

    p.swap(q);

    sk::println("Swapped p and q");
    sk::println("p = {}", p);
    sk::println("q = {}", q);

    sk::NonNull<int> r;
    if (sk::NonNull<int>::make(nullptr).try_unwrap(r)) {
        sk::println("We shouldn't have reached {}:{}", FILE, LINE);
    } else {
        sk::println("NonNull::make successfully returned None");
    }
}

void arena_allocator_example() {    
    {
        auto ator = sk::ArenaAllocator{ &sk::c_allocator, 32 };
        defer { ator.destroy(); };

        auto ns = ator.alloc<int>(8);
        std::fill(ns.begin(), ns.end(), 66);

        auto fs = ator.alloc<float>(5);
        std::fill(fs.begin(), fs.end(), 1.23);

        auto cs = ator.alloc<char>(12);
        std::fill(cs.begin(), cs.end(), 'X');

        auto sbs = ator.alloc<sk::StringBuilder>(3);
        std::fill(sbs.begin(), sbs.end(), sk::StringBuilder{});

        sk::println("ns  = {}", ns);
        sk::println("fs  = {}", fs);
        sk::println("cs  = {}", cs);
        sk::println("sbs = {}", sbs);
    }

    {
        auto arena = sk::ArenaAllocator{ &sk::c_allocator, 16 };
        defer { arena.destroy(); };

        sk::Array<int> ints = { 0, nullptr };

        ints = arena.resize(ints, 3).unwrap_or({ 0, nullptr });
        for (int i = 0; i < ints.len; i++) {
            ints[i] = i + 1;
        }

        sk::println("{} at {}", ints, ints.items);

        ints = arena.resize(ints, 4).unwrap_or({ 0, nullptr });
        for (int i = 0; i < ints.len; i++) {
            ints[i] = i + 1;
        }

        sk::println("{} at {}", ints, ints.items);

        ints = arena.resize(ints, 10).unwrap_or({ 0, nullptr });
        for (int i = 0; i < ints.len; i++) {
            ints[i] = i + 1;
        }

        sk::println("{} at {}", ints, ints.items);
    }
}

void owned_example() {
    auto list = sk::OwnedList<int>{ sk::c_allocator }; 

    list.append(1);
    list.append(2);
    list.append(3);
    
    auto _2nd = list[1];
    auto first = list.first();
    auto last = list.last();

    sk::println("list = {}", list);
    sk::println("_2nd = {}", _2nd);
    sk::println("first = {}", first);
    sk::println("last = {}", last);
}

void canvas_example() {
    auto canvas = sk::Canvas::make(sk::c_allocator, 15, 15);
    defer { canvas.destroy(sk::c_allocator); };

    {
        auto start = canvas.pixel_index(3, 3);
        auto end = canvas.pixel_index(5, 5);

        for (auto i = start; i <= end; i++) {
            auto& pixel = canvas.pixels[i];
            pixel = 15;
        }

        canvas.pixels[start] = 51;
        canvas.pixels[end - 1] = 51;
    }

    auto osc = canvas.subcanvas(3, 3, 3, 3);
    if (osc.is_none()) {
        sk::println("Failed to make subcanvas.");
        return;
    }

    auto sc = osc.unwrap();

    sk::println("sc.width  = {}", sc.width);
    sk::println("sc.height = {}", sc.height);
    sk::println("sc.stride = {}", sc.stride);
    sk::println("sc.pixels = {}", sc.pixels);
}

int main() {
    string_example();
    std::cout << std::endl;

    string_builder_example();
    std::cout << std::endl;

    optional_example();
    std::cout << std::endl;

    optional_reference_example();
    std::cout << std::endl;

    result_example();
    std::cout << std::endl;

    array_example();
    std::cout << std::endl;

    list_example();
    std::cout << std::endl;

    sum_example();
    std::cout << std::endl;

    defer_example();
    std::cout << std::endl;

    nonnull_example();
    std::cout << std::endl;

    arena_allocator_example();
    std::cout << std::endl;

    owned_example();
    std::cout << std::endl;

    canvas_example();
    std::cout << std::endl;

    return 0;
}
