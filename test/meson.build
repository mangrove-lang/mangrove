# SPDX-License-Identifier: BSD-3-Clause
caseCopier = find_program('caseCopier.py')

caseSrcs = [
	'cases/tokenisation/stringLiterals.case',
	'cases/tokenisation/integralLiterals.case',
	'cases/tokenisation/assignments.case',
	'cases/tokenisation/keywords.case',
	'cases/tokenisation/punctuation.case',
]

caseFiles = custom_target(
	'copyTestCases',
	command: [caseCopier, meson.current_source_dir(), meson.current_build_dir()],
	output: 'cases',
	depend_files: caseSrcs
)

subdir('bootstrap')
