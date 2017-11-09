//
// Created by charlliz on 3.11.2017.
//
#include "transaction.h"
#include <iomanip>
#include "../utils/parsing_utils.h"
#include "../external/crypto/sha256.h"
#include "../utils/hash_utils.h"

std::ostream& operator<<(std::ostream& os, const transaction& t){

    os << "Version: " << t._version << std::endl;

    os << "Input" << std::endl;
    for (auto it : t._vin) os << it;
    os << "Output" << std::endl;
    for (auto it : t._vout) os << it;
    os << "Lock time: " << t._lock_time << std::endl;
    os << "Transaction hash: ";
    for (auto it : t.compute_hash()) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl << std::endl;

    return os;
}

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

    if (t._vin_count == 253) {
        if (parsing_utils::parse_bytes(is, static_cast<void*>(&t._extended_vin_count), sizeof(t._extended_vin_count), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read in transaction extended vin count" << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    for (auto i = 0; i < ((t._extended_vin_count == 0) ? t._vin_count : t._extended_vin_count); i++) {
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

    if(t._vin_count == 0 && t._vout_count == 1) { //in witness present
        t._has_witness = true;

        if (parsing_utils::parse_bytes(is, static_cast<void*>(&t._vin_count), sizeof(t._vin_count), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read in transaction count" << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }



        if (t._vin_count == 253) {
            if (parsing_utils::parse_bytes(is, static_cast<void*>(&t._extended_vin_count), sizeof(t._extended_vin_count), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
                std::cout << "Failed to read in transaction extended vin count" << std::endl;
                is.setstate(std::ios::failbit);
                return is;
            }
        }

        for (auto i = 0; i < ((t._extended_vin_count == 0) ? t._vin_count : t._extended_vin_count); i++) {
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
    }

    if (t._vout_count == 253) {
        if (parsing_utils::parse_bytes(is, static_cast<void*>(&t._extended_vout_count), sizeof(t._extended_vout_count), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read in transaction extended vout count" << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    for (auto i = 0; i < t.get_vout_count(); i++) {
        ctxout outTransaction;
        is >> outTransaction;

        if (!is) {
            std::cout << "Failed to read out transaction number [" << i << "]" << std::endl;
            return is;
        }
        t._vout.push_back(std::move(outTransaction));
    }

    if (t._has_witness) {
        if (parsing_utils::parse_bytes(is, static_cast<void*>(&t._witnesses_number), sizeof(t._witnesses_number), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read lock time" << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }

        for(int i = 0; i < t._witnesses_number; i++) {
            uint8_t size_of_witness;
            if (parsing_utils::parse_bytes(is, static_cast<void*>(&size_of_witness), sizeof(size_of_witness), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
                std::cout << "Failed to read lock time" << std::endl;
                is.setstate(std::ios::failbit);
                return is;
            }
            std::vector<uint8_t> witness(size_of_witness);

            if (parsing_utils::parse_bytes(is, static_cast<void*>(&(witness[0])), size_of_witness, parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
                std::cout << "Failed to read lock time" << std::endl;
                is.setstate(std::ios::failbit);
                return is;
            }

            t._witnesses.push_back(std::move(witness));
        }

    }

    if (parsing_utils::parse_bytes(is, static_cast<void*>(&t._lock_time), sizeof(t._lock_time), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read lock time" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const ctxin& in){
    os << "Prevout " << std::endl;
    os << in._prevout;
    os << "scriptPubKey: " << in._pub_key_script;
    os << "nSequence: " << in._sequence << std::endl;

    return os;
}

hash_type transaction::compute_hash() const {
    CSHA256 hasher;
    hash_utils::addIntegral(hasher, _version);

    if (_extended_vin_count != 0) {
        hash_utils::addIntegral(hasher, uint8_t(253));
        hash_utils::addIntegral(hasher, _extended_vin_count);
    } else
        hash_utils::addIntegral(hasher, _vin_count);
    for(auto it : _vin) {

        hash_utils::addContainer(hasher, it._prevout.get_hash());
        hash_utils::addIntegral(hasher, it._prevout.get_index_n());


        if (it._pub_key_script.is_extended()) {
            hash_utils::addIntegral(hasher, uint8_t(253));
            hash_utils::addIntegral(hasher, it._pub_key_script._extended_size);
        } else {
            hash_utils::addIntegral(hasher, it._pub_key_script._size);
        }

        for(auto before_script : it._pub_key_script._before_flags) {
            hash_utils::addIntegral(hasher, before_script);
        }
        if (it._pub_key_script._storage_size != 0) {
            hash_utils::addIntegral(hasher, it._pub_key_script._storage_size);
            hash_utils::addContainer(hasher, it._pub_key_script._storage);
        }
        for(auto after_script : it._pub_key_script._after_flags) {
            hash_utils::addIntegral(hasher, after_script);
        }

        hash_utils::addIntegral(hasher, it._sequence);
    }


    if (_extended_vout_count != 0) {
        hash_utils::addIntegral(hasher, uint8_t(253));
        hash_utils::addIntegral(hasher, _extended_vout_count);
    } else
        hash_utils::addIntegral(hasher, _vout_count);

    for(auto it : _vout) {
        hash_utils::addIntegral(hasher, it._amount);


        if (it._pub_key_script.is_extended()) {
            hash_utils::addIntegral(hasher, uint8_t(253));
            hash_utils::addIntegral(hasher, it._pub_key_script._extended_size);
        } else {
            hash_utils::addIntegral(hasher, it._pub_key_script._size);
        }

        if (it._pub_key_script.get_size() != 0) {
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

    hash_utils::addIntegral(hasher, _lock_time);

    hash_type ret;
    hasher.doubleHashFinalize(ret.data());

    return ret;
}

uint32_t transaction::get_version() const {
    return _version;
}

uint16_t transaction::get_vin_count() const {
    return _extended_vin_count == 0 ? _vin_count : _extended_vin_count;
}

const std::vector<ctxin> &transaction::get_vin() const {
    return _vin;
}

uint16_t transaction::get_vout_count() const {
    return _extended_vout_count == 0 ? _vout_count : _extended_vout_count;;
}

const std::vector<ctxout> &transaction::get_vout() const {
    return _vout;
}

uint32_t transaction::get_lock_time() const {
    return _lock_time;
}

uint16_t transaction::get_extended_vin_count() const {
    return _extended_vin_count;
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

std::ostream& operator<<(std::ostream& os, const c_out_point& out_point){
    os << "Hash: " ;
    for (auto it : out_point._hash) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << std::endl << "Index n: ";
    os << out_point._index_n << std::endl;

    return os;
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

std::ostream& operator<<(std::ostream& os, const c_script& script){
    for (auto it : script._before_flags) os << std::setfill('0') << std::setw(2) << std::hex << (int) it << " ";

    for (auto it : script._storage) os << std::setfill('0') << std::setw(2) << std::hex << (int) it;
    os << " ";

    for (auto it : script._after_flags) os << std::setfill('0') << std::setw(2) << std::hex << (int) it << " ";
    os << std::endl;

    return os;
}

std::istream& operator>>(std::istream& is, c_script& script) {
    if (parsing_utils::parse_bytes(is, static_cast<void*>(&script._size), sizeof(script._size), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read pub_script size" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    if (script._size == 253) {
        if (parsing_utils::parse_bytes(is, static_cast<void*>(&script._extended_size), sizeof(script._extended_size), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read in transaction extended vin count" << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    if (script._size == 0)
        return is;

    if(parsing_utils::parse_bytes(is, static_cast<void*>(&script._storage_size), sizeof(script._storage_size),
                                  parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read first storage size/before flag" << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    int read_flags = 0;

    while (script._storage_size == 0 || script._storage_size >= 0x4c) {
        script._before_flags.push_back(script._storage_size);
        read_flags++;
        if (script.get_size() == read_flags) {
            script._storage_size = 0;
            break;
        }

        if(parsing_utils::parse_bytes(is, static_cast<void*>(&script._storage_size), sizeof(script._storage_size),
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

    size_t after_flag_number;
    if (script._storage_size == 0) {
        after_flag_number = 0;
    } else {
        after_flag_number = script.get_size() - (script._storage_size + 1 + script._before_flags.size());
    }
    uint8_t flag;
    for (auto i = 0; i < after_flag_number; i++) {
        if(parsing_utils::parse_bytes(is, static_cast<void*>(&flag), sizeof(flag),
                                      parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
            std::cout << "Failed to read after flag" << std::endl;
            is.setstate(std::ios::failbit);
            return is;
        }

        script._after_flags.push_back(flag);
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const ctxout& out){
    auto amount = out._amount / 100000000 + 0.00000001 * (out._amount % 100000000);
    os << "Amount: " << std::dec << amount << " QTUM" << std::endl;
    os << "scriptPubKey: " << out._pub_key_script ;

    return os;
}

std::istream& operator>>(std::istream& is, ctxout& out) {
    if (parsing_utils::parse_bytes(is, static_cast<void*>(&out._amount), sizeof(out._amount), parsing_utils::is_big_endian()) != parsing_utils::SUCCESS) {
        std::cout << "Failed to read amount" << std::endl;
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

