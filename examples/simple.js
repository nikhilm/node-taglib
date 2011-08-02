var taglib = require(__dirname+"/../lib/taglib")
var sys = require("sys")

var p = taglib.parse(process.ARGV[2]);
sys.debug( sys.inspect(p) )
