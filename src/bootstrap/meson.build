# SPDX-License-Identifier: BSD-3-Clause
cxx = meson.get_compiler('cpp')

substrate = subproject(
	'substrate',
	default_options: [
		'build_tests=false',
	],
).get_variable(
	'substrate_dep'
)

mangroveSrc = [
	'mangrove.cxx'
]

subdir('parser')
subdir('formats')

executable(
	'mangrove',
	mangroveSrc,
	dependencies:[substrate],
	gnu_symbol_visibility: 'inlineshidden',
)
