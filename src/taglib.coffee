cLib = require __dirname + "/module"

exports.Tag = cLib.Tag
exports.getFileTags = (filename) ->
    return new cLib.Tag(filename)
