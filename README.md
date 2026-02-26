> [!WARNING]
> This project is a work in progress. Some features are incomplete or may change.

# SoftwareRenderer

SoftwareRenderer is a CPU-based software rendering project written in C.
It demonstrates fundamental rendering techniques without relying on GPU APIs.

---

## Build

### Native build

```bash
cmake --workflow native
```

### Windows build (from Linux using MinGW)

```bash
cmake --workflow mingw
```

---

## Contributing

1. Create a branch from `development`.
2. Open a pull request.
3. Assign `T1ag0Card0s0` as reviewer.

### Requirements

- No failing builds.
- The demo must run on all platforms listed in the `platform/` directory.
- Code must be formatted before submitting:

```bash
cmake --workflow native
cmake --build build --target format
```

---

## License

This project is licensed under the MIT License.
See the `LICENSE` file for details.
