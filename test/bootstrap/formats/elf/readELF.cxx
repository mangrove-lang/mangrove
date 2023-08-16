// SPDX-License-Identifier: BSD-3-Clause
#include <filesystem>
#include <substrate/fd>
#include <substrate/console>
#include <substrate/indexed_iterator>
#include <substrate/command_line/options>
#include <substrate/command_line/arguments>
#include <fmt/format.h>
#include <formats/elf/elf.hxx>

using namespace std::literals::string_view_literals;
using substrate::fd_t;
using substrate::console;
using mangrove::elf::ELF;
using namespace mangrove::elf::types;
using namespace mangrove::elf::enums;
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

template<> struct fmt::formatter<Class>
{
	constexpr auto parse(format_parse_context &ctx)
	{
		if (ctx.begin() != ctx.end())
			throw format_error{"invalid format"};
		return ctx.end();
	}

	template<typename FormatContext> auto format(const Class &elfClass, FormatContext &ctx) const
	{
		if (elfClass != Class::elf32Bit && elfClass != Class::elf64Bit)
			return fmt::format_to(ctx.out(), "<invalid ELF class>"sv);
		return fmt::format_to(ctx.out(), "{}"sv, elfClass == Class::elf32Bit ? 32 : 64);
	}
};

template<> struct fmt::formatter<Endian> : formatter<std::string_view>
{
	template<typename FormatContext> auto format(const Endian &endian, FormatContext &ctx) const
	{
		if (endian != Endian::little && endian != Endian::big)
			return formatter<std::string_view>::format("<invalid ELF endian>"sv, ctx);
		const auto endianName{endian == Endian::little ? "Little Endian"sv : "Big Endian"sv};
		return formatter<std::string_view>::format(endianName, ctx);
	}
};

template<> struct fmt::formatter<Type> : formatter<std::string_view>
{
	template<typename FormatContext> auto format(const Type &type, FormatContext &ctx) const
	{
		switch (type)
		{
			case Type::unknown:
				return formatter<std::string_view>::format("Unknown type"sv, ctx);
			case Type::relocatable:
				return formatter<std::string_view>::format("Relocatable binary"sv, ctx);
			case Type::executable:
				return formatter<std::string_view>::format("Executable"sv, ctx);
			case Type::shared:
				return formatter<std::string_view>::format("Shared library"sv, ctx);
			case Type::core:
				return formatter<std::string_view>::format("Core file"sv, ctx);
			default:
				return formatter<std::string_view>::format("<invalid ELF type>"sv, ctx);
		}
	}
};

template<> struct fmt::formatter<Machine> : formatter<std::string_view>
{
	template<typename FormatContext> auto format(const Machine &machine, FormatContext &ctx) const
	{
		switch (machine)
		{
			case Machine::nonSpecific:
				return formatter<std::string_view>::format("Non-specific machine"sv, ctx);
			case Machine::sparc:
				return formatter<std::string_view>::format("SPARC"sv, ctx);
			case Machine::x86:
				return formatter<std::string_view>::format("x86 (i386)"sv, ctx);
			case Machine::mips:
				return formatter<std::string_view>::format("MIPS"sv, ctx);
			case Machine::powerPC:
				return formatter<std::string_view>::format("PowerPC"sv, ctx);
			case Machine::s390:
				return formatter<std::string_view>::format("s390"sv, ctx);
			case Machine::arm:
				return formatter<std::string_view>::format("ARM"sv, ctx);
			case Machine::superH:
				return formatter<std::string_view>::format("SuperH"sv, ctx);
			case Machine::ia64:
				return formatter<std::string_view>::format("Itanium"sv, ctx);
			case Machine::x86_64:
				return formatter<std::string_view>::format("x86_64 (amd64)"sv, ctx);
			case Machine::aarch64:
				return formatter<std::string_view>::format("AArch64"sv, ctx);
			case Machine::pic:
				return formatter<std::string_view>::format("PIC"sv, ctx);
			case Machine::riscV:
				return formatter<std::string_view>::format("Risc-V"sv, ctx);
			default:
				return formatter<std::string_view>::format("<invalid machine type>"sv, ctx);
		}
	}
};

template<> struct fmt::formatter<Version> : formatter<uint32_t>
{
	template<typename FormatContext> auto format(const Version &version, FormatContext &ctx) const
		{ return formatter<uint32_t>::format(uint32_t(version), ctx); }
};

template<> struct fmt::formatter<ELFHeader>
{
	constexpr auto parse(format_parse_context &ctx)
	{
		if (ctx.begin() != ctx.end())
			throw format_error{"invalid format"};
		return ctx.end();
	}

	template<typename FormatContext> auto format(const ELFHeader &header, FormatContext &ctx) const
	{
		ctx.advance_to(fmt::format_to(ctx.out(), "Header: {}bit ELF\n"sv, header.elfClass()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> {} mode\n"sv, header.endian()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> ELF version is {}\n"sv, header.version()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> For {} ({})\n"sv, header.machine(), header.type()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Entry point is {:016x}\n"sv, header.entryPoint()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Program headers are at offset {} (+{:x})\n"sv,
			header.phdrOffset(), header.phdrOffset()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Section headers are at offset {} (+{:x})\n"sv,
			header.shdrOffset(), header.shdrOffset()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Section names are in section {}\n"sv, header.sectionNamesIndex()));
		return ctx.out();
	}
};

template<> struct fmt::formatter<ProgramHeaderType> : formatter<std::string_view>
{
	template<typename FormatContext> auto format(const ProgramHeaderType &type, FormatContext &ctx) const
	{
		switch (type)
		{
			case ProgramHeaderType::empty:
				return formatter<std::string_view>::format("empty"sv, ctx);
			case ProgramHeaderType::load:
				return formatter<std::string_view>::format("load"sv, ctx);
			case ProgramHeaderType::dynamic:
				return formatter<std::string_view>::format("dynamic"sv, ctx);
			case ProgramHeaderType::interp:
				return formatter<std::string_view>::format("interpreter"sv, ctx);
			case ProgramHeaderType::note:
				return formatter<std::string_view>::format("note"sv, ctx);
			case ProgramHeaderType::shlib:
				return formatter<std::string_view>::format("shared library"sv, ctx);
			case ProgramHeaderType::phdr:
				return formatter<std::string_view>::format("program header table"sv, ctx);
			default:
				return fmt::format_to(ctx.out(), "<invalid program header type ({:x})>"sv, uint32_t(type));
		}
	}
};

template<> struct fmt::formatter<ProgramHeader>
{
	constexpr auto parse(format_parse_context &ctx)
	{
		if (ctx.begin() != ctx.end())
			throw format_error{"invalid format"};
		return ctx.end();
	}

	template<typename FormatContext> auto format(const ProgramHeader &header, FormatContext &ctx) const
	{
		ctx.advance_to(fmt::format_to(ctx.out(), "Type '{}'\n"sv, header.type()));
		//ctx.advance_to(fmt::format_to(ctx.out(), "-> Chunk is {}\n"sv, header.flags()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Chunk data at offset {} (+{:x})\n"sv,
			header.offset(), header.offset()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Loaded at +0x{:x} from loading base address\n"sv,
			header.physicalAddress()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Mapped to +0x{:x} from virtual base address\n"sv,
			header.virtualAddress()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Chunk is {} bytes long in the file\n"sv, header.fileLength()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Chunk is {} bytes long when loaded\n"sv, header.memoryLength()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Aligned to {} bytes\n"sv, header.alignment()));
		return ctx.out();
	}
};

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

	const auto *const fileNameArg{args["elfFile"sv]};
	if (!fileNameArg)
	{
		console.error("You must specify the name of the file to read"sv);
		return 1;
	}
	const auto fileName{std::any_cast<std::filesystem::path>(std::get<flag_t>(*fileNameArg).value())};

	fd_t underlyingFile{fileName, O_RDONLY | O_NOCTTY};
	if (!underlyingFile.valid())
		return 1;
	ELF elfFile{std::move(underlyingFile)};

	console.info("Read ELF file '"sv, fileName.u8string(), "'"sv);
	fmt::print("{}"sv, elfFile.header());
	console.info("Program headers:"sv);
	for (const auto &[index, header] : substrate::indexedIterator_t{elfFile.programHeaders()})
		fmt::print("{}: {}"sv, index, header);
	return 0;
}
