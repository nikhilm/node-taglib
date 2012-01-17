var taglib = require('./build/Release/taglib');
var async = require('async');
var match = require('match-files');

var args = process.argv.slice(2);

var isMp3 = function(path) { return path.match(/\.mp3$/); }

match.find('/Users/nikhilmarathe/Music', {fileFilters: [isMp3]}, function(err, files) {
    var t = Date.now();
    async.forEach(files, function(fn, cb) {
        try {
            taglib.tag(fn, function() {});
        } catch(e) {}
        cb(false);
    }, function() {
        console.log("Async", Date.now() - t);
    });

    t = Date.now();
    files.forEach(function(elem) {
        try {
            taglib.tagSync(elem);
        } catch(e) {}
    });
    console.log("Sync", Date.now() - t);
});
