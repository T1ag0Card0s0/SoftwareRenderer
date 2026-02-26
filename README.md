> [!WARNING]  
> This is still a work in progress project, not all of the features are implemented.

# SoftwareRenderer

SoftwareRenderer is a software rendering project demonstrating CPU-based rendering techniques.

## To build

```bash
cmake --workflow native
```

On linux you can also generate windows binary using the mingw toolchain.

```bash
cmake --workflow mingw
```

## To contribute

Create a branch from development and then create a pull request assigning T1ag0Card0s0 as reviewer. 

No tests failling.
The demo should work for all the platforms in the platform/ folder.
The code have to be formated with:

```bash
cmake --workflow native 
cmake --build build --target format
```
