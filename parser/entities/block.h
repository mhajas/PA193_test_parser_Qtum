//
// Created by mhajas on 10/13/17.
//

#ifndef PA193_QTUM_PARSER_BLOCK_H
#define PA193_QTUM_PARSER_BLOCK_H


#include <array>
#include <ostream>

class block {
    using hash_type = std::array<uint8_t, 32>;

    uint32_t _version;
    hash_type _hash_prev_block;
    hash_type _hash_merkle_root;

    uint32_t _n_time;
    uint32_t _n_bits;
    uint32_t _n_nonce;

    hash_type _hash_state_root;
    hash_type _hash_UTXO_root;

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

public:
    friend std::ostream& operator<<(std::ostream& os, const block& block1);
    friend std::istream& operator>>(std::istream& is, block& block1);
};


#endif //PA193_QTUM_PARSER_BLOCK_H
