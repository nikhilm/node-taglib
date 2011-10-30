node-taglib
===========

node-taglib is a simple binding to [TagLib](http://developer.kde.org/~wheeler/taglib/) in Javascript.

It requires [node.js](http://nodejs.org).

For now it exposes Tag and AudioProperties.

## Usage

    var Tag = require('taglib').Tag;
    var t = new Tag(path);
    t.title # => "Another one bites the dust"
    t.artist # => "Kween"
    t.artist = "Queen"
    t.save() # => true

## Build

    # make sure you have node and taglib installed
    git clone git://github.com/nikhilm/node-taglib.git
    cd node-taglib
    node-waf configure build
    node examples/simple.js /path/to/mp3_or_ogg_file

The `examples` show usage.

Contributors are listed at: <https://github.com/nikhilm/node-taglib/contributors>
