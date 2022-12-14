#include "sk/fmt.h"
#include "sk/string.h"
#include "sk/optional.h"
#include "sk/array.h"
#include "sk/defer.h"
#include "sk/list.h"
#include "sk/mem/c-allocator.h"
#include "sk/mem/arena-allocator.h"
#include "sk/ptr/nonnull.h"
#include "sk/ptr/owned.h"

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

void array_example() {
    int ns[] = { 1, 2, 3, 4, 5 };
    auto arr = sk::Array{ sizeof(ns) / sizeof(ns[0]), ns };

    auto _3rd = arr[2];
    auto first = arr.first();
    auto last = arr.last();

    sk::println("arr = {}", arr);
    sk::println("_3rd = {}", _3rd);
    sk::println("first = {}", first);
    sk::println("last = {}", last);
}

void defer_example() {
    defer { 
        sk::println("First Deferred");
    };

    defer { 
        sk::println("Second Deferred");
    };
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

int main() {
    string_example();
    std::cout << std::endl;

    string_builder_example();
    std::cout << std::endl;

    optional_example();
    std::cout << std::endl;

    optional_reference_example();
    std::cout << std::endl;

    array_example();
    std::cout << std::endl;

    defer_example();
    std::cout << std::endl;

    list_example();
    std::cout << std::endl;

    nonnull_example();
    std::cout << std::endl;

    arena_allocator_example();
    std::cout << std::endl;

    owned_example();
    std::cout << std::endl;

    return 0;
}
