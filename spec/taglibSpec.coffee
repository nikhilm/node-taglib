assert = require 'assert'
vows = require 'vows'
Taglib = require __dirname + '/../lib/taglib'

vows.describe('taglib bindings').addBatch
  'parsing tags from given file':
    topic: Taglib.parse(__dirname+'/sample.mp3')
    'should be a `A bit-bucket full of tags`': (tags) ->
      assert.equal 'A bit-bucket full of tags', tags.title
    'should be by `gitzer\'s`': (tags) ->
      assert.equal 'gitzer\'s', tags.artist
    'should be on `Waffles for free!`': (tags) ->
      assert.equal 'Waffles for free!', tags.album
    'should be the first': (tags) ->
      assert.equal 1, tags.track
    'should be from 2011': (tags) ->
      assert.equal 2011, tags.year
.export(module)
