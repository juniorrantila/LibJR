project('LibJR', 'cpp',
  default_options: [
    'cpp_std=c++20',
    'cpp_eh=none',
    'cpp_rtti=false',
    'default_library=static',
    'warning_level=3',
    'buildtype=release'
  ])

if not meson.is_subproject()
  add_global_arguments('-Wno-c99-designator', language: 'cpp')
  add_global_arguments('-Wno-c99-extensions', language: 'cpp')
  add_global_arguments('-Wno-gnu-zero-variadic-macro-arguments', language: 'cpp')
  add_global_arguments('-Wno-keyword-macro', language: 'cpp')
  add_global_arguments('-Wno-gnu-conditional-omitted-operand', language: 'cpp')
  add_global_arguments('-Wno-format-pedantic', language: 'cpp')
  add_global_arguments('-Wno-nullability-extension', language: 'cpp')
  add_global_arguments('-Wno-gnu-statement-expression', language: 'cpp')
  add_global_arguments('-Wno-gnu-anonymous-struct', language: 'cpp')
  add_global_arguments('-Wno-c++2b-extensions', language: 'cpp')

  add_global_arguments('-fdebug-prefix-map=../=', language: 'cpp')
  add_global_arguments('-ffile-prefix-map=../=', language: 'cpp')
endif

libJR_lib = library('JR', [
    'JR/StringView.cpp',
    'JR/String.cpp',
  ],
  include_directories: ['.'])
libJR_dep = declare_dependency(
  link_with: libJR_lib,
  include_directories: '.')

libMain_lib = static_library('Main', [
    'Main/Main.cpp'
  ],
  include_directories: ['.'],
  dependencies: libJR_dep)

libMain_dep = declare_dependency(
  link_with: libMain_lib,
  include_directories: '.')

libCore_lib = library('Core', [
      'Core/Folder.cpp',
      'Core/MappedFile.cpp',
      'Core/System.cpp',
      'Core/Threads.cpp',
    ],
    include_directories: ['.'],
    dependencies: libJR_dep)
libCore_dep = declare_dependency(
  link_with: libCore_lib,
  include_directories: '.')

libCli_lib = library('Cli',
    'Cli/ArgumentParser.cpp',
    'Cli/ProgressBar.cpp',
    include_directories: ['.'],
    dependencies: libJR_dep)
libCli_dep = declare_dependency(
  link_with: libCli_lib,
  include_directories: '.')

