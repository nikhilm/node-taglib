var assert = require('assert'),
    vows = require('vows'),
    fs = require('fs'),
    Taglib = require('../index')

if (Taglib.addResolvers) {

Taglib.addResolvers(function(fn) {
  var data = fs.readFileSync(fn, 'ascii');
  if (data.substring(0, 3) == 'ID3')
    return 'mpeg';
  return '';
});

vows.describe('taglib bindings: Dumb mp3 Resolver')
.addBatch({
/*
  'reading Tags from File using readSync': {
    topic: Taglib.readSync(__dirname+'/sample.mp3'),
    'should be a `Metadata`': function (metadata) {
      assert.equal(Taglib.Metadata, metadata.constructor);
    },
    'should be `A bit-bucket full of tags`': function (tag) {
      assert.equal('A bit-bucket full of tags', tag.title);
    },
    'should be by `gitzer\'s`': function (tag) {
      assert.equal('gitzer\'s', tag.artist);
    },
    'should be on `Waffles for free!`': function (tag) {
      assert.equal("Waffles for free!", tag.album);
    },
    'should be the first': function (tag) {
      assert.equal(1, tag.track)
    },
    'should be from 2011': function (tag) {
      assert.equal(2011, tag.year);
    },
    'should have a silly comment': function(tag) {
      assert.equal("Salami Wiglet.", tag.comment);
    }
  },

  'reading Tags from non-existent file using readSync': {
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

    'should have an empty tag': function(topic) {
      assert.throws(topic, /extract metadata/);
    }
  },

  'writing metadata to File': {
    topic: function() {
      var filename = __dirname+'/sample-write.mp3';
      fs.writeFileSync(filename, fs.readFileSync(__dirname+'/sample.mp3'));
      var t = Taglib.readSync(filename);
      t.title = 'Something completely different…';
      t.writeSync();
      return filename;
    },
    'should have written `Something completely different…` to title': function (filename) {
      var m = Taglib.readSync(filename);
      assert.equal(m.title, "Something completely different…");
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
    'should result in empty metadata': function(filename) {
      var m = Taglib.readSync(filename);
      assert.isNull(m.artist);
      assert.isNull(m.title);
      assert.isNull(m.genre);
      assert.isNull(m.comment);
      assert.equal(m.year, 0);
      assert.equal(m.track, 0);
    }
  },

  'writing metadata to a file asynchronously': {
    topic: function() {
      var filename = __dirname+'/sample-write-async.mp3';
      fs.writeFileSync(filename, fs.readFileSync(__dirname+'/sample.mp3'));
      var self = this;
      Taglib.read(filename, function(err, m) {
        if (err) {
          self.callback(err);
        }
        m.title = 'Something completely different…';
        m.write(function(err) {
          self.callback(err, filename);
        });
      });
    },
    'should have written `Something completely different…` to title': function (filename) {
      var m = Taglib.readSync(filename);
      assert.equal(m.title, "Something completely different…");
    }
  },

*/
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

      'title should be `A bit-bucket full of tags`': function (err, metadata) {
        assert.equal(metadata.title, 'A bit-bucket full of tags');
      },
      'artist should be by `gitzer\'s`': function (err, metadata) {
        assert.equal(metadata.artist, 'gitzer\'s');
      },
      'album should be on `Waffles for free!`': function (err, metadata) {
        assert.equal(metadata.album, "Waffles for free!");
      },
      'track should be the first': function (err, metadata) {
        assert.equal(metadata.track, 1);
      },
      'should be from 2011': function (err, metadata) {
        assert.equal(metadata.year, 2011);
      },
      'should have a silly comment': function(err, metadata) {
        assert.equal(metadata.comment, "Salami Wiglet.");
      },
    },

    'reading audio properties': {
      topic: function() {
        Taglib.read(__dirname+'/blip.mp3', this.callback);
      },
      'should have length 1 second': function(err, metadata) {
      console.log(metadata);
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

    'should have empty metadata object': function(err, metadata) {
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

}
