//
// Created by charlliz on 2.11.2017.
//

#ifndef PA193_QTUM_PARSER_TRANSACTION_H
#define PA193_QTUM_PARSER_TRANSACTION_H

#include <array>
#include <vector>

using hash_type = std::array<uint8_t, 32>;

struct c_out_point{
    hash_type _hash;
    uint32_t _index_n;
};

struct ctxin {
    c_out_point _prevout;
    uint8_t _script_sig_size;
    std::vector<uint8_t> _script_sig;
    uint32_t _n_sequence;
};

struct ctxout {
    int64_t _n_value;
    uint8_t _script_pub_key_size;
    std::vector<uint8_t> _script_pub_key;
};

class transaction {

    uint32_t _n_version;
    uint8_t _vin_count;
    std::vector<ctxin> _vin;
    uint8_t _vout_count;
    std::vector<ctxout> _vout;

};

#endif //PA193_QTUM_PARSER_TRANSACTION_H
