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

### read(path, callback)

The function you will most likely want to use. `callback` should have signature
`callback(err, tag, audioProperties)` where `tag` and `audioProperties` are
plain-old JavaScript objects. For the distinction between these and `Tag`, see
`Tag` below.

tag can have the following fields. node-taglib currently supports only the
fields common to all formats:

* title   (string)
* album   (string)
* comment (string)
* artist  (string)
* track   (string)
* year    (integer)
* genre   (string)

The following fields are available in audioProperties, all are integers:

* length
* bitrate
* sampleRate
* channels

Writing audio properties is not supported.

### tag(path, callback)

Read the tag from the file at `path` _asynchronously_. The callback should have
signature `(err, tag)`. On success, `err` will be `null` and `tag` will be
a `Tag`. If errors occurred, `err` will contain the error and
`tag` will be `null`. `err` will be an object with field `code` having the
integer error code (`errno.h`) and field `message` will have a string
representation.

### tagSync(path)

Read the tag from the file at `path` _synchronously_. Returns a `Tag`. If
errors occurred, throws an exception.

### Tag

**NOTE: A Tag object should *NOT* be created using `new`. Instead use `tag()`
or `tagSync()`**

A Tag object allows _read-write_ access to all the meta-data fields. For valid
field names see `read()` above.

To get a value, simply access the field -- `tag.artist`.

To set a value, assign a value to the field -- `tag.year = 2012`. You **will
have to call `saveSync()`** to actually save the changes to the file on disc.

##### Large number of files

Due to TagLib's design, every `Tag` object in memory has to keep its backing
file descriptor open. If you are dealing with a large number of files, you will
soon run into problems because operating systems impose limits on how many
files a process can have open simultaneously. If you want to only read tags,
use `read()` instead as it will immediately close the file after the tag is
read.

### Tag.save(callback)

Save any changes in the Tag meta-data to disk _asynchronously_. `callback` will
be invoked once the save is done, and should have a signature `(err)`. `err`
will be `null` if the save was successful, otherwise it will be an object with
`message` having the error string and `path` having the file path.

### Tag.saveSync()

Save any changes in the Tag meta-data to disk _synchronously_.

### Tag.isEmpty()

Returns whether the tag is empty or not.

### taglib.WITH_ASF

A boolean representing whether node-taglib supports ASF files. Depends on
feature being enabled in TagLib.

### taglib.WITH_MP4

A boolean representing whether node-taglib supports MP4 files. Depends on
feature being enabled in TagLib.

Contributors are listed at: <https://github.com/nikhilm/node-taglib/contributors>
