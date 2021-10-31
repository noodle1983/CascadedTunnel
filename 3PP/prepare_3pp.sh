#!/bin/bash

rm -rf libevent && \
tar -xf libevent-2.1.12-stable.tar.gz && \
cd libevent-2.1.12-stable && \
./configure --prefix=$PWD/../libevent && \
make && make install && \
cd - && \
rm -rf libevent-2.1.12-stable

tar -xf libunwind-0.99-beta.tar.gz && tar -xf libunwind-0.99-beta.patch.tar.bz2 && cd libunwind-0.99-beta && CFLAGS="-g -fPIC" ./configure --prefix=$PROJ_BASE/3PP/libunwind && make install && cd -

tar -xf gperftools-2.6.90.tar.gz && cd gperftools-2.6.90 && ./configure --prefix=$PROJ_BASE/3PP/gperftools CPPFLAGS="-I$PROJ_BASE/libunwind/include -fPIC" LDFLAGS=-L/$PROJ_BASE/libunwind/lib && make install && cd -


