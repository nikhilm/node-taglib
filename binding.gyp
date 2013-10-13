{
  'variables': {
    'taglib%': 'internal'
  },
  "targets": [
    {
      "target_name": "taglib",
      "sources": ["src/bufferstream.c", "src/tag.cc", "src/taglib.cc"],
      'direct_dependent_settings': {
        "include_dirs": ["deps/zlib"]
      },
      'conditions': [
        ['taglib != "internal"', {
          "libraries": [
            "<!(taglib-config --libs)",
            "-ltag",
          ],
          "conditions": [
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
        }, {
          "dependencies": [
           "deps/zlib.gyp:z",
           "deps/taglib.gyp:tag"
          ]
        }]
      ]
    }
  ]
}
