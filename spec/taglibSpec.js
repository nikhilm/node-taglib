var assert = require('assert')
var vows = require('vows')
var fs = require('fs')
var Taglib = require(__dirname + '/../lib/taglib')

vows.describe('taglib bindings')
.addBatch({
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

  'reading Tags from File': {
    topic: new Taglib.Tag(__dirname+'/sample.mp3'),
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

  'reading Tags from non-existent file': {
    // nested functions because vows automatically calls a topic
    // function
    topic: function() {
        return function() {
          return new Taglib.Tag('thisfileobviouslyshouldnot.exist');
        }
    },

    'should throw an exception': function(topic) {
        assert.throws(topic, /readable/);
    }
  },

  'reading Tags from a non-audio file': {
    topic: function() {
        return function() {
          return new Taglib.Tag(__filename);
        }
    },

    'should throw an exception': function(topic) {
        assert.throws(topic, /extract tags/);
    }
  },

  'reading AudioProperties from non-existent file': {
    topic: function() {
        return function() {
          return new Taglib.AudioProperties('thisfileobviouslyshouldnot.exist');
        }
    },

    'should throw an exception': function(topic) {
        assert.throws(topic, /readable/);
    }
  },

  'reading AudioProperties from a non-audio file': {
    topic: function() {
        return function() {
          return new Taglib.AudioProperties(__filename);
        }
    },

    'should throw an exception': function(topic) {
        assert.throws(topic, /extract audio properties/);
    }
  },

  'writing Tags to File': {
    topic: function() {
      var filename = __dirname+'/sample-write.mp3';
      fs.writeFileSync(filename, fs.readFileSync(__dirname+'/sample.mp3'));
      var t = new Taglib.Tag(filename);
      t.title = 'Something completely different…';
      t.save();
      return filename;
    },
    'should have written `Something completely different…` to title': function (filename) {
      var tag = new Taglib.Tag(filename);
      assert.equal("Something completely different…", tag.title);
    }
  },

  'reading Properties from File': {
    topic: new Taglib.AudioProperties(__dirname+'/blip.mp3'),
    'should be a `AudioProperties`': function (prop) {
      assert.equal(Taglib.AudioProperties, prop.constructor);
    },
    'should have length 1 second': function(prop) {
      assert.equal(1, prop.length);
    },
    'should have bitrate 128kbps': function(prop) {
      assert.equal(128, prop.bitrate);
    },
    'should have sampleRate 44100Hz': function(prop) {
      assert.equal(44100, prop.sampleRate);
    },
    'should have 2 channels': function(prop) {
      assert.equal(2, prop.channels);
    },
  }
}).export(module);
