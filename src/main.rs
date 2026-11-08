// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Mangrove Programming Language
// SPDX-FileContributor: Written by Rachel Mant <git@dragonmux.network>

use clap::Parser;

use self::cli::CliArguments;

mod cli;

fn main()
{
	// Parse the command line to figure out what we're doing
	let _cli_args = CliArguments::parse();
}
