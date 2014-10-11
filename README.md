game_assets
===========

utilities for creating video games

## Basis:
This library is heavily based on the cppcon talk: _"Quick game development with C++11/C++14 - Vittorio Romero"_ see: <https://github.com/SuperV1234/>

## Features:

* Type erased entity type, removing the need for inheritance
* Compile time mapped entity groups, removing the need for both typeid and it's runtime overhead
* `std::shared_ptr` so that entities are entities have lightweight copies
* More to come... oh yeah, clang wont compile this, I blame libc++
