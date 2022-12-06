// SPDX-License-Identifier: BSD-3-Clause
#include "types.hxx"

using namespace mangrove::elf::types;

std::array<uint8_t, 4> elfHeader_t::magic() const noexcept
	{ return std::visit([](const auto &header) { return header.magic(); }, _header); }
class_t elfHeader_t::elfClass() const noexcept
	{ return std::visit([](const auto &header) { return header.elfClass(); }, _header); }
endian_t elfHeader_t::endian() const noexcept
	{ return std::visit([](const auto &header) { return header.endian(); }, _header); }
abi_t elfHeader_t::abi() const noexcept
	{ return std::visit([](const auto &header) { return header.abi(); }, _header); }
elfType_t elfHeader_t::type() const noexcept
	{ return std::visit([](const auto &header) { return header.type(); }, _header); }
machine_t elfHeader_t::machine() const noexcept
	{ return std::visit([](const auto &header) { return header.machine(); }, _header); }
version_t elfHeader_t::version() const noexcept
	{ return std::visit([](const auto &header) { return header.version(); }, _header); }
uint64_t elfHeader_t::entryPoint() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.entryPoint(); }, _header); }
uint64_t elfHeader_t::phdrOffset() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.phdrOffset(); }, _header); }
uint64_t elfHeader_t::shdrOffset() const noexcept
	{ return std::visit([](const auto &header) -> uint64_t { return header.shdrOffset(); }, _header); }
uint32_t elfHeader_t::flags() const noexcept
	{ return std::visit([](const auto &header) { return header.flags(); }, _header); }
uint16_t elfHeader_t::headerSize() const noexcept
	{ return std::visit([](const auto &header) { return header.headerSize(); }, _header); }
uint16_t elfHeader_t::programHeaderSize() const noexcept
	{ return std::visit([](const auto &header) { return header.programHeaderSize(); }, _header); }
uint16_t elfHeader_t::programHeaderCount() const noexcept
	{ return std::visit([](const auto &header) { return header.programHeaderCount(); }, _header); }
uint16_t elfHeader_t::sectionHeaderSize() const noexcept
	{ return std::visit([](const auto &header) { return header.sectionHeaderSize(); }, _header); }
uint16_t elfHeader_t::sectionHeaderCount() const noexcept
	{ return std::visit([](const auto &header) { return header.sectionHeaderCount(); }, _header); }
uint16_t elfHeader_t::sectionNamesIndex() const noexcept
	{ return std::visit([](const auto &header) { return header.sectionNamesIndex(); }, _header); }
