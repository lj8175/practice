#include "arrays.h"
#include "gtest/gtest.h"

TEST(array_gcd, array_gcd)
{
    int arr[] = {2, 4, 6, 8};
    ASSERT_EQ(2, array_gcd(arr, 4));
    int arr1[] = {-2, 4, 6, 8};
    ASSERT_EQ(-1, array_gcd(arr1, 4));
    int arr2[] = {1, 4, 6, 8};
    ASSERT_EQ(1, array_gcd(arr2, 4));
    int arr3[] = {4};
    ASSERT_EQ(4, array_gcd(arr3, 1));
    int arr4[] = {50000, 40000, 30000, 20000, 60000, 80000, 70000};
    ASSERT_EQ(10000, array_gcd(arr4, 7));
    int arr5[] = {731, 269};
    ASSERT_EQ(1, array_gcd(arr5, 2));
    ASSERT_EQ(731, arr5[0]);
    int arr6[] = {1, 0};
    ASSERT_EQ(1, array_gcd(arr6, 2));
    int arr7[] = {2, 0};
    ASSERT_EQ(2, array_gcd(arr7, 2));
    int arr8[] = {0, 0};
    ASSERT_EQ(0, array_gcd(arr8, 2));
    int arr9[] = {14, 9, 4, 0};
    ASSERT_EQ(1, array_gcd(arr9, sizeof(arr9)/sizeof(int)));
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
