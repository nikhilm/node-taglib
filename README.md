node-taglib
===========

node-taglib is a simple binding to
[TagLib](http://developer.kde.org/~wheeler/taglib/) in Javascript.

It requires [node.js](http://nodejs.org).

node-taglib offers only an abstract interface without giving access to extended
file-specific attributes. It does allow custom resolvers though. Synchronous
write support is supported for Tag.

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
        tag.saveSync(); // synchronous
    });

    // synchronous API
    var tag = taglib.tagSync(path);

    tag.title; // => "Another one bites the dust"
    tag.artist; // => "Kween"
    tag.artist = "Queen";

    tag.isEmpty(); // => false

    tag.save(function(err) {
    });

## Installation

### via npm (Recommended)

    npm install taglib

### From source

    # make sure you have node and taglib installed
    git clone git://github.com/nikhilm/node-taglib.git
    cd node-taglib
    npm install .
    node examples/simple.js /path/to/mp3_or_ogg_file
    # you can now require('./taglib')

The `examples` show usage.

## API

### read(path, callback)
### read(buffer, format, callback)

The function you will most likely want to use. `callback` should have signature
`callback(err, metadata)`.
If there was an error reading the file, `err` will be non-null and `metadata`
will be `null`.

`metadata` will be a Metadata object with the following tag information.
(node-taglib currently supports only the fields common to all formats):

* title   (string)
* album   (string)
* comment (string)
* artist  (string)
* track   (string)
* year    (integer)
* genre   (string)

In addition, the following audio properties may be available. All of them are
integers:

* length
* bitrate
* sampleRate
* channels

Writing audio properties is not supported.

In the second variant, which can read from a buffer, `format` should be
a string as specified in [Formats](#formats).

### readSync(path)
### readSync(buffer, format)

Read the metadata from the file at `path` _synchronously_. Returns a `Metadata`
object. If errors occurred, throws an exception.

The exception will be an object with field `code` having the integer error code
(`errno.h`) and field `message` will have a string representation.

In the second variant, which can read from a buffer, `format` should be
a string as specified in [Formats](#formats).

### Metadata

**NOTE: A Metadata object should *NOT* be created using `new`. Instead use
`read()` or `readSync()`**

A Metadata object allows _read-write_ access to all the tag fields. Audio
properties are read-only. For valid field names see `read()` above.

To get a value, simply access the field -- `metadata.artist`.

To set a value, assign a value to the field -- `metadata.year = 2012`. You
**will have to call `save()` or `saveSync()`** to actually save the changes to
the file on disc.

### Metadata.write(callback)

Save any changes in the meta-data to disk _asynchronously_. `callback` will be
invoked once the save is done, and should have a signature `(err)`. `err` will
be `null` if the save was successful, otherwise it will be an object with
`message` having the error string and `path` having the file path.

Metadata objects obtained from a buffer will throw an exception if `save()` is
called.

### Metadata.writeSync()

Save any changes in the Tag meta-data to disk _synchronously_. Throws an
exception if the save failed.

Metadata objects obtained from a buffer will throw an exception if `save()` is
called.

### taglib.addResolvers(\[resolver1\[, resolver2\[, ...]]])

Adds JavaScript functions that will be called to resolve the filetype of
a file. Each resolver will be added to the front of the resolver queue. So the
last resolver will be called first. Multiple calls to `addResolvers` are
allowed.

Each resolver must be a JavaScript function which takes a `filename` parameter
and returns a format `string`. List of [formats](#formats).

### Formats {#formats}

Any place where `node-taglib` expects a format can be passed one of these
(case-insensitive):

    "MPEG"
    "OGG"      - Ogg Vorbis
    "OGG/FLAC" - Ogg FLAC
    "FLAC"
    "MPC"
    "WV"
    "SPX"      - Ogg Speex
    "TTA"
    "MP4"
    "ASF"
    "AIFF"     - RIFF AIFF
    "WAV"      - RIFF WAV
    "APE"
    "MOD"
    "S3M"
    "IT"
    "XM"

These correspond directly to the [filetypes
supported](http://developer.kde.org/~wheeler/taglib/api/classTagLib_1_1File.html)
by TagLib.  If the filetype cannot be determined, return anything other than
one of these literals.

Asynchronous resolvers (which indicate the filetype via a callback rather than
a return value) are not supported.

### taglib.WITH_ASF

A boolean representing whether node-taglib supports ASF files. Depends on
feature being enabled in TagLib.

### taglib.WITH_MP4

A boolean representing whether node-taglib supports MP4 files. Depends on
feature being enabled in TagLib.

Contributors are listed at: <https://github.com/nikhilm/node-taglib/contributors>
