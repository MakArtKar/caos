// satsum 2
#include<inttypes.h>

int32_t satsum(int32_t v1, int32_t v2) {
    int32_t res;

    if (__builtin_add_overflow(v1, v2, &res)) {
        uint32_t ures = 0 - 1U;
        if (v1 > 0 && v2 > 0) {
            return ures / 2;
        } else {
            return ~(ures / 2);
        }
    }

    return res;
}

//#include <stdio.h>
//#include <limits.h>

//int main() {
//    printf("%" PRId32 "\n", satsum(1, 2));
//    printf("%" PRId32 "\n", satsum(INT32_MAX, INT32_MAX));
//    printf("%" PRId32 "\n", satsum(INT32_MIN, INT32_MIN));
//}
