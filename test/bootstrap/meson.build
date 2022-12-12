# SPDX-License-Identifier: BSD-3-Clause
crunchProject = subproject(
	'crunch',
	default_options: [
		'default_library=static'
	],
)

crunchMake = crunchProject.get_variable('crunchMake')
crunchpp = crunchProject.get_variable('crunchpp')
crunchppInc = crunchProject.get_variable('crunchppSrcDir')
libCrunchppPath = crunchProject.get_variable('libCrunchppPath')

command = [
	crunchMake,
	'-s',
	'-I@0@'.format(crunchppInc),
	'-L@0@'.format(libCrunchppPath),
	'@INPUT@',
	'-o', '@OUTPUT@',
	'-std=@0@'.format(get_option('cpp_std')),
	'-I@0@/../../src/bootstrap'.format(meson.current_source_dir()),
	'-z', 'defs',
	substrate.get_variable('compile_args'),
	substrate.get_variable('link_args')
]

if not isWindows
	testExt = '.so'
else
	testExt = '.dll'
endif

subdir('parser')
