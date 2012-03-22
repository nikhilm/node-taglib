node-taglib
===========

node-taglib is a simple binding to
[TagLib](http://developer.kde.org/~wheeler/taglib/) in Javascript.

It requires [node.js](http://nodejs.org).

For now it exposes Tag and AudioProperties. Synchronous write support is
supported for Tag.

**NOTE: Asynchronous API requires use of TagLib [from git][taglib-git] since
certain bugs present in the released v1.7 cause problems.**

[taglib-git]: https://github.com/taglib/taglib

## Example

    // load the library
    var taglib = require('taglib');

    // asynchronous API
    taglib.tag(path, function(err, tag) {
        tag.artist; // => "Queen"
        tag.title = "Erm";
        tag.saveSync();
    });

    // synchronous API
    var tag = taglib.tagSync(path);

    tag.title; // => "Another one bites the dust"
    tag.artist; // => "Kween"
    tag.artist = "Queen";

    tag.isEmpty(); // => false

    tag.saveSync(); // => true

## Installation

### via npm (Recommended)

    npm install taglib

### From source

    # make sure you have node and taglib installed
    git clone git://github.com/nikhilm/node-taglib.git
    cd node-taglib
    node-waf configure build
    node examples/simple.js /path/to/mp3_or_ogg_file
    # you can now require('./taglib')

The `examples` show usage.

## API

TODO

Contributors are listed at: <https://github.com/nikhilm/node-taglib/contributors>
