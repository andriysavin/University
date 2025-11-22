//////////////////////////////////////////////////////////////////////////
#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__
//////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

#include "Utils.h"
#include "BitArray.h"
#include "INode.h"
#include "SuperBlock.h"
//////////////////////////////////////////////////////////////////////////
namespace NFileSystem
{
	class CFile;

	class CFileSystem
	{
	public:
		static const int MaxPartitionName = 255;
		static const int MaxFileName = 255;

		CFileSystem(void);
		~CFileSystem(void);

		friend CFile;

		static void CreatePartition(
			const string& sPartitionName,
			DWORD         dwDataBlockSize,
			DWORD         dwPartitionSize);

		void        MountPartition(const string& sPartitionName);

		CFile*		OpenFile(const string& sFileName);
		void        CreateFile(const string& sFileName);
		void        CloseFile(NFileSystem::CFile* pFile);
		bool        FileExists(const string& sFileName);
		void        DeleteFile(const string& sFileName);
		void        GetFileList(vector<string>* plstFileNames);

	protected:

		DWORD       ReadFile(ST_INODE* pINode, void* pData, DWORD dwSize, DWORD dwOffset);
		void        WriteFile(ST_INODE* pINode, const void* pData, DWORD dwSize, DWORD dwOffset);

	private:

		void        SaveINode(ST_INODE& INode, DWORD dwINodeIndex);
		void        LoadINode(ST_INODE& INode, DWORD dwINodeIndex);
		void        SeekToDataBlock(DWORD dwBlockNumber);

		void        WriteDataBlock(DWORD dwBlockInd, DWORD dwOffset, const void* pData, DWORD dwSize);
		void        ReadDataBlock(DWORD dwBlockInd, DWORD dwOffset, void* pData, DWORD dwSize);
		DWORD       AllocateDataBlock(ST_INODE& INode);
	private:
		static const DWORD TerminalIndex = 0xFFFFFFFF;

		ST_SUPERBLOCK m_sb;
		iostream* m_pFSStream;
	};
}
//////////////////////////////////////////////////////////////////////////
#endif // __FILE_SYSTEM_H__
//////////////////////////////////////////////////////////////////////////
