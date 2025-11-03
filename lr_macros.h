#ifndef LR_MACROS_H
#define LR_MACROS_H

// Zero
#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif
#if !defined(COMPILER_GNU)
#define COMPILER_GNU 0
#endif

// Detect compiler
#if __clang__
# define COMPILER_CLANG 1
#elif _MSC_VER
# define COMPILER_MSVC 1
#elif __GNUC__
# define COMPILER_GNU 1
#endif

// Push/Pop warnings
#if defined(COMPILER_GNU)
# define PUSH_WARNINGS \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Weverything\"")
# define POP_WARNINGS \
_Pragma("GCC diagnostic pop")
#elif defined(COMPILER_CLANG)
# define PUSH_WARNINGS \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Weverything\"")
# define POP_WARNINGS \
_Pragma("clang diagnostic pop")
#else
# define PUSH_WARNINGS \
_Pragma("message \"No compatible compiler found\"")
# define POP_WARNINGS
#endif

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define Assert(Expression) if(!(Expression)) { __asm__ volatile("int3"); }
#define DebugBreakOnce do { local_persist b32 X = false; Assert(X); X = true; } while(0)
#define NullExpression { int X = 0; }

#define Min(A, B) (((A) < (B)) ? (A) : (B))
#define Max(A, B) (((A) > (B)) ? (A) : (B))

#endif // LR_MACROS_H