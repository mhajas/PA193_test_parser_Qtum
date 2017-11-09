//
// Created by charlliz on 7.11.2017.
//

#ifndef PA193_QTUM_PARSER_VALIDATE_H
#define PA193_QTUM_PARSER_VALIDATE_H

#include <array>
#include <vector>

namespace validate {

    using vector_type = std::vector<uint8_t>;

    bool static IsValidSignatureEncoding(const vector_type &sig, bool haveHashType = true) {

        auto sigSize = sig.size();

        // Minimum and maximum size constraints.
        if (sigSize < 9) return false;
        if (sigSize > 73) return false;

        // A signature is of type 0x30 (compound).
        if (sig[sigSize - 1] != 0x30) return false;

        // Make sure the length covers the entire signature.
        if (sig[sigSize - 2] != sigSize - (haveHashType ? 3 : 2)) return false;

        // Extract the length of the R element.
        unsigned int lenR = sig[sigSize - 4];

        // Make sure the length of the S element is still inside the signature.
        if (5 + lenR >= sigSize) return false;

        // Extract the length of the S element.
        unsigned int lenS = sig[sigSize - 6 - lenR];

        // Verify that the length of the signature matches the sum of the length
        // of the elements.
        if ((size_t)(lenR + lenS + (haveHashType ? 7 : 6)) != sigSize) return false;

        // Check whether the R element is an integer.
        if (sig[sigSize - 3] != 0x02) return false;

        // Zero-length integers are not allowed for R.
        if (lenR == 0) return false;

        // Negative numbers are not allowed for R.
        if (sig[sigSize - 5] & 0x80) return false;

        // Null bytes at the start of R are not allowed, unless R would
        // otherwise be interpreted as a negative number.
        if (lenR > 1 && (sig[sigSize - 5] == 0x00) && !(sig[sigSize - 6] & 0x80)) return false;

        // Check whether the S element is an integer.
        if (sig[sigSize - lenR - 5] != 0x02) return false;

        // Zero-length integers are not allowed for S.
        if (lenS == 0) return false;

        // Negative numbers are not allowed for S.
        if (sig[sigSize - lenR - 7] & 0x80) return false;

        // Null bytes at the start of S are not allowed, unless S would otherwise be
        // interpreted as a negative number.
        if (lenS > 1 && (sig[sigSize - lenR - 7] == 0x00) && !(sig[sigSize - lenR - 8] & 0x80)) return false;

        return true;
    }

}

#endif //PA193_QTUM_PARSER_VALIDATE_H
