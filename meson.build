# SPDX-License-Identifier: BSD-3-Clause
project(
	'mangrove',
	'cpp',
	default_options: [
		'cpp_std=c++17',
		'warning_level=3',
		'buildtype=release',
		'strip=true',
		'b_ndebug=if-release',
		'b_lto=true'
	],
	version: '0.0.1',
	meson_version: '>= 0.63',
	subproject_dir: 'deps'
)

sanitizer = get_option('b_sanitize')
coverage = get_option('b_coverage')
noUndefined = get_option('b_lundef')
debug = get_option('debug')
isWindows = target_machine.system() == 'windows'

subdir('src/bootstrap')
subdir('test')

runClangTidy = find_program('runClangTidy.py')
run_target(
	'clang-tidy',
	command: [runClangTidy, '-s', meson.current_source_dir(), '-p', meson.current_build_dir()]
)
