#!/bin/sh

# first, npm install -g nodemon

nodemon --delay 60ms --watch bin --watch bin/engine_test --watch shaders -e dylib,frag,vert,glsl,hlsl --exec "cd bin && DYLD_LIBRARY_PATH=. LD_LIBRARY_PATH=. ./engine_test"
