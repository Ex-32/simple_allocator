# Simple Allocator

This is a simple memory manager for POSIX systems (requires `unistd.h` and `sys/mman.h`) that implements two functions: `simple_malloc()` and `simple_free()` that behave identically* (but are not interchangeable with) `malloc()` and `free()`. Please don't use this, as `malloc()` and `free()` are not only built into the c standard library but almost certainly less buggy.

## Building

If you want to build it for some reason; after cloning the repo simply run the following from the root of the repo:

```bash
mkdir build
cd build
cmake ..
make
```

###### *identical behavior attempted but not guaranteed, actually nothing is guaranteed, this library might eat your cat for all i know, i can not stress enough that you should not use this
