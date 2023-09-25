// SPDX-License-Identifier: BSD-3-Clause
#include <filesystem>
#include <substrate/fd>
#include <substrate/console>
#include <substrate/indexed_iterator>
#include <substrate/command_line/options>
#include <substrate/command_line/arguments>
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4061 4365)
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
#include <fmt/format.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#include <formats/elf/elf.hxx>
#include <core/version.hxx>

using namespace std::literals::string_view_literals;
using substrate::fd_t;
using substrate::console;
using mangrove::elf::ELF;
using namespace mangrove::elf::types;
using namespace mangrove::elf::enums;
using namespace substrate::commandLine;

constexpr static auto elfFileOption
{
	option_t
	{
		optionValue_t{"elfFile"sv},
		"Read from the given ELF file (including path relative to your working directory)"sv
	}.takesParameter(optionValueType_t::path)
};

constexpr static auto headersOptions
{
	options
	(
		option_t{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv},
		option_t{optionFlagPair_t{"-a"sv, "--all"sv}, "List all headers in the binary"sv},
		option_t{optionFlagPair_t{"-f"sv, "--file"sv}, "List the file main header"sv},
		option_t{optionFlagPair_t{"-p"sv, "--program"sv}, "Lists the program headers"sv},
		option_t{optionFlagPair_t{"-s"sv, "--section"sv}, "Lists the section headers"sv},
		elfFileOption
	)
};

constexpr static auto symbolsOptions
{
	options
	(
		option_t{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv},
		elfFileOption
	)
};

constexpr static auto actions
{
	optionAlternations
	({
		{
			"headers"sv,
			"Lists the headers from a binary (by default, all headers)"sv,
			headersOptions,
		},
		{
			"symbols"sv,
			"Lists the symbols defined by a binary"sv,
			symbolsOptions,
		},
	})
};

constexpr static auto programOptions
{
	options
	(
		option_t{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv},
		option_t{optionFlagPair_t{"-v"sv, "--version"sv}, "Display the program version information and exit"sv},
		optionSet_t{"action", actions}
	)
};

arguments_t args{};

void displayFooter() noexcept
{
	console.writeln();
	console.writeln("This utility is licensed under BSD-3-Clause"sv);
	console.writeln("Please report bugs to https://github.com/mangrove-lang/mangrove/issues"sv);
}

void displayHelp() noexcept
{
	console.info("readELF - Mangrove Language project utility for reading ELF files"sv);
	console.info("    and displaying information about their contents"sv);
	console.writeln();
	console.writeln("Usage:"sv);
	console.writeln("\treadELF [options] {action}"sv);
	console.writeln();
	programOptions.displayHelp();
	displayFooter();
}

void displayHeadersHelp() noexcept
{
	console.info("readELF headers - Lists the headers from an ELF binary (by default, all headers)"sv);
	console.writeln();
	console.writeln("Usage:"sv);
	console.writeln("\treadelf headers [options] elfFile"sv);
	console.writeln();
	headersOptions.displayHelp();
	displayFooter();
}

void displaySymbolsHelp() noexcept
{
	console.info("readELF symbols - Lists the symbols defined by an ELF binary"sv);
	console.writeln();
	console.writeln("Usage:"sv);
	console.writeln("\treadelf symbols [options] elfFile"sv);
	console.writeln();
	symbolsOptions.displayHelp();
	displayFooter();
}

void displayVersion() noexcept { console.info("readELF "sv, mangrove::core::versionString); }

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

template<> struct fmt::formatter<SectionHeaderType> : formatter<std::string_view>
{
	template<typename FormatContext> auto format(const SectionHeaderType &type, FormatContext &ctx) const
	{
		switch (type)
		{
			case SectionHeaderType::empty:
				return formatter<std::string_view>::format("empty"sv, ctx);
			case SectionHeaderType::program:
				return formatter<std::string_view>::format("program"sv, ctx);
			case SectionHeaderType::symbolTable:
				return formatter<std::string_view>::format("symbol table"sv, ctx);
			case SectionHeaderType::stringTable:
				return formatter<std::string_view>::format("string table"sv, ctx);
			case SectionHeaderType::relocAddend:
				return formatter<std::string_view>::format("relocations with addends"sv, ctx);
			case SectionHeaderType::symbolHash:
				return formatter<std::string_view>::format("symbol hash"sv, ctx);
			case SectionHeaderType::dynamic:
				return formatter<std::string_view>::format("dynamic"sv, ctx);
			case SectionHeaderType::note:
				return formatter<std::string_view>::format("note"sv, ctx);
			case SectionHeaderType::bss:
				return formatter<std::string_view>::format("bss"sv, ctx);
			case SectionHeaderType::reloc:
				return formatter<std::string_view>::format("relocations"sv, ctx);
			case SectionHeaderType::dynamicSymbols:
				return formatter<std::string_view>::format("dynamic symbols"sv, ctx);
			case SectionHeaderType::initArray:
				return formatter<std::string_view>::format("initialisation array"sv, ctx);
			case SectionHeaderType::finiArray:
				return formatter<std::string_view>::format("finalisation array"sv, ctx);
			case SectionHeaderType::preInitArray:
				return formatter<std::string_view>::format("pre-initialisation array"sv, ctx);
			case SectionHeaderType::group:
				return formatter<std::string_view>::format("group"sv, ctx);
			case SectionHeaderType::symbolTableIndex:
				return formatter<std::string_view>::format("symbol table index"sv, ctx);
			default:
				return fmt::format_to(ctx.out(), "<invalid section header type ({:x})>"sv, uint32_t(type));
		}
	}
};

template<> struct fmt::formatter<SectionHeader>
{
	constexpr auto parse(format_parse_context &ctx)
	{
		if (ctx.begin() != ctx.end())
			throw format_error{"invalid format"};
		return ctx.end();
	}

	template<typename FormatContext> auto format(const SectionHeader &header, FormatContext &ctx) const
	{
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Type '{}'\n"sv, header.type()));
		//ctx.advance_to(fmt::format_to(ctx.out(), "-> Chunk is {}\n"sv, header.flags()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Loaded at +0x{:x} from loading base address\n"sv, header.address()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Section data at offset {} (+{:x})\n"sv,
			header.fileOffset(), header.fileOffset()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Section is {} bytes long\n"sv, header.fileLength()));
		return ctx.out();
	}
};

template<> struct fmt::formatter<SymbolBinding> : formatter<std::string_view>
{
	template<typename FormatContext> auto format(const SymbolBinding &binding, FormatContext &ctx) const
	{
		switch (binding)
		{
			case SymbolBinding::local:
				return formatter<std::string_view>::format("local"sv, ctx);
			case SymbolBinding::global:
				return formatter<std::string_view>::format("global"sv, ctx);
			case SymbolBinding::weak:
				return formatter<std::string_view>::format("weak"sv, ctx);
			default:
				return fmt::format_to(ctx.out(), "<invalid symbol binding ({:x})>sv", uint8_t(binding));
		}
	}
};

template<> struct fmt::formatter<SymbolType> : formatter<std::string_view>
{
	template<typename FormatContext> auto format(const SymbolType &type, FormatContext &ctx) const
	{
		switch (type)
		{
			case SymbolType::none:
				return formatter<std::string_view>::format("none"sv, ctx);
			case SymbolType::object:
				return formatter<std::string_view>::format("object"sv, ctx);
			case SymbolType::function:
				return formatter<std::string_view>::format("function"sv, ctx);
			case SymbolType::section:
				return formatter<std::string_view>::format("section"sv, ctx);
			case SymbolType::file:
				return formatter<std::string_view>::format("file"sv, ctx);
			case SymbolType::common:
				return formatter<std::string_view>::format("common"sv, ctx);
			case SymbolType::threadLocal:
				return formatter<std::string_view>::format("thread local"sv, ctx);
			default:
				return fmt::format_to(ctx.out(), "<invalid symbol type ({:x})>sv", uint8_t(type));
		}
	}
};

template<> struct fmt::formatter<SymbolVisibility> : formatter<std::string_view>
{
	template<typename FormatContext> auto format(const SymbolVisibility &visibility, FormatContext &ctx) const
	{
		switch (visibility)
		{
			case SymbolVisibility::defaultVis:
				return formatter<std::string_view>::format("default"sv, ctx);
			case SymbolVisibility::internal:
				return formatter<std::string_view>::format("internal"sv, ctx);
			case SymbolVisibility::hidden:
				return formatter<std::string_view>::format("hidden"sv, ctx);
			case SymbolVisibility::protectedVis:
				return formatter<std::string_view>::format("protected"sv, ctx);
			default:
				return fmt::format_to(ctx.out(), "<invalid symbol visibility ({:x})>sv", uint8_t(visibility));
		}
	}
};

template<> struct fmt::formatter<ELFSymbol>
{
	constexpr auto parse(format_parse_context &ctx)
	{
		if (ctx.begin() != ctx.end())
			throw format_error{"invalid format"};
		return ctx.end();
	}

	template<typename FormatContext> auto format(const ELFSymbol &symbol, FormatContext &ctx) const
	{
		ctx.advance_to(fmt::format_to(ctx.out(), "-> {} bytes @0x{:x}\n"sv, symbol.symbolLength(), symbol.value()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Symbol is in section {} and has {} visibility\n"sv,
			symbol.sectionIndex(), symbol.visibility()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Symbol is {}ly bound\n", symbol.binding()));
		ctx.advance_to(fmt::format_to(ctx.out(), "-> Symbol has type {}\n", symbol.type()));
		return ctx.out();
	}
};

std::optional<ELF> openFile(const arguments_t &args) noexcept
{
	const auto *const fileNameArg{args["elfFile"sv]};
	if (!fileNameArg)
	{
		console.error("You must specify the name of the file to read"sv);
		return std::nullopt;
	}
	const auto fileName{std::any_cast<std::filesystem::path>(std::get<flag_t>(*fileNameArg).value())};

	fd_t underlyingFile{fileName, O_RDONLY | O_NOCTTY};
	if (!underlyingFile.valid())
		return std::nullopt;
	console.info("Read ELF file '"sv, fileName.u8string(), "'"sv);
	return ELF{std::move(underlyingFile)};
}

bool listHeaders(const arguments_t &headersArguments)
{
	if (headersArguments["help"sv])
	{
		displayHeadersHelp();
		return true;
	}

	const auto &file{openFile(headersArguments)};
	if (!file)
		return false;
	const auto &elfFile{*file};

	const auto specificsChosen{headersArguments["file"sv] || headersArguments["program"sv] || headersArguments["section"sv]};
	const auto all{headersArguments["all"sv] || !specificsChosen};

	if (headersArguments["file"sv] || all)
		fmt::print("{}"sv, elfFile.header());
	if (headersArguments["program"sv] || all)
	{
		console.info("Program headers:"sv);
		for (const auto &[index, header] : substrate::indexedIterator_t{elfFile.programHeaders()})
			fmt::print("{}: {}"sv, index, header);
	}
	if (headersArguments["section"sv] || all)
	{
		console.info("Section headers:"sv);
		for (const auto &[index, header] : substrate::indexedIterator_t{elfFile.sectionHeaders()})
		{
			fmt::print("{}: {}\n"sv, index, elfFile.sectionNames().stringFromOffset(header.nameOffset()));
			fmt::print("{}"sv, header);
		}
	}
	return true;
}

bool listSymbols(const arguments_t &symbolsArguments)
{
	if (symbolsArguments["help"sv])
	{
		displaySymbolsHelp();
		return true;
	}

	// Open the file, and check that we have a valid ELF
	auto file{openFile(symbolsArguments)};
	if (!file)
		return false;

	// Try and get the symbol table
	const auto maybeSymbolTable{file->symbolTable()};
	if (!maybeSymbolTable)
	{
		console.info("ELF file does not contain a symbol table"sv);
		return true;
	}
	// If we have one, validate it
	const auto &symbolTable{*maybeSymbolTable};
	if (!symbolTable.valid())
	{
		console.info("ELF file contains a symbol table but it is invalid"sv);
		return true;
	}
	// Then display how many symbols are in the table before looping through them to display them
	console.info("Symbol table in ELF file has "sv, symbolTable.count(), " symbols"sv);

	//  Try and get the string table
	const auto maybeStringTable{file->stringTable()};
	if (!maybeStringTable)
	{
		console.info("ELF file does not contain a string table"sv);
		return false;
	}
	const auto stringTable{*maybeStringTable};

	console.info("Symbols:"sv);
	for (const auto symbol : symbolTable)
		fmt::print("{}\n{}"sv, stringTable.stringFromOffset(symbol.nameOffset()), symbol);

	return false;
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
		return 1;
	}
	if (version)
	{
		displayVersion();
		return 0;
	}
	if (help || args.count() == 0U)
	{
		displayHelp();
		return 0;
	}

	const auto *const actionArg{args["action"sv]};
	if (!actionArg)
	{
		console.error("The action to perform must be specified"sv);
		displayHelp();
		return 1;
	}
	const auto &action{std::get<choice_t>(*actionArg)};

	const auto result
	{
		[&]()
		{
			if (action.value() == "headers"sv)
				return listHeaders(action.arguments());
			if (action.value() == "symbols"sv)
				return listSymbols(action.arguments());
			return false;
		}()
	 };
	 return result ? 0 : 1;
}
