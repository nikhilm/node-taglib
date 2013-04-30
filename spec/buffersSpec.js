var assert = require('assert'),
    vows = require('vows'),
    fs = require('fs'),
    Taglib = require('../index')

vows.describe('taglib bindings: Buffers')
.addBatch({
  'reading metadata from mp3 buffer': {
    topic: function() {
      var buf = fs.readFileSync(__dirname+'/sample.mp3');
      Taglib.read(buf, 'mpeg', this.callback);
    },

    'should be called with two arguments': function (err, metadata) {
      assert.isNull(err);
      assert.isObject(metadata);
    },

    'reading metadata': {
      topic: function() {
        var buf = fs.readFileSync(__dirname+'/sample.mp3');
        Taglib.read(buf, 'mpeg', this.callback);
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

    'reading audioProperties': {
      topic: function() {
        var buf = fs.readFileSync(__dirname+'/blip.mp3');
        Taglib.read(buf, 'mpeg', this.callback);
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

  'reading data from a buffer with unknown format': {
    topic: function() {
      var buf = fs.readFileSync(__dirname+'/sample.mp3');
      Taglib.read(buf, '', this.callback);
    },

    'should raise an error': function(err, _, _) {
      assert.isNotNull(err);
      assert.match(err.message, /Unknown file format/);
    }
  },

  'reading data from a buffer with wrong format': {
    topic: function() {
      var buf = fs.readFileSync(__dirname+'/sample.mp3');
      Taglib.read(buf, 'ogg', this.callback);
    },

    'should raise an error': function(err, _, _) {
      assert.isNotNull(err);
      assert.match(err.message, /Failed to extract metadata/);
    }
  },

  'reading data from empty buffer': {
    topic: function() {
      var buf = new Buffer(0);
      Taglib.read(buf, 'mpeg', this.callback);
    },

    'should lead to empty metadatas and properties': function(err, metadata) {
      assert.isNull(err);
      assert.ok(metadata.artist == null);
    }
  },

  'writing to a metadata from a buffer': {
    topic: function() {
      return function() {
        var buf = fs.readFileSync(__dirname+'/sample.mp3');
        var metadata = Taglib.readSync(buf, 'mpeg');
        metadata.artist = 'nsm';
        metadata.writeSync();
      }
    },

    'should fail': function(topic) {
      assert.throws(topic);
    }
  }
}).export(module);
