var Taglib = require('bindings')('taglib');
var util = require("util")

for (var i = 2; i < process.argv.length; i++) {
    Taglib.read(process.argv[i], function(err, tag, props) {
        console.dir(err ? err : {'tag': tag, 'audioProperties': props});
    });
}
