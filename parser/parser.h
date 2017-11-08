//
// Created by mhajas on 10/5/17.
//

#ifndef PA193_QTUM_PARSER_PARSER_H
#define PA193_QTUM_PARSER_PARSER_H


#include <fstream>
#include <iostream>
#include <cstring>
#include "entities/block.h"

class parser {
private:
    block _b;

public:
    explicit parser(std::string&& filename) {
        std::ifstream file(filename, std::ios::binary);

        if (!file) {
            std::cout << "failed to open file" << std::endl;
            std::cout << strerror(errno) << std::endl;
            return;
        }

        file >> _b;
        if(file.fail()){
            std::cout << "block was not parsed correctly" << std::endl;
        }else{
            std::cout << _b;
        }
    }

    block get_block() const;
};


#endif //PA193_QTUM_PARSER_PARSER_H
