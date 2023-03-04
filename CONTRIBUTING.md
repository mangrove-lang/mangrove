# Contribution Guidelines

## Contributing

Contributions to this project are done under the [BSD-3 Clause license](https://github.com/mangrove-lang/mangrove/blob/main/LICENSE).
Please use this license for any new files and mark that file with a SPDX license notice at the top of the file as in:

```c++
// SPDX-License-Identifier: BSD-3-Clause
```

Whenever this is missing from the file header (at the top of the file), please add the line in its own commit.

## Development and Testing

When developing this project, the following tools are necessary:

* Git
* Meson >= 0.58
* Ninja
* A working host-architecture C++ toolchain (GCC, MSVC, Clang)

We suggest the versions here to be taken as absolute minimum versions.

## Common tasks

There are a few common tasks you will encounter, such as needing to lint the code and check that
the bootstrap compiler and compiler still build. We automate these via `ninja` targets.
Except for creating the build environment, the rest of these steps expect to be run from in the build directory.

* Creating the build environment: `meson setup build`
* Configuring the build environment: `meson configure <options>`
* Building the code: `ninja`
* Linting the code: `ninja clang-tidy`

## Submitting a pull request

### If contributing for the first time

1. [Fork](https://github.com/mangrove-lang/mangrove) and clone the repository
2. Create a new branch: `git switch -c type/branch-name`
3. Make your change
4. Push to your fork (`git push -u`)
5. Submit a [pull request](https://github.com/mangrove-lang/mangrove/compare)

If you wish to fix a bug, `type` in the new branch name should be `fix`, otherwise if you wish to implement
a new feature, `type` should be `feature`.

### If you are working from an existing clone of the repository

1. Ensure you have our repo as a remote (`git remote add upstream https://github.com/mangrove-lang/mangrove`)
2. Switch back to `main` (`git switch main`/`git checkout main`)
3. Pull to ensure you're up to date (`git pull upstream`)
4. Push to your fork (`git push`)
5. Continue at 2. [in the steps for first time](#if-contributing-for-the-first-time)

Additionally, please write good and descriptive commit messages that both summarise the change and,
if necessary, expand on the summary using description lines.
"More AST types" is, while terse and correct, an example of a bad commit message.
"bootstrap/ast: Implemented an AST type for template scopes" is an example of a better commit message.

When writing commit messages, please prefix the component being modified by taking the path to the component
relative to the directory of your working copy, chopping off the `src/` component, then appending a ":" after.

Examples of this are:

* "src/bootstrap/mangrove.cxx" -> "bootstrap: "
* "src/bootstrap/ast/symbolTable.cxx" -> "bootstrap/ast: "
* "src/bootstrap/formats/elf/elf.cxx" -> "bootstrap/formats/elf: "

If the file is not in the src/ directory then either supply a purpose (eg "linting: ", "github: "),
or use the tag "misc: " for the commit

We would like to be able to look back through the commit history and tell what happened, when, and why without having
to dig into the commit descriptions as this improves the general Git experience and everyone's lives.

Try to keep commits focused on a single, small, and atomic change to ease review, and aid the process if we end up having
to `git bisect` through your changes, or `git revert` in the extreme case something seriously broke.

We use rebasing to merge pull requests, so please keep this in mind.

## Licensing

As mentioned at the top of this file, contributions made to this code base must be made under the terms of the BSD 3-Clause license.

When contributing to existing code, we ask that you update the copyright and authorship notice at the top of the files touched
with any authorship information you wish to provide in SPDX format. This is so that you get proper attribution.

When making an original contribution, copyright by default falls to you. However, if you wish to hand copyright over, then
please use the SPDX line `SPDX-FileCopyrightText: [year] The Mangrove Language`, replacing [year] with the current year.

If you wrote the file and wish to be attributed as having done so, regardless of copyright, please add the SPDX line
`SPDX-FileContributor: Written by [Your Name/Handle] <[Email Address]>`.
If you modified a file and wish to be attributed for that, please add the SPDX line
`SPDX-FileContributor: Modified by [Your Name/Handle] <[Email Address]>`.

These lines must go after the SPDX-License-Identifier line but before anything else in the rest of the file
(excluding shebangs for shell scripts). They will be used to automatically build contribution and attribution lists.
