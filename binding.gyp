{
  "targets": [
    {
      "target_name": "taglib",
      "sources": ["src/bufferstream.c", "src/tag.cc", "src/taglib.cc"],
      "libraries": ["<!(taglib-config --libs)"],
	  "include_dirs" : [
		  "<!(node -e \"require('nan')\")"
	  ],
      'conditions': [
        ['OS=="mac"', {
          # cflags on OS X are stupid and have to be defined like this
          # copied from libxmljs
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '<!@(taglib-config --cflags)'
            ]
          }
        }, {
          'cflags': [
            '<!@(taglib-config --cflags)'
          ],
        }]
      ]
    }
  ]
}
