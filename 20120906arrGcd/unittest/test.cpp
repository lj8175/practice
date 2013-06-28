#include "arrgcd.h"
#include "gtest/gtest.h"

TEST(arrgcd, arrgcd)
{
    int arr[] = {2, 4, 6, 8};
    ASSERT_EQ(2, arrgcd(arr, 4));
    int arr1[] = {-2, 4, 6, 8};
    ASSERT_EQ(0, arrgcd(arr1, 4));
    int arr2[] = {1, 4, 6, 8};
    ASSERT_EQ(1, arrgcd(arr2, 4));
    int arr3[] = {4};
    ASSERT_EQ(4, arrgcd(arr3, 1));
    int arr4[] = {50000, 40000, 30000, 20000, 60000, 80000, 70000};
    ASSERT_EQ(10000, arrgcd(arr4, 7));
    int arr5[] = {731, 269};
    ASSERT_EQ(1, arrgcd(arr5, 2));
    ASSERT_EQ(731, arr5[0]);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
