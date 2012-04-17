var assert = require('assert'),
    vows = require('vows'),
    fs = require('fs'),
    Taglib = require(__dirname + '/../taglib');

vows.describe('taglib bindings')
.addBatch({
  'opening UTF-8 Path': {
    topic: Taglib.tagSync(__dirname + '/sample-with-ütf.mp3'),
    'should be a `Tag`': function(tag) {
      assert.equal(Taglib.Tag, tag.constructor);
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

  'reading Tags from File using tagSync': {
    topic: Taglib.tagSync(__dirname+'/sample.mp3'),
    'should be a `Tag`': function (tag) {
      assert.equal(Taglib.Tag, tag.constructor);
    },
    'should be `A bit-bucket full of tags`': function (tag) {
      assert.equal('A bit-bucket full of tags', tag.title);
    },
    'should be a `A bit-bucket full of tags`': function (tag) {
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

  'reading Tags from non-existent file using tagSync': {
    // nested functions because vows automatically calls a topic
    // function
    topic: function() {
        return function() {
          return Taglib.tagSync('thisfileobviouslyshouldnot.exist');
        }
    },

    'should throw an exception': function(topic) {
        assert.throws(topic, /readable/);
    }
  },

  'reading Tags from a non-audio file using tagSync': {
    topic: function() {
        return function() {
          return Taglib.tagSync(__filename);
        }
    },

    'should throw an exception': function(topic) {
        assert.throws(topic, /extract tags/);
    }
  },

  'writing Tags to File': {
    topic: function() {
      var filename = __dirname+'/sample-write.mp3';
      fs.writeFileSync(filename, fs.readFileSync(__dirname+'/sample.mp3'));
      var t = Taglib.tagSync(filename);
      t.title = 'Something completely different…';
      t.saveSync();
      return filename;
    },
    'should have written `Something completely different…` to title': function (filename) {
      var tag = Taglib.tagSync(filename);
      assert.equal(tag.title, "Something completely different…");
    }
  },

  'stripping Tags from File': {
    topic: function() {
      var filename, t;
      filename = __dirname + '/sample-clean.mp3';
      fs.writeFileSync(filename, fs.readFileSync(__dirname + '/sample.mp3'));
      t = Taglib.tagSync(filename);
      t.title = null;
      t.artist = null;
      t.album = null;
      t.genre = null;
      t.year = null;
      t.comment = null;
      t.track = null;
      t.saveSync();
      return filename;
    },
    'should result in a Tag that `isEmpty`': function(filename) {
      var tag;
      tag = Taglib.tagSync(filename);
      assert.ok(tag.isEmpty());
    }
  },

  'reading Tag from a file asynchronously': {
    topic: function() {
        Taglib.tag(__dirname+'/sample.mp3', this.callback);
    },
    'should be a `Tag`': function (tag) {
      assert.equal(Taglib.Tag, tag.constructor);
    },
    'should be `A bit-bucket full of tags`': function (tag) {
      assert.equal('A bit-bucket full of tags', tag.title);
    },
    'should be a `A bit-bucket full of tags`': function (tag) {
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

  'writing Tag to a file asynchronously': {
    topic: function() {
      var filename = __dirname+'/sample-write-async.mp3';
      fs.writeFileSync(filename, fs.readFileSync(__dirname+'/sample.mp3'));
      var self = this;
      Taglib.tag(filename, function(err, tag) {
        if (err) {
          self.callback(err);
        }
        tag.title = 'Something completely different…';
        tag.save(function(err) {
          self.callback(err, filename);
        });
      });
    },
    'should have written `Something completely different…` to title': function (filename) {
      var tag = Taglib.tagSync(filename);
      assert.equal(tag.title, "Something completely different…");
    }
  },

  'reading file metadata asynchronously': {
    topic: function() {
      Taglib.read(__dirname+'/sample.mp3', this.callback);
    },

    'should be called with three arguments': function (err, tag, props) {
      assert.isNull(err);
      assert.isObject(tag);
      assert.isObject(props);
    },

    'reading tags': {
      topic: function() {
        Taglib.read(__dirname+'/sample.mp3', this.callback);
      },

      'title should be `A bit-bucket full of tags`': function (tag) {
        assert.equal(tag.title, 'A bit-bucket full of tags');
      },
      'artist should be by `gitzer\'s`': function (tag) {
        assert.equal(tag.artist, 'gitzer\'s');
      },
      'album should be on `Waffles for free!`': function (tag) {
        assert.equal(tag.album, "Waffles for free!");
      },
      'track should be the first': function (tag) {
        assert.equal(tag.track, 1);
      },
      'should be from 2011': function (tag) {
        assert.equal(tag.year, 2011);
      },
      'should have a silly comment': function(tag) {
        assert.equal(tag.comment, "Salami Wiglet.");
      }
    },

    'reading audioProperties': {
      topic: function() {
        Taglib.read(__dirname+'/blip.mp3', this.callback);
      },

      'should have length 1 second': function(err, _, prop) {
        assert.equal(prop.length, 1);
      },
      'should have bitrate 128kbps': function(err, _, prop) {
        assert.equal(prop.bitrate, 128);
      },
      'should have sampleRate 44100Hz': function(err, _, prop) {
        assert.equal(prop.sampleRate, 44100);
      },
      'should have 2 channels': function(err, _, prop) {
        assert.equal(prop.channels, 2);
      }
    },
  },

  'read() on a file without tags': {
    topic: function() {
      Taglib.read(__dirname+'/blip.mp3', this.callback);
    },

    'should have empty tag object': function(err, tag, _) {
      assert.isObject(tag) && assert.isEmpty(tag);
    }
  },

  'read() on non-existent file': {
    topic: function() {
      Taglib.read('thisfileobviouslyshouldnot.exist', this.callback);
    },

    'should error': function(err, _, _) {
      assert.isNotNull(err);
    }
  }
}).export(module);
