{
  "targets": [
    {
      "target_name": "taglib",
      "sources": ["src/bufferstream.c", "src/metadata.cc", "src/taglib.cc"],
      "libraries": ["<!(taglib-config --libs)"],
      'conditions': [
        ['OS=="mac"', {
          # cflags on OS X are stupid and have to be defined like this
          # copied from libxmljs
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '<!@(taglib-config --cflags)'
            ],
            'OTHER_LDFLAGS': [
              '-dynamiclib'
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
