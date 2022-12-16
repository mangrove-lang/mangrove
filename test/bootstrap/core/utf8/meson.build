# SPDX-License-Identifier: BSD-3-Clause
custom_target(
	'bootstrapTestUTF8Helpers',
	command: command,
	input: ['testHelpers.cxx'],
	output: 'testHelpers' + testExt,
	depends: caseFiles,
	build_by_default: true
)

test(
	'bootstrapTestUTF8Helpers',
	crunchpp,
	args: ['testHelpers'],
	workdir: meson.current_build_dir()
)
