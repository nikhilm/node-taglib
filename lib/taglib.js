var binding = require(__dirname + "/taglib_binding");
exports.Tag = binding.Tag;
exports.getFileTags = function(filename) {
  return new binding.Tag(filename);
};
