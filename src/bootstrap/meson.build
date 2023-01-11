# SPDX-License-Identifier: BSD-3-Clause
cxx = meson.get_compiler('cpp')

extended_warnings = [
	'-Wdouble-promotion',
	'-Wformat=2',
	'-Wformat-overflow=2',
	'-Wformat-signedness',
	'-Wformat-truncation',
	'-Wnull-dereference',
	'-Wmissing-attributes',
	'-Wmissing-braces',
	'-Wsequence-point',
	'-Wreturn-type',
	'-Wunused',
	'-Wunused-local-typedefs',
	'-Wunused-const-variable=2',
	'-Wmaybe-uninitialized',
	'-Wunknown-pragmas',
	'-Wstrict-aliasing',
	'-Wstrict-overflow=3',
	'-Wstring-compare',
	'-Wstringop-overflow',
	'-Warith-conversion',
	'-Wvla-parameter',
	'-Wduplicated-branches',
	'-Wshadow=local',
	'-Wunsafe-loop-optimizations',
	'-Wbad-function-cast',
	'-Wcast-qual',
	'-Wcast-align=strict',
	'-Wcast-function-type',
	'-Wconversion',
	'-Wdangling-else',
	'-Wsign-conversion',
	'-Wfloat-conversion',
	'-Wpacked',
#	'-Wpadded',
	'-Wredundant-decls',
	'-Winline',
	'-Wvla',
	'-Wstack-protector',
	'-Wunsuffixed-float-constant',
	'-Wimplicit-fallthrough',
]

add_project_arguments(
	cxx.get_supported_arguments(extended_warnings),
	language: 'cpp'
)

picArgs = cxx.get_supported_arguments(['-fPIC', '-DPIC'])
add_global_arguments(
	picArgs,
	language: ['c', 'cpp']
)
add_global_link_arguments(
	picArgs,
	language: ['c', 'cpp']
)

substrate = subproject(
	'substrate',
	default_options: [
		'build_tests=false',
		'default_library=static'
	],
).get_variable(
	'substrate_dep'
)

fmt = subproject(
	'fmt',
	default_options: [
		'default_library=static'
	],
).get_variable(
	'fmt_dep'
)

mangroveSrc = [
	'mangrove.cxx'
]

subdir('parser')
subdir('formats')

mangrove = executable(
	'mangrove',
	mangroveSrc,
	cpp_args: ['-D_FORTIFY_SOURCE=2'],
	dependencies: [substrate, fmt],
	gnu_symbol_visibility: 'inlineshidden'
)
