{
  'variables': {
    'taglib_version': 'aa61823432218fb6143a688aae1ed694d21889c9'
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
                  "AdditionalLibraryDirectories": [
                      "../external/thelibrary/lib/release"
                  ]
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
          }
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
      'target_name': 'tag_before',
      'type': 'none',
      'hard_dependency': 1,
      'actions': [
        {
          'action_name': 'unpack_taglib_dep',
          'inputs': [
            './taglib-<@(taglib_version).tar.gz'
          ],
          'outputs': [
             '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/mpegfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/mpegproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/mpegheader.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/xingheader.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v1/id3v1tag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v1/id3v1genres.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2framefactory.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2synchdata.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2tag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2header.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2frame.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2footer.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2extendedheader.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/attachedpictureframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/commentsframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/ownershipframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/popularimeterframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/privateframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/relativevolumeframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/textidentificationframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/uniquefileidentifierframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/unknownframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/urllinkframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/oggfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/oggpage.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/oggpageheader.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/xiphcomment.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/vorbis/vorbisfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/vorbis/vorbisproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac/flacfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac/flacpicture.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac/flacproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac/flacmetadatablock.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac/flacunknownmetadatablock.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/flac/oggflacfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/opus/opusfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/opus/opusproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpc/mpcfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpc/mpcproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4file.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4atom.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4tag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4item.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4properties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4coverart.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape/apetag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape/apefooter.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape/apeitem.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape/apefile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape/apeproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/wavpack/wavpackfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/wavpack/wavpackproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/speex/speexfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/speex/speexproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/trueaudio/trueaudiofile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/trueaudio/trueaudioproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf/asftag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf/asffile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf/asfproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf/asfattribute.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf/asfpicture.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/rifffile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/aiff/aifffile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/aiff/aiffproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/wav/wavfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/wav/wavproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/wav/infotag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mod/modfilebase.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mod/modfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mod/modtag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mod/modproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/s3m/s3mfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/s3m/s3mproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/it/itfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/it/itproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/xm/xmfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/xm/xmproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tstring.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tstringlist.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tbytevector.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tbytevectorlist.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tbytevectorstream.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tiostream.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tfilestream.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tdebug.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tdebuglistener.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tpropertymap.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tiostream.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/trefcounter.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tstring.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tstringlist.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/unicode.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/tag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/tagunion.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/fileref.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/audioproperties.cpp', 
          ],
          'action': ['python','./extract.py','./taglib-<@(taglib_version).tar.gz','<(SHARED_INTERMEDIATE_DIR)']
        }
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/',
        ]
      },
    },
    {
      'variables': { 'target_arch%': 'ia32' }, # default for node v0.6.x
      'target_name': 'tag',
      'product_prefix': 'lib',
      'type': 'static_library',
      'cflags_cc!': ['-fno-rtti'],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': { 'GCC_ENABLE_CPP_RTTI': 'YES' }
        }]
      ],
      'export_dependent_settings': [
        'tag_before'
      ],
      'dependencies': [
        'tag_before'
      ],
      'sources': [
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/mpegfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/mpegproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/mpegheader.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/xingheader.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v1/id3v1tag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v1/id3v1genres.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2framefactory.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2synchdata.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2tag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2header.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2frame.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2footer.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/id3v2extendedheader.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/attachedpictureframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/commentsframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/ownershipframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/popularimeterframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/privateframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/relativevolumeframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/textidentificationframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/uniquefileidentifierframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/unknownframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames/urllinkframe.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/oggfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/oggpage.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/oggpageheader.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/xiphcomment.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/vorbis/vorbisfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/vorbis/vorbisproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac/flacfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac/flacpicture.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac/flacproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac/flacmetadatablock.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac/flacunknownmetadatablock.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/flac/oggflacfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/opus/opusfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/opus/opusproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpc/mpcfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpc/mpcproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4file.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4atom.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4tag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4item.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4properties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4/mp4coverart.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape/apetag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape/apefooter.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape/apeitem.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape/apefile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape/apeproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/wavpack/wavpackfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/wavpack/wavpackproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/speex/speexfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/speex/speexproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/trueaudio/trueaudiofile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/trueaudio/trueaudioproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf/asftag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf/asffile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf/asfproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf/asfattribute.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf/asfpicture.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/rifffile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/aiff/aifffile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/aiff/aiffproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/wav/wavfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/wav/wavproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/wav/infotag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mod/modfilebase.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mod/modfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mod/modtag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mod/modproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/s3m/s3mfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/s3m/s3mproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/it/itfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/it/itproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/xm/xmfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/xm/xmproperties.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tstring.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tstringlist.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tbytevector.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tbytevectorlist.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tbytevectorstream.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tiostream.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tfile.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tfilestream.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tdebug.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tdebuglistener.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tpropertymap.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tiostream.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/trefcounter.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tstring.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/tstringlist.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit/unicode.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/tag.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/tagunion.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/fileref.cpp',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/audioproperties.cpp',
      ],
      'defines': [
        'HAVE_CONFIG_H',
        'HAVE_ZLIB',
        'TAGLIB_WITH_ASF',
        'TAGLIB_WITH_MP4',
        'SYSTEM_BYTEORDER=1',
        'TAGLIB_STATIC'
      ],
      'include_dirs': [
        '.',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/flac',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpc',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/vorbis',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/speex',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/flac',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/opus',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v1',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/wavpack',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/trueaudio',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/aiff',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/wav',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mod',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/s3m',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/it',
        '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/xm',
        '../zlib'
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '.',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/toolkit',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/asf',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/flac',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/flac',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpc',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mp4',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/vorbis',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/speex',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/flac',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ogg/opus',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v2/frames',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mpeg/id3v1',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/ape',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/wavpack',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/trueaudio',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/aiff',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/riff/wav',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/mod',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/s3m',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/it',
          '<(SHARED_INTERMEDIATE_DIR)/taglib-<@(taglib_version)/taglib/xm',
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
