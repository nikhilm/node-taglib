var Taglib = require('bindings')('taglib');
var util = require("util")

var p = new Taglib.AudioProperties(process.argv[2]);
util.debug( util.inspect(p) )
