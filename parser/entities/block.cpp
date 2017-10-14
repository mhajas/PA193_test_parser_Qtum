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
