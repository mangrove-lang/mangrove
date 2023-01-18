// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/index_sequence>
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
	} // This initially allows the vectors for the headers to be default constructed.
	{
		const auto data{toSpan(std::get<mmap_t>(_backingStorage))};
		const auto elfClass{_header.elfClass()};
		const auto endian{_header.endian()};
		const auto programHeaderSize{_header.programHeaderSize()};

		size_t offset{_header.phdrOffset()};
		// Once the ELFHeader has been read and handled, we then loop through pulling out the program headers.
		for ([[maybe_unused]] const auto index : substrate::indexSequence_t{_header.programHeaderCount()})
		{
			if (elfClass == Class::elf32Bit)
				_programHeaders.emplace_back
				(
					elf32::ProgramHeader
					{
						data.subspan(offset, elf32::ProgramHeader::size()),
						endian
					}
				);
			else
				_programHeaders.emplace_back
				(
					elf64::ProgramHeader
					{
						data.subspan(offset, elf64::ProgramHeader::size()),
						endian
					}
				);
			offset += programHeaderSize;
		}

		const auto sectionHeaderSize{_header.sectionHeaderSize()};
		offset = _header.shdrOffset();
		// Now loop through and pull out all the section headers.
		for ([[maybe_unused]] const auto index : substrate::indexSequence_t{_header.sectionHeaderCount()})
		{
			if (elfClass == Class::elf32Bit)
				_sectionHeaders.emplace_back
				(
					elf32::SectionHeader
					{
						data.subspan(offset, elf32::SectionHeader::size()),
						endian
					}
				);
			else
				_sectionHeaders.emplace_back
				(
					elf64::SectionHeader
					{
						data.subspan(offset, elf64::SectionHeader::size()),
						endian
					}
				);
			offset += sectionHeaderSize;
		}

		// Extract the section names
		const auto &sectionNamesHeader{_sectionHeaders[_header.sectionNamesIndex()]};
		_sectionNames = data.subspan(sectionNamesHeader.fileOffset(), sectionNamesHeader.fileLength());
	}

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
