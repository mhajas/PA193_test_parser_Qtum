//
// Created by charlliz on 3.11.2017.
//
#include "transaction.h"
#include "../utils/parsing_utils.h"

std::istream& operator>>(std::istream& is, transaction& t) {
    if (parsing_utils::parse_bytes(is, static_cast<void*>(&t._version), sizeof(t._version), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read Version" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&t._vin_count), sizeof(t._vin_count), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read in transaction count" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    for (auto i = 0; i < t._vin_count; i++) {
        ctxin inTransaction;

        is >> inTransaction;

        if (!is) {
            std::cout << "Failed to read in transaction number [" << i << "]" << std::endl;
            return is;
        }

        t._vin.push_back(std::move(inTransaction));
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&t._vout_count), sizeof(t._vout_count), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read out transaction count" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    for (auto i = 0; i < t._vin_count; i++) {
        ctxout outTransaction;
        is >> outTransaction;

        if (!is) {
            std::cout << "Failed to read out transaction number [" << i << "]" << std::endl;
            return is;
        }
        t._vout.push_back(std::move(outTransaction));
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&t._lock_time), sizeof(t._lock_time), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read lock time" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    return is;
}

std::istream& operator>>(std::istream& is, ctxin& in) {
    is >> in._prevout;
    if (!is) {
        std::cout << "Failed to read prevout for in transaction" << std::endl;
        return is;
    }

    is >> in._pub_key_script;
    if (!is) {
        std::cout << "Failed to read pub_key_script for in transaction" << std::endl;
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&in._sequence), sizeof(in._sequence), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read Sequence" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    return is;
}

std::istream& operator>>(std::istream& is, c_out_point& out_point) {
    if (parsing_utils::parse_reverse_bytes(is, static_cast<void*>(out_point._hash.data()), out_point._hash.size(), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read hash of previous block" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&out_point._index_n), sizeof(out_point._index_n), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read Version" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    return is;
}

std::istream& operator>>(std::istream& is, c_script& script) {
    if (parsing_utils::parse_bytes(is, static_cast<void*>(script._size), sizeof(script._size), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read pub_script size" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (script._size == 0)
        return is;

    if(parsing_utils::parse_bytes(is, static_cast<void*>(script._storage_size), sizeof(script._storage_size),
                                  parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read first storage size/before flag" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    while (script._storage_size == 0 || script._storage_size > 0x4c) {
        script._before_flags.push_back(script._storage_size);

        if(parsing_utils::parse_bytes(is, static_cast<void*>(script._storage_size), sizeof(script._storage_size),
                                      parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read storage size/before flag" << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    script._storage.resize(script._storage_size);

    if(parsing_utils::parse_reverse_bytes(is, static_cast<void*>(script._storage.data()), script._storage_size,
                                  parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read storage within c_script" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    auto after_flag_number = script._size - (script._storage_size + 1 + script._after_flags.size());
    uint8_t flag;
    for (auto i = 0; i < after_flag_number; i++) {
        if(parsing_utils::parse_bytes(is, static_cast<void*>(flag), sizeof(flag),
                                      parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read after flag" << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }

        script._after_flags.push_back(flag);
    }

    return is;
}

std::istream& operator>>(std::istream& is, ctxout& out) {
    if (parsing_utils::parse_bytes(is, static_cast<void*>(&out._amount), sizeof(out._amount), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read Sequence" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    is >> out._pub_key_script;
    if (!is) {
        std::cout << "Failed to read pub key script for out transaction" << std::endl;
        return is;
    }

    return is;
}
