add_library(parser-lib STATIC
        parser.cpp
        parser.h
        entities/block.cpp
        entities/block.h
        entities/transaction.h
        entities/transaction.cpp
        external/crypto/sha256
        external/crypto/common.h
        external/crypto/endian.h
        external/crypto/byteswap.h)

target_include_directories(parser-lib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        )

set_target_properties(parser-lib PROPERTIES
        LINKER_LANGUAGE CXX
        )