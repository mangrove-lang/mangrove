// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF_ENUMS_HXX
#define FORMATS_ELF_ENUMS_HXX

#include <cstdint>

namespace mangrove::elf::enums
{
	enum class class_t : uint8_t
	{
		elf32Bit = 1,
		elf64Bit = 2
	};

	enum class identVersion_t : uint8_t
	{
		current = 1
	};

	enum class endian_t : uint8_t
	{
		little = 1,
		big = 2
	};

	enum class abi_t : uint8_t
	{
		systemV = 0x00,
		hpUX = 0x01,
		netBSD = 0x02,
		linux = 0x03,
		gnuHurd = 0x04,
		solaris = 0x06,
		aix = 0x07,
		irix = 0x08,
		freeBSD = 0x09,
		tru64 = 0x0A,
		novelModesto = 0x0B,
		openBSD = 0x0C,
		openVMS = 0x0D,
		nonStopKernel = 0x0E,
		aros = 0x0F,
		fenixOS = 0x10,
		cloudABI = 0x11,
		sortix = 0x53,
		pic = 0x82
	};

	enum class elfType_t : uint16_t
	{
		unknown = 0,
		relocatable = 1,
		executable = 2,
		shared = 3,
		core = 4
	};

	enum class machine_t : uint16_t
	{
		nonSpecific = 0x0000,
		sparc = 0x0002,
		x86 = 0x0003,
		mips = 0x0008,
		powerPC = 0x0014,
		s390 = 0x0016,
		arm = 0x0028,
		superH = 0x002A,
		ia64 = 0x0032,
		x86_64 = 0x003E,
		aarch64 = 0x00B7,
		pic = 0x00CC,
		riscV = 0x00F3
	};

	enum class version_t : uint32_t
	{
		current = 1
	};

	enum class progType_t : uint32_t
	{
		empty = 0x00000000, // PT_NULL
		load = 0x00000001, // PT_LOAD
		dynamic = 0x00000002, // PT_DYNAMIC
		interp = 0x00000003, // PT_INTERP
		note = 0x00000004, // PT_NOTE
		shlib = 0x00000005, // PT_SHLIB
		phdr = 0x00000006 // PT_PHDR
#if 0
		lowOS = 0x60000000, // PT_LOOS
		highOS = 0x6FFFFFFF, // PT_HIOS
		lowProc = 0x70000000, // PT_LOPROC
		highProc = 0x7FFFFFFF // PT_HIPROC
#endif
	};

	enum class sectType_t : uint32_t
	{
		empty = 0x00000000, // SHT_NULL
		program = 0x00000001, // SHT_PROGBITS
		symbolTable = 0x00000002, // SHT_SYMTAB
		stringTable = 0x00000003, // SHT_STRTAB
		relocAddend = 0x00000004, // SHT_RELA
		symbolHash = 0x00000005, // SHT_HASH
		dynamic = 0x00000006, // SHT_DYNAMIC
		note = 0x00000007, // SHT_NOTE
		bss = 0x00000008, // SHT_NOBITS
		reloc = 0x00000009, // SHT_REL
		reserved = 0x0000000A, // SHT_SHLIB
		dynamicSymbols = 0x0000000B, // SHT_DYNSYM
		initArray = 0x0000000E, // SHT_INIT_ARRAY
		finiArray = 0x0000000F, // SHT_FINI_ARRAY
		preInitArray = 0x00000010, // SHT_PREINIT_ARRAY
		group = 0x00000011, // SHT_GROUP
		symbolTableIndex = 0x00000012, // SHT_SYMTAB_SHNDX
		numberOfTypes = 0x00000013 // SHT_NUM
	};

	enum class sectFlag_t : uint64_t
	{
		writeable = 0x00000001, // SHF_WRITE
		allocate = 0x00000002, // SHF_ALLOC
		execuable = 0x00000004, // SHF_EXECINSTR
		merge = 0x00000010, // SHF_MERGE
		strings = 0x00000020, // SHF_STRINGS
		infoLink = 0x00000040, // SHF_INFO_LINK
		linkOrder = 0x00000080, // SHF_LINK_ORDER,
		osNonConforming = 0x00000100, // SHF_OS_NONCONFORMING
		group = 0x00000200, // SHF_GROUP
		tls = 0x00000400, // SHF_TLS
		osMask = 0x0FF00000, // SHF_MASKOS
		processorMask = 0xF0000000, // SHF_MASKPROC
		solOrdered = 0x04000000, // SHF_ORDERED
		solExclude = 0x08000000 // SHF_EXCLUDE
	};

/*
	struct sectFlags_t final
	{
	private:
		uint64_t flags_;

	public:
		constexpr sectFlags_t() noexcept : flags_{0} { }
		constexpr sectFlags_t(const uint64_t flags) noexcept : flags_{flags} { }
		constexpr sectFlags_t(const sectFlag_t flag) noexcept : flags_{uint64_t(flag)} { }
		void operator =(const sectFlags_t flags) noexcept { flags_ = flags; }
		void operator |=(const sectFlag_t flag) noexcept { flags_ |= uint64_t(flag); }
		constexpr sectFlags_t operator |(const sectFlag_t flag) const noexcept { return flags_ | uint64_t(flag); }
		constexpr uint64_t operator &(const sectFlag_t flag) const noexcept { return flags_ & uint64_t(flag); }
		constexpr operator uint64_t() const noexcept { return flags_; }
		constexpr bool has(const sectFlag_t flag) noexcept { return *this & flag; }
	};

	struct types32_t { using offset_t = uint32_t; };
	struct types64_t { using offset_t = uint64_t; };
*/
} // namespace mangrove::elf::enums

#endif /*FORMATS_ELF_ENUMS_HXX*/
