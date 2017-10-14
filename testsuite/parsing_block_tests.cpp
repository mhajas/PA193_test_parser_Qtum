//
// Created by mhajas on 10/5/17.
//

#include "gtest/gtest.h"
#include <parser/entities/block.h>
#include <fstream>

TEST(parsing, correct_block) {
    std::ifstream block_file("resources/test-resources/blocks/22380.bin", std::ios::binary);

    ASSERT_TRUE(block_file) << "Failed to open file. Error: '" << strerror(errno) << "'";

    block b(block_file);

    EXPECT_EQ(536870912, b.get_version()) << "Version wasn't parsed correctly";
}