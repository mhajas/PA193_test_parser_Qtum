//
// Created by lenka on 7. 11. 2017.
//

#include "gtest/gtest.h"
#include <parser/entities/block.h>
#include <fstream>
#include "testsuite/external/json.hpp"

using json = nlohmann::json;

inline unsigned char hex2char(char input) {
    if (input >= '0' && input <= '9')
        return input - '0';
    if (input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if (input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");

}

inline std::string hex_reduction(std::string &&str) {
    assert(str.size()  % 2 == 0);

    for (uint32_t i = 0; i < str.size() / 2; i++) {
        str[i] = (hex2char(str.at(2 * i)) << 4) + hex2char(str.at(2 * i + 1));
    }

    str.resize(str.size()/2);

    return str;
}

template <typename T>
inline hash_type to_array(T&& str) {
    assert(str.size() == 32);

    hash_type a;
    std::copy(std::begin(str), std::end(str), a.data());

    return a;
}

void compare_bin_json(int block_number) {
    //read bin file
    std::ifstream block_bin_file("resources/test-resources/blocks/" + std::to_string(block_number) + ".bin", std::ios::binary);
    if (!block_bin_file) {
        FAIL() << "Can't open binary file: " << std::to_string(block_number) << ".bin";
    }
    block b(block_bin_file);

    std::ifstream json_file("resources/test-resources/blocks/" + std::to_string(block_number) + ".json");
    if (!json_file) {
        FAIL() << "Can't open json file: " << std::to_string(block_number) << ".json";
    }
    json j = json::parse(json_file);


    EXPECT_EQ(b.compute_hash(), to_array(hex_reduction(j.at("hash")))) << "Hashes are different for block: " << block_number;
    EXPECT_EQ(b.get_version(), j.at("version"));
    EXPECT_EQ(b.get_hash_merkle_root(), to_array(hex_reduction(j.at("merkleroot")))) << "Parsed merkle root is different for block: " << block_number;
    EXPECT_EQ(b.compute_merkle_root(), to_array(hex_reduction(j.at("merkleroot")))) << "Computed merkle root is different for block: " << block_number;
    EXPECT_EQ(b.get_hash_state_root(), to_array(hex_reduction(j.at("hashStateRoot")))) << "HashStateRoot is different for block " << block_number;
    EXPECT_EQ(b.get_hash_UTXO_root(), to_array(hex_reduction(j.at("hashUTXORoot")))) << "HashStateRoot is different for block " << block_number;
}

TEST(parsing, correct_block_22380_json) {
    compare_bin_json(22380);
}
