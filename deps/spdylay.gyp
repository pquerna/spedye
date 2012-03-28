{
  'target_defaults': {
    'default_configuration': 'Debug',
    'configurations': {
      # TODO: hoist these out and put them somewhere common, because
      #       RuntimeLibrary MUST MATCH across the entire project
      'Debug': {
        'defines': [ 'DEBUG', '_DEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 1, # static debug
          },
        },
      },
      'Release': {
        'defines': [ 'NDEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 0, # static release
          },
        },
      }
    },
    'msvs_settings': {
      'VCCLCompilerTool': {
      },
      'VCLibrarianTool': {
      },
      'VCLinkerTool': {
        'GenerateDebugInformation': 'true',
      },
    },
    'conditions': [
      ['OS == "win"', {
        'defines': [
          'WIN32'
        ],
      }]
    ],
  },

  'targets': [
    {
      'target_name': 'spdylay',
      'type': 'static_library',
      'include_dirs': [ 'spdylay/lib', 'spdylay/lib/includes', 'spdylay-configs' ],
      'direct_dependent_settings': {
        'include_dirs': [ 'spdylay/lib/includes', 'spdylay-configs' ],
      },
      'sources': [
        'spdylay/lib/spdylay_pq.c',
        'spdylay/lib/spdylay_map.c',
        'spdylay/lib/spdylay_queue.c',
        'spdylay/lib/spdylay_buffer.c',
        'spdylay/lib/spdylay_frame.c',
        'spdylay/lib/spdylay_zlib.c',
        'spdylay/lib/spdylay_session.c',
        'spdylay/lib/spdylay_helper.c',
        'spdylay/lib/spdylay_stream.c',
        'spdylay/lib/spdylay_npn.c',
        'spdylay/lib/spdylay_submit.c',
        'spdylay/lib/spdylay_outbound_item.c',
        'spdylay/lib/spdylay_pq.h',
        'spdylay/lib/spdylay_int.h',
        'spdylay/lib/spdylay_map.h',
        'spdylay/lib/spdylay_queue.h',
        'spdylay/lib/spdylay_buffer.h',
        'spdylay/lib/spdylay_frame.h',
        'spdylay/lib/spdylay_zlib.h',
        'spdylay/lib/spdylay_session.h',
        'spdylay/lib/spdylay_helper.h',
        'spdylay/lib/spdylay_stream.h',
        'spdylay/lib/spdylay_int.h',
        'spdylay/lib/spdylay_npn.h',
        'spdylay/lib/spdylay_submit.h',
        'spdylay/lib/spdylay_outbound_item.h',
        'spdylay/lib/spdylay_net.h'
      ],
      'conditions': [
        ['OS=="win"', {
          'msvs_settings': {
            'VCCLCompilerTool': {
              # Compile as C++. http_parser.c is actually C99, but C++ is
              # close enough in this case.
              'CompileAs': 2,
            },
          },
        }]
      ],
    },
  ]
}

