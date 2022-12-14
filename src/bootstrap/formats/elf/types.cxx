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
