assert = require 'assert'
vows = require 'vows'
fs = require 'fs'
Taglib = require __dirname + '/../lib/taglib'

vows.describe('taglib bindings')#.addBatch
#  'parsing tags from given file':
#    topic: Taglib.parse(__dirname+'/sample.mp3')
.addBatch
  'reading Tags from File':
    topic: new Taglib.Tag(__dirname+'/sample.mp3')
    'should be a `Tag`': (tag) ->
      assert.equal Taglib.Tag, tag.constructor
    'should be `A bit-bucket full of tags`': (tag) ->
      assert.equal 'A bit-bucket full of tags', tag.title
    'should be a `A bit-bucket full of tags`': (tag) ->
      assert.equal 'A bit-bucket full of tags', tag.title
    'should be by `gitzer\'s`': (tag) ->
      assert.equal 'gitzer\'s', tag.artist
    'should be on `Waffles for free!`': (tag) ->
      assert.equal "Waffles for free!", tag.album
    'should be the first': (tag) ->
      assert.equal 1, tag.track
    'should be from 2011': (tag) ->
      assert.equal 2011, tag.year
  'writing Tags to File':
    topic: ->
      filename = __dirname+'/sample-write.mp3'
      fs.writeFileSync filename, fs.readFileSync(__dirname+'/sample.mp3')
      t = new Taglib.Tag filename
      t.title = 'Something completely different…'
      t.save()
      filename
    'should have written `Something completely different…` to title': (filename) ->
      tag = new Taglib.Tag filename
      assert.equal "Something completely different…", tag.title

.export(module)
