//
// Created by charlliz on 2.11.2017.
//

#ifndef PA193_QTUM_PARSER_TRANSACTION_H
#define PA193_QTUM_PARSER_TRANSACTION_H

#include <array>
#include <vector>
#include "../utils/parsing_utils.h"

using hash_type = std::array<uint8_t, 32>;
using vector_type = std::vector<uint8_t>;

struct c_out_point{
    hash_type _hash;
    uint32_t _index_n;

    const hash_type& get_hash() const {
        return _hash;
    }

    uint32_t get_index_n() const {
        return _index_n;
    }

    c_out_point() : _index_n(0) {};

public:
    friend std::ostream& operator<<(std::ostream& os, const c_out_point& out_point);
    friend std::istream& operator>>(std::istream& is, c_out_point& out_point);
};

struct c_script{
    uint8_t _size;
    uint16_t _extended_size;
    std::vector<uint8_t> _before_flags;
    uint8_t _storage_size;
    std::vector<uint8_t> _storage;
    std::vector<uint8_t> _after_flags;
    bool is_extended() {return _extended_size != 0 ;}

    uint16_t get_size() {return _extended_size == 0 ? _size : _extended_size;}

public:
    c_script() : _size(0)
            , _extended_size(0)
            , _storage_size(0)
    {};
    friend std::ostream& operator<<(std::ostream& os, const c_script& script);
    friend std::istream& operator>>(std::istream& is, c_script& script);
};

struct ctxin {
    c_out_point _prevout;
    c_script _pub_key_script;
    uint32_t _sequence;


public:

    friend std::ostream& operator<<(std::ostream& os, const ctxin& in);
    friend std::istream& operator>>(std::istream& is, ctxin& in);
};

struct ctxout {
    int64_t _amount;
    c_script _pub_key_script;

public:
    friend std::ostream& operator<<(std::ostream& os, const ctxout& out);
    friend std::istream& operator>>(std::istream& is, ctxout& out);
};

class transaction {

    uint32_t _version;
    uint8_t _vin_count;
    std::vector<ctxin> _vin;
    uint8_t _vout_count;
    std::vector<ctxout> _vout;
    uint32_t _lock_time;
    uint16_t _extended_vin_count = 0;
    uint16_t _extended_vout_count = 0;
    bool _has_witness;
    uint8_t _witnesses_number;
    std::vector<std::vector<uint8_t>> _witnesses;

public:
    transaction() : _version(0)
            , _vin_count(0)
            , _vout_count(0)
            , _lock_time(0)
            , _extended_vin_count(0)
            , _extended_vout_count(0)
            , _has_witness(false)
            , _witnesses_number(0)
    {};

    uint16_t get_extended_vin_count() const;

public:
    uint32_t get_version() const;

    uint16_t get_vin_count() const;

    const std::vector<ctxin> &get_vin() const;

    uint16_t get_vout_count() const;

    const std::vector<ctxout> &get_vout() const;

    uint32_t get_lock_time() const;

public:
    hash_type compute_hash() const;

public:
    friend std::ostream& operator<<(std::ostream& os, const transaction& t);
    friend std::istream& operator>>(std::istream& is, transaction& t);

};

#endif //PA193_QTUM_PARSER_TRANSACTION_H
