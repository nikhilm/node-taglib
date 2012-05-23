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
}).export(module);
