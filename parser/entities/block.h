//
// Created by mhajas on 10/13/17.
//

#ifndef PA193_QTUM_PARSER_BLOCK_H
#define PA193_QTUM_PARSER_BLOCK_H


#include <array>
#include <ostream>
#include <vector>

class block {
    using hash_type = std::array<uint8_t, 32>;
    using vector_type = std::vector<uint8_t>;

    uint32_t _version;
    hash_type _hash_prev_block;
    hash_type _hash_merkle_root;

    uint32_t _n_time;
    uint32_t _n_bits;
    uint32_t _n_nonce;

    hash_type _hash_state_root;
    hash_type _hash_UTXO_root;

    hash_type _hash_prevout_stake;
    uint32_t _n_prevout_stake;

    uint8_t _vch_block_sig_size;
    vector_type _vch_block_sig;

    uint8_t _number_of_transactions;

    //First transaction - coin for miner
    uint32_t _n_version_coinbase;
    std::array<uint8_t, 35> _unknown_val_1;
    uint32_t _n_coinbase;
    uint8_t _cscript_size_coinbase;
    vector_type _cscript_coinbase;
    uint32_t _n_seq_coinbase;
    std::array<uint8_t, 20> _unknown_val_2;
    uint8_t _script_pub_key_size_coinbase;
    vector_type _script_pub_key_coinbase;
    std::array<uint8_t, 38> _unknown_val_3;


public:
    block();
    explicit block(std::istream& stream);

    uint32_t get_version() const;

    const hash_type& get_hash_prev_block() const;

    const hash_type& get_hash_merkle_root() const;

    uint32_t get_n_time() const;

    uint32_t get_n_bits() const;

    uint32_t get_n_nonce() const;

    const hash_type& get_hash_state_root() const;

    const hash_type& get_hash_UTXO_root() const;

    const hash_type& get_hash_prevout_stake() const;

    uint32_t get_n_prevout_stake() const;

    uint8_t get_vch_block_sig_size() const;

    const vector_type& get_vch_block_sig() const;

    uint8_t get_number_of_transactions() const;

public:
    friend std::ostream& operator<<(std::ostream& os, const block& block1);
    friend std::istream& operator>>(std::istream& is, block& block1);
};


#endif //PA193_QTUM_PARSER_BLOCK_H
