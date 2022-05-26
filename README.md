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
    
## Building on local (Ubuntu)

**Install dependencies**

- sudo apt install autoconf automake pkg-config libglib2.0-dev libfabric-dev pandoc libncurses5-dev

**Build PMDK**

- git clone https://github.com/pmem/pmdk
- cd pmdk
- make -j$(nproc)
- sudo make install

**Build memkind**

- git clone https://github.com/memkind/memkind
- cd memkind
- ./autogen.sh
- ./configure
- make
- sudo make install

**Build libpmemobj-cpp**

- git clone https://github.com/pmem/libpmemobj-cpp
- cd libpmemobj-cpp
- mkdir build
- cd build
- cmake .. -DTESTS_USE_VALGRIND=OFF
- make -j$(nproc)
- sudo make install

**Build Pmemdb**

- git clone https://github.com/SarthakMakhija/pmemdb
- Once these steps are done, (Remove/comment, `pmdk` / `libpmemobjcpp` from vcpkg.json)
- mkdir `build` inside pmemdb
- from the project root execute, cmake ../ -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg.cmake>
- from the build directory execute, cmake --build .
