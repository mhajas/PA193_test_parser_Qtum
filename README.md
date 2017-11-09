[![Build Status](https://travis-ci.org/mitko501/PA193_test_parser_Qtum.svg?branch=master)](https://travis-ci.org/mitko501/PA193_test_parser_Qtum)

# Qtum block parser
This project is designated only for educational purposes. It is not recommended to use it in production as it is developed as part of project for university course PA193.

The purpose of the project was create program which will parse following blocks of cryptocurrency Qtum, validate these blocks and verify that they are following blocks.

## Build
```
mkdir build
cd build
cmake ..
make
```
## Useful links

  - Qtum website, https://qtum.org/
  - Explorer of the blocks, https://explorer.qtum.org/
  - Qtum repository, https://github.com/qtumproject/qtum
  - Important components of QTUM, https://github.com/qtumproject/qtum/tree/master/src/primitives

## Block structure

Block structure can be found in project presentation in `doc` folder.

## Tests

All tests are located in folder `testuite` 
For build with tests, run cmake with flag `-DBUILD_testsuite=ON`
After that just run tests with `cd testsuite && ./testsuite`

## Team
 - **Michal Hajas**
 - **Lenka Svetlovská**
 - **Andrea Turiaková**
