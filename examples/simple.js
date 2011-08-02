var Taglib = require(__dirname+"/../lib/taglib")
var sys = require("sys")

var p = new Taglib.Tag(process.ARGV[2]);
sys.debug( sys.inspect(p) )
