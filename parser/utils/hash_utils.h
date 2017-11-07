//
// Created by mhajas on 11/6/17.
//

#ifndef PA193_QTUM_PARSER_HASH_UTILS_H
#define PA193_QTUM_PARSER_HASH_UTILS_H
namespace hash_utils {
    template<typename T>
    void addIntegral(CSHA256 &hasher, T number) {
        int size = sizeof(T);
        unsigned char cArray[size];

        for (int i = 0; i < size; i++)
            cArray[i] = (number >> (i * 8));

        hasher.Write(cArray, size);
    }

    template<typename T, typename M = typename T::value_type>
    void addContainer(CSHA256 &hasher, const T &container) {
        std::vector <M> copy(std::begin(container), std::end(container));

        std::reverse(std::begin(copy), std::end(copy));
        hasher.Write(copy.data(), copy.size());
    }
}
#endif //PA193_QTUM_PARSER_HASH_UTILS_H
