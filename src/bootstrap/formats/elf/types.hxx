// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF_TYPES_HXX
#define FORMATS_ELF_TYPES_HXX

#include <variant>
#include "io.hxx"
#include "enums.hxx"
#include "commonTypes.hxx"
#include "elf32Types.hxx"
#include "elf64Types.hxx"

namespace mangrove::elf::types
{
	inline namespace internal
	{
		using mangrove::elf::io::overloaded_t;
	} // namespace internal

	struct elfHeader_t
	{
	private:
		std::variant<elf32::elfHeader_t, elf64::elfHeader_t> _header;

	public:
		template<typename T> elfHeader_t(T header) noexcept : _header{header} { }

		std::array<uint8_t, 4> magic() const;
		class_t elfClass() const;
		endian_t endian() const;
		abi_t abi() const;
		elfType_t type() const;
		machine_t machine() const;
		version_t version() const;
		uint64_t entryPoint() const;
		uint64_t phdrOffset() const;
		uint64_t shdrOffset() const;
		uint32_t flags() const;
		uint16_t headerSize() const;
		uint16_t programHeaderSize() const;
		uint16_t programHeaderCount() const;
		uint16_t sectionHeaderSize() const;
		uint16_t sectionHeaderCount() const;
		uint16_t sectionNamesIndex() const;
	};
} // namespace mangrove::elf::types

#endif /*FORMATS_ELF_TYPES_HXX*/
