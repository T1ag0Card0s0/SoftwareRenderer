> [!WARNING]  
> This is still a work in progress project, not all of the features are implemented.

# SoftwareRenderer

SoftwareRenderer is a software rendering project demonstrating CPU-based rendering techniques.

## To build

```bash
cmake -S . -B build
cmake --build build
```

On linux you can also generate windows binary using the mingw toolchain.

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain/mingw-toolchain.cmake
cmake --build build
```
