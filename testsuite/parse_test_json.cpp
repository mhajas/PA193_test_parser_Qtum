//
// Created by lenka on 7. 11. 2017.
//

#include "gtest/gtest.h"
#include <parser/entities/block.h>
#include <fstream>
#include "testsuite/external/json.hpp"

using json = nlohmann::json;

static unsigned char hex2char(char input) {
    if (input >= '0' && input <= '9')
        return input - '0';
    if (input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if (input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");

}

static void hex_string2string(std::string& str) {
    assert(str.length()  % 2 == 0);

    for (uint32_t i = 0; i < str.size() / 2; i++) {
        str[i] = (hex2char(str.at(2 * i)) << 4) + hex2char(str.at(2 * i + 1));
    }

    str.resize(str.length()/2);
}

int compare_hashes(hash_type hash, std::string hash_j, bool rev) {
    hex_string2string(hash_j);
    if (rev) {
        std::reverse(hash_j.begin(), hash_j.end());
    }

    for (int it = 0; it < 32; it++) {
        if (hash[it] != (uint8_t) hash_j[it]) {
            //printf("%d %x %x\n", it, hash[it], hash_j[it]);
            return -1;
        }
    }
    return 0;
}

int compare_bin_json(std::string bin_path, const char* json_path) {
    //read bin file
    std::ifstream block_bin_file(bin_path, std::ios::binary);
    if (!block_bin_file) {
        return -1;
    }

    block b(block_bin_file);

    std::ifstream json_file(json_path);
    if (!json_file) {
        return -1;
    }
    json j;
    json_file >> j;

    //compare hash
    hash_type hash = b.compute_hash();

    std::string hash_j = j.at("hash");

    if (compare_hashes(hash, hash_j, true) != 0) return -2;

    //compare version
    uint32_t b_version = b.get_version();

    uint32_t j_version = j.at("version");

    if (b_version != j_version) {
        return -3;
    }

    //compare merkle root
    hash_type hash_merkle_root = b.get_hash_merkle_root();

    std::string j_hash_merkle_root = j.at("merkleroot");

    if (compare_hashes(hash_merkle_root, j_hash_merkle_root, false) != 0) return -2;

    //compare hash state root
    hash_type hash_state_root = b.get_hash_state_root();

    std::string j_hash_state_root = j.at("hashStateRoot");

    if (compare_hashes(hash_state_root, j_hash_state_root, false) != 0) return -3;


    //compare hash UTXO root
    hash_type hash_UTXO_root = b.get_hash_UTXO_root();

    std::string j_hash_UTXO_root = j.at("hashUTXORoot");

    if (compare_hashes(hash_UTXO_root, j_hash_UTXO_root, false) != 0) return -3;


    return 0;
}
TEST(parsing, correct_block_22380_json) {
    EXPECT_EQ(0, compare_bin_json("resources/test-resources/blocks/22380.bin", "resources/test-resources/blocks/22380.json"));
}
