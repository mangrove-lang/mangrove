// SPDX-License-Identifier: BSD-3-Clause
#include "elf.hxx"

namespace mangrove::elf
{
	ELF::ELF(fd_t &&file) : _backingStorage{file.map(PROT_READ)}, _header
	{
		[this]() -> ELFHeader
		{
			auto &map{std::get<mmap_t>(_backingStorage)};
			const auto data{toSpan(map)};
			const ELFIdent ident{data};
			// TODO: Check the validity of ident
			if (ident.elfClass() == Class::elf32Bit)
				return elf32::ELFHeader{data};
			return elf64::ELFHeader{data};
		}()
	} { }

	ELF::ELF(const Class elfClass) : _backingStorage{FragmentStorage{}}, _header
	{
		[this](const Class fileClass) -> ELFHeader
		{
			if (fileClass == Class::elf32Bit)
				return allocate<elf32::ELFHeader>();
			return allocate<elf64::ELFHeader>();
		}(elfClass)
	} { }
} // namespace mangrove::elf
