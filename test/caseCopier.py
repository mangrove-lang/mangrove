#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-3-Clause
from pathlib import Path
from sys import argv, exit

if len(argv) != 3:
	print('Invalid arguments, case copier must be called in the form:')
	print(f'{argv[0]} <inputDir> <outputDir>')
	exit(1)

inputPath = Path(argv[1]) / 'cases'
outputPath = Path(argv[2]) / 'cases'

if not inputPath.exists():
	print(f'Failure, path "{inputPath}" does not exist!')
	exit(1)
if not inputPath.is_dir():
	print(f'Failure, path "{inputPath}" does not refer to a directory')

def recursiveCopy(inputPath: Path, outputPath: Path) -> None:
	outputPath.mkdir(exist_ok = True)
	for inputFile in inputPath.iterdir():
		outputFile = outputPath / inputFile.name
		if inputFile.is_dir():
			recursiveCopy(inputFile, outputFile)
		else:
			symlink = True
			if outputFile.exists():
				if not outputFile.samefile(inputFile):
					outputFile.unlink()
				else:
					symlink = False
			if symlink:
				outputFile.symlink_to(inputFile)

recursiveCopy(inputPath, outputPath)
