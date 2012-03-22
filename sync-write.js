var taglib = require('./build/Release/taglib');
var fs = require('fs');
var match = require('match-files');

var isMp3 = function(path) { return path.match(/\.mp3$/); }

//for (var i = 0; i < 10; i++)
match.find(process.argv[2], {fileFilters: [isMp3]}, function(err, files) {
    var t = Date.now();
    var count = 0;
    console.log(files.length, "files");
    files.forEach(function(fn) {
        try {
            var tag = taglib.tagSync(fn);
            count++;
            var t = tag.title;
            var rev = "";
            for (var i = 0; i < t.length; i++)
                rev = t[i] + rev;
            tag.title = rev;
            tag.saveSync();
        } catch(e) {}
    });
    console.log("sync", Date.now() - t);
    console.log("Tag succeeded on", count);
});
