{
  'targets': [
    {'target_name': 'libspedye',
     'type': 'static_library',
     'dependencies': [
       'deps/http-parser/http_parser.gyp:http_parser',
       'deps/uv/uv.gyp:uv',
       'deps/openssl/openssl.gyp:openssl',
       'deps/spdylay.gyp:spdylay',
     ],
     'export_dependent_settings': [
       'deps/http-parser/http_parser.gyp:http_parser',
       'deps/uv/uv.gyp:uv',
       'deps/openssl/openssl.gyp:openssl',
       'deps/spdylay.gyp:spdylay',
      ],
      'conditions': [
        ['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
          'cflags': [ '--std=c89' ],
          'defines': [ '_GNU_SOURCE' ]
        }],
      ],
     'sources': [
       'src/spedye.h',
       'src/spedye_error.h',
       'src/spedye_init.c',
       'src/spedye_master.c',
       'src/spedye_worker.c',
       'src/spedye_conf.c',
       'src/spedye_error.c',
     ],
     'include_dirs': [
       'src',
       'deps/uv/src/ares'
     ],
     'direct_dependent_settings': {
       'include_dirs': [
          'src',
          'deps/uv/src/ares'
         ]
       },
     },
    {
      'target_name': 'spedye',
      'type': 'executable',
      'dependencies': [
        'libspedye',
      ],
      'sources': [
        'src/spedye_main.c',
      ],
      'msvs-settings': {
        'VCLinkerTool': {
          'SubSystem': 1, # /subsystem:console
        },
      },
      'conditions': [
        ['OS == "linux"', {
          'libraries': ['-ldl'],
        }],
        ['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
          'cflags': [ '--std=c89' ],
          'defines': [ '_GNU_SOURCE' ]
        }]
      ],
    },
  ],
}
