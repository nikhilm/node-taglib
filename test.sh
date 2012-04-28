#!/bin/sh
node-waf configure build
npm link vows
./node_modules/vows/bin/vows spec/taglibSpec.js --spec
