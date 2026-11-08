#pragma once

#include <cstddef>
#include <cstdint>

#define _align alignof(std::max_align_t)

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f63 = double;

#define KiB_1   1<<10
#define KiB_4   1<<12
#define KiB_64  1<<16
#define KiB_256 1<<18

#define MiB_1   1<<20
#define MiB_4   1<<22
#define MiB_64  1<<26
#define MiB_256 1<<28

#define GiB_1   1<<30
#define GiB_4   1<<32


#ifdef __clang__
    #define _private_  __attribute__((visibility("hidden")))
    #define _public_ __attribute__((visibilty("default")))
    #define _inline [[clang::always_inline]]

#elif defined(__GNUC__)
    #define _inline [[gnu::force_inline]]
#endif