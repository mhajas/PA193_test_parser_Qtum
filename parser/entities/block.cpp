//
// Created by mhajas on 10/13/17.
//

#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include "block.h"

namespace parsing_utils {
    enum RETURN_VALUE {SUCCESS, FAILURE};
    bool is_big_endian()
    {
        union {
            uint32_t i;
            char c[4];
        } bint = {0x01020304};

        return bint.c[0] == 1;
    }

    RETURN_VALUE parse_bytes(std::istream& is, void* storage, size_t number_of_bytes, bool is_big_endian) {
        if (!is_big_endian) {
            is.read(static_cast<char*>(storage), number_of_bytes);

            if (!is || is.gcount() != number_of_bytes) {
                std::cout << "Failed to read " << number_of_bytes << "bytes. Read only: " << is.gcount() << std::endl;
                return FAILURE;
            }

            return SUCCESS;
        }

        std::vector<char> buffer(number_of_bytes);
        is.read(buffer.data(), number_of_bytes);

        if (!is || is.gcount() != number_of_bytes) {
            std::cout << "Failed to read " << number_of_bytes << "bytes. Read only: " << is.gcount() << std::endl;
            return FAILURE;
        }

        std::reverse(std::begin(buffer), std::end(buffer));
        std::swap_ranges(std::begin(buffer), std::end(buffer), static_cast<char*>(storage));

        return SUCCESS;
    }

    RETURN_VALUE parse_reverse_bytes(std::istream& is, void* storage, size_t number_of_bytes, bool is_big_endian) {
        return parse_bytes(is, storage, number_of_bytes, !is_big_endian);
    }
}

std::ostream& operator<<(std::ostream& os, const block& block1) {
    os << "Version: " << block1._version << std::endl;
    os << "Hash previous block: ";
    for (auto it : block1._hash_prev_block) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl << "Merkle root: ";
    for (auto it : block1._hash_merkle_root) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;

    os << std::endl;
    os << "nTime: " << block1._n_time << std::endl;
    os << "nBits: " << block1._n_bits << std::endl;
    os << "nNonce: " << block1._n_nonce << std::endl;

    os << "Hash state root: ";
    for (auto it : block1._hash_state_root) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl;
    os << "Hash UTXO root: ";
    for (auto it : block1._hash_UTXO_root) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl << std::endl;

    os << "Proof-of-stake specific fields: " << std::endl;
    os << "Hash prevout stake: ";
    for (auto it : block1._hash_prevout_stake) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl;
    os << "Index n of prevout stake: " << block1._n_prevout_stake << std::endl;
    os << "vch Block signature: " ;
    for (auto it = block1._vch_block_sig.begin(); it != block1._vch_block_sig.end(); it++) os << std::setfill('0') << std::setw(2) << std::hex << (int) *it;
    os << std::endl << std::endl;

    os << "Number of transactions: " << (int) block1._number_of_transactions << std::endl;

    os << "First transaction" << std::endl;
    os << "Version: " << (int) block1._n_version_coinbase << std::endl;
    os << "Index n: " << (int) block1._n_coinbase << std::endl;
    os << "CScript: " ;
    for (auto it = block1._cscript_coinbase.begin(); it != block1._cscript_coinbase.end(); it++) os << std::setfill('0') << std::setw(2) << std::hex << (int) *it;
    os << std::endl << "N sequence: " << (int) block1._n_seq_coinbase << std::endl;
    os << "Script pub key: " ;
    for (auto it = block1._script_pub_key_coinbase.begin(); it != block1._script_pub_key_coinbase.end(); it++) os << std::setfill('0') << std::setw(2) << std::hex << (int) *it;
    os << std::endl;

    return os;
}

std::istream& operator>>(std::istream& is, block& block1) {
    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._version), sizeof(block1._version), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read Version" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(block1._hash_prev_block.data()), block1._hash_prev_block.size(), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read hash of previous block" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(block1._hash_merkle_root.data()), block1._hash_merkle_root.size(), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read merkle root hash" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._n_time), sizeof(block1._n_time), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read nTime" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._n_bits), sizeof(block1._n_bits), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read nBits" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._n_nonce), sizeof(block1._n_nonce), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read nNonce" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(block1._hash_state_root.data()), block1._hash_state_root.size(), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read state root hash" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(block1._hash_UTXO_root.data()), block1._hash_UTXO_root.size(), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read UTXO root hash" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(block1._hash_prevout_stake.data()), block1._hash_prevout_stake.size(), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read prevout stake hash" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._n_prevout_stake), sizeof(block1._n_prevout_stake), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read index n of prevout stake" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._vch_block_sig_size), sizeof(block1._vch_block_sig_size), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read size of block signature" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    block1._vch_block_sig.resize(block1._vch_block_sig_size);

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(&(block1._vch_block_sig[0])), block1._vch_block_sig_size, parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read vch block signature" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._number_of_transactions), sizeof(block1._number_of_transactions), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read number of transactions" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._n_version_coinbase), sizeof(block1._n_version_coinbase), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read version of first transaction" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(block1._unknown_val_1.data()), block1._unknown_val_1.size(), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read unknown val 1" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._n_coinbase), sizeof(block1._n_coinbase), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read index n of coinbase" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._cscript_size_coinbase), sizeof(block1._cscript_size_coinbase), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read size of cscript" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    block1._cscript_coinbase.resize(block1._cscript_size_coinbase);

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&(block1._cscript_coinbase[0])), block1._cscript_size_coinbase, parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read script of coinbase" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._n_seq_coinbase), sizeof(block1._n_seq_coinbase), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read n sequence of coinbase" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(block1._unknown_val_2.data()), block1._unknown_val_2.size(), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read unknown val 2" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._script_pub_key_size_coinbase), sizeof(block1._script_pub_key_size_coinbase), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read size of script pub key" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    block1._script_pub_key_coinbase.resize(block1._script_pub_key_size_coinbase);

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&(block1._script_pub_key_coinbase[0])), block1._script_pub_key_size_coinbase, parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read script pub key of coinbase" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(block1._unknown_val_3.data()), block1._unknown_val_3.size(), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read unknown val 3" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    return is;
}

block::block() : _version(0) {}

block::block(std::istream& stream) {
    stream >> *this;
}

uint32_t block::get_version() const {
    return _version;
}

const block::hash_type &block::get_hash_prev_block() const {
    return _hash_prev_block;
}

const block::hash_type &block::get_hash_merkle_root() const {
    return _hash_merkle_root;
}

uint32_t block::get_n_time() const {
    return _n_time;
}

uint32_t block::get_n_bits() const {
    return _n_bits;
}

uint32_t block::get_n_nonce() const {
    return _n_nonce;
}

const block::hash_type &block::get_hash_state_root() const {
    return _hash_state_root;
}

const block::hash_type &block::get_hash_UTXO_root() const {
    return _hash_UTXO_root;
}

const block::hash_type &block::get_hash_prevout_stake() const {
    return _hash_prevout_stake;
}

uint32_t block::get_n_prevout_stake() const {
    return _n_prevout_stake;
}

uint8_t block::get_vch_block_sig_size() const {
    return _vch_block_sig_size;
}

const block::vector_type &block::get_vch_block_sig() const {
    return _vch_block_sig;
}

uint8_t block::get_number_of_transactions() const {
    return _number_of_transactions;
}
