#include <iostream>
#include <parser/parser.h>

int main(int argc, char** argv) {
    parser p(5);

    std::cout << p.getValue() << std::endl;
}
