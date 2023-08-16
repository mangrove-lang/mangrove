# SPDX-License-Identifier: BSD-3-Clause
readELFSrcs = [
	'../../../../src/bootstrap/formats/elf/elf.cxx', '../../../../src/bootstrap/formats/elf/types.cxx',
	'readELF.cxx',
]

executable(
	'readELF',
	readELFSrcs,
	cpp_args: ['-D_FORTIFY_SOURCE=2'],
	dependencies: [substrate, fmt],
	include_directories: include_directories('..' / '..' / '..' / '..' / 'src' / 'bootstrap'),
	gnu_symbol_visibility: 'inlineshidden',
	install: false,
)
