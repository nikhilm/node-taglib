{
  'variables': {
    'zlib_version': '50893291621658f355bc5b4d450a8d06a563053d',
  },
  'target_defaults': {
    'default_configuration': 'Debug',
    'configurations': {
      # TODO: hoist these out and put them somewhere common, because
      #       RuntimeLibrary MUST MATCH across the entire project
      'Debug': {
        'defines': [ 'DEBUG', '_DEBUG' ],
        "msvs_settings": {
            "VCCLCompilerTool": {
                "ExceptionHandling": "0",
                "AdditionalOptions": [
                    "/MP /EHsc"
                ]
            },
            "VCLibrarianTool": {
                "AdditionalOptions": [
                    "/LTCG"
                ]
            },
            "VCLinkerTool": {
                "LinkTimeCodeGeneration": 1,
                "LinkIncremental": 1,
              
            }
        }
      },
      'Release': {
        'conditions': [
        # hack to override node's common.gypi defaults and compile with RTTI and EHsc on
        # Conditions seem to be evaluated later than standard settings, so we add RTTI/EHsc to both architectures
          ['target_arch=="ia32"', {
            'msvs_settings': {
              "VCCLCompilerTool": {
                  "RuntimeLibrary": 0,
                  "Optimization": 3,
                  "FavorSizeOrSpeed": 1,
                  "InlineFunctionExpansion": 2,
                  "WholeProgramOptimization": "true",
                  "OmitFramePointers": "true",
                  "EnableFunctionLevelLinking": "true",
                  "EnableIntrinsicFunctions": "true",
                  "RuntimeTypeInfo": "false",
                  "ExceptionHandling": "0",
                  "AdditionalOptions": [
                      "/MP /EHsc"
                  ]
              },
              "VCLibrarianTool": {
                  "AdditionalOptions": [
                      "/LTCG"
                  ]
              },
              "VCLinkerTool": {
                  "LinkTimeCodeGeneration": 1,
                  "OptimizeReferences": 2,
                  "EnableCOMDATFolding": 2,
                  "LinkIncremental": 1,
                
              }
            },
          }, {
            'msvs_settings': {
              "VCCLCompilerTool": {
                  "RuntimeLibrary": 0,
                  "Optimization": 3,
                  "FavorSizeOrSpeed": 1,
                  "InlineFunctionExpansion": 2,
                  "WholeProgramOptimization": "true",
                  "OmitFramePointers": "true",
                  "EnableFunctionLevelLinking": "true",
                  "EnableIntrinsicFunctions": "true",
                  "RuntimeTypeInfo": "false",
                  "ExceptionHandling": "0",
                  "AdditionalOptions": [
                      "/MP /EHsc"
                  ]
              },
              "VCLibrarianTool": {
                  "AdditionalOptions": [
                      "/LTCG"
                  ]
              },
              "VCLinkerTool": {
                  "LinkTimeCodeGeneration": 1,
                  "OptimizeReferences": 2,
                  "EnableCOMDATFolding": 2,
                  "LinkIncremental": 1,
              }
            },
          },
          
        ]],
        'defines': [ 'NDEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 0, # static release,
          },
        },
      }
    },
    'msvs_settings': {
      'VCCLCompilerTool': {
      'RuntimeTypeInfo': 'true'
      },
      'VCLibrarianTool': {
      },
      'VCLinkerTool': {
        'GenerateDebugInformation': 'true',
      },
    },
  },
  'targets': [
     {
      'target_name': 'z_before',
      'type': 'none',
      'hard_dependency': 1,
      'actions': [
        {
          'action_name': 'unpack_zlib_dep',
          'inputs': [
            './zlib-<@(zlib_version).tar.gz'
          ],
          'outputs': [
              '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/adler32.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/crc32.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/deflate.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/infback.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/inffast.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/inflate.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/inftrees.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/trees.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/zutil.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/compress.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/uncompr.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/gzclose.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/gzlib.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/gzread.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/gzwrite.c',
          ],
          'action': ['python','./extract.py','./zlib-<@(zlib_version).tar.gz','<(SHARED_INTERMEDIATE_DIR)']
        }
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/',
        ]
      },
    },
    {
      'variables': { 'target_arch%': 'ia32' }, # default for node v0.6.x
      'target_name': 'z',
      'product_prefix': 'lib',
      'type': 'static_library',
      'sources': [
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/adler32.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/crc32.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/deflate.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/infback.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/inffast.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/inflate.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/inftrees.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/trees.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/zutil.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/compress.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/uncompr.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/gzclose.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/gzlib.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/gzread.c',
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)/gzwrite.c',
      ],
      'export_dependent_settings': [
        'z_before'
      ],
      'dependencies': [
        'z_before'
      ],
      'defines': [
      ],
      'include_dirs': [
        '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(SHARED_INTERMEDIATE_DIR)/zlib-<@(zlib_version)',
        ],
        'conditions': [
          ['OS=="win"', {
            'defines': [
              "_WIN32",
              "_WINDOWS"
            ]
          }]
        ],
      },
    },

  ]
}
