# mizu

### Introduction
1.  `mizu` is/(will be) a build system and package manager for C/C++ projects.
2.  `mizu` is inspired by Rust's `cargo` package manager. `cargo` is able to fetch dependencies,
    compile code, and publish code to hosting platforms.
3.  As of **October 14**, `mizu` aims to only pull packages and manage projects. Compilation
    features are yet to be put in works.

### Dependencies
```
jansson (end-user and dev libraries)
```

### Building
0.  System requirements:
    - Unix/Linux
    - Microsoft Windows (in a [POSIX environment (hyperlink to msys2.org)](https://www.msys2.org/))
1.  **To build**
    ```commandline
    $ make
    ```
2.  **To install**
    ```commandline
    $ make install
    # NOTE: you may or may not need `sudo` to install systemwide depending on your system configuration
    ```

### License
1.  Check [LICENSE](LICENSE)
