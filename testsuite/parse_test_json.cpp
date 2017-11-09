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
inline hash_type to_hash_type(T&& str) {
    assert(str.size() == 32);

    hash_type a;
    std::copy(std::begin(str), std::end(str), a.data());

    return a;
}

template <typename S>
inline vector_type to_vector_type(S&& str) {
    vector_type a(str.begin(), str.end());
    std::copy(std::begin(str), std::end(str), a.data());

    return a;
}

void compare_bin_json(int block_number) {
    std::ifstream block_bin_file("resources/test-resources/blocks/" + std::to_string(block_number) + ".bin", std::ios::binary);
    if (!block_bin_file) {
        FAIL() << "Can't open binary file: " << std::to_string(block_number) << ".bin";
    }
    block b(block_bin_file);

    std::ifstream json_file("resources/test-resources/blocks/" + std::to_string(block_number) + ".json");
    if (!json_file) {
        FAIL() << "Can't open json file: " << std::to_string(block_number) << ".json";
    }
    json parsed_json = json::parse(json_file);


    EXPECT_EQ(b.compute_hash(), to_hash_type(hex_reduction(parsed_json.at("hash")))) << "Hashes are different for block: " << block_number;
    EXPECT_EQ(b.get_version(), parsed_json.at("version"));
    EXPECT_EQ(b.get_hash_merkle_root(), to_hash_type(hex_reduction(parsed_json.at("merkleroot")))) << "Parsed merkle root is different for block: " << block_number;
    EXPECT_EQ(b.compute_merkle_root(), to_hash_type(hex_reduction(parsed_json.at("merkleroot")))) << "Computed merkle root is different for block: " << block_number;
    EXPECT_EQ(b.get_hash_state_root(), to_hash_type(hex_reduction(parsed_json.at("hashStateRoot")))) << "HashStateRoot is different for block " << block_number;
    EXPECT_EQ(b.get_hash_UTXO_root(), to_hash_type(hex_reduction(parsed_json.at("hashUTXORoot")))) << "HashUTXORoot is different for block " << block_number;

    EXPECT_EQ(b.get_n_time(), parsed_json.at("time")) << "Time is different for block " << block_number;
    EXPECT_EQ(b.get_n_nonce(), parsed_json.at("nonce")) << "Nonce is different for block " << block_number;

    std::string bits = parsed_json.at("bits");
    uint32_t bits_num = std::stoul(bits, nullptr, 16);
    EXPECT_EQ(b.get_n_bits(), bits_num) << "Block n bits is different for block " << block_number;

    EXPECT_EQ(b.get_block_height(), parsed_json.at("height")) << "Block height is different for block " << block_number;

    EXPECT_EQ(b.get_hash_prev_block(), to_hash_type(hex_reduction(parsed_json.at("previousblockhash")))) << "HashPrevBlock is different for block " << block_number;

    // add REVERSE!
    if (parsed_json.find("signature") != parsed_json.end()) {
        auto rev_hash = to_vector_type(hex_reduction(parsed_json.at("signature")));
        std::reverse(rev_hash.begin(), rev_hash.end());
        EXPECT_EQ(b.get_vch_block_sig(), rev_hash) << "Signature is different for block " << block_number;

        std::string str = parsed_json.at("signature");
        unsigned long size = str.length() / 2;
        EXPECT_EQ(b.get_vch_block_sig_size(), size) << "Signature length is different for " << block_number;
    }
    // test transactions
    std::vector<transaction> block_transactions = b.get_transactions();
    int current_transaction_index = 0;
    for (auto current_transaction : block_transactions) {
        auto expected_transaction = parsed_json.at("tx")[current_transaction_index];
        EXPECT_EQ(current_transaction.get_version(), expected_transaction.at("version")) << "Transaction (" << current_transaction_index << ") version is diffrent for block " << block_number;
        EXPECT_EQ(current_transaction.get_lock_time(), expected_transaction.at("locktime")) << "Transaction (" << current_transaction_index << ") lockTime is diffrent for block " << block_number;

        int current_position_json = 0;
        for(auto vin : current_transaction.get_vin()) {
            if (current_transaction_index != 0) {
                EXPECT_EQ(vin._sequence,
                          parsed_json.at("tx")[current_transaction_index].at("vin")[current_position_json].at(
                                  "sequence"))
                                    << "Transaction (" << current_transaction_index << ") vin ("
                                    << current_position_json << ") is diffrent for block " << block_number;


                EXPECT_EQ(vin._prevout.get_index_n(),
                          parsed_json.at("tx")[current_transaction_index].at("vin")[current_position_json].at("vout"))
                                    << "Transaction (" << current_transaction_index << ") vin ("
                                    << current_position_json << ") index n is diffrent for block " << block_number;

                EXPECT_EQ(vin._prevout.get_hash(), to_hash_type(hex_reduction(
                        parsed_json.at("tx")[current_transaction_index].at("vin")[current_position_json].at("txid"))))
                                    << "Transaction (" << current_transaction_index << ") vin ("
                                    << current_position_json << ") txid (hash) is diffrent for block " << block_number;
            }
            current_position_json++;
        }

        EXPECT_EQ(current_position_json, current_transaction.get_vin_count()) << "Transaction (" << current_transaction_index << ") vin count is diffrent for block " << block_number;

        current_position_json = 0;
        for(auto vout : current_transaction.get_vout()) {
            int64_t amount = static_cast<double>(parsed_json.at("tx")[current_transaction_index].at("vout")[current_position_json].at("value")) * 100000000; // Converting double to int64_t
            EXPECT_TRUE(std::abs(vout._amount - amount) <=1);
            current_position_json++;
        }
        EXPECT_EQ(current_position_json, current_transaction.get_vout_count()) << "Transaction (" << current_transaction_index << ") vout count is diffrent for block " << block_number;
        current_transaction_index++;
    }
    // Check whether number of transactions checked is equal to transactions present in json
    EXPECT_EQ(current_transaction_index, b.get_number_of_transactions()) << "Transaction number is different for block " << block_number;


}

TEST(parsing, correct_block_22380_json) {
    compare_bin_json(22380);
}

TEST(parsing, correct_block_22381_json) {
    compare_bin_json(22381);
}

TEST(parsing, correct_block_22382_json) {
    compare_bin_json(22382);
}

TEST(parsing, correct_block_22383_json) {
    compare_bin_json(22383);
}

TEST(parsing, correct_block_22384_json) {
    compare_bin_json(22384);
}

TEST(parsing, correct_block_22385_json) {
    compare_bin_json(22385);
}

TEST(parsing, correct_block_22386_json) {
    compare_bin_json(22386);
}

TEST(parsing, correct_block_22387_json) {
    compare_bin_json(22387);
}

TEST(parsing, correct_block_22388_json) {
    compare_bin_json(22388);
}

TEST(parsing, correct_block_22389_json) {
    compare_bin_json(22389);
}

TEST(parsing, correct_block_22390_json) {
    compare_bin_json(22390);
}

//TEST(parsing, block_chain) {
//
//    for (auto i = 15; i >= 0; i--) {
//        std::cout << "CHECKING BLOCK NUMBER: " << i << std::endl;
//
//        compare_bin_json(i);
//        std::cout << "END OF CHECKING BLOCK NUMBER: " << i << std::endl << std::endl;
//
//        if (i != 0) {
//            std::cout << "DOWNLOADING BLOCK NUMBER: " << i - 1 << std::endl;
//            std::ifstream f("resources/test-resources/blocks/" + std::to_string(i) + ".bin", std::ios::binary);
//            block b(f);
//
//            std::stringstream ss;
//            for (auto i : b.get_hash_prev_block()) {
//                ss << std::setfill('0') << std::setw(2) << std::hex << (int) i;
//            }
//
//            system(("resources/test-resources/blocks/download_blocks.sh " + ss.str() + " " +
//                    std::to_string(i - 1)).data());
//            std::cout << "END OF DOWNLOADING BLOCK NUMBER: " << i - 1 << std::endl << std::endl << std::endl
//                      << std::endl;
//        }
//    }
//}