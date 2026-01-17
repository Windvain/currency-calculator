project(
  'currency-calculator',
  'c', 'cpp',

  license: 'MIT',
  version: '1.0.0',

  meson_version: '>=1.1.0',
  default_options: ['cpp_std=c++20', 'buildtype=debug'],
)

cc = meson.get_compiler('cpp')

libcurl_dirs = [
  meson.project_source_root() / 'vendor/libcurl/lib',
  meson.project_source_root() / 'vendor/libcurl/bin'
]

libcurl_dep = cc.find_library('libcurl', dirs: libcurl_dirs)

if not libcurl_dep.found()
  error('libcurl was not found.')
endif

libjson = static_library(
  'libjson',
  'vendor/libjson/json.c',
  'vendor/libjson/jsonlint.c',
  'vendor/libjson/json.h',

  include_directories: 'vendor/libjson',
)

libjson_dep = declare_dependency(link_with: libjson)

executable(
  'currency-calculator',
  'src/cpp/main.cpp',

  include_directories: [
    'src',
    'vendor/libcurl/include',
    'vendor/libjson',
  ],
  dependencies: [libcurl_dep, libjson_dep]
)
