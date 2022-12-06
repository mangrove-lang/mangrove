// SPDX-License-Identifier: BSD-3-Clause
#include "types.hxx"

using namespace mangrove::elf::types;

std::array<uint8_t, 4> elfHeader_t::magic() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.magic(); },
		[](std::monostate) -> std::array<uint8_t, 4>
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

class_t elfHeader_t::elfClass() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.elfClass(); },
		[](std::monostate) -> class_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

endian_t elfHeader_t::endian() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.endian(); },
		[](std::monostate) -> endian_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

abi_t elfHeader_t::abi() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.abi(); },
		[](std::monostate) -> abi_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

elfType_t elfHeader_t::type() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.type(); },
		[](std::monostate) -> elfType_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

machine_t elfHeader_t::machine() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.machine(); },
		[](std::monostate) -> machine_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

version_t elfHeader_t::version() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.version(); },
		[](std::monostate) -> version_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

uint64_t elfHeader_t::entryPoint() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) -> uint64_t { return header.entryPoint(); },
		[](std::monostate) -> uint64_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

uint64_t elfHeader_t::phdrOffset() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) -> uint64_t { return header.phdrOffset(); },
		[](std::monostate) -> uint64_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

uint64_t elfHeader_t::shdrOffset() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) -> uint64_t { return header.shdrOffset(); },
		[](std::monostate) -> uint64_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

uint32_t elfHeader_t::flags() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.flags(); },
		[](std::monostate) -> uint32_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

uint16_t elfHeader_t::headerSize() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.headerSize(); },
		[](std::monostate) -> uint16_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

uint16_t elfHeader_t::programHeaderSize() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.programHeaderSize(); },
		[](std::monostate) -> uint16_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

uint16_t elfHeader_t::programHeaderCount() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.programHeaderCount(); },
		[](std::monostate) -> uint16_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

uint16_t elfHeader_t::sectionHeaderSize() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.sectionHeaderSize(); },
		[](std::monostate) -> uint16_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

uint16_t elfHeader_t::sectionHeaderCount() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.sectionHeaderCount(); },
		[](std::monostate) -> uint16_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}

uint16_t elfHeader_t::sectionNamesIndex() const
{
	return std::visit(overloaded_t
	{
		[](const auto &header) { return header.sectionNamesIndex(); },
		[](std::monostate) -> uint16_t
			{ throw std::logic_error{"Header must be initialised before calling this"}; }
	}, _header);
}
