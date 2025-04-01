# ErikBus

ErikBus is the RPC protocol provided by ErikOS. Here, you can find a helper library implementation for it.

## Usage

You can build ErikBus with CMake: 

```bash
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../x86_64.cmake ..
make
```

The CMake variable `CMAKE_TOOLCHAIN_FILE` needs to point to a toolchain specification. This repository includes x86_64.cmake that uses clang to cross-compile for their respective architectures.

## License

ErikBus is licensed under [BSD-2-Clause](COPYING) license.
