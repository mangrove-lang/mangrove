# SPDX-License-Identifier: BSD-3-Clause
custom_target(
	'bootstrapTestTokeniser',
	command: command,
	input: [
		'testTokeniser.cxx',
		# We would extract just the tokeniser, but this doesn't actually work on Meson 0.61.1
		mangrove.extract_all_objects(recursive: true)
	],
	output: 'testTokeniser' + testExt,
	depends: caseFiles,
	build_by_default: true
)

test(
	'bootstrapTestTokeniser',
	crunchpp,
	args: ['testTokeniser'],
	workdir: meson.current_build_dir()
)
