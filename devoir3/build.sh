#!/bin/bash

[[ ! -d "bin" ]] && mkdir bin
ln -sf bin/tp3client tp3client
ln -sf bin/tp3srv tp3srv

cd bin/
cmake ../
make
