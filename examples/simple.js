var Taglib = require(__dirname+"/../taglib")
var util = require("util")

for (var i = 2; i < process.argv.length; i++) {
    var p = new Taglib.Tag(process.argv[i]);
    util.debug( process.argv[i] + ": " + util.inspect(p) )
}
