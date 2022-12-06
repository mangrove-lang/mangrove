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
		using fragmentStorage_t = std::vector<std::unique_ptr<uint8_t []>>;

		static inline span<uint8_t> toSpan(mmap_t &map) noexcept
			{ return {map.address<uint8_t>(), map.length()}; }
	} // namespace internal

	struct elf_t
	{
	private:
		std::variant<mmap_t, fragmentStorage_t> _backingStorage;
		elfHeader_t _header;

		template<typename T> T allocate()
		{
			auto &storage{std::get<fragmentStorage_t>(_backingStorage)};
			const auto size{T::size()};
			const auto &allocation{storage.emplace_back(std::make_unique<uint8_t []>(size))};
			return {span{allocation.get(), size}};
		}

	public:
		elf_t(const int32_t fd) : _backingStorage{mmap_t{fd, 0, PROT_READ}}, _header{
			[this]() -> elfHeader_t
			{
				auto &map{std::get<mmap_t>(_backingStorage)};
				const auto data{toSpan(map)};
				elfIdent_t ident{data};
				// TODO: Check the validity of ident
				if (ident.elfClass() == class_t::elf32Bit)
					return elf32::elfHeader_t{data};
				else
					return elf64::elfHeader_t{data};
			}()
		} { }

		elf_t(const class_t elfClass) : _backingStorage{fragmentStorage_t{}}, _header{
			[this](const class_t elfClass) -> elfHeader_t
			{
				if (elfClass == class_t::elf32Bit)
					return allocate<elf32::elfHeader_t>();
				else
					return allocate<elf64::elfHeader_t>();
			}(elfClass)
		} { }

		auto &header() noexcept { return _header; }
		const auto &header() const noexcept { return _header; }
	};
} // namespace mangrove::elf

#endif /*FORMATS_ELF_HXX*/
