//
// Created by mhajas on 10/5/17.
//

#include "gtest/gtest.h"
#include <parser/parser.h>

TEST(basic, basi_test) {
    parser p(5);

    ASSERT_EQ(5, p.getValue());
}