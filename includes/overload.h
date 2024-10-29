#include <stdio.h>
#include <stdarg.h>

#ifdef __RA_OVERLOAD_H__

#define __RA_OVERLOAD_H__

    #define OVERLOAD(ar)
    
    /*
#include <stdio.h>
#include <stdarg.h>

static inline void _FOO_A(int val) {
    printf("%d\n", val);
}

static inline void _FOO_B(char *str) {
    printf("%s\n", str);
}

static inline void _FOO_C(int val1, int val2) {
    printf("%d %d\n", val1, val2);
}

static inline void _FOO_D(char *str1, char *str2) {
    printf("%s %s\n", str1, str2);
}

#define FOO(...) _FOO_GENERIC(__VA_ARGS__)
#define _FOO_GENERIC(arg1, arg2) \
            _Generic((arg1), \
                int: _Generic((arg2), \
                    int: _FOO_C, \
                    default: _FOO_D \
                ), \
                default: _Generic((arg2), \
                    int: _FOO_D, \
                    default: _FOO_D \
                ) \
            )(arg1, arg2)

#define OVERLOAD(prefix, ...) _OVERLOAD_GENERIC(prefix, __VA_ARGS__)
#define _OVERLOAD_GENERIC(prefix, arg1, arg2) \
            _Generic((arg1), \
                int: _Generic((arg2), \
                    int: _##prefix##_C, \
                    default: _##prefix##_D \
                ), \
                default: _Generic((arg2), \
                    int: _##prefix##_D, \
                    default: _##prefix##_D \
                ) \
            )(arg1, arg2)

static inline void _BAR_A(int val) {
    printf("BAR A: %d\n", val);
}

static inline void _BAR_B(char *str) {
    printf("BAR B: %s\n", str);
}

static inline void _BAR_C(int val1, int val2) {
    printf("BAR C: %d %d\n", val1, val2);
}

static inline void _BAR_D(char *str1, char *str2) {
    printf("BAR D: %s %s\n", str1, str2);
}

int main() {
    FOO(5, 10);              // Calls _FOO_C
    FOO("Hello", "World");   // Calls _FOO_D
    FOO("String", 5);        // Calls _FOO_D

    OVERLOAD(BAR, 5, 10);              // Calls _BAR_C
    OVERLOAD(BAR, "Hello", "World");   // Calls _BAR_D
    OVERLOAD(BAR, "String", 5);        // Calls _BAR_D

    return 0;
}
*/

#endif
