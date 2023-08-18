// SPDX-License-Identifier: BSD-3-Clause
#include "types.hxx"

using namespace mangrove::elf::types;
using mangrove::elf::io::Match;

// NOLINTBEGIN(bugprone-exception-escape)

std::array<uint8_t, 4> ELFHeader::magic() const noexcept
	{ return std::visit([](const auto &header) { return header.magic(); }, _header); }
Class ELFHeader::elfClass() const noexcept
	{ return std::visit([](const auto &header) { return header.elfClass(); }, _header); }
Endian ELFHeader::endian() const noexcept
	{ return std::visit([](const auto &header) { return header.endian(); }, _header); }
ABI ELFHeader::abi() const noexcept
	{ return std::visit([](const auto &header) { return header.abi(); }, _header); }
Type ELFHeader::type() const noexcept
	{ return std::visit([](const auto &header) { return header.type(); }, _header); }
Machine ELFHeader::machine() const noexcept
	{ return std::visit([](const auto &header) { return header.machine(); }, _header); }
Version ELFHeader::version() const noexcept
	{ return std::visit([](const auto &header) { return header.version(); }, _header); }
uint64_t ELFHeader::entryPoint() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.entryPoint(); }, _header); }
uint64_t ELFHeader::phdrOffset() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.phdrOffset(); }, _header); }
uint64_t ELFHeader::shdrOffset() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.shdrOffset(); }, _header); }
uint32_t ELFHeader::flags() const noexcept
	{ return std::visit([](const auto &header) { return header.flags(); }, _header); }
uint16_t ELFHeader::headerSize() const noexcept
	{ return std::visit([](const auto &header) { return header.headerSize(); }, _header); }
uint16_t ELFHeader::programHeaderSize() const noexcept
	{ return std::visit([](const auto &header) { return header.programHeaderSize(); }, _header); }
uint16_t ELFHeader::programHeaderCount() const noexcept
	{ return std::visit([](const auto &header) { return header.programHeaderCount(); }, _header); }
uint16_t ELFHeader::sectionHeaderSize() const noexcept
	{ return std::visit([](const auto &header) { return header.sectionHeaderSize(); }, _header); }
uint16_t ELFHeader::sectionHeaderCount() const noexcept
	{ return std::visit([](const auto &header) { return header.sectionHeaderCount(); }, _header); }
uint16_t ELFHeader::sectionNamesIndex() const noexcept
	{ return std::visit([](const auto &header) { return header.sectionNamesIndex(); }, _header); }

ProgramHeaderType ProgramHeader::type() const noexcept
	{ return std::visit([](const auto &header) { return header.type(); }, _header); }
uint32_t ProgramHeader::flags() const noexcept
	{ return std::visit([](const auto &header) { return header.flags(); }, _header); }
uint64_t ProgramHeader::offset() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.offset(); }, _header); }
uint64_t ProgramHeader::virtualAddress() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.virtualAddress(); }, _header); }
uint64_t ProgramHeader::physicalAddress() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.physicalAddress(); }, _header); }
uint64_t ProgramHeader::fileLength() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.fileLength(); }, _header); }
uint64_t ProgramHeader::memoryLength() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.memoryLength(); }, _header); }
uint64_t ProgramHeader::alignment() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.alignment(); }, _header); }

uint32_t SectionHeader::nameOffset() const noexcept
	{ return std::visit([](const auto &header) { return header.nameOffset(); }, _header); }
SectionHeaderType SectionHeader::type() const noexcept
	{ return std::visit([](const auto &header) { return header.type(); }, _header); }
Flags<SectionFlag> SectionHeader::flags() const noexcept
	{ return std::visit([](const auto &header) { return header.flags(); }, _header); }
uint64_t SectionHeader::address() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.address(); }, _header); }
uint64_t SectionHeader::fileOffset() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.fileOffset(); }, _header); }
uint64_t SectionHeader::fileLength() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.fileLength(); }, _header); }
uint32_t SectionHeader::link() const noexcept
	{ return std::visit([](const auto &header) { return header.link(); }, _header); }
uint32_t SectionHeader::info() const noexcept
	{ return std::visit([](const auto &header) { return header.info(); }, _header); }
uint64_t SectionHeader::alignment() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.alignment(); }, _header); }
uint64_t SectionHeader::entityLength() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.entityLength(); }, _header); }

uint32_t ELFSymbol::nameOffset() const noexcept
	{ return std::visit([](const auto &header) { return header.nameOffset(); }, _header); }
uint64_t ELFSymbol::value() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.value(); }, _header); }
uint64_t ELFSymbol::symbolLength() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.symbolLength(); }, _header); }
uint8_t ELFSymbol::info() const noexcept
	{ return std::visit([](const auto &header) { return header.info(); }, _header); }
uint8_t ELFSymbol::other() const noexcept
	{ return std::visit([](const auto &header) { return header.other(); }, _header); }
uint16_t ELFSymbol::sectionIndex() const noexcept
	{ return std::visit([](const auto &header) { return header.sectionIndex(); }, _header); }
size_t ELFSymbol::size() const noexcept
	{ return std::visit([](const auto &header) { return header.size(); }, _header); }

SymbolBinding ELFSymbol::binding() const noexcept
{
	const auto value{info()};
	return static_cast<SymbolBinding>(value >> 4U);
}

SymbolType ELFSymbol::type() const noexcept
{
	const auto value{info()};
	return static_cast<SymbolType>(value & 0x0fU);
}

SymbolVisibility ELFSymbol::visibility() const noexcept
{
	const auto value{other()};
	return static_cast<SymbolVisibility>(value & 0x03U);
}

size_t ELFSymbol::size(Class elfClass) noexcept
{
	switch (elfClass)
	{
		case Class::elf32Bit:
			return elf32::ELFSymbol::size();
		case Class::elf64Bit:
			return elf64::ELFSymbol::size();
	}
	return 0U;
}

std::string_view StringTable::stringFromOffset(const size_t offset) const noexcept
{
	// Start by getting a subspan at the correct offset
	const auto data{_storage.dataSpan().subspan(offset)};
	// Now, we need to figure out how long the string is.. we could use strlen(),
	// but that would introduce a risk of memory unsafety here, so we instead iterate
	// till we find '\0' or fall off the end of the span, which should mean we
	// can't accidentally do a memory safety bad here.
	size_t length{};
	for (const auto &value : data)
	{
		++length;
		if (value == 0x00U)
			break;
	}
	// Finally, put the whole thing together as a string view
	// NOTLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	return {reinterpret_cast<const char *>(data.data()), length};
}

// NOLINTEND(bugprone-exception-escape)
