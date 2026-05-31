git submodule update --init --recursive
mkdir -p src/boost
local link="https://archives.boost.io/release/1.83.0/source/boost_1_83_0.tar.gz"
wget $link || curl -o boost_1_83_0.tar.gz $link
tar -xzf boost_1_83_0.tar.gz --verbose
cd boost_1_83_0
./bootstrap.sh --prefix=../src/boost && ./b2 install --prefix=../src/boost