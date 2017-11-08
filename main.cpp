#include <memory>
#include <parser/parser.h>

int main(int argc, char** argv) {

    auto p1 = std::make_unique<parser>("testsuite/resources/test-resources/blocks/22380.bin");
    auto p2 = std::make_unique<parser>("testsuite/resources/test-resources/blocks/22381.bin");

    block firstBlock = p1.get()->get_block();
    block secondBlock = p2.get()->get_block();

    if(!firstBlock.validate()){
        std::cout << "Invalid first block" << std::endl;
    }

    if(!secondBlock.validate()){
        std::cout << "Invalid second block" << std::endl;
    }

    bool followingBlocks = p1.get()->get_block().verify_following_transactions(p2.get()->get_block());

    if(!followingBlocks){
        std::cout << "Error. Blocks are not following" << std::endl;
    }

}
