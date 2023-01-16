// SPDX-License-Identifier: BSD-3-Clause
#include "types.hxx"

using namespace mangrove::elf::types;
using mangrove::elf::io::Match;

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
