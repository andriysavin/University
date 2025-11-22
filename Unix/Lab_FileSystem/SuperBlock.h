//////////////////////////////////////////////////////////////////////////
#ifndef __SUPERBLOCK_H__
#define __SUPERBLOCK_H__
//////////////////////////////////////////////////////////////////////////
#include "Utils.h"
#include "BitArray.h"
#include "INode.h"
//////////////////////////////////////////////////////////////////////////
namespace NFileSystem
{
	struct ST_SUPERBLOCK
	{
		ST_SUPERBLOCK()
		{
			pINodesMap = new CBitArray();
			pBlockMap = new CBitArray();

			dwDataBlockSize = 0;
			dwPartitionSize = 0;
		}

		ST_SUPERBLOCK(DWORD dwPartitionSize, DWORD dwDataBlockSize)
		{
			if (dwPartitionSize == 0) throw(invalid_argument("Invalid partition size"));

			// align the data block size on a 32-bit boundary
			dwDataBlockSize = (dwDataBlockSize / sizeof(DWORD)) * sizeof(DWORD)
				+ ((dwDataBlockSize % sizeof(DWORD)) ? sizeof(DWORD) : 0);

			// round the partition size to the data block size
			dwPartitionSize = (dwPartitionSize / dwDataBlockSize) * dwDataBlockSize
				+ ((dwPartitionSize % dwDataBlockSize) ? dwDataBlockSize : 0);

			// the maximum number of files is equal to the number of free blocks
			DWORD dwNumINodes = dwPartitionSize / dwDataBlockSize;

			pINodesMap = new CBitArray(dwNumINodes);
			pBlockMap = new CBitArray(dwPartitionSize / dwDataBlockSize);

			this->dwDataBlockSize = dwDataBlockSize;
			this->dwPartitionDataSize = dwPartitionSize;

			DWORD dwINodesSize = dwNumINodes * ST_INODE::GetDataSize();

			this->dwPartitionSize = dwPartitionDataSize + GetDataSize() + dwINodesSize;

			dwINodesOffset = GetDataSize();
			dwDataBlocksOffset = dwINodesOffset + dwINodesSize;
		}

		~ST_SUPERBLOCK()
		{
			delete      pINodesMap;
			delete      pBlockMap;
		}

		void SaveToStream(ostream& out) const
		{
			pINodesMap->SaveToStream(out);
			pBlockMap->SaveToStream(out);
			out.write((char*)&dwPartitionSize, sizeof(dwPartitionSize));
			out.write((char*)&dwPartitionDataSize, sizeof(dwPartitionDataSize));
			out.write((char*)&dwDataBlockSize, sizeof(dwDataBlockSize));
			out.write((char*)&dwINodesOffset, sizeof(dwINodesOffset));
			out.write((char*)&dwDataBlocksOffset, sizeof(dwDataBlocksOffset));
		}

		void LoadFromStream(istream& in)
		{
			pINodesMap->LoadFromStream(in);
			pBlockMap->LoadFromStream(in);
			in.read((char*)&dwPartitionSize, sizeof(dwPartitionSize));
			in.read((char*)&dwPartitionDataSize, sizeof(dwPartitionDataSize));
			in.read((char*)&dwDataBlockSize, sizeof(dwDataBlockSize));
			in.read((char*)&dwINodesOffset, sizeof(dwINodesOffset));
			in.read((char*)&dwDataBlocksOffset, sizeof(dwDataBlocksOffset));
		}

		DWORD GetDataSize() const
		{
			return
				pINodesMap->GetDataSize() +
				pBlockMap->GetDataSize() +
				sizeof(dwPartitionSize) +
				sizeof(dwDataBlockSize) +
				sizeof(dwINodesOffset) +
				sizeof(dwDataBlocksOffset) +
				sizeof(dwPartitionDataSize);
		}

		CBitArray* pINodesMap;
		CBitArray* pBlockMap;

		DWORD      dwPartitionSize;
		DWORD      dwPartitionDataSize;
		DWORD      dwDataBlockSize;

		DWORD      dwINodesOffset;
		DWORD      dwDataBlocksOffset;
	};
}
//////////////////////////////////////////////////////////////////////////
#endif // __SUPERBLOCK_H__
//////////////////////////////////////////////////////////////////////////


