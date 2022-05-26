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

1)
sudo apt install autoconf automake pkg-config libglib2.0-dev libfabric-dev pandoc libncurses5-dev

2)
git clone https://github.com/pmem/pmdk
cd pmdk
make -j$(nproc)
sudo make install

3)
git clone https://github.com/memkind/memkind
cd memkind
./autogen.sh
./configure
make
sudo make install

4) 
git clone https://github.com/pmem/libpmemobj-cpp
cd libpmemobj-cpp
mkdir build
cd build
cmake .. -DTESTS_USE_VALGRIND=OFF
make -j$(nproc)
sudo make install

5)
git clone https://github.com/SarthakMakhija/pmemdb

Once these steps are done,
(Remove/comment, `pmdk` / `libpmemobjcpp` from vcpkg.json)

1) mkdir build inside pmemdb
2) from the project root execute, cmake ../ -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg.cmake>
3) from the build directory execute, cmake --build .
