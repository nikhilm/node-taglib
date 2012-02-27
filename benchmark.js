var taglib = require('./build/Release/taglib');
var async = require('async');
var fs = require('fs');
var match = require('match-files');

var isMp3 = function(path) { return path.match(/\.mp3$/); }

match.find('/Users/nikhilmarathe/Music', {fileFilters: [isMp3]}, function(err, files) {
    var count = 0;
    var fEC = 0;
    var t = Date.now();
    async.forEach(files, function(fn, cb) {
        fEC++;
        //console.log("forEach", fEC);
        taglib.tag(fn, function(err, tag) {
            count++;
            console.log(tag.title);
            //tag.dispose();
            cb(false);
        });
    }, function() {
        console.log("Async", Date.now() - t);
        console.log("Tag succeeded on ", count);
    });
});
