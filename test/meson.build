# SPDX-License-Identifier: BSD-3-Clause
caseCopier = find_program('caseCopier.py')
caseFiles = custom_target(
	'copyTestCases',
	command: [caseCopier, meson.current_source_dir(), meson.current_build_dir()],
	output: 'cases'
)

subdir('bootstrap')
