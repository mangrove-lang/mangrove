// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Mangrove Programming Language
// SPDX-FileContributor: Written by Rachel Mant <git@dragonmux.network>

use anstyle::{AnsiColor, Color, Style};
use clap::builder::Styles;
use clap::clap_derive::{Parser, Subcommand};
use clap::{crate_description, crate_version};

/// Clap v3 style (approximate)
/// See https://stackoverflow.com/a/75343828
fn style() -> clap::builder::Styles
{
	Styles::styled()
		.usage(Style::new().fg_color(Some(Color::Ansi(AnsiColor::Yellow))).bold())
		.header(Style::new().bold().fg_color(Some(Color::Ansi(AnsiColor::Yellow))))
		.literal(Style::new().fg_color(Some(Color::Ansi(AnsiColor::Green))))
}

#[derive(Parser)]
#[command(
	version,
	about = format!("{} v{}", crate_description!(), crate_version!()),
	styles(style()),
	disable_colored_help(false),
	arg_required_else_help(true)
)]
pub struct CliArguments
{
	#[command(subcommand)]
	/// Which primary action to take
	pub subcommand: ToplevelCommands,
}

/// Defines the top-level CLI commands
#[derive(Default, Subcommand, PartialEq)]
pub enum ToplevelCommands
{
	/// Compile the grove in the current working directory
	#[command(subcommand)]
	#[default]
	Compile,
	/// Run the Mangrove LSP server in the current directory
	#[command(subcommand)]
	StartLSP,
}
