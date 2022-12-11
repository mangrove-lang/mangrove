# SPDX-License-Identifier: BSD-3-Clause
crunchProject = subproject(
	'crunch',
	default_options: [
		'default_library=static'
	],
)

crunchMake = crunchProject.get_variable('crunchMake')
crunchpp = crunchProject.get_variable('crunchpp')

command = [
	crunchMake,
	'-s',
	'@INPUT@',
	'-o', '@OUTPUT@',
	'-std=@0@'.format(get_option('cpp_std')),
	'-I@0@/../../src/bootstrap'.format(meson.current_source_dir()),
	'-z', 'defs'
]

if not isWindows
	testExt = '.so'
else
	testExt = '.dll'
endif

subdir('parser')
