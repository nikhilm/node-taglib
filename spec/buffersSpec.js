var assert = require('assert'),
    vows = require('vows'),
    fs = require('fs'),
    Taglib = require(__dirname + '/../taglib');

vows.describe('taglib bindings: Buffers')
.addBatch({
  'reading metadata from mp3 buffer': {
    topic: function() {
      var buf = fs.readFileSync(__dirname+'/sample.mp3');
      Taglib.read(buf, 'mpeg', this.callback);
    },

    'should be called with three arguments': function (err, tag, props) {
      assert.isNull(err);
      assert.isObject(tag);
      assert.isObject(props);
    },

    'reading tags': {
      topic: function() {
        var buf = fs.readFileSync(__dirname+'/sample.mp3');
        Taglib.read(buf, 'mpeg', this.callback);
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
        var buf = fs.readFileSync(__dirname+'/blip.mp3');
        Taglib.read(buf, 'mpeg', this.callback);
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
      assert.match(err.message, /Failed to extract tags/);
    }
  },

  'reading data from empty buffer': {
    topic: function() {
      var buf = new Buffer(0);
      Taglib.read(buf, 'mpeg', this.callback);
    },

    'should lead to empty tags and properties': function(err, tag, prop) {
      assert.isNull(err);
      assert.isEmpty(tag);
      assert.isObject(prop);
    }
  },

  /* * T A G S Y N C * */
  'tagSync metadata from mp3 buffer': {
    topic: function() {
      var buf = fs.readFileSync(__dirname+'/sample.mp3');
      return Taglib.tagSync(buf, 'mpeg');
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

  'tagSync data from a buffer with unknown format': {
    topic: function() {
      return function() {
        var buf = fs.readFileSync(__dirname+'/sample.mp3');
        return Taglib.tagSync(buf, '', this.callback);
      }
    },

    'should raise an error': function(topic) {
      assert.throws(topic, /Unknown file format/);
    }
  },

  'tagSync data from a buffer with wrong format': {
    topic: function() {
      return function() {
        var buf = fs.readFileSync(__dirname+'/sample.mp3');
        return Taglib.tagSync(buf, 'ogg');
      }
    },

    'should raise an error': function(topic) {
      assert.throws(topic, /Failed to extract tags/);
    }
  },

  'tagSync data from empty buffer': {
    topic: function() {
      var buf = new Buffer(0);
      return Taglib.tagSync(buf, 'mpeg');
    },

    'should lead to empty tags': function(tag) {
      assert.isObject(tag);
    }
  }
}).export(module);
