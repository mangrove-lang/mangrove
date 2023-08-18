// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF_ENUMS_HXX
#define FORMATS_ELF_ENUMS_HXX

#include <cstdint>

/**
 * @file enums.hxx
 * @brief Enumerations for information in ELF files
 */

namespace mangrove::elf::enums
{
	enum class Class : uint8_t
	{
		elf32Bit = 1U,
		elf64Bit = 2U,
	};

	enum class IdentVersion : uint8_t
	{
		current = 1U,
	};

	enum class Endian : uint8_t
	{
		little = 1U,
		big = 2U,
	};

	enum class ABI : uint8_t
	{
		systemV = 0x00U,
		hpUX = 0x01U,
		netBSD = 0x02U,
		linux = 0x03U,
		gnuHurd = 0x04U,
		solaris = 0x06U,
		aix = 0x07U,
		irix = 0x08U,
		freeBSD = 0x09U,
		tru64 = 0x0aU,
		novelModesto = 0x0bU,
		openBSD = 0x0cU,
		openVMS = 0x0dU,
		nonStopKernel = 0x0eU,
		aros = 0x0fU,
		fenixOS = 0x10U,
		cloudABI = 0x11U,
		sortix = 0x53U,
		pic = 0x82U,
	};

	enum class Type : uint16_t
	{
		unknown = 0U,
		relocatable = 1U,
		executable = 2U,
		shared = 3U,
		core = 4U,
	};

	enum class Machine : uint16_t
	{
		nonSpecific = 0x0000U,
		sparc = 0x0002U,
		x86 = 0x0003U,
		mips = 0x0008U,
		powerPC = 0x0014U,
		s390 = 0x0016U,
		arm = 0x0028U,
		superH = 0x002aU,
		ia64 = 0x0032U,
		x86_64 = 0x003eU,
		aarch64 = 0x00b7U,
		pic = 0x00ccU,
		riscV = 0x00f3U,
	};

	enum class Version : uint32_t
	{
		current = 1U,
	};

	enum class ProgramHeaderType : uint32_t
	{
		empty = 0x00000000U, // PT_NULL
		load = 0x00000001U, // PT_LOAD
		dynamic = 0x00000002U, // PT_DYNAMIC
		interp = 0x00000003U, // PT_INTERP
		note = 0x00000004U, // PT_NOTE
		shlib = 0x00000005U, // PT_SHLIB
		phdr = 0x00000006U, // PT_PHDR
#if 0
		lowOS = 0x60000000U, // PT_LOOS
		highOS = 0x6fffffffU, // PT_HIOS
		lowProc = 0x70000000U, // PT_LOPROC
		highProc = 0x7fffffffU, // PT_HIPROC
#endif
	};

	enum class SectionHeaderType : uint32_t
	{
		empty = 0U, // SHT_NULL
		program = 1U, // SHT_PROGBITS
		symbolTable = 2U, // SHT_SYMTAB
		stringTable = 3U, // SHT_STRTAB
		relocAddend = 4U, // SHT_RELA
		symbolHash = 5U, // SHT_HASH
		dynamic = 6U, // SHT_DYNAMIC
		note = 7U, // SHT_NOTE
		bss = 8U, // SHT_NOBITS
		reloc = 9U, // SHT_REL
		reserved = 10U, // SHT_SHLIB
		dynamicSymbols = 11U, // SHT_DYNSYM
		initArray = 14U, // SHT_INIT_ARRAY
		finiArray = 15U, // SHT_FINI_ARRAY
		preInitArray = 16U, // SHT_PREINIT_ARRAY
		group = 17U, // SHT_GROUP
		symbolTableIndex = 18U, // SHT_SYMTAB_SHNDX
		numberOfTypes = 19U, // SHT_NUM
	};

	enum class SectionFlag : uint64_t
	{
		writeable = 0x00000001U, // SHF_WRITE
		allocate = 0x00000002U, // SHF_ALLOC
		execuable = 0x00000004U, // SHF_EXECINSTR
		merge = 0x00000010U, // SHF_MERGE
		strings = 0x00000020U, // SHF_STRINGS
		infoLink = 0x00000040U, // SHF_INFO_LINK
		linkOrder = 0x00000080U, // SHF_LINK_ORDER,
		osNonConforming = 0x00000100U, // SHF_OS_NONCONFORMING
		group = 0x00000200U, // SHF_GROUP
		tls = 0x00000400U, // SHF_TLS
		osMask = 0x0ff00000U, // SHF_MASKOS
		processorMask = 0xf0000000U, // SHF_MASKPROC
		solOrdered = 0x04000000U, // SHF_ORDERED
		solExclude = 0x08000000U, // SHF_EXCLUDE
	};

	enum class SymbolBinding : uint8_t
	{
		local = 0U, // STB_LOCAL
		global = 1U, // STB_GLOBAL
		weak = 2U, // STB_WEAK
#if 0
		lowOS = 10U, // STB_LOOS
		highOS = 12U, // STB_HIOS
		lowProc = 13U, // STB_LOPROC
		highProc = 15U, // STB_HIPROC
#endif
	};

	enum class SymbolType : uint8_t
	{
		none = 0U, // STT_NOTYPE
		object = 1U, // STT_OBJECT
		function = 2U, // STT_FUNC
		section = 3U, // STT_SECTION
		file = 4U, // STT_FILE
		common = 5U, // STT_COMMON
		threadLocal = 6U, // STT_TLS
#if 0
		lowOS = 10U, // STT_LOOS
		highOS = 12U, // STT_HIOS
		lowProc = 13U, // STT_LOPROC
		highProc = 15U, // STT_HIPROC
#endif
	};

	enum class SymbolVisibility : uint8_t
	{
		defaultVis = 0U, // STV_DEFAULT
		internal = 1U, // STV_INTERNAL
		hidden = 2U, // STV_HIDDEN
		protectedVis = 3U, // STV_PROTECTED
	};

	enum class SymbolInfoFlag : uint8_t
	{
		directBound = 0x01U,
		passthrough = 0x02U,
		copyReloc = 0x04U,
		lazyLoaded = 0x08U,
	};
} // namespace mangrove::elf::enums

#endif /*FORMATS_ELF_ENUMS_HXX*/
