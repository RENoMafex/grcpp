#!/bin/bash

# check if git, tar and wget or curl are installed
if ! command -v git &> /dev/null ; then
	echo "git is not installed. Please install git and try again."
	exit 1
fi
if ! command -v tar &> /dev/null ; then
	echo "tar is not installed. Please install tar and try again."
	exit 1
fi
if ! command -v wget &> /dev/null && ! command -v curl &> /dev/null ; then
	echo "wget or curl is not installed. Please install wget or curl and try again."
	exit 1
fi

# init catch2
git submodule update --init --recursive

# init boost libs
mkdir -p src/boost
link="https://archives.boost.io/release/1.83.0/source/boost_1_83_0.tar.gz"
wget $link || curl -o boost_1_83_0.tar.gz $link
tar -xzf boost_1_83_0.tar.gz --verbose
rm -rfv boost_1_83_0.tar.gz
cd boost_1_83_0
./bootstrap.sh --prefix=../src/boost
./b2 install --prefix=../src/boost
cd ..
rm -rfv boost_1_83_0