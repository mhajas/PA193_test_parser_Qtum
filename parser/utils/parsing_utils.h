//
// Created by mhajas on 11/6/17.
//

#ifndef PA193_QTUM_PARSER_PARSING_UTILS_H
#define PA193_QTUM_PARSER_PARSING_UTILS_H

#include <iostream>
#include <vector>
#include <algorithm>

namespace parsing_utils {
    enum RETURN_VALUE {SUCCESS, FAILURE};

    inline bool is_big_endian()
    {
        union {
            uint32_t i;
            char c[4];
        } bint = {0x01020304};

        return bint.c[0] == 1;
    }

    inline RETURN_VALUE parse_bytes(std::istream& is, void* storage, size_t number_of_bytes, bool is_big_endian) {
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

    inline RETURN_VALUE parse_reverse_bytes(std::istream& is, void* storage, size_t number_of_bytes, bool is_big_endian) {
        return parse_bytes(is, storage, number_of_bytes, !is_big_endian);
    }

}
#endif //PA193_QTUM_PARSER_PARSING_UTILS_H
