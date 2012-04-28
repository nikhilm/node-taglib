#!/bin/sh
npm link vows
./node_modules/vows/bin/vows spec/taglibSpec.js --spec
