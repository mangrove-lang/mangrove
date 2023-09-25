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
	{ return std::visit([](const auto &header) { return header.nameOffset(); }, _symbol); }
uint64_t ELFSymbol::value() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.value(); }, _symbol); }
uint64_t ELFSymbol::symbolLength() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.symbolLength(); }, _symbol); }
uint8_t ELFSymbol::info() const noexcept
	{ return std::visit([](const auto &header) { return header.info(); }, _symbol); }
uint8_t ELFSymbol::other() const noexcept
	{ return std::visit([](const auto &header) { return header.other(); }, _symbol); }
uint16_t ELFSymbol::sectionIndex() const noexcept
	{ return std::visit([](const auto &header) { return header.sectionIndex(); }, _symbol); }
size_t ELFSymbol::size() const noexcept
	{ return std::visit([](const auto &header) { return header.size(); }, _symbol); }

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
		if (value == 0x00U)
			break;
		++length;
	}
	// Finally, put the whole thing together as a string view
	// NOTLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	return {reinterpret_cast<const char *>(data.data()), length};
}

ELFSymbol SymbolIterator::operator *() const noexcept
{
	const auto &symbolTable{*_table};
	// Index the symbol table and assume the resulting symbol is valid.
	// This also assumes that _index is not set to the end of the table.
	return *symbolTable[_index];
}

SymbolIterator &SymbolIterator::operator ++() noexcept
{
	// If we're not yet at the end, allow the increment
	if (_index < _table->count())
		++_index;
	return *this;
}

SymbolIterator &SymbolIterator::operator --() noexcept
{
	// Disallow decrementing past 0
	if (_index)
		--_index;
	return *this;
}

// Two `SymbolIterator`s are equal if they refer to the same symbol table and have the same index
bool SymbolIterator::operator ==(const SymbolIterator &other) const noexcept
	{ return _table == other._table && _index == other._index; }

bool SymbolTable::valid() const noexcept
{
	const auto symbolLength{ELFSymbol::size(_class)};
	const auto symbols{count()};
	// If there are symbols in the table
	if (symbols)
	{
		// Grab the first
		const auto maybeEntry0{(*this)[0]};
		if (!maybeEntry0)
			return false;
		// Validate the entry is the special reserved undefined symbol entry
		const auto &entry0{*maybeEntry0};
		if (entry0.nameOffset() != 0 ||
			entry0.value() != 0 ||
			entry0.symbolLength() != 0 ||
			entry0.binding() != SymbolBinding::local ||
			entry0.type() != SymbolType::none ||
			entry0.visibility() != SymbolVisibility::defaultVis ||
			entry0.sectionIndex() != 0)
			return false;
	}
	// Finish up validity checking by making sure the table has no left over bytes to it that
	// would indicate some kind of corruption (eg, wrong ELF class symbols used, incomplete final entry)
	return symbols * symbolLength == _storage.length();
}

size_t SymbolTable::count() const noexcept
{
	const auto symbolLength{ELFSymbol::size(_class)};
	return _storage.length() / symbolLength;
}

std::optional<ELFSymbol> SymbolTable::operator [](const size_t index) const noexcept
{
	const auto symbolLength{ELFSymbol::size(_class)};
	const auto offset{index * symbolLength};
	if (offset + symbolLength > _storage.length())
		return std::nullopt;

	const auto data{_storage.dataSpan().subspan(offset, symbolLength)};
	if (_class == Class::elf32Bit)
		return elf32::ELFSymbol{data, _endian};
	return elf64::ELFSymbol{data, _endian};
}

// NOLINTEND(bugprone-exception-escape)
