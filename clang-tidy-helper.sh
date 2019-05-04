#!/bin/bash

rm -rf build-clang-tidy
mkdir build-clang-tidy
cd build-clang-tidy

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_CLANG_TIDY:STRING="clang-tidy;--header-filter=.*;-checks=*,-google-readability-braces-around-statements,-readability-braces-around-statements,-fuchsia-*,-hicpp-*,-llvm-header-guard,-google-runtime-references,-hicpp-explicit-conversions,-misc-non-private-member-variables-in-classes,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-readability-uppercase-literal-suffix" ..
make
