//
// Created by Я on 15.12.2021.
//


#include "gtest/gtest.h"
#include "trit.h"

TEST(TritSetTest, CapacityTest_create){
    TritSet tritset(123);
    size_t cap = tritset.get_capacity();
    ASSERT_EQ(cap, 123);
    TritSet tritset2 (tritset);
    cap = tritset2.get_capacity();
    ASSERT_EQ(cap, 123);
}

TEST(TritSetTest, CapacityTest_set){
    TritSet tritset(123);
    size_t cap;
    tritset[124] = TRUE;
    cap = tritset.get_capacity();
    ASSERT_EQ(cap, 125);
    tritset[234] = UNKNOWN;
    cap = tritset.get_capacity();
    ASSERT_EQ(cap, 125);
}

TEST(TritSetTest, CapacityTest_method_shrink){
    TritSet tritset(113);
    size_t cap;
    tritset[10] = TRUE;
    tritset.shrink();
    cap = tritset.get_capacity();
    ASSERT_EQ(cap, 11);
}

// SetTritTest

TEST(TritSetTest, SetTritTest_1){
    TritSet tritset(100);
    tritset[10] = TRUE;
    ASSERT_EQ(tritset[10], TRUE);
}

TEST(TritSetTest, SetTritTest_2){
    TritSet tritset(100);
    tritset[10] = FALSE;
    ASSERT_EQ(tritset[10], FALSE);
}

TEST(TritSetTest, SetTritTest_3){
    TritSet tritset(100);
    tritset[10] = UNKNOWN;
    ASSERT_EQ(tritset[10], UNKNOWN);
}

TEST(TritSetTest, SetTritTest_4){
    TritSet tritset(100);
    tritset[10] = TRUE;
    ASSERT_EQ(tritset[120], UNKNOWN);
}

TEST(TritSetTest, SetTritTest_5){
    TritSet tritset(100);
    tritset[10] = TRUE;
    tritset[10] = FALSE;
    ASSERT_EQ(tritset[10], FALSE);
}

TEST(TritSetTest, SetTritTest_6){
    TritSet tritset(100);
    tritset[10] = TRUE;
    tritset[10] = UNKNOWN;
    ASSERT_EQ(tritset[10], UNKNOWN);
}

// OperatorsTest

TEST(TritSetTest, OperatorTest_and){
    TritSet first(50), second(60), expect(60);
    first[0] = TRUE;
    first[17] = FALSE;
    first[20] = FALSE;
    second[0] = TRUE;
    second[17] = TRUE;
    second[20] = FALSE;
    second[49] = FALSE;
    second[59] = TRUE;
    expect[0] = TRUE;
    expect[17] = FALSE;
    expect[20] = FALSE;
    expect[49] = FALSE;
    expect[59] = UNKNOWN;
    TritSet result = first & second;

    ASSERT_EQ(result.get_capacity(), 60);
    for (int i = 0; i < 60; i++)
        ASSERT_EQ(result[i], expect[i]);
}

TEST(TritSetTest, OperatorTest_or){
    TritSet first(50), second(60), expect(60);
    first[0] = TRUE;
    first[17] = FALSE;
    first[20] = FALSE;
    second[0] = TRUE;
    second[17] = TRUE;
    second[20] = FALSE;
    second[49] = FALSE;
    second[58] = TRUE;
    expect[0] = TRUE;
    expect[17] = TRUE;
    expect[20] = FALSE;
    expect[49] = UNKNOWN;
    expect[58] = TRUE;
    TritSet result = first | second;

    ASSERT_EQ(result.get_capacity(), 60);
    for (int i = 0; i < 60; i++)
        ASSERT_EQ(result[i], expect[i]);
}

TEST(TritSetTest, OperatorTest_not){
    TritSet tritset(100);
    tritset[56] = TRUE;
    tritset[0] = FALSE;
    tritset[18] = TRUE;
    TritSet newSet = ~tritset;
    TritSet expect(100);
    expect[56] = FALSE;
    expect[0] = TRUE;
    expect[18] = FALSE;
    for (int i = 0; i < newSet.get_capacity(); i++)
        ASSERT_EQ(newSet[i], expect[i]);
}

// Cardinality

TEST(TritSetTest, CardinalityTest){
    TritSet tritset(100);
    tritset[56] = TRUE;
    tritset[0] = FALSE;
    tritset[18] = TRUE;
    auto res = tritset.cardinality();
    ASSERT_EQ(tritset.cardinality(TRUE), 2);
    ASSERT_EQ(tritset.cardinality(FALSE), 1);
    ASSERT_EQ(tritset.cardinality(UNKNOWN), 97);
    ASSERT_EQ(res[TRUE], 2);
    ASSERT_EQ(res[FALSE], 1);
    ASSERT_EQ(res[UNKNOWN], 97);
}

TEST(TritSetTest, MemoryTest){
    TritSet p(100);
    p[10] = TRUE;
    ASSERT_EQ(p[30], UNKNOWN);
    p[30] = p[10];
    ASSERT_EQ(p[30], TRUE);
}

TEST(iteratorTest, RangeBasedForTest){
    TritSet a(100);
    TritSet exp(100);

    for (auto  it : a){
        &(*it) = TRUE;
    }

    for (int i = 0; i < 100; i++){
        exp[i] = TRUE;
    }
    ASSERT_EQ(a, exp);
}

TEST(ConstTritSetTest, eqTest){
    TritSet a(5);
    a[1] = TRUE;
    a[2] = FALSE;
    a[4] = FALSE;
    a[5] = FALSE;
    const TritSet b(a);
    ASSERT_EQ(a,b);
}

