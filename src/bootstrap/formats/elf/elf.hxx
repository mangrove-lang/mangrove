// SPDX-License-Identifier: BSD-3-Clause
#ifndef FORMATS_ELF_HXX
#define FORMATS_ELF_HXX

#include <variant>
#include <memory>
#include <substrate/fd>
#include <substrate/mmap>
#include <substrate/span>
#include "types.hxx"

namespace mangrove::elf
{
	inline namespace internal
	{
		using substrate::fd_t;
		using substrate::span;
		using namespace types;

		using substrate::mmap_t;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		using FragmentStorage = std::vector<std::unique_ptr<uint8_t []>>;

		[[nodiscard]] static inline span<uint8_t> toSpan(mmap_t &map) noexcept
			{ return {map.address<uint8_t>(), map.length()}; }
	} // namespace internal

	struct ELF final
	{
	private:
		std::variant<mmap_t, FragmentStorage> _backingStorage;
		ELFHeader _header;

		template<typename T> T allocate()
		{
			auto &storage{std::get<FragmentStorage>(_backingStorage)};
			const auto size{T::size()};
			// NOLINTNEXTLINE(modernize-avoid-c-arrays)
			const auto &allocation{storage.emplace_back(std::make_unique<uint8_t []>(size))};
			return {span{allocation.get(), size}};
		}

	public:
		ELF(fd_t &&file) : _backingStorage{file.map(PROT_READ)}, _header{
			[this]() -> ELFHeader
			{
				auto &map{std::get<mmap_t>(_backingStorage)};
				const auto data{toSpan(map)};
				ELFIdent ident{data};
				// TODO: Check the validity of ident
				if (ident.elfClass() == Class::elf32Bit)
					return elf32::ELFHeader{data};
				return elf64::ELFHeader{data};
			}()
		} { }

		ELF(const Class elfClass) : _backingStorage{FragmentStorage{}}, _header{
			[this](const Class fileClass) -> ELFHeader
			{
				if (fileClass == Class::elf32Bit)
					return allocate<elf32::ELFHeader>();
				return allocate<elf64::ELFHeader>();
			}(elfClass)
		} { }

		[[nodiscard]] auto &header() noexcept { return _header; }
		[[nodiscard]] const auto &header() const noexcept { return _header; }
	};
} // namespace mangrove::elf

#endif /*FORMATS_ELF_HXX*/
