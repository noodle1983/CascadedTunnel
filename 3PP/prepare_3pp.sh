#!/bin/bash

rm -rf libevent && \
tar -xf libevent-2.1.11-stable.tar.gz && \
cd libevent-2.1.11-stable && \
./configure --prefix=$PWD/../libevent && \
make && make install && \
cd - && \
rm -rf libevent-2.1.11-stable

# rm -rf libboost && \
# tar -xf boost_1_73_0.tar.bz2 && \
# cd boost_1_73_0 && \
# ./bootstrap.sh --prefix=$PWD/../libboost
# ./b2 install --prefix=$PWD/../libboost && \
# cd - && \
# rm -rf boost_1_73_0
