#!/bin/sh
make || exit 1
npm link vows
./node_modules/vows/bin/vows --spec spec/taglibSpec.js spec/resolverSpec.js spec/buffersSpec.js
