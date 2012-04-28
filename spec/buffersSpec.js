var assert = require('assert'),
    vows = require('vows'),
    fs = require('fs'),
    Taglib = require(__dirname + '/../taglib');

vows.describe('taglib bindings: Buffers')
.addBatch({
  'reading from an mp3 Buffer': {
    topic: function() {
      var contents = fs.readFileSync(__dirname + '/sample.mp3');
      var resolver = function(buffer) {
        return 'mp3';
      }
      Taglib.readBuffer(contents, [resolver], this.callback);
    },

    'should not error': function(err, tag, props) {
      assert.isNull(err);
      assert.isObject(tag);
      assert.isObject(props);
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
  }
}).export(module);
