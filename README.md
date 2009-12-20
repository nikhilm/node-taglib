node-taglib
===========

node-taglib is ( currently ) a simple binding to [TagLib](http://developer.kde.org/~wheeler/taglib/) in Javascript.

It requires [node.js](http://nodejs.org).

For now it exposes one simple function, `getFileTags( path )`

## Build

    # make sure you have node and taglib installed
    # checkout repository
    cd node-taglib
    node-waf configure build
    node example.js /path/to/mp3_or_ogg_file

Just view the 4-line example.js to see how what you get back.

Right now it doesn't fail with very nice errors since I haven't figured out node and v8 that well.
