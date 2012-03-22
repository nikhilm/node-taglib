var Taglib = require(__dirname+"/../taglib")
var util = require("util")

for (var i = 2; i < process.argv.length; i++) {
    Taglib.tag(process.argv[i], function(err, tag) {
        console.dir(err ? err : tag);
    });
}
