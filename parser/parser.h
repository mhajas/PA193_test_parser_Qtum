//
// Created by mhajas on 10/5/17.
//

#ifndef PA193_QTUM_PARSER_PARSER_H
#define PA193_QTUM_PARSER_PARSER_H


class parser {
private:
    int value;

public:
    parser(int value) : value(value)
    {};

    int getValue() {
        return value;
    }
};


#endif //PA193_QTUM_PARSER_PARSER_H
