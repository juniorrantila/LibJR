
libJR = library('JR', [
    'JR/StringView.cpp',
    'JR/String.cpp',
  ],
  include_directories: ['.'])


libCore = library('Core', [
      'Core/Folder.cpp',
      'Core/MappedFile.cpp',
      'Core/System.cpp',
      'Core/Threads.cpp',
    ],
    include_directories: ['.'],
    link_with: [libJR])

libCli = library('Cli',
    'Cli/ArgumentParser.cpp',
    'Cli/ProgressBar.cpp',
    include_directories: ['.'],
    link_with: [libJR])

