var assert = require('assert'),
    vows = require('vows'),
    fs = require('fs'),
    Taglib = require('../index')

vows.describe('taglib bindings')
.addBatch({
  'opening UTF-8 Path': {
    topic: Taglib.readSync(__dirname + '/sample-with-ütf.mp3'),
    'should be a `Metadata`': function(m) {
      assert.equal(Taglib.Metadata, m.constructor);
    }
  },

  'ASF support yes/no should be defined': {
    topic: Taglib.WITH_ASF,
    'is boolean': function(topic) {
      assert.isBoolean(topic);
    }
  },

  'MP4 support yes/no should be defined': {
    topic: Taglib.WITH_MP4,
    'is boolean': function(topic) {
      assert.isBoolean(topic);
    }
  },

  'reading metadata from File using readSync': {
    topic: Taglib.readSync(__dirname+'/sample.mp3'),
    'should be a `Metadata`': function (metadata) {
      assert.equal(Taglib.Metadata, metadata.constructor);
    },
    'should be `A bit-bucket full of tags`': function (metadata) {
      assert.equal('A bit-bucket full of tags', metadata.title);
    },
    'should be a `A bit-bucket full of tags`': function (metadata) {
      assert.equal('A bit-bucket full of tags', metadata.title);
    },
    'should be by `gitzer\'s`': function (metadata) {
      assert.equal('gitzer\'s', metadata.artist);
    },
    'should be on `Waffles for free!`': function (metadata) {
      assert.equal("Waffles for free!", metadata.album);
    },
    'should be the first': function (metadata) {
      assert.equal(1, metadata.track)
    },
    'should be from 2011': function (metadata) {
      assert.equal(2011, metadata.year);
    },
    'should have a silly comment': function(metadata) {
      assert.equal("Salami Wiglet.", metadata.comment);
    }
  },

  'reading metadata from non-existent file using readSync': {
    // nested functions because vows automatically calls a topic
    // function
    topic: function() {
        return function() {
          return Taglib.readSync('thisfileobviouslyshouldnot.exist');
        }
    },

    'should throw an exception': function(topic) {
        assert.throws(topic, /readable/);
    }
  },

  'reading metadata from a non-audio file using readSync': {
    topic: function() {
        return function() {
          return Taglib.readSync(__filename);
        }
    },

    'should throw an exception': function(topic) {
        assert.throws(topic, /extract metadata/);
    }
  },

  'writing metadata to a file': {
    topic: function() {
      var filename = __dirname+'/sample-write.mp3';
      fs.writeFileSync(filename, fs.readFileSync(__dirname+'/sample.mp3'));
      var t = Taglib.readSync(filename);
      t.title = 'Something completely different…';
      t.writeSync();
      return filename;
    },
    'should have written `Something completely different…` to title': function (filename) {
      var metadata = Taglib.readSync(filename);
      assert.equal(metadata.title, "Something completely different…");
    }
  },

  'stripping metadata from File': {
    topic: function() {
      var filename, t;
      filename = __dirname + '/sample-clean.mp3';
      fs.writeFileSync(filename, fs.readFileSync(__dirname + '/sample.mp3'));
      m = Taglib.readSync(filename);
      m.title = null;
      m.artist = null;
      m.album = null;
      m.genre = null;
      m.comment = null;
      // for numeric, test with both 0 and null
      m.year = 0;
      m.track = null;
      m.writeSync();
      return filename;
    },

    'should result in a metadata that is empty': function(filename) {
      var m = Taglib.readSync(filename);
      assert.isNull(m.artist);
      assert.isNull(m.title);
      assert.isNull(m.album);
      assert.isNull(m.genre);
      assert.isNull(m.comment);
      assert.equal(m.year, 0);
      assert.equal(m.track, 0);
    }
  },

  'reading metadata asynchronously from a file': {
    topic: function() {
        Taglib.read(__dirname+'/sample.mp3', this.callback);
    },
    'should be a `Metadata`': function (metadata) {
      assert.equal(Taglib.Metadata, metadata.constructor);
    },
    'should be `A bit-bucket full of tags`': function (metadata) {
      assert.equal('A bit-bucket full of tags', metadata.title);
    },
    'should be by `gitzer\'s`': function (metadata) {
      assert.equal('gitzer\'s', metadata.artist);
    },
    'should be on `Waffles for free!`': function (metadata) {
      assert.equal("Waffles for free!", metadata.album);
    },
    'should be the first': function (metadata) {
      assert.equal(1, metadata.track)
    },
    'should be from 2011': function (metadata) {
      assert.equal(2011, metadata.year);
    },
    'should have a silly comment': function(metadata) {
      assert.equal("Salami Wiglet.", metadata.comment);
    }
  },

  'writing metadata asynchronously to a file': {
    topic: function() {
      var filename = __dirname+'/sample-write-async.mp3';
      fs.writeFileSync(filename, fs.readFileSync(__dirname+'/sample.mp3'));
      var self = this;
      Taglib.read(filename, function(err, metadata) {
        if (err) {
          self.callback(err);
        }
        metadata.title = 'Something completely different…';
        metadata.write(function(err) {
          self.callback(err, filename);
        });
      });
    },
    'should have written `Something completely different…` to title': function (err, filename) {
      assert.isNull(err);
      var m = Taglib.readSync(filename);
      assert.equal(m.title, "Something completely different…");
    }
  },

  'reading file metadata asynchronously': {
    topic: function() {
      Taglib.read(__dirname+'/sample.mp3', this.callback);
    },

    'should be called with two arguments': function (err, metadata) {
      assert.isNull(err);
      assert.isObject(metadata);
    },

    'reading tags': {
      topic: function() {
        Taglib.read(__dirname+'/sample.mp3', this.callback);
      },

      'title should be `A bit-bucket full of tags`': function (metadata) {
        assert.equal(metadata.title, 'A bit-bucket full of tags');
      },
      'artist should be by `gitzer\'s`': function (metadata) {
        assert.equal(metadata.artist, 'gitzer\'s');
      },
      'album should be on `Waffles for free!`': function (metadata) {
        assert.equal(metadata.album, "Waffles for free!");
      },
      'track should be the first': function (metadata) {
        assert.equal(metadata.track, 1);
      },
      'should be from 2011': function (metadata) {
        assert.equal(metadata.year, 2011);
      },
      'should have a silly comment': function(metadata) {
        assert.equal(metadata.comment, "Salami Wiglet.");
      }
    },

    'reading audio properties': {
      topic: function() {
        Taglib.read(__dirname+'/blip.mp3', this.callback);
      },

      'should have length 1 second': function(err, metadata) {
        assert.equal(metadata.length, 1);
      },
      'should have bitrate 128kbps': function(err, metadata) {
        assert.equal(metadata.bitrate, 128);
      },
      'should have sampleRate 44100Hz': function(err, metadata) {
        assert.equal(metadata.sampleRate, 44100);
      },
      'should have 2 channels': function(err, metadata) {
        assert.equal(metadata.channels, 2);
      }
    },
  },

  'read() on a file without tags': {
    topic: function() {
      Taglib.read(__dirname+'/blip.mp3', this.callback);
    },

    'should have empty artist': function(err, metadata, _) {
      assert.isObject(metadata) && assert.ok(metadata.artist == null);
    }
  },

  'read() on non-existent file': {
    topic: function() {
      Taglib.read('thisfileobviouslyshouldnot.exist', this.callback);
    },

    'should error': function(err, m) {
      assert.isNotNull(err);
    }
  }
}).export(module);
