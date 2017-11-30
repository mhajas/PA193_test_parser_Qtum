#include <memory>
#include <parser/parser.h>

int main(int argc, char** argv) {

    // Example of address: testsuite/resources/test-resources/blocks/22380.bin
    if (argc != 3) {
        std::cout << "Wrong number of arguments! Programs expects 2 arguments which are paths to QTUM blocks." << std::endl;
        return 1;
    }

    auto p1 = std::make_unique<parser>(argv[1]);
    auto p2 = std::make_unique<parser>(argv[2]);

    block firstBlock = p1.get()->get_block();
    block secondBlock = p2.get()->get_block();

    if(!firstBlock.validate()){
        std::cout << "Invalid first block" << std::endl;
        return 2;
    }

    if(!secondBlock.validate()){
        std::cout << "Invalid second block" << std::endl;
        return 2;
    }

    bool followingBlocks = p1.get()->get_block().verify_following_transactions(p2.get()->get_block());

    if(!followingBlocks){
        std::cout << "Error. These blocks are not the following blocks." << std::endl;
        return 3;
    }

    return 0;
}
