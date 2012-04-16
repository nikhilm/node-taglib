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
        taglib.tag(fn, function(err, tag) {
            if (err) {
                console.log("ERROR");
                return cb(false);
            }
            count++;
            console.log(tag.title);
            cb(false);
        });
    }, function() {
        console.log("async", Date.now() - t);
        console.log("Tag succeeded on", count);
    });
});
