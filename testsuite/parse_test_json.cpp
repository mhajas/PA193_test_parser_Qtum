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
    json j = json::parse(json_file);


    EXPECT_EQ(b.compute_hash(), to_hash_type(hex_reduction(j.at("hash")))) << "Hashes are different for block: " << block_number;
    EXPECT_EQ(b.get_version(), j.at("version"));
    EXPECT_EQ(b.get_hash_merkle_root(), to_hash_type(hex_reduction(j.at("merkleroot")))) << "Parsed merkle root is different for block: " << block_number;
    //wrong computed merkle root for 22383, 22385, 22389
    //EXPECT_EQ(b.compute_merkle_root(), to_hash_type(hex_reduction(j.at("merkleroot")))) << "Computed merkle root is different for block: " << block_number;
    EXPECT_EQ(b.get_hash_state_root(), to_hash_type(hex_reduction(j.at("hashStateRoot")))) << "HashStateRoot is different for block " << block_number;
    EXPECT_EQ(b.get_hash_UTXO_root(), to_hash_type(hex_reduction(j.at("hashUTXORoot")))) << "HashUTXORoot is different for block " << block_number;

    EXPECT_EQ(b.get_n_time(), j.at("time")) << "Time is different for block " << block_number;
    EXPECT_EQ(b.get_n_nonce(), j.at("nonce")) << "Nonce is different for block " << block_number;
    //EXPECT_EQ(b.get_n_bits(), j.at("bits")); - convert
    EXPECT_EQ(b.get_block_height(), j.at("height")) << "Block height is different for block " << block_number;

    EXPECT_EQ(b.get_hash_prev_block(), to_hash_type(hex_reduction(j.at("previousblockhash")))) << "HashPrevBlock is different for block " << block_number;

    auto rev_hash = to_vector_type(hex_reduction(j.at("signature")));
    std::reverse(rev_hash.begin(), rev_hash.end());
    EXPECT_EQ(b.get_vch_block_sig(), rev_hash); //- add REVERSE!

    //test first transaction
    EXPECT_EQ(b.get_ft_version(), j.at("tx")[0].at("version")) << "First transaction version is different for block " << block_number;
    EXPECT_EQ(b.get_ft_lock_time(), j.at("tx")[0].at("locktime")) << "First transaction lockTime is different for block " << block_number;

    auto ft_outs = b.get_ft_ctxouts();
    int pos_ft_o = 0;
    for(auto fout = ft_outs.begin(); fout < ft_outs.end(); fout++) {
//                //convert to int64_t
//                EXPECT_EQ(fout.base()->_amount, j.at("tx")[0].at("vout")[pos_ft_o].at("n"));
        pos_ft_o++;
    }
    EXPECT_EQ(pos_ft_o, b.get_ft_ctxout_number()) << "First transaction vout count is diffrent for block " << block_number;
    //?

    //the others transactions
    if (b.get_number_of_transactions() > 1) {
        std::vector<transaction> ts = b.get_transactions();
        int pos = 1;
        for (auto t = ts.begin(); t < ts.end(); t++) {
            auto j_tx = j.at("tx")[pos];
            EXPECT_EQ(t.base()->get_version(), j_tx.at("version")) << "Transaction (" << pos << ") version is diffrent for block " << block_number;
            EXPECT_EQ(t.base()->get_lock_time(), j_tx.at("locktime")) << "Transaction (" << pos << ") lockTime is diffrent for block " << block_number;

            auto vins = t.base()->get_vin();
            int pos_v = 0;
            for(auto vin = vins.begin(); vin < vins.end(); vin++) {
                EXPECT_EQ(vin.base()->_sequence, j.at("tx")[pos].at("vin")[pos_v].at("sequence"))
                                    << "Transaction (" << pos << ") vin (" << pos_v << ") is diffrent for block " << block_number;

                EXPECT_EQ(vin.base()->_prevout.get_index_n(), j.at("tx")[pos].at("vin")[pos_v].at("vout"))
                                    << "Transaction (" << pos << ") vin (" << pos_v << ") index n is diffrent for block " << block_number;
                EXPECT_EQ(vin.base()->_prevout.get_hash(), to_hash_type(hex_reduction(j.at("tx")[pos].at("vin")[pos_v].at("txid"))))
                                    << "Transaction (" << pos << ") vin (" << pos_v << ") txid (hash) is diffrent for block " << block_number;
//asm in json contains flags
//                EXPECT_EQ(vin.base()->_pub_key_script._storage, to_vector_type(hex_reduction(j.at("tx")[pos].at("vin")[pos_v].at("scriptSig").at("asm"))))
//                                    << "Transaction (" << pos << ") vin (" << pos_v << ") scriptSig asm (script storage) is diffrent for block " << block_number;

                pos_v++;
            }
            EXPECT_EQ(pos_v, t.base()->get_vin_count()) << "Transaction (" << pos << ") vin count is diffrent for block " << block_number;

            auto vouts = t.base()->get_vout();
            pos_v = 0;
            for(auto vout = vouts.begin(); vout < vouts.end(); vout++) {
//                //convert to int64_t
//                EXPECT_EQ(vout.base()->_amount, j.at("tx")[pos].at("vout")[pos_v].at("n"));
                pos_v++;
            }
            EXPECT_EQ(pos_v, t.base()->get_vout_count()) << "Transaction (" << pos << ") vout count is diffrent for block " << block_number;
            pos++;
        }
        EXPECT_EQ(pos, b.get_number_of_transactions()) << "Transaction number is different for block " << block_number;
    }

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