[![Actions Status](https://github.com/SarthakMakhija/pmemdb/workflows/PmemCI/badge.svg)](https://github.com/SarthakMakhija/pmemdb/actions)

### pmemdb
K/V store for persistent memory

### Building the project
- Install CMake

    `On Mac - brew install cmake`

- Clone the project

    `git@github.com:SarthakMakhija/pmemdb.git`

- Create a build directory

    `mkdir build`

- Change to build directory

    `cd build`

- Execute CMake

    `cmake ../`

    `cmake --build . -v`

### Running tests

- Change to build directory

    `cd build`

- Execute pmemdb_tests

    `./pmemdb_tests`
