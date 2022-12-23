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

if coverage
	command += ['--coverage']
endif
if debug
	command += ['--debug']
endif
if sanitizer != 'none'
	command += ['-fsanitize=' + sanitizer]
endif
if not isWindows and cxx.get_linker_id() != 'ld64' and noUndefined
	command += ['-z', 'defs']
endif

if isWindows
	command += ['-DWIN32', '-D_WINDOWS']

	# Substrate API needs Windows 10
	# https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=msvc-160
	if cxx.get_id() == 'gcc' or cxx.get_id() == 'clang'
		command += ['-DWINVER=NTDDI_WIN10_RS5', '-D_WIN32_WINNT=NTDDI_WIN10_RS5', '-DNTDDI_VERSION=NTDDI_WIN10_RS5']
	endif
endif

if not isWindows
	testExt = '.so'
else
	testExt = '.dll'
endif

subdir('parser')
subdir('core/utf8')
