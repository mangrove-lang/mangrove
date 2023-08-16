// SPDX-License-Identifier: BSD-3-Clause
#include <filesystem>
#include <substrate/fd>
#include <substrate/console>
#include <substrate/command_line/options>
#include <substrate/command_line/arguments>

using namespace std::literals::string_view_literals;
using substrate::fd_t;
using substrate::console;
using namespace substrate::commandLine;

constexpr static auto programOptions
{
	options
	(
		option_t{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv},
		option_t{optionFlagPair_t{"-v"sv, "--version"sv}, "Display the program version information and exit"sv},
		option_t
		{
			optionValue_t{"elfFile"sv},
			"Read from the given ELF file (including path relative to your working directory)"sv
		}.takesParameter(optionValueType_t::path)
	)
};

arguments_t args{};

void displayHelp() noexcept
{
	console.info("readELF - Mangrove Language project utility for reading ELF files"sv);
	console.info("    and displaying information about their contents"sv);
	console.writeln();
	console.writeln("Usage:"sv);
	console.writeln("\treadELF [options] <elfFile>"sv);
	console.writeln();
	programOptions.displayHelp();
	console.writeln();
	console.writeln("This utility is licensed under BSD-3-Clause"sv);
	console.writeln("Please report bugs to https://github.com/mangrove-lang/mangrove/issues"sv);
}

int main(int argCount, char **argList)
{
	console = {stdout, stderr};
	console.showDebug(false);

	if (const auto result{parseArguments(static_cast<size_t>(argCount), argList, programOptions)}; !result)
	{
		console.error("Failed to parser command line arguments"sv);
		return 1;
	}
	else // NOLINT(readability-else-after-return)
		args = *result;

	const auto *const version{args["version"sv]};
	const auto *const help{args["help"sv]};
	if (version && help)
	{
		console.error("Can only specify one of --help or --version, not both"sv);
		return 1;;
	}
	if (help || args.count() == 0U)
	{
		displayHelp();
		return 0;
	}

	const auto *const fileName{args["elfFile"sv]};
	if (!fileName)
	{
		console.error("You must specify the name of the file to read"sv);
		return 1;
	}

	fd_t underlyingFile{std::any_cast<std::filesystem::path>(std::get<flag_t>(*fileName).value()), O_RDONLY | O_NOCTTY};
	if (!underlyingFile.valid())
		return 1;
	return 0;
}
