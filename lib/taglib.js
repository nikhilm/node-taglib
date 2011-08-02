(function() {
  var cLib;
  cLib = require(__dirname + "/module");
  exports.Tag = cLib.Tag;
  exports.getFileTags = function(filename) {
    return new cLib.Tag(filename);
  };
}).call(this);
