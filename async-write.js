var taglib = require('./build/Release/taglib');
var async = require('async');
var fs = require('fs');
var match = require('match-files');

var isMp3 = function(path) { return path.match(/\.mp3$/); }

//for (var i = 1; i < 30; i++)
match.find(process.argv[2], {fileFilters: [isMp3]}, function(err, files) {
    var t = Date.now();
    var count = 0;
    console.log(files.length, "files");
    async.forEach(files, function(fn, cb) {
        var tag = taglib.tag(fn, function(err, tag) {
            if (err)
                return cb(err);
            count++;
            var t = tag.title;
            var rev = "";
            for (var i = 0; i < t.length; i++)
                rev = t[i] + rev;
            tag.title = rev;
            tag.save(function(err) {
                cb(err);
            });
        });
    }, function() {
        console.log("async", Date.now() - t);
        console.log("Tag succeeded on", count);
    });
});
