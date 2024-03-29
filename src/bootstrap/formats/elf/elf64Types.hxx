// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF64_TYPES_HXX
#define FORMATS_ELF64_TYPES_HXX

#include "io.hxx"
#include "enums.hxx"
#include "commonTypes.hxx"
#include "../../core/flags.hxx"

namespace mangrove::elf::types::elf64
{
	using mangrove::core::Flags;

	struct ELFHeader final : ELFIdent
	{
	public:
		ELFHeader(const Memory &storage) : ELFIdent{storage} { }

		[[nodiscard]] auto type() const noexcept { return _storage.read<Type>(16, _endian); }
		[[nodiscard]] auto machine() const noexcept { return _storage.read<Machine>(18, _endian); }
		[[nodiscard]] auto version() const noexcept { return _storage.read<Version>(20, _endian); }
		[[nodiscard]] auto entryPoint() const noexcept { return _storage.read<uint64_t>(24, _endian); }
		[[nodiscard]] auto phdrOffset() const noexcept { return _storage.read<uint64_t>(32, _endian); }
		[[nodiscard]] auto shdrOffset() const noexcept { return _storage.read<uint64_t>(40, _endian); }
		[[nodiscard]] auto flags() const noexcept { return _storage.read<uint32_t>(48, _endian); }
		[[nodiscard]] auto headerSize() const noexcept { return _storage.read<uint16_t>(52, _endian); }
		[[nodiscard]] auto programHeaderSize() const noexcept { return _storage.read<uint16_t>(54, _endian); }
		[[nodiscard]] auto programHeaderCount() const noexcept { return _storage.read<uint16_t>(56, _endian); }
		[[nodiscard]] auto sectionHeaderSize() const noexcept { return _storage.read<uint16_t>(58, _endian); }
		[[nodiscard]] auto sectionHeaderCount() const noexcept { return _storage.read<uint16_t>(60, _endian); }
		[[nodiscard]] auto sectionNamesIndex() const noexcept { return _storage.read<uint16_t>(62, _endian); }

		[[nodiscard]] bool valid() const noexcept
		{
			return
				magic() == elfMagic &&
				ELFIdent::version() == IdentVersion::current &&
				version() == Version::current &&
				headerSize() == size();
		}

		[[nodiscard]] constexpr static size_t size() noexcept { return ELFIdent::size() + 48U; }
	};

	struct ProgramHeader final
	{
	private:
		Memory _storage;
		Endian _endian;

	public:
		ProgramHeader(const Memory &storage, const Endian &endian) : _storage{storage}, _endian{endian} { }

		[[nodiscard]] auto type() const noexcept { return _storage.read<ProgramHeaderType>(0, _endian); }
		[[nodiscard]] auto flags() const noexcept { return _storage.read<uint32_t>(4, _endian); }
		[[nodiscard]] auto offset() const noexcept { return _storage.read<uint64_t>(8, _endian); }
		[[nodiscard]] auto virtualAddress() const noexcept { return _storage.read<uint64_t>(16, _endian); }
		[[nodiscard]] auto physicalAddress() const noexcept { return _storage.read<uint64_t>(24, _endian); }
		[[nodiscard]] auto fileLength() const noexcept { return _storage.read<uint64_t>(32, _endian); }
		[[nodiscard]] auto memoryLength() const noexcept { return _storage.read<uint64_t>(40, _endian); }
		[[nodiscard]] auto alignment() const noexcept { return _storage.read<uint64_t>(48, _endian); }

		[[nodiscard]] constexpr static size_t size() noexcept { return 56U; }
	};

	struct SectionHeader final
	{
	private:
		Memory _storage;
		Endian _endian;

	public:
		SectionHeader(const Memory &storage, const Endian &endian) : _storage{storage}, _endian{endian} { }

		[[nodiscard]] auto nameOffset() const noexcept { return _storage.read<uint32_t>(0, _endian); }
		[[nodiscard]] auto type() const noexcept { return _storage.read<SectionHeaderType>(4, _endian); }
		[[nodiscard]] Flags<SectionFlag> flags() const noexcept { return {_storage.read<uint64_t>(8, _endian)}; }
		[[nodiscard]] auto address() const noexcept { return _storage.read<uint64_t>(16, _endian); }
		[[nodiscard]] auto fileOffset() const noexcept { return _storage.read<uint64_t>(24, _endian); }
		[[nodiscard]] auto fileLength() const noexcept { return _storage.read<uint64_t>(32, _endian); }
		[[nodiscard]] auto link() const noexcept { return _storage.read<uint32_t>(40, _endian); }
		[[nodiscard]] auto info() const noexcept { return _storage.read<uint32_t>(44, _endian); }
		[[nodiscard]] auto alignment() const noexcept { return _storage.read<uint64_t>(48, _endian); }
		[[nodiscard]] auto entityLength() const noexcept { return _storage.read<uint64_t>(56, _endian); }

		[[nodiscard]] constexpr static size_t size() noexcept { return 64U; }
	};

	struct ELFSymbol final
	{
	private:
		Memory _storage;
		Endian _endian;

	public:
		ELFSymbol(const Memory &storage, const Endian &endian) : _storage{storage}, _endian{endian} { }

		[[nodiscard]] auto nameOffset() const noexcept { return _storage.read<uint32_t>(0, _endian); }
		[[nodiscard]] auto info() const noexcept { return _storage.read<uint8_t>(4); }
		[[nodiscard]] auto other() const noexcept { return _storage.read<uint8_t>(5); }
		[[nodiscard]] auto sectionIndex() const noexcept { return _storage.read<uint16_t>(6, _endian); }
		[[nodiscard]] auto value() const noexcept { return _storage.read<uint64_t>(8, _endian); }
		[[nodiscard]] auto symbolLength() const noexcept { return _storage.read<uint64_t>(16, _endian); }

		[[nodiscard]] constexpr static size_t size() noexcept { return 24U; }
	};
} // namespace mangrove::elf::types::elf64

#endif /*FORMATS_ELF64_TYPES_HXX*/
