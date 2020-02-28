# Execprog Maker

This is a generator for [execprog.h](https://github.com/arter97/android_kernel_oneplus_sm8150/blob/master/init/execprog.h).

So we can calculate it easily.

## Usage

generator [INPUT_FILE] [OUTPUT_FILE]

## Build this file:

`clang`:
```
clang++ -flto -O3 ./generator.cpp -o generator
```

`gcc`:
```
g++ -O3 ./generator.cpp -o generator
```