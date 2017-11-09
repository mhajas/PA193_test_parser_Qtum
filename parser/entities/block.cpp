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
#include "../utils/validate.h"
#include "transaction.h"

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
    os << "vch Block signature: ";
    for (auto it : block1._vch_block_sig) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl << std::endl;

    os << "Number of transactions: " << (int) block1.get_number_of_transactions() << std::endl << std::endl;

    int i = 1;
    for (auto it : block1._transactions) {
        os << "Transaction number " << i++ << ":" << std::endl << it;
    }

    os << "Block Hash: ";
    for (auto it : block1.compute_hash()) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl;

    return os;
}

std::istream& operator>>(std::istream& is, block& block1) {
    if (parsing_utils::parse_bytes(is, static_cast<void *>(&block1._version), sizeof(block1._version),
                                   parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read Version" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void *>(block1._hash_prev_block.data()),
                                           block1._hash_prev_block.size(), parsing_utils::is_big_endian()) !=
        parsing_utils::SUCCESS) {
        std::cout << "Failed to read hash of previous block" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void *>(block1._hash_merkle_root.data()),
                                           block1._hash_merkle_root.size(), parsing_utils::is_big_endian()) !=
        parsing_utils::SUCCESS) {
        std::cout << "Failed to read merkle root hash" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void *>(&block1._n_time), sizeof(block1._n_time),
                                   parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read nTime" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void *>(&block1._n_bits), sizeof(block1._n_bits),
                                   parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read nBits" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void *>(&block1._n_nonce), sizeof(block1._n_nonce),
                                   parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read nNonce" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void *>(block1._hash_state_root.data()),
                                           block1._hash_state_root.size(), parsing_utils::is_big_endian()) !=
        parsing_utils::SUCCESS) {
        std::cout << "Failed to read state root hash" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void *>(block1._hash_UTXO_root.data()),
                                           block1._hash_UTXO_root.size(), parsing_utils::is_big_endian()) !=
        parsing_utils::SUCCESS) {
        std::cout << "Failed to read UTXO root hash" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    is >> block1._prevout_stake;

    if (!is) {
        std::cout << "Failed to read prevout stake of first transaction" << std::endl;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void *>(&block1._vch_block_sig_size),
                                   sizeof(block1._vch_block_sig_size), parsing_utils::is_big_endian()) !=
        parsing_utils::SUCCESS) {
        std::cout << "Failed to read size of block signature" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    block1._vch_block_sig.resize(block1._vch_block_sig_size);

    if (parsing_utils::parse_reverse_bytes(is, static_cast<void *>(&(block1._vch_block_sig[0])),
                                           block1._vch_block_sig_size, parsing_utils::is_big_endian()) !=
        parsing_utils::SUCCESS) {
        std::cout << "Failed to read vch block signature" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void *>(&block1._number_of_transactions),
                                   sizeof(block1._number_of_transactions), parsing_utils::is_big_endian()) !=
        parsing_utils::SUCCESS) {
        std::cout << "Failed to read number of transactions" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (block1._number_of_transactions == 253) {
        if (parsing_utils::parse_bytes(is, static_cast<void *>(&block1._extended_number_of_transactions),
                                       sizeof(block1._extended_number_of_transactions),
                                       parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read number of transactions" << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    for (auto i = 0; i < block1.get_number_of_transactions(); i++) {
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

block::block() : _version(0), _n_time(0),
                 _n_bits(0), _n_nonce(0),
                 _vch_block_sig_size(0),
                 _vch_block_sig(0),
                 _number_of_transactions(0),
                 _extended_number_of_transactions(0),
                 _transactions(0) {}

block::block(std::istream &stream) : _version(0), _n_time(0),
                                     _n_bits(0), _n_nonce(0),
                                     _vch_block_sig_size(0),
                                     _vch_block_sig(0),
                                     _number_of_transactions(0),
                                     _extended_number_of_transactions(0),
                                     _transactions(0) {
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

uint16_t block::get_number_of_transactions() const {
    return _extended_number_of_transactions == 0 ? _number_of_transactions : _extended_number_of_transactions;
}

std::vector<transaction> block::get_transactions() {
    return _transactions;
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

hash_type block::compute_merkle_root() const {
    std::vector<hash_type> hashes;
    CSHA256 hasher;

    for (const auto &it : _transactions) hashes.push_back(it.compute_hash());

    while (hashes.size() != 1) {
        if (hashes.size() % 2 == 1) {
            hasher.Write(hashes.at(hashes.size() - 1).data(), CSHA256::OUTPUT_SIZE)
                    .Write(hashes.at(hashes.size() - 1).data(), CSHA256::OUTPUT_SIZE);
            hasher.doubleHashFinalize(hashes.at(hashes.size() - 1).data());
            hasher.Reset();
        }

        size_t size = hashes.size() / 2;
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

bool block::validate() const{

    // Check if version was initialized
     if(_version == 0){
         std::cout << "Error. Wrong block version." << std::endl;
         return false;
     }

    //Check if merkle root is same as computed
    hash_type computed_merkle_root = compute_merkle_root();
    for(int i = 0; i < 32; i++){
        if(_hash_merkle_root[i] != computed_merkle_root[i]){
            std::cout << "Error. Wrong Merkle root" << std::endl;
            return false;
        }
    }

    //Check if time was initialized
    if(_n_time == 0){
        std::cout << "Error. Wrong time of creating block" << std::endl;
        return false;
    }

    //Check size of block
    if(_n_bits == 0){
        std::cout << "Error. Wrong bits of block" << std::endl;
        return false;
    }

    //Check if signature is valid
    if(! validate::IsValidSignatureEncoding(_vch_block_sig, false)){
        std::cout << "Error. Signature is nor valid" << std::endl;
        return false;
    }

    //Check number of transactions
    if(_number_of_transactions == 0 || _number_of_transactions == 1){
        std::cout << "Error. Wrong number of transactions." << std::endl;
        return false;
    }

    return true;
}


bool block::verify_following_transactions(const block& block2) const{

    hash_type hash_first_block = compute_hash();

    //Verify hash of previous block
    for(int i = 0; i < 32; i++){
        if(block2.get_hash_prev_block()[i] != hash_first_block[i]){
            std::cout << "Error. Hash of previous block is not matching" << std::endl;
            return false;
        }
    }

    //Verify if height have difference of 1
    if((get_block_height() + 1) != block2.get_block_height()){
        std::cout << "Error. Block heights have not difference of 1" << std::endl;
        return false;
    }

    return true;

}

uint32_t block::get_block_height() const {
    uint32_t ret = 0;

    for (auto i = 0; i < _transactions[0].get_vin()[0]._pub_key_script._storage_size; i++) {
        ret += _transactions[0].get_vin()[0]._pub_key_script._storage[i];
        if (i != _transactions[0].get_vin()[0]._pub_key_script._storage_size - 1) {
            ret <<= 8;
        }
    }
    return ret;
}
