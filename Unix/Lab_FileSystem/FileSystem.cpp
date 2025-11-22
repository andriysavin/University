//////////////////////////////////////////////////////////////////////////
#include "File.h"
#include "FileSystem.h"
//////////////////////////////////////////////////////////////////////////
using namespace NFileSystem;
//////////////////////////////////////////////////////////////////////////
CFileSystem::CFileSystem(void)
{
	m_pFSStream = NULL;
}
//////////////////////////////////////////////////////////////////////////
CFileSystem::~CFileSystem(void)
{
	m_pFSStream->seekp(0, ios::beg);
	m_sb.SaveToStream(*m_pFSStream);
	delete m_pFSStream;
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::CreatePartition(
	const string& sPartitionName,
	DWORD  dwDataBlockSize,
	DWORD  dwPartitionSize)
{
	string sName = sPartitionName;
	if (sName.length() > MaxPartitionName) sName.resize(MaxPartitionName);

	// round the partition size to the data block size
	dwPartitionSize = (dwPartitionSize / dwDataBlockSize) * dwDataBlockSize
		+ ((dwPartitionSize % dwDataBlockSize) ? dwDataBlockSize : 0);

	ST_SUPERBLOCK sb(dwPartitionSize, dwDataBlockSize);

	if (!CreateSizedFile(sName, sb.dwPartitionSize))
	{
		throw(string("Error creating partition ") + sPartitionName);
	}

	fstream out(sName.c_str(), ios::binary | ios::out | ios::in | ios::ate);
	if (!out) throw(string("Error creating partition ") + sPartitionName);

	out.exceptions(ios::badbit | ios::eofbit);

	out.seekp(0, ios::beg);

	sb.SaveToStream(out);

	out.close();
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::SaveINode(ST_INODE& INode, DWORD dwINodeIndex)
{
	if (dwINodeIndex >= m_sb.pINodesMap->GetSizeInBits())
		throw string("INode index out of range");

	m_pFSStream->seekp(m_sb.dwINodesOffset +
		dwINodeIndex * INode.GetDataSize(), ios::beg);

	INode.SaveToStream(*m_pFSStream);

	// number of indices in a block, the first index is occupied by the next block number
	DWORD dwNumbersPerBlock = m_sb.dwDataBlockSize / sizeof(DWORD) - 1;
	DWORD dwIndex = INode.dwNumOfFirstIndexBlock;

	DWORD dwTemp;

	for (DWORD i = 0; i < INode.arIndexBlockIndices.size(); i++)
	{
		// move on to the block
		SeekToDataBlock(INode.arIndexBlockIndices[i]);

		// save the number of the next block with indices
		if ((i + 1) < INode.arIndexBlockIndices.size())
		{
			m_pFSStream->write((char*)&INode.arIndexBlockIndices[i + 1], sizeof(dwIndex));
		}
		else
		{
			dwTemp = TerminalIndex;
			m_pFSStream->write((char*)&dwTemp, sizeof(dwTemp));
		}

		// save the numbers of blocks allocated to the file
		for (DWORD j = 0; j < dwNumbersPerBlock; j++)
		{
			if ((j + i * dwNumbersPerBlock) < INode.arDataBlockIndices.size())
			{
				dwTemp = INode.arDataBlockIndices[j + i * dwNumbersPerBlock];
				m_pFSStream->write((char*)&dwTemp, sizeof(dwTemp));
			}
			else
			{
				dwTemp = TerminalIndex;
				m_pFSStream->write((char*)&dwTemp, sizeof(dwTemp));
				break;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::LoadINode(ST_INODE& INode, DWORD dwINodeIndex)
{
	if (dwINodeIndex >= m_sb.pINodesMap->GetSizeInBits())
		throw string("INode index out of range");

	m_pFSStream->seekg(m_sb.dwINodesOffset +
		dwINodeIndex * INode.GetDataSize(), ios::beg);

	INode.LoadFromStream(*m_pFSStream);

	// number of indices in a block, the first index is occupied by the next block number
	DWORD dwNumbersPerBlock = m_sb.dwDataBlockSize / sizeof(DWORD) - 1;
	DWORD dwIndexBlockNum = INode.dwNumOfFirstIndexBlock;

	DWORD dwTemp;

	INode.arIndexBlockIndices.clear();
	INode.arDataBlockIndices.clear();

	while (dwIndexBlockNum != TerminalIndex)
	{
		// move on to the block
		SeekToDataBlock(dwIndexBlockNum);

		// store the block number with indices, we will need it when saving
		INode.arIndexBlockIndices.push_back(dwIndexBlockNum);

		// read the number of the next block with indices
		m_pFSStream->read((char*)&dwIndexBlockNum, sizeof(dwIndexBlockNum));

		// read and add to the array the numbers of blocks allocated to the file
		for (DWORD j = 0; j < dwNumbersPerBlock; j++)
		{
			m_pFSStream->read((char*)&dwTemp, sizeof(dwTemp));

			// If the index read is the last one, exit
			if (dwTemp == TerminalIndex) break;

			INode.arDataBlockIndices.push_back(dwTemp);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
DWORD CFileSystem::AllocateDataBlock(ST_INODE& INode)
{
	// Since we're allocating new block, we're also storing its (new) index.
	// Thus, we need to ensure we have storage for the index as well.
	// If the last block which stores indices is full (that is, the size in bytes 
	// of data block indices + index block indices is exactly factor of block size), 
	// allocate a new one. We add index block indices size because in each index block
	// we not only store data block indices, but also an index of the next index block.
	if ((INode.arDataBlockIndices.size() * sizeof(DWORD) + 
		INode.arIndexBlockIndices.size() * sizeof(DWORD)) % m_sb.dwDataBlockSize == 0)
	{
		DWORD dwFreeIndexBlockPos = m_sb.pBlockMap->FindFirst(false);

		// check if a free block was found
		if (dwFreeIndexBlockPos == m_sb.pBlockMap->GetSizeInBits())
			throw(string("There is no free data block"));

		// Mark the block as allocated, but don't write the index to the block.
		// Instead, add the index to the collection of indices. We store
		// something to blocks only when saving an inode to storage.
		m_sb.pBlockMap->SetBit(dwFreeIndexBlockPos, true);
		INode.arIndexBlockIndices.push_back(dwFreeIndexBlockPos);
	}

	DWORD dwFreeDataBlockPos = m_sb.pBlockMap->FindFirst(false);

	// check if a free block was found
	if (dwFreeDataBlockPos == m_sb.pBlockMap->GetSizeInBits())
		throw(string("There is no free data block"));

	// As in case with index blocks above, mar the block
	// as allocated and remember its index in the collection.
	m_sb.pBlockMap->SetBit(dwFreeDataBlockPos, true);
	INode.arDataBlockIndices.push_back(dwFreeDataBlockPos);

	// If this is the first time we allocate a block for this inode.
	if (INode.dwNumOfFirstIndexBlock == TerminalIndex) 
		INode.dwNumOfFirstIndexBlock = INode.arIndexBlockIndices[0];

	return dwFreeDataBlockPos;
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::MountPartition(const string& sPartitionName)
{
	string sName = sPartitionName;
	if (sName.length() > MaxPartitionName) sName.resize(MaxPartitionName);

	m_pFSStream = new fstream(sName.c_str(), ios::binary | ios::in | ios::ate | ios::out);

	if (!*m_pFSStream) throw(string("Cannot find partition with the specified name"));

	m_pFSStream->exceptions(ios::badbit | ios::eofbit);

	m_pFSStream->seekg(0, ios::beg);

	m_sb.LoadFromStream(*m_pFSStream);
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::CreateFile(const string& sFileName)
{
	if (FileExists(sFileName))
		throw(string("File ") + sFileName + " already exists");

	// looking for the first free INode
	DWORD dwFreeINodePos = m_sb.pINodesMap->FindFirst(false);

	// check if a free INode was found
	if (dwFreeINodePos == m_sb.pINodesMap->GetSizeInBits())
		throw(string("There is no free inode"));

	ST_INODE INode;

	INode.dwFileSize = 0;
	INode.dwNumOfFirstIndexBlock = TerminalIndex;
	INode.dwIndex = dwFreeINodePos;

	strncpy(
		INode.sFileName,
		sFileName.c_str(),
		ST_INODE::MaxFileName);

	// In case sFileName is longer than MaxFileName
	// replace last character with '\0'.
	INode.sFileName[ST_INODE::MaxFileName] = '\0';

	SaveINode(INode, dwFreeINodePos);

	// mark the INode as allocated
	m_sb.pINodesMap->SetBit(dwFreeINodePos, true);
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::SeekToDataBlock(DWORD dwBlockNumber)
{
	if (dwBlockNumber >= m_sb.pBlockMap->GetSizeInBits())
		throw string("Block index out of range");

	m_pFSStream->seekp(m_sb.dwDataBlocksOffset + dwBlockNumber * m_sb.dwDataBlockSize, ios::beg);
	m_pFSStream->seekg(m_sb.dwDataBlocksOffset + dwBlockNumber * m_sb.dwDataBlockSize, ios::beg);
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::WriteDataBlock(
	DWORD dwBlockInd,
	DWORD dwOffset,
	const void* pData,
	DWORD dwSize)
{
	if (dwOffset >= m_sb.dwDataBlockSize)
		throw(string("Offset is bigger or equal to block size"));

	if (dwSize > (m_sb.dwDataBlockSize - dwOffset))
		throw(string("Block data size out of range"));

	SeekToDataBlock(dwBlockInd);

	m_pFSStream->seekp(dwOffset, ios::cur);
	m_pFSStream->write((char*)pData, dwSize);
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::ReadDataBlock(
	DWORD dwBlockInd,
	DWORD dwOffset,
	void* pData,
	DWORD dwSize)
{
	if (dwOffset >= m_sb.dwDataBlockSize)
		throw(string("Offset is bigger or equal to block size"));

	if (dwSize > (m_sb.dwDataBlockSize - dwOffset))
		throw(string("Block data size out of range"));

	SeekToDataBlock(dwBlockInd);

	m_pFSStream->seekg(dwOffset, ios::cur);
	m_pFSStream->read((char*)pData, dwSize);
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::WriteFile(ST_INODE* pINode, const void* pData, DWORD dwSize, DWORD dwOffset)
{
	DWORD dwBeforEOFSize = MIN(pINode->dwFileSize - dwOffset, dwSize);
	DWORD dwAfterEOFSize = dwSize - dwBeforEOFSize;

	// Pre-allocate needed space after end of file if needed.
	if (dwAfterEOFSize)
	{
		DWORD dwNumBlocksToAlloc =
			dwAfterEOFSize / m_sb.dwDataBlockSize + ((dwAfterEOFSize % m_sb.dwDataBlockSize) ? 1 : 0);
		
		for (DWORD i = 0; i < dwNumBlocksToAlloc; i++)
		{
			AllocateDataBlock(*pINode);
		}

		pINode->dwFileSize += dwAfterEOFSize;
	}

	DWORD dwStartBlockNumInFile = dwOffset / m_sb.dwDataBlockSize;
	DWORD dwOffsetInStartBlock = dwOffset % m_sb.dwDataBlockSize;

	// First, write at the offset within the start block.
	if (dwOffsetInStartBlock)
	{
		DWORD dwSizeToWrite = MIN((m_sb.dwDataBlockSize - dwOffsetInStartBlock), dwSize);

		DWORD dwStartBlockNumInFileSys = pINode->arDataBlockIndices[dwStartBlockNumInFile];

		WriteDataBlock(dwStartBlockNumInFileSys, dwOffsetInStartBlock, pData, dwSizeToWrite);

		dwOffset += dwSizeToWrite;

		// if all the data fits into this block, exit
		if (dwSizeToWrite == dwSize) return;

		pData = ((uint8_t*)pData) + dwSizeToWrite;
		dwSize -= dwSizeToWrite;

		dwStartBlockNumInFile++;
	}

	// Now, write all data which fits in whole blocks.
	DWORD dwBlockNumInFile = dwStartBlockNumInFile;

	DWORD dwNumFullBlocks = dwSize / m_sb.dwDataBlockSize;
	DWORD dwNotFullBlockSize = dwSize % m_sb.dwDataBlockSize;

	for (DWORD i = 0; i < dwNumFullBlocks; i++, dwBlockNumInFile++)
	{
		DWORD dwBlockNumInSys = pINode->arDataBlockIndices[dwBlockNumInFile];

		WriteDataBlock(dwBlockNumInSys, 0, pData, m_sb.dwDataBlockSize);

		pData = ((uint8_t*)pData) + m_sb.dwDataBlockSize;

		dwOffset += m_sb.dwDataBlockSize;
	}

	// And finally, write the leftover which is smaller than block size.
	if (dwNotFullBlockSize)
	{
		DWORD dwBlockNumInSys = pINode->arDataBlockIndices[dwBlockNumInFile];

		WriteDataBlock(dwBlockNumInSys, 0, pData, dwNotFullBlockSize);

		dwOffset += dwNotFullBlockSize;
	}
}
//////////////////////////////////////////////////////////////////////////
DWORD CFileSystem::ReadFile(ST_INODE* pINode, void* pData, DWORD dwSize, DWORD dwOffset)
{
	DWORD dwSizeToRead = MIN((pINode->dwFileSize - dwOffset), dwSize);

	if (dwSizeToRead == 0) return 0;

	DWORD dwRetSizeRead = dwSizeToRead;

	DWORD dwStartBlockNumInFile = dwOffset / m_sb.dwDataBlockSize;
	DWORD dwOffsetInStartBlock = dwOffset % m_sb.dwDataBlockSize;

	if (dwOffsetInStartBlock)
	{
		DWORD dwStartSizeToRead = MIN((m_sb.dwDataBlockSize - dwOffsetInStartBlock), dwSizeToRead);

		DWORD dwStartBlockNumInSys = pINode->arDataBlockIndices[dwStartBlockNumInFile];

		ReadDataBlock(dwStartBlockNumInSys, dwOffsetInStartBlock, pData, dwStartSizeToRead);

		dwOffset += dwStartSizeToRead;

		// if all the data fits into this block, exit
		if (dwSizeToRead == dwStartSizeToRead) return dwStartSizeToRead;

		pData = ((uint8_t*)pData) + dwStartSizeToRead;
		dwSizeToRead -= dwStartSizeToRead;

		dwStartBlockNumInFile++;
	}

	DWORD dwBlockNumInFile = dwStartBlockNumInFile;

	DWORD dwNumFullBlocks = dwSizeToRead / m_sb.dwDataBlockSize;
	DWORD dwNotFullBlockSize = dwSizeToRead % m_sb.dwDataBlockSize;

	for (DWORD i = 0; i < dwNumFullBlocks; i++, dwBlockNumInFile++)
	{
		DWORD dwBlockNumInSys = pINode->arDataBlockIndices[dwBlockNumInFile];

		ReadDataBlock(dwBlockNumInSys, 0, pData, m_sb.dwDataBlockSize);

		pData = ((uint8_t*)pData) + m_sb.dwDataBlockSize;

		dwOffset += m_sb.dwDataBlockSize;
	}

	if (dwNotFullBlockSize)
	{
		DWORD dwBlockNumInSys = pINode->arDataBlockIndices[dwBlockNumInFile];

		ReadDataBlock(dwBlockNumInSys, 0, pData, dwNotFullBlockSize);

		dwOffset += dwNotFullBlockSize;
	}

	return dwRetSizeRead;
}
//////////////////////////////////////////////////////////////////////////
bool CFileSystem::FileExists(const string& sFileName)
{
	// Being lazy here...
	try
	{
		NFileSystem::CFile* pFile = OpenFile(sFileName);
		CloseFile(pFile);
		return true;
	}
	catch (...)
	{
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////
CFile* CFileSystem::OpenFile(const string& sFileName)
{
	string sTruncName = sFileName;
	if (sTruncName.size() > MaxFileName) sTruncName.resize(MaxFileName);

	ST_INODE* pInode = new ST_INODE;

	for (DWORD i = 0; i < m_sb.pINodesMap->GetSizeInBits(); i++)
	{
		if (m_sb.pINodesMap->GetBit(i))
		{
			LoadINode(*pInode, i);
			if (sTruncName == pInode->sFileName)
				return new CFile(pInode, this);
		}
	}

	delete pInode;
	throw(string("Can not find file ") + sFileName);
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::CloseFile(CFile* pFile)
{
	SaveINode(*pFile->m_pINode, pFile->m_pINode->dwIndex);

	delete pFile->m_pINode;
	delete pFile;
}
//////////////////////////////////////////////////////////////////////////
void CFileSystem::DeleteFile(const string& sFileName)
{
	string sTruncName = sFileName;
	if (sTruncName.size() > MaxFileName) sTruncName.resize(MaxFileName);

	ST_INODE Inode;

	for (DWORD i = 0; i < m_sb.pINodesMap->GetSizeInBits(); i++)
	{
		if (m_sb.pINodesMap->GetBit(i))
		{
			LoadINode(Inode, i);
			if (sTruncName == Inode.sFileName)
			{
				for (DWORD j = 0; j < Inode.arIndexBlockIndices.size(); j++)
				{
					m_sb.pBlockMap->SetBit(Inode.arIndexBlockIndices[j], false);
				}

				for (DWORD j = 0; j < Inode.arDataBlockIndices.size(); j++)
				{
					m_sb.pBlockMap->SetBit(Inode.arDataBlockIndices[j], false);
				}

				m_sb.pINodesMap->SetBit(i, false);

				return;
			}
		}
	}

	throw(string("Can not find file ") + sFileName);
}
//////////////////////////////////////////////////////////////////////////
void  CFileSystem::GetFileList(vector<string>* plstFileNames)
{
	plstFileNames->clear();

	ST_INODE Inode;

	for (DWORD i = 0; i < m_sb.pINodesMap->GetSizeInBits(); i++)
	{
		if (m_sb.pINodesMap->GetBit(i))
		{
			LoadINode(Inode, i);
			plstFileNames->push_back(Inode.sFileName);
		}
	}

	sort(plstFileNames->begin(), plstFileNames->end());
}
//////////////////////////////////////////////////////////////////////////

