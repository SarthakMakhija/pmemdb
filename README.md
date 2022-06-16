[![Actions Status](https://github.com/SarthakMakhija/pmemdb/workflows/PmemCI/badge.svg)](https://github.com/SarthakMakhija/pmemdb/actions)

### pmemdb
K/V store for persistent memory
    
## Building on local (Ubuntu)

- Install `git` and `CMake` on local
- Execute `build-ubuntu` and pass a directory path where all the dependencies will be installed 
- `sudo ./build-ubuntu.sh /projects`

### Running tests

- Mode to the `build` directory inside `pmemdb` directory

    `cd build`

- Execute pmemdb_tests

    `./pmemdb_tests`
