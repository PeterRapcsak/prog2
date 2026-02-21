#include <iostream>
#include "gtest_lite.h"

int abs_osszeg(const int* ptr, size_t m) {
    if (ptr == nullptr)
        return 0;
    int ossz = 0;
    for(size_t i = 0; i < m; i++) {
        if(ptr[i] > 0)
            ossz += ptr[i];
        else
            ossz += -1 * ptr[i];
    }
    return ossz;
}

int main()
{
    TEST(osszeg, csak_pozitiv) {
        int t[] = { 2, 4, 1, 9 };
        int res = abs_osszeg(t, 4);
        EXPECT_EQ(16, res);
    } END

    TEST(osszeg, csak_negativ) {
        int t[] = { -3, -5, -2 };
        int res = abs_osszeg(t, 3);
        EXPECT_EQ(10, res);
    } END

    TEST(osszeg, vegyes) {
        int t[] = { -2, 5, -3, 4 };
        int res = abs_osszeg(t, 4);
        EXPECT_EQ(14, res);
    } END

    TEST(osszeg, nullptr_teszt) {
        int res = abs_osszeg(nullptr, 5);
        EXPECT_EQ(0, res);
    } END

    TEST(osszeg, ures_tomb) {
        int t[] = { 1 };
        int res = abs_osszeg(t, 0);
        EXPECT_EQ(0, res);
    } END

    TEST(osszeg, nulla_ertek) {
        int t[] = { 0, 0, 0 };
        int res = abs_osszeg(t, 3);
        EXPECT_EQ(0, res);
    } END

    return 0;
}
