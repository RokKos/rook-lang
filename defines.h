#ifndef INCLUDE_DEFINES_H
#define INCLUDE_DEFINES_H

typedef char unsigned u8;
typedef short unsigned u16;
typedef int unsigned u32;
typedef long long unsigned u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef float f32;
typedef double f64;

#define internal static
#define global static
#define local_persist static

#define STATIC_ASSERT(COND, MSG)                                               \
  typedef char static_assertion_##MSG[(!!(COND)) * 2 - 1]
// token pasting madness:
#define COMPILE_TIME_ASSERT3(X, L)                                             \
  STATIC_ASSERT(X, static_assertion_at_line_##L)
#define COMPILE_TIME_ASSERT2(X, L) COMPILE_TIME_ASSERT3(X, L)
#define COMPILE_TIME_ASSERT(X) COMPILE_TIME_ASSERT2(X, __LINE__)

#endif // INCLUDE_DEFINES_H
