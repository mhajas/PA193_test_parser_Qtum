//
// Created by mhajas on 10/5/17.
//

#include "gtest/gtest.h"
#include <parser/entities/block.h>
#include <fstream>

TEST(parsing, correct_block_22380) {
    std::ifstream block_file("resources/test-resources/blocks/22380.bin", std::ios::binary);

    ASSERT_TRUE(block_file) << "Failed to open file. Error: '" << strerror(errno) << "'";

    block b(block_file);

    EXPECT_EQ(536870912, b.get_version()) << "Version wasn't parsed correctly";

    std::array<uint8_t, 32> hash_merkle_test = {0x41, 0xc3, 0xef, 0x1c, 0x55, 0x19, 0x2a, 0xe2,
                                                0x11, 0x2a, 0x35, 0x07, 0x55, 0x43, 0xa3, 0xc4,
                                                0x94, 0xaf, 0x17, 0xce, 0xbc, 0x81, 0x8d, 0x73,
                                                0x0c, 0xc3, 0xc7, 0x7a, 0xfc, 0x18, 0xf7, 0x4c};
    EXPECT_EQ(hash_merkle_test, b.get_hash_merkle_root()) << "Hash merkle root was not parsed correctly";

    std::array<uint8_t, 32> hash_prev_block_test = {0xc0, 0x3b, 0x79, 0xe6, 0xa8, 0xdc, 0x15, 0x13,
                                                    0x1b, 0xa5, 0xf5, 0xd4, 0x9f, 0x8a, 0xcf, 0x8b,
                                                    0x6f, 0xd3, 0xac, 0xf6, 0x6f, 0xb1, 0x1b, 0xbc,
                                                    0xb1, 0x76, 0xed, 0xd7, 0x98, 0x7b, 0xe3, 0x78};
    EXPECT_EQ(hash_prev_block_test, b.get_hash_prev_block()) << "Hash previous block was not parsed correctly";

    EXPECT_EQ(1507372912, b.get_n_time()) << "nTime wasn't parsed correctly";

    EXPECT_EQ(437585635, b.get_n_bits()) << "nBits wasn't parsed correctly";

    EXPECT_EQ(0, b.get_n_nonce()) << "nNonce wasn't parsed correctly";
}

TEST(parsing, correct_block_22381) {
    std::ifstream block_file("resources/test-resources/blocks/22381.bin", std::ios::binary);

    ASSERT_TRUE(block_file) << "Failed to open file. Error: '" << strerror(errno) << "'";

    block b(block_file);

    EXPECT_EQ(536870912, b.get_version()) << "Version wasn't parsed correctly";

    std::array<uint8_t, 32> hash_merkle_test = {0x7d, 0x0a, 0x34, 0xb6, 0x03, 0xd3, 0x05, 0xe6,
                                                0x90, 0xa5, 0x54, 0xaa, 0xe3, 0x75, 0x02, 0xba,
                                                0xfb, 0xf3, 0xba, 0x89, 0xae, 0x6e, 0x86, 0x63,
                                                0x00, 0x3b, 0xe0, 0xb8, 0x3b, 0x47, 0xbf, 0x57};

    EXPECT_EQ(hash_merkle_test, b.get_hash_merkle_root()) << "Hash merkle root was not parsed correctly";

    std::array<uint8_t, 32> hash_prev_block_test = {0x1f, 0x21, 0x31, 0xfc, 0xa1, 0x63, 0xcd, 0x6c,
                                                    0xcb, 0x72, 0x1a, 0x65, 0x20, 0x3b, 0x43, 0x6d,
                                                    0x43, 0x24, 0x06, 0xb0, 0xdd, 0x3f, 0x28, 0xaa,
                                                    0xf9, 0x13, 0xf4, 0x3f, 0x91, 0x34, 0x57, 0x8e};
    EXPECT_EQ(hash_prev_block_test, b.get_hash_prev_block()) << "Hash previous block was not parsed correctly";

    EXPECT_EQ(1507372960, b.get_n_time()) << "nTime wasn't parsed correctly";

    EXPECT_EQ(437370319, b.get_n_bits()) << "nBits wasn't parsed correctly";

    EXPECT_EQ(0, b.get_n_nonce()) << "nNonce wasn't parsed correctly";
}

TEST(parsing, correct_block_22382) {
    std::ifstream block_file("resources/test-resources/blocks/22382.bin", std::ios::binary);

    ASSERT_TRUE(block_file) << "Failed to open file. Error: '" << strerror(errno) << "'";

    block b(block_file);

    EXPECT_EQ(536870912, b.get_version()) << "Version wasn't parsed correctly";

    std::array<uint8_t, 32> hash_merkle_test = {0x84, 0x42, 0xa6, 0x99, 0x48, 0x56, 0x32, 0xf5,
                                                0x24, 0x87, 0xf5, 0x89, 0x34, 0x3b, 0x98, 0x46,
                                                0xcd, 0x49, 0x6e, 0xf8, 0xde, 0x8f, 0xfc, 0x80,
                                                0x9d, 0xd5, 0x22, 0x2f, 0x60, 0x39, 0xcf, 0x99};

    EXPECT_EQ(hash_merkle_test, b.get_hash_merkle_root()) << "Hash merkle root was not parsed correctly";

    std::array<uint8_t, 32> hash_prev_block_test = {0x05, 0xf0, 0xf1, 0x75, 0x65, 0x57, 0x7b, 0xac,
                                                    0x8d, 0xc9, 0x16, 0x2c, 0x70, 0xec, 0x12, 0x54,
                                                    0x1c, 0x0a, 0xfb, 0x8c, 0x52, 0x20, 0xa8, 0x49,
                                                    0xb9, 0x62, 0x99, 0x23, 0x75, 0x3f, 0x92, 0x48};
    EXPECT_EQ(hash_prev_block_test, b.get_hash_prev_block()) << "Hash previous block was not parsed correctly";

    EXPECT_EQ(1507373136, b.get_n_time()) << "nTime wasn't parsed correctly";

    EXPECT_EQ(437188646, b.get_n_bits()) << "nBits wasn't parsed correctly";

    EXPECT_EQ(0, b.get_n_nonce()) << "nNonce wasn't parsed correctly";
}

TEST(parsing, correct_block_22383) {
    std::ifstream block_file("resources/test-resources/blocks/22383.bin", std::ios::binary);

    ASSERT_TRUE(block_file) << "Failed to open file. Error: '" << strerror(errno) << "'";

    block b(block_file);

    EXPECT_EQ(536870912, b.get_version()) << "Version wasn't parsed correctly";

    std::array<uint8_t, 32> hash_merkle_test = {0x83, 0x69, 0x9e, 0x01, 0x7c, 0x3a, 0xc2, 0x8f,
                                                0x00, 0xfe, 0xf4, 0xb9, 0x27, 0x7f, 0x58, 0x1c,
                                                0x8e, 0x60, 0xa7, 0x4f, 0x28, 0x4c, 0x40, 0xe5,
                                                0xd5, 0x89, 0x64, 0xf6, 0x68, 0x3d, 0x60, 0xf3};

    EXPECT_EQ(hash_merkle_test, b.get_hash_merkle_root()) << "Hash merkle root was not parsed correctly";

    std::array<uint8_t, 32> hash_prev_block_test = {0x45, 0x80, 0xe4, 0x64, 0x45, 0x5d, 0x3b, 0x6b,
                                                    0x3e, 0x7b, 0xfb, 0xda, 0x45, 0xa2, 0x6a, 0x24,
                                                    0xec, 0x95, 0xec, 0x99, 0x3e, 0x94, 0xb3, 0xe2,
                                                    0xb3, 0x52, 0xb2, 0x12, 0x1e, 0x8b, 0x59, 0x80};
    EXPECT_EQ(hash_prev_block_test, b.get_hash_prev_block()) << "Hash previous block was not parsed correctly";

    EXPECT_EQ(1507373168, b.get_n_time()) << "nTime wasn't parsed correctly";

    EXPECT_EQ(437280617, b.get_n_bits()) << "nBits wasn't parsed correctly";

    EXPECT_EQ(0, b.get_n_nonce()) << "nNonce wasn't parsed correctly";
}

TEST(parsing, correct_block_22384) {
    std::ifstream block_file("resources/test-resources/blocks/22384.bin", std::ios::binary);

    ASSERT_TRUE(block_file) << "Failed to open file. Error: '" << strerror(errno) << "'";

    block b(block_file);

    EXPECT_EQ(536870912, b.get_version()) << "Version wasn't parsed correctly";

    std::array<uint8_t, 32> hash_merkle_test = {0x3f, 0xdd, 0x54, 0x81, 0xee, 0xf6, 0x2c, 0xc1,
                                                0x05, 0x26, 0xbc, 0x65, 0x83, 0x39, 0x37, 0x2c,
                                                0xcc, 0xae, 0x41, 0x84, 0xd1, 0x0a, 0xa1, 0x63,
                                                0xfb, 0x81, 0x71, 0x17, 0xde, 0x7b, 0x17, 0x26};

    EXPECT_EQ(hash_merkle_test, b.get_hash_merkle_root()) << "Hash merkle root was not parsed correctly";

    std::array<uint8_t, 32> hash_prev_block_test = {0x5f, 0x8c, 0xd7, 0xb2, 0x7d, 0x6a, 0xe0, 0x6d,
                                                    0x04, 0x48, 0x7a, 0x36, 0x36, 0x96, 0x17, 0x09,
                                                    0xbd, 0x05, 0x87, 0x21, 0x28, 0xaf, 0x8a, 0x50,
                                                    0x15, 0x46, 0x4b, 0x09, 0x16, 0x17, 0x1c, 0x1b};
    EXPECT_EQ(hash_prev_block_test, b.get_hash_prev_block()) << "Hash previous block was not parsed correctly";

    EXPECT_EQ(1507373376, b.get_n_time()) << "nTime wasn't parsed correctly";

    EXPECT_EQ(437079429, b.get_n_bits()) << "nBits wasn't parsed correctly";

    EXPECT_EQ(0, b.get_n_nonce()) << "nNonce wasn't parsed correctly";
}

TEST(parsing, correct_block_22385) {
    std::ifstream block_file("resources/test-resources/blocks/22385.bin", std::ios::binary);

    ASSERT_TRUE(block_file) << "Failed to open file. Error: '" << strerror(errno) << "'";

    block b(block_file);

    EXPECT_EQ(536870912, b.get_version()) << "Version wasn't parsed correctly";

    std::array<uint8_t, 32> hash_merkle_test = {0xd6, 0x34, 0x50, 0x5b, 0xb0, 0xb4, 0xe9, 0x5f,
                                                0xcf, 0xe6, 0x6d, 0x64, 0x78, 0xdb, 0xab, 0x39,
                                                0x59, 0x5f, 0x04, 0x17, 0xf7, 0xc1, 0x19, 0x5e,
                                                0x87, 0xee, 0x16, 0xcd, 0x7c, 0x7a, 0x87, 0x33};

    EXPECT_EQ(hash_merkle_test, b.get_hash_merkle_root()) << "Hash merkle root was not parsed correctly";

    std::array<uint8_t, 32> hash_prev_block_test = {0x47, 0xb9, 0xda, 0x11, 0x21, 0x3b, 0x60, 0xdb,
                                                    0x6d, 0x20, 0x3d, 0xa0, 0xaf, 0x5e, 0x2c, 0xb9,
                                                    0x17, 0x47, 0x56, 0x3a, 0x68, 0xb3, 0x4c, 0x8b,
                                                    0x49, 0x6a, 0x7b, 0x9b, 0x37, 0xb0, 0xe5, 0x04};
    EXPECT_EQ(hash_prev_block_test, b.get_hash_prev_block()) << "Hash previous block was not parsed correctly";

    EXPECT_EQ(1507373536, b.get_n_time()) << "nTime wasn't parsed correctly";

    EXPECT_EQ(437215649, b.get_n_bits()) << "nBits wasn't parsed correctly";

    EXPECT_EQ(0, b.get_n_nonce()) << "nNonce wasn't parsed correctly";
}