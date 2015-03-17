#!/bin/sh
cp bin/simple_rendering bin/simple_rendering.bc
em++ bin/simple_rendering.bc lib/* -o web/index.html -s USE_SDL=2 -s FULL_ES2=1 -O2 --preload-file resources/ --shell-file web/shell.html
