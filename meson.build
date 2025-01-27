project('reversi', 'cpp', default_options: ['warning_level=3', 'cpp_std=c++23'])
catch2defs = dependency('catch2', version: '>=3.8.0')
catch2main = dependency('catch2-with-main', version: '>=3.8.0')

subdir('src')
main = executable('reversi', sources + main, dependencies: [catch2defs])
unit_tests = executable('test_reversi', sources, dependencies: [catch2main])
test('unit tests', unit_tests)
