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

    std::array<uint8_t, 32> hash_state_root_test = {0x2f, 0xb5, 0xde, 0x60, 0xde, 0x4c, 0x96, 0x1e,
                                                    0xa8, 0x09, 0xc9, 0x6d, 0x4d, 0x83, 0xfe, 0xad,
                                                    0x82, 0x15, 0xac, 0xb4, 0xce, 0x92, 0x27, 0xa4,
                                                    0xde, 0x3c, 0x3c, 0x7b, 0x42, 0x9e, 0xf6, 0xa0};
    EXPECT_EQ(hash_state_root_test, b.get_hash_state_root()) << "Hash state root was not parsed correctly";

    std::array<uint8_t, 32> hash_UTXO_root_test = {0x21, 0xb4, 0x63, 0xe3, 0xb5, 0x2f, 0x62, 0x01,
                                                   0xc0, 0xad, 0x6c, 0x99, 0x1b, 0xe0, 0x48, 0x5b,
                                                   0x6e, 0xf8, 0xc0, 0x92, 0xe6, 0x45, 0x83, 0xff,
                                                   0xa6, 0x55, 0xcc, 0x1b, 0x17, 0x1f, 0xe8, 0x56};
    EXPECT_EQ(hash_UTXO_root_test, b.get_hash_UTXO_root()) << "Hash UTXO root was not parsed correctly";

    std::array<uint8_t, 32> hash_prevout_stake = {0x0e, 0xee, 0xc9, 0x9b, 0xec, 0xef, 0x18, 0xff,
                                                  0x6f, 0x65, 0x23, 0x74, 0xa7, 0xa4, 0xa2, 0xf3,
                                                  0x79, 0xec, 0x32, 0x10, 0x52, 0x47, 0x0c, 0x23,
                                                  0xc0, 0x78, 0xeb, 0x6c, 0xa0, 0x15, 0x3f, 0x8b};
    EXPECT_EQ(hash_prevout_stake, b.get_prevout_stake().get_hash()) << "Hash prevout stake was not parsed correctly";

    EXPECT_EQ(1,  b.get_prevout_stake().get_index_n()) << "Index n of prevout stake wasn't parsed correctly";

    std::vector<uint8_t> vch_block_signature =  {0xc4, 0xc6, 0x53, 0x39, 0xb0, 0x03, 0x4d, 0xe1,
                                                 0x09, 0xd7, 0x78, 0x5a, 0x24, 0xdc, 0xc5, 0x6d,
                                                 0x8f, 0xc3, 0x29, 0x96, 0xd0, 0x40, 0xe7, 0x4d,
                                                 0x9c, 0x6f, 0x5f, 0x91, 0x80, 0x63, 0x1b, 0x61,
                                                 0x20, 0x02, 0x66, 0xb8, 0xc9, 0x2a, 0xb3, 0xd6,
                                                 0x14, 0x78, 0x31, 0xeb, 0xe9, 0x63, 0x7d, 0x83,
                                                 0x19, 0x88, 0x22, 0x4c, 0xa1, 0xd8, 0xc1, 0xdf,
                                                 0x20, 0xd1, 0x18, 0x8e, 0xbf, 0x88, 0x03, 0x82,
                                                 0x2e, 0xc4, 0x00, 0x21, 0x02, 0x45, 0x30};
    EXPECT_EQ(vch_block_signature, b.get_vch_block_sig()) << "Block signature was not parsed correctly";

    EXPECT_EQ(71, b.get_vch_block_sig_size()) << "Block signature size wasn't parsed correctly";

    EXPECT_EQ(2, b.get_number_of_transactions()) << "Number of transactions wasn't parsed correctly";

    //test first transaction
    EXPECT_EQ(2, b.get_ft_version()) << "First transaction version wasn't parsed correctly";

    EXPECT_EQ(22380, b.get_block_height()) << "First transaction height wasn't parsed correctly";

    EXPECT_EQ(4294967295, b.get_ft_sequence()) << "First transaction sequence wasn't parsed correctly";

    auto co = b.get_ft_ctxouts();
    auto ctxout = co.begin();

    EXPECT_EQ(0, ctxout.base()->get_amount()) << "First transaction ctxout amount (0) wasn't parsed correctly";

    ctxout++;
    EXPECT_EQ(0, ctxout.base()->get_amount()) << "First transaction ctxout amount (1) wasn't parsed correctly";

    ctxout++;
    EXPECT_EQ(ctxout, co.end()) << "First transaction number of ctxouts wasn't parsed correctly";

    EXPECT_EQ(2, b.get_ft_ctxout_number()) << "First transaction ctxout number wasn't parsed correctly";

    EXPECT_EQ(0, b.get_ft_n_time()) << "First transaction time number wasn't parsed correctly";

    //test second transaction
    auto ts = b.get_transactions();
    auto t = ts.begin();

    EXPECT_EQ(2, t.base()->get_version()) << "Second transaction version wasn't parsed correctly";

    EXPECT_EQ(0, t.base()->get_lock_time()) << "Second transaction lock time wasn't parsed correctly";

    EXPECT_EQ(1, t.base()->get_vin_count()) << "Second transaction vin count wasn't parsed correctly";

    auto vins = t.base()->get_vin();
    auto vin = vins.begin();

    EXPECT_EQ(4294967295, vin.base()->get_sequence()) << "Second transaction sequence wasn't parsed correctly";

    auto vin_script = vin.base()->get_pub_key_script();

    EXPECT_EQ(72, vin_script.get_size()) << "Second transaction c_script size wasn't parsed correctly";

    EXPECT_EQ(71, vin_script.get_storage_size()) << "Second transaction c_script storage size wasn't parsed correctly";

    //add checking vin_script flags and storage

    auto prevout = vin.base()->get_prevout();

    EXPECT_EQ(1, prevout.get_index_n()) << "Second transaction prevout index n wasn't parsed correctly";

    EXPECT_EQ(hash_prevout_stake, prevout.get_hash())<< "Second transaction prevout hash size wasn't parsed correctly";

    vin++;
    EXPECT_EQ(vin, vins.end()) << "Second transaction number of vins wasn't parsed correctly";

    EXPECT_EQ(11, t.base()->get_vout_count()) << "Second transaction vout count wasn't parsed correctly";

    auto vouts = t.base()->get_vout();
    auto vout = vouts.begin();
    auto vout_script = vout.base()->get_pub_key_script();

    EXPECT_EQ(0, vout_script.get_size()) << "Second transaction c_script size wasn't parsed correctly";

    EXPECT_EQ(192, vout_script.get_storage_size()) << "Second transaction c_script storage size wasn't parsed correctly";

    //add checking vout_script flags and storage

    EXPECT_EQ(0, vout.base()->get_amount()) << "Second transaction vout (0) amount wasn't parsed correctly";

    vout = vout + 11; //add checking the others vouts
    EXPECT_EQ(vout, vouts.end()) << "Second transaction number of vouts wasn't parsed correctly";

    t++;
    EXPECT_EQ(t++, ts.end()) << "Num transactions wasn't parse correctly";
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

    std::array<uint8_t, 32> hash_state_root_test = {0x2f, 0xb5, 0xde, 0x60, 0xde, 0x4c, 0x96, 0x1e,
                                                    0xa8, 0x09, 0xc9, 0x6d, 0x4d, 0x83, 0xfe, 0xad,
                                                    0x82, 0x15, 0xac, 0xb4, 0xce, 0x92, 0x27, 0xa4,
                                                    0xde, 0x3c, 0x3c, 0x7b, 0x42, 0x9e, 0xf6, 0xa0};
    EXPECT_EQ(hash_state_root_test, b.get_hash_state_root()) << "Hash state root was not parsed correctly";

    std::array<uint8_t, 32> hash_UTXO_root_test = {0x21, 0xb4, 0x63, 0xe3, 0xb5, 0x2f, 0x62, 0x01,
                                                   0xc0, 0xad, 0x6c, 0x99, 0x1b, 0xe0, 0x48, 0x5b,
                                                   0x6e, 0xf8, 0xc0, 0x92, 0xe6, 0x45, 0x83, 0xff,
                                                   0xa6, 0x55, 0xcc, 0x1b, 0x17, 0x1f, 0xe8, 0x56};
    EXPECT_EQ(hash_UTXO_root_test, b.get_hash_UTXO_root()) << "Hash UTXO root was not parsed correctly";

    std::array<uint8_t, 32> hash_prevout_stake = {0x53, 0x74, 0x73, 0xa5, 0x8c, 0xb2, 0xf1, 0x07,
                                                  0x88, 0xbd, 0x68, 0xda, 0xfd, 0x60, 0x97, 0xcc,
                                                  0x4d, 0x6a, 0x6a, 0xc4, 0xe4, 0xf2, 0xfb, 0x81,
                                                  0x25, 0xc3, 0x1c, 0x17, 0x3d, 0xae, 0x81, 0xfd};
    EXPECT_EQ(hash_prevout_stake, b.get_prevout_stake().get_hash()) << "Hash prevout stake was not parsed correctly";

    EXPECT_EQ(2,  b.get_prevout_stake().get_index_n()) << "Index n of prevout stake wasn't parsed correctly";

    std::vector<uint8_t> vch_block_signature =  {0x83, 0x96, 0x7a, 0xec, 0x72, 0x92, 0x71, 0x5c,
                                                 0x84, 0x0b, 0x43, 0xfd, 0xd3, 0x94, 0xc1, 0xf2,
                                                 0x6b, 0xe4, 0x6f, 0xf3, 0x2a, 0x2e, 0xca, 0x22,
                                                 0xe0, 0x43, 0xf2, 0x64, 0xdc, 0x9e, 0x89, 0x46,
                                                 0x20, 0x02, 0xf9, 0x8b, 0x10, 0xba, 0xc0, 0xa7,
                                                 0x11, 0xb8, 0x2a, 0x5e, 0x42, 0xd7, 0x97, 0x56,
                                                 0x8c, 0x9d, 0x26, 0xa9, 0x06, 0xd4, 0x5b, 0xb7,
                                                 0x32, 0x6a, 0x7b, 0xf7, 0xfc, 0x44, 0xc6, 0x2a,
                                                 0x37, 0x93, 0x00, 0x21, 0x02, 0x45, 0x30};
    EXPECT_EQ(vch_block_signature, b.get_vch_block_sig()) << "Block signature was not parsed correctly";

    EXPECT_EQ(2, b.get_number_of_transactions()) << "Number of transactions wasn't parsed correctly";
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

    std::array<uint8_t, 32> hash_state_root_test = {0x2f, 0xb5, 0xde, 0x60, 0xde, 0x4c, 0x96, 0x1e,
                                                    0xa8, 0x09, 0xc9, 0x6d, 0x4d, 0x83, 0xfe, 0xad,
                                                    0x82, 0x15, 0xac, 0xb4, 0xce, 0x92, 0x27, 0xa4,
                                                    0xde, 0x3c, 0x3c, 0x7b, 0x42, 0x9e, 0xf6, 0xa0};
    EXPECT_EQ(hash_state_root_test, b.get_hash_state_root()) << "Hash state root was not parsed correctly";

    std::array<uint8_t, 32> hash_UTXO_root_test = {0x21, 0xb4, 0x63, 0xe3, 0xb5, 0x2f, 0x62, 0x01,
                                                   0xc0, 0xad, 0x6c, 0x99, 0x1b, 0xe0, 0x48, 0x5b,
                                                   0x6e, 0xf8, 0xc0, 0x92, 0xe6, 0x45, 0x83, 0xff,
                                                   0xa6, 0x55, 0xcc, 0x1b, 0x17, 0x1f, 0xe8, 0x56};
    EXPECT_EQ(hash_UTXO_root_test, b.get_hash_UTXO_root()) << "Hash UTXO root was not parsed correctly";

    std::array<uint8_t, 32> hash_prevout_stake = {0x9f, 0x53, 0x16, 0x95, 0xd1, 0xfe, 0x17, 0xec,
                                                  0x92, 0x43, 0xf6, 0xe8, 0xb4, 0xe7, 0xce, 0x66,
                                                  0x89, 0x42, 0x04, 0x91, 0x93, 0xc3, 0xf0, 0x1d,
                                                  0x0e, 0xf3, 0xf8, 0x47, 0xa7, 0xbd, 0xb7, 0x71};
    EXPECT_EQ(hash_prevout_stake, b.get_prevout_stake().get_hash()) << "Hash prevout stake was not parsed correctly";

    EXPECT_EQ(2,  b.get_prevout_stake().get_index_n()) << "Index n of prevout stake wasn't parsed correctly";

    std::vector<uint8_t> vch_block_signature =  {0xba, 0x1c, 0x7e, 0x78, 0x14, 0x8d, 0x8d, 0x84,
                                                 0x14, 0x62, 0x34, 0x63, 0x22, 0x86, 0xad, 0xde,
                                                 0x21, 0x9a, 0x45, 0xfd, 0x24, 0x8a, 0x7e, 0xf2,
                                                 0xf3, 0x31, 0x3e, 0x74, 0x59, 0x07, 0x1e, 0x7e,
                                                 0x20, 0x02, 0x9b, 0x19, 0xf7, 0x8e, 0xaf, 0x24,
                                                 0x8f, 0xee, 0x80, 0x3a, 0xe2, 0x5f, 0x98, 0x8b,
                                                 0x8a, 0x04, 0xc5, 0x57, 0xc8, 0xd3, 0x5f, 0xee,
                                                 0x94, 0x85, 0x5d, 0xd9, 0xc3, 0xe0, 0x9d, 0xf3,
                                                 0x45, 0x70, 0x20, 0x02, 0x44, 0x30};
    EXPECT_EQ(vch_block_signature, b.get_vch_block_sig()) << "Block signature was not parsed correctly";

    EXPECT_EQ(3, b.get_number_of_transactions()) << "Number of transactions wasn't parsed correctly";

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

    std::array<uint8_t, 32> hash_state_root_test = {0x2f, 0xb5, 0xde, 0x60, 0xde, 0x4c, 0x96, 0x1e,
                                                    0xa8, 0x09, 0xc9, 0x6d, 0x4d, 0x83, 0xfe, 0xad,
                                                    0x82, 0x15, 0xac, 0xb4, 0xce, 0x92, 0x27, 0xa4,
                                                    0xde, 0x3c, 0x3c, 0x7b, 0x42, 0x9e, 0xf6, 0xa0};
    EXPECT_EQ(hash_state_root_test, b.get_hash_state_root()) << "Hash state root was not parsed correctly";

    std::array<uint8_t, 32> hash_UTXO_root_test = {0x21, 0xb4, 0x63, 0xe3, 0xb5, 0x2f, 0x62, 0x01,
                                                   0xc0, 0xad, 0x6c, 0x99, 0x1b, 0xe0, 0x48, 0x5b,
                                                   0x6e, 0xf8, 0xc0, 0x92, 0xe6, 0x45, 0x83, 0xff,
                                                   0xa6, 0x55, 0xcc, 0x1b, 0x17, 0x1f, 0xe8, 0x56};
    EXPECT_EQ(hash_UTXO_root_test, b.get_hash_UTXO_root()) << "Hash UTXO root was not parsed correctly";

    std::array<uint8_t, 32> hash_prevout_stake = {0x5c, 0xc7, 0xc4, 0x5d, 0xd2, 0x9f, 0xf8, 0x25,
                                                  0xfc, 0x92, 0x65, 0x9c, 0x48, 0x0d, 0xb1, 0xc3,
                                                  0x82, 0xc5, 0x57, 0xb0, 0xc1, 0x14, 0xf6, 0xb5,
                                                  0xdd, 0xff, 0xfb, 0xfb, 0x30, 0x75, 0xd8, 0x70};
    EXPECT_EQ(hash_prevout_stake, b.get_prevout_stake().get_hash()) << "Hash prevout stake was not parsed correctly";

    EXPECT_EQ(0,  b.get_prevout_stake().get_index_n()) << "Index n of prevout stake wasn't parsed correctly";

    std::vector<uint8_t> vch_block_signature =  {0x1a, 0xcc, 0x56, 0x36, 0x33, 0x6f, 0xb2, 0x80,
                                                 0x16, 0xe9, 0x42, 0xc6, 0x21, 0xb5, 0x10, 0x90,
                                                 0xa0, 0xe7, 0xbf, 0x8f, 0x58, 0x6c, 0x9a, 0xba,
                                                 0x98, 0x1b, 0x87, 0xd4, 0xf2, 0x77, 0x52, 0x48,
                                                 0x20, 0x02, 0xff, 0xfa, 0x37, 0x61, 0xaf, 0x3e,
                                                 0xe9, 0x46, 0x34, 0x1b, 0x0f, 0x29, 0xda, 0xd4,
                                                 0x53, 0x2b, 0x44, 0x29, 0x3a, 0x1f, 0x77, 0xbe,
                                                 0xc6, 0x5f, 0x2a, 0xab, 0x06, 0x25, 0xb8, 0xce,
                                                 0xe8, 0xb2, 0x00, 0x21, 0x02, 0x45, 0x30};
    EXPECT_EQ(vch_block_signature, b.get_vch_block_sig()) << "Block signature was not parsed correctly";

    EXPECT_EQ(4, b.get_number_of_transactions()) << "Number of transactions wasn't parsed correctly";
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

    std::array<uint8_t, 32> hash_state_root_test = {0x2f, 0xb5, 0xde, 0x60, 0xde, 0x4c, 0x96, 0x1e,
                                                    0xa8, 0x09, 0xc9, 0x6d, 0x4d, 0x83, 0xfe, 0xad,
                                                    0x82, 0x15, 0xac, 0xb4, 0xce, 0x92, 0x27, 0xa4,
                                                    0xde, 0x3c, 0x3c, 0x7b, 0x42, 0x9e, 0xf6, 0xa0};
    EXPECT_EQ(hash_state_root_test, b.get_hash_state_root()) << "Hash state root was not parsed correctly";

    std::array<uint8_t, 32> hash_UTXO_root_test = {0x21, 0xb4, 0x63, 0xe3, 0xb5, 0x2f, 0x62, 0x01,
                                                   0xc0, 0xad, 0x6c, 0x99, 0x1b, 0xe0, 0x48, 0x5b,
                                                   0x6e, 0xf8, 0xc0, 0x92, 0xe6, 0x45, 0x83, 0xff,
                                                   0xa6, 0x55, 0xcc, 0x1b, 0x17, 0x1f, 0xe8, 0x56};
    EXPECT_EQ(hash_UTXO_root_test, b.get_hash_UTXO_root()) << "Hash UTXO root was not parsed correctly";

    std::array<uint8_t, 32> hash_prevout_stake = {0xab, 0xce, 0xde, 0xb9, 0x3f, 0x59, 0xa6, 0x0c,
                                                  0x09, 0x84, 0xe9, 0xe9, 0x2e, 0x8c, 0xe6, 0x8b,
                                                  0xa9, 0xb3, 0xae, 0x62, 0x48, 0xbf, 0x0d, 0xf4,
                                                  0xe8, 0x3e, 0x9e, 0xa5, 0x04, 0xbc, 0x94, 0x7e};
    EXPECT_EQ(hash_prevout_stake, b.get_prevout_stake().get_hash()) << "Hash prevout stake was not parsed correctly";

    EXPECT_EQ(1,  b.get_prevout_stake().get_index_n()) << "Index n of prevout stake wasn't parsed correctly";

    std::vector<uint8_t> vch_block_signature =  {0x45, 0xe9, 0x1c, 0x32, 0x3c, 0xf2, 0xe1, 0xa6,
                                                 0xa7, 0x62, 0x5f, 0x7b, 0xc0, 0xbc, 0xd3, 0xb6,
                                                 0x23, 0x5c, 0x53, 0xd5, 0xf1, 0xb9, 0xc3, 0x0e,
                                                 0x72, 0xe8, 0xd3, 0xe5, 0xa1, 0x8a, 0x56, 0x5d,
                                                 0x20, 0x02, 0x2b, 0xac, 0x33, 0x65, 0x11, 0x9d,
                                                 0xa6, 0xe7, 0xc7, 0xb4, 0x8e, 0xa8, 0x2b, 0xf4,
                                                 0x99, 0xe4, 0xb1, 0xca, 0xc7, 0xbb, 0x20, 0xe5,
                                                 0x1c, 0x4d, 0xfd, 0x5e, 0x18, 0x32, 0x82, 0xe8,
                                                 0x25, 0x25, 0x20, 0x02, 0x44, 0x30};
    EXPECT_EQ(vch_block_signature, b.get_vch_block_sig()) << "Block signature was not parsed correctly";

    EXPECT_EQ(3, b.get_number_of_transactions()) << "Number of transactions wasn't parsed correctly";
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

    std::array<uint8_t, 32> hash_state_root_test = {0x2f, 0xb5, 0xde, 0x60, 0xde, 0x4c, 0x96, 0x1e,
                                                    0xa8, 0x09, 0xc9, 0x6d, 0x4d, 0x83, 0xfe, 0xad,
                                                    0x82, 0x15, 0xac, 0xb4, 0xce, 0x92, 0x27, 0xa4,
                                                    0xde, 0x3c, 0x3c, 0x7b, 0x42, 0x9e, 0xf6, 0xa0};
    EXPECT_EQ(hash_state_root_test, b.get_hash_state_root()) << "Hash state root was not parsed correctly";

    std::array<uint8_t, 32> hash_UTXO_root_test = {0x21, 0xb4, 0x63, 0xe3, 0xb5, 0x2f, 0x62, 0x01,
                                                   0xc0, 0xad, 0x6c, 0x99, 0x1b, 0xe0, 0x48, 0x5b,
                                                   0x6e, 0xf8, 0xc0, 0x92, 0xe6, 0x45, 0x83, 0xff,
                                                   0xa6, 0x55, 0xcc, 0x1b, 0x17, 0x1f, 0xe8, 0x56};
    EXPECT_EQ(hash_UTXO_root_test, b.get_hash_UTXO_root()) << "Hash UTXO root was not parsed correctly";

    std::array<uint8_t, 32> hash_prevout_stake = {0x3c, 0x4f, 0x43, 0x51, 0x1c, 0x4a, 0xe3, 0xe5, 
                                                  0xd9, 0xe2, 0x24, 0x2b, 0xb6, 0xe8, 0x31, 0x45,
                                                  0xb1, 0xc5, 0x5e, 0x8d, 0xe4, 0x45, 0xc8, 0xa6,
                                                  0x88, 0x1e, 0x2b, 0x31, 0xa7, 0x0c, 0xcf, 0x39};
    EXPECT_EQ(hash_prevout_stake, b.get_prevout_stake().get_hash()) << "Hash prevout stake was not parsed correctly";

    EXPECT_EQ(1,  b.get_prevout_stake().get_index_n()) << "Index n of prevout stake wasn't parsed correctly";

    std::vector<uint8_t> vch_block_signature =  {0xe2, 0x8c, 0xfa, 0x23, 0xca, 0xec, 0xc7, 0xdf, 
                                                 0x02, 0xd4, 0x91, 0xad, 0x9e, 0x45, 0xa6, 0x24,
                                                 0x26, 0x15, 0x72, 0x9d, 0x0c, 0x50, 0xa1, 0xe5,
                                                 0xa4, 0x21, 0xb0, 0x4b, 0x7a, 0x68, 0xf9, 0x19, 
                                                 0x20, 0x02, 0x42, 0x3a, 0x23, 0x0b, 0xc9, 0xb2,
                                                 0x48, 0x84, 0x6c, 0x6b, 0x18, 0xbb, 0xdc, 0x60,
                                                 0xc3, 0x2f, 0x0f, 0x3c, 0x7c, 0x1c, 0x9f, 0x22,
                                                 0xd3, 0xef, 0x2a, 0x7f, 0x3f, 0xb0, 0x0d, 0xb7,
                                                 0xe4, 0x93, 0x00, 0x21, 0x02, 0x45, 0x30};
    EXPECT_EQ(vch_block_signature, b.get_vch_block_sig()) << "Block signature was not parsed correctly";

    EXPECT_EQ(6, b.get_number_of_transactions()) << "Number of transactions wasn't parsed correctly";
}