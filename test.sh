#!/bin/sh
node-gyp rebuild || exit 1
npm link vows bindings
./node_modules/vows/bin/vows --spec spec/taglibSpec.js spec/resolverSpec.js spec/buffersSpec.js
