//////////////////////////////////////////////////////////////////////////
#ifndef __INODE_H__
#define __INODE_H__
//////////////////////////////////////////////////////////////////////////
#include <ostream>
#include <vector>
#include "Utils.h"
//////////////////////////////////////////////////////////////////////////
namespace NFileSystem
{
	struct ST_INODE
	{
		static const int MaxFileName = 255;

		void SaveToStream(ostream& out) const
		{
			out.write(sFileName, sizeof(sFileName));
			out.write((char*)&dwFileSize, sizeof(dwFileSize));
			out.write((char*)&dwNumOfFirstIndexBlock, sizeof(dwNumOfFirstIndexBlock));
			out.write((char*)&dwIndex, sizeof(dwIndex));
		}

		void LoadFromStream(istream& in)
		{
			in.read(sFileName, sizeof(sFileName));
			in.read((char*)&dwFileSize, sizeof(dwFileSize));
			in.read((char*)&dwNumOfFirstIndexBlock, sizeof(dwNumOfFirstIndexBlock));
			in.read((char*)&dwIndex, sizeof(dwIndex));
		}

		static DWORD GetDataSize()
		{
			return 
				sizeof(ST_INODE) - 
				sizeof arDataBlockIndices  -
				sizeof arIndexBlockIndices;
		}

		char    sFileName[MaxFileName + 1];
		DWORD   dwFileSize;
		DWORD   dwNumOfFirstIndexBlock;
		DWORD   dwIndex;

		// To store indices of blocks allocated 
		// to the file we use linked list of 
		// index data blocks. Each index block starts 
		// with an index of the next index block.
		// The rest of the block is indices of the
		// the file data blocks.
		// 
		// When we load an inode to memory, we also
		// load the index block indices and data block
		// indices. Thus we can easily manipulate that
		// information. In particular, we can trac which
		// datablocks are allocated to data and to indicies,
		// and decide when we need to allocate more blocks
		// of each type.
		vector<DWORD> arIndexBlockIndices;
		vector<DWORD> arDataBlockIndices;
	};
}
//////////////////////////////////////////////////////////////////////////
#endif // __INODE_H__
//////////////////////////////////////////////////////////////////////////


