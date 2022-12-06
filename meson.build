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
	meson_version: '>= 0.53',
	subproject_dir: 'deps'
)

subdir('src/bootstrap')
