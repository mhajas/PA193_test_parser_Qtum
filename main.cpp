#include <memory>
#include <parser/parser.h>

int main(int argc, char** argv) {
    auto p = std::make_unique<parser>("testsuite/resources/test-resources/blocks/22380.bin");
}
