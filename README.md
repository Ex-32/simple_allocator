# Simple Allocator

This is a simple memory manager for POSIX systems (requres `unistd.h` and `sys/mman.h`) that implements two functions: `simple_malloc()` and `simple_free()` that behave identically* (but are not interchangable with) `malloc()` and `free()`. It is _not_ thread safe (yet) and honestly shouldn't be used at all, as `malloc()` and `free()` are not only built into the c standard library but almost certainly less buggy.

## Building:
If you want to build it for some reason; after cloning the repo simply run the following from the root of the repo:
```bash
mkdir build
cd build
cmake ..
make
```

###### *identical behavior attempted but not guaranteed
