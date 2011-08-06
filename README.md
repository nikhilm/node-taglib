node-taglib
===========

node-taglib is ( currently ) a simple binding to [TagLib](http://developer.kde.org/~wheeler/taglib/) in Javascript.

It requires [node.js](http://nodejs.org).

For now it exposes one simple class,

use it like this

    var Tag = require('taglib').Tag;
    var t = new Tag( path );
    t.title # => "Another one bites the dust"
    t.artist # => "Kween"
    t.artist = "Queen"
    t.save() # => true

## Build

    # make sure you have node and taglib installed
    # checkout repository
    cd node-taglib
    node-waf configure build
    node examples/simple.js /path/to/mp3_or_ogg_file

Just view the 4-line `examples/simple.js` to see how and what you get back.

Right now it doesn't fail with very nice errors since I haven't figured out node and v8 that well.

Credits
-------

Original author - Nikhil Marathe <nsm.nikhil@gmail.com>
Write support, coffee-script conversion, package.json - Lennart Melzer (https://github.com/lennart)
