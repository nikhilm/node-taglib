var taglib = require("./build/default/taglib")
var sys = require("sys")

try {
var p = taglib.getFileTags(process.ARGV[2]);
sys.debug( sys.inspect(p) )
} catch(e) {
    sys.debug("Not a valid file");
}
