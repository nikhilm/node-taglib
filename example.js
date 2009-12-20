var taglib = require("./build/default/taglib")
var sys = require("sys")

var p = taglib.getFileTags(process.ARGV[2]);
sys.debug( sys.inspect(p) )
