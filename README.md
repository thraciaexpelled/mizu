# mizu

## Introduction
`mizu` is/(will be) a build system and package manager for C/C++ projects.
`mizu` is inspired by Rust's `cargo` package manager. `cargo` is able to fetch dependencies,
compile code, and publish code to hosting platforms.
As of **October 14**, `mizu` aims to only pull packages and manage projects. Compilation
features are yet to be put in works.


## Building
**Dependencies (names as of Ubuntu 24.04)**
```
libjansson4
libjansson-dev
```

**System requirements**:
- Unix/Linux
- Microsoft Windows (in a [POSIX environment (hyperlink to msys2.org)](https://www.msys2.org/)) To build

**Compiling `mizu`**
```bash
make; make install
```


## License
Check [LICENSE](LICENSE)
