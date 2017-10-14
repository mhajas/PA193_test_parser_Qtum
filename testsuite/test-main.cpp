//
// Created by mhajas on 10/5/17.
//
#include "gtest/gtest.h"

// Main file which triggers all tests in testsuite
int main(int argc, char** argv) {
    /* initialize google test framework */
    ::testing::InitGoogleTest(&argc, argv);

    /* run all testsuite */
    return RUN_ALL_TESTS();
}