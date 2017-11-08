//
// Created by mhajas on 10/13/17.
//

#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include "block.h"
#include "../external/crypto/sha256.h"
#include "../utils/parsing_utils.h"
#include "../utils/hash_utils.h"

std::ostream& operator<<(std::ostream& os, const block& block1) {
    os << "Version: " << block1._version << std::endl;
    os << "Hash previous block: ";
    for (auto it : block1._hash_prev_block) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl << "Merkle root: ";
    for (auto it : block1._hash_merkle_root) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl << "Computed Merkle root: ";
    for (auto it : block1.compute_merkle_root()) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
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
    for (auto it : block1._prevout_stake.get_hash()) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl;
    os << "Index n of prevout stake: " << block1._prevout_stake.get_index_n() << std::endl;
    os << "vch Block signature: " ;
    for (auto it : block1._vch_block_sig) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl << std::endl;

    os << "Number of transactions: " << (int) block1._number_of_transactions << std::endl << std::endl;

    os  << "First transaction (coin for miner)" << std::endl;
    os << "Version: " << (int) block1._ft_version << std::endl;
    os << "Height: " << std::dec << block1._block_height << std::endl;
    os << "CScript " << std::endl;
    for (auto it : block1._ft_ctxouts) os << it;
    os << "Time: " << block1._ft_lock_time << std::endl;
    os << "Transaction hash: ";
    for (auto it : block1.compute_first_transaction_hash()) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl;

    int i = 2;
    for (auto it : block1._transactions) {
        os << "Transaction number " << i++ << ":" << std::endl << it;
    }

    os << "Block Hash: " ;
    for (auto it : block1.compute_hash()) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
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

    is >> block1._prevout_stake;

    if (!is) {
        std::cout << "Failed to read prevout stake of first transaction" << std::endl;
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

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._ft_version), sizeof(block1._ft_version), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read first transaction version" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(&(block1._ft_unknown_val_1[0])), block1._ft_unknown_val_1.size(), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read first transaction unknown val 1" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._block_height), sizeof(block1._block_height), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read block height" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._ft_unknown_val2), sizeof(block1._ft_unknown_val2), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read unknown value 2" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._ft_sequence), sizeof(block1._ft_sequence), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read first transaction sequence" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._ft_ctxout_number), sizeof(block1._ft_ctxout_number), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read first transaction number out contexts" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    for (auto i = 0; i < block1._ft_ctxout_number; i++) {
        ctxout out;

        is >> out;

        if (!is) {
            std::cout << "Failed to read out context number: " << i << std::endl;
        }

        block1._ft_ctxouts.push_back(std::move(out));
    }


    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._ft_number_of_unknown_sequences), sizeof(block1._ft_number_of_unknown_sequences), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read first transaction out scripts number" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    for (auto i = 0; i < block1._ft_number_of_unknown_sequences; i++) {
        uint8_t size;
        if (parsing_utils::parse_bytes(is, static_cast<void*>(&size), sizeof(size), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read size of unknown block number: " << i << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }

        std::vector<uint8_t> vector1(size);

        if (parsing_utils::parse_bytes(is, static_cast<void*>(vector1.data()), size, parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read unknown block number: " << i << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }

        block1._ft_unknown_sequences.push_back(std::move(vector1));
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&block1._ft_lock_time), sizeof(block1._ft_lock_time), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read first transaction out scripts number" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    for (auto i = 0; i < block1._number_of_transactions - 1; i++) {
        transaction t;
        is >> t;

        if (!is) {
            std::cout << "Failed to read transaction number: " << i << std::endl;
            return is;
        }

        block1._transactions.push_back(std::move(t));
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

const c_out_point& block::get_prevout_stake() const {
    return _prevout_stake;
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

block::hash_type block::compute_hash() const {
    CSHA256 hasher;

    hash_utils::addIntegral(hasher, _version);
    hash_utils::addContainer(hasher, _hash_prev_block);
    hash_utils::addContainer(hasher, _hash_merkle_root);
    hash_utils::addIntegral(hasher, _n_time);
    hash_utils::addIntegral(hasher, _n_bits);
    hash_utils::addIntegral(hasher, _n_nonce);
    hash_utils::addContainer(hasher, _hash_state_root);
    hash_utils::addContainer(hasher, _hash_UTXO_root);
    hash_utils::addContainer(hasher, _prevout_stake.get_hash());
    hash_utils::addIntegral(hasher, _prevout_stake.get_index_n());
    hash_utils::addIntegral(hasher, _vch_block_sig_size);
    hash_utils::addContainer(hasher, _vch_block_sig);

    hash_type ret;
    hasher.doubleHashFinalize(ret.data());

    std::reverse(std::begin(ret), std::end(ret));
    return ret;
}

hash_type block::compute_first_transaction_hash() const {
    CSHA256 hasher;
    hash_utils::addIntegral(hasher, _ft_version);

    std::vector<uint8_t > newVec(_ft_unknown_val_1.begin(), _ft_unknown_val_1.end() - 2);

    hash_utils::addContainer(hasher, newVec);
    hash_utils::addIntegral(hasher, _block_height);
    hash_utils::addIntegral(hasher, _ft_unknown_val2);
    hash_utils::addIntegral(hasher, _ft_sequence);

    hash_utils::addIntegral(hasher, _ft_ctxout_number);
    for(auto it : _ft_ctxouts) {
        hash_utils::addIntegral(hasher, it._amount);


        hash_utils::addIntegral(hasher, it._pub_key_script._size);

        if (it._pub_key_script._size != 0) {
            for (auto before_script : it._pub_key_script._before_flags) {
                hash_utils::addIntegral(hasher, before_script);
            }

            hash_utils::addIntegral(hasher, it._pub_key_script._storage_size);
            hash_utils::addContainer(hasher, it._pub_key_script._storage);

            for (auto after_script : it._pub_key_script._after_flags) {
                hash_utils::addIntegral(hasher, after_script);
            }
        }
    }

    hash_utils::addIntegral(hasher, _ft_lock_time);

    hash_type ret;
    hasher.doubleHashFinalize(ret.data());
    return ret;
}

hash_type block::compute_merkle_root() const {
    std::vector<hash_type> hashes;
    CSHA256 hasher;

    hashes.push_back(compute_first_transaction_hash());
    for (const auto &it : _transactions) hashes.push_back(it.compute_hash());

    while (hashes.size() != 1) {
        if(hashes.size() % 2 == 1) {
            hasher.Write(hashes.at(hashes.size() - 1).data(), CSHA256::OUTPUT_SIZE)
                    .Write(hashes.at(hashes.size() - 1).data(), CSHA256::OUTPUT_SIZE);
            hasher.doubleHashFinalize(hashes.at(hashes.size() - 1).data());
            hasher.Reset();
        }

        size_t size = hashes.size()/2;
        for (auto i = 0; i < size; i++) {
            hasher.Write(hashes.at(i).data(), CSHA256::OUTPUT_SIZE)
                    .Write(hashes.at(i + 1).data(), CSHA256::OUTPUT_SIZE);

            hashes.erase(hashes.begin() + i);
            hasher.doubleHashFinalize(hashes.at(i).data());
            hasher.Reset();
        }
    }

    std::reverse(std::begin(hashes.at(0)), std::end(hashes.at(0)));

    return hashes.at(0);
}

std::vector<transaction> block::get_transactions() {
    return _transactions;
}

uint32_t block::get_ft_version() const {
    return _ft_version;
}

const std::array<uint8_t, 41> &block::get_ft_unknown_val_1() const {
    return _ft_unknown_val_1;
}

uint16_t block::get_block_height() const {
    return _block_height;
}

uint8_t block::get_ft_unknown_val2() const {
    return _ft_unknown_val2;
}

uint32_t block::get_ft_sequence() const {
    return _ft_sequence;
}

uint8_t block::get_ft_ctxout_number() const {
    return _ft_ctxout_number;
}

const std::vector<ctxout> &block::get_ft_ctxouts() const {
    return _ft_ctxouts;
}

uint8_t block::get_ft_number_of_unknown_sequences() const {
    return _ft_number_of_unknown_sequences;
}

const std::vector<std::vector<uint8_t>> &block::get_ft_unknown_sequences() const {
    return _ft_unknown_sequences;
}

uint32_t block::get_ft_lock_time() const {
    return _ft_lock_time;
}
