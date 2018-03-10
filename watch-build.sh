#!/bin/sh

# first, npm install -g nodemon

nodemon --watch src/game -e h,c,cpp,frag,vert,glsl,hlsl --exec "cd src/build && make"
