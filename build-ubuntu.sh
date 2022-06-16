#!/bin/sh

rootDirectory=$1
currentDirectory=$(pwd)

# create a directory pmem inside the rootDirectory
createPmemDependenciesRootDirectory() {
    directory=$(getPmemDependenciesRootDirectory)
    mkdir -p $directory
}

# root directory = <RootPath>/pmem/
getPmemDependenciesRootDirectory() {
    local directory="$rootDirectory/pmem-dependencies/"
    echo $directory
}

# change the directory to pmem root directory
changeToPmemDependenciesRootDirectory() {
    directory=$(getPmemDependenciesRootDirectory)
    cd $directory
}

# install vcpkg
getVcpkg () {
   git clone https://github.com/Microsoft/vcpkg.git
    ./vcpkg/bootstrap-vcpkg.sh
}

# install PMDK
getPmdk() {
    git clone https://github.com/pmem/pmdk
    cd pmdk
    make -j$(nproc)
    make install
}

# install memkind
getMemkind() {
    git clone https://github.com/memkind/memkind
    cd memkind
    ./autogen.sh
    ./configure
    make
    make install
}

# install libpmemobj-cpp
getLibpmemobj() {
    git clone https://github.com/pmem/libpmemobj-cpp
    cd libpmemobj-cpp
    mkdir build
    cd build
    cmake .. -DTESTS_USE_VALGRIND=OFF
    make -j$(nproc)
    make install
}

# build pmemdb
buildPmemdb() {
    cd $currentDirectory
    if [ ! -d "build" ]
    then
        mkdir build    
    fi
  
    cd build
    directory=$(getPmemDependenciesRootDirectory)
    cmake -DCMAKE_TOOLCHAIN_FILE=$directory/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_MANIFEST_DIR=$currentDirectory/manifest/ubuntu/ -DCMAKE_MAKE_PROGRAM=make ../
    cmake --build .
}

# install dependencies
getDependencies() {
    apt install curl zip unzip tar autoconf automake build-essential libdaxctl-dev libndctl-dev libnuma-dev libtbb-dev libtool rapidjson-dev pkg-config libglib2.0-dev libfabric-dev pandoc libncurses5-dev
}

# entry point into the script
main() {
    createPmemDependenciesRootDirectory
    changeToPmemDependenciesRootDirectory
    getVcpkg

    changeToPmemDependenciesRootDirectory
    getDependencies
    
    changeToPmemDependenciesRootDirectory
    getPmdk
    
    changeToPmemDependenciesRootDirectory
    getMemkind

    changeToPmemDependenciesRootDirectory
    getLibpmemobj
    
    buildPmemdb
}

main