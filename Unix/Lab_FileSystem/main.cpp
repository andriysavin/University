//////////////////////////////////////////////////////////////////////////
#include "File.h"
//////////////////////////////////////////////////////////////////////////
using namespace NFileSystem;
//////////////////////////////////////////////////////////////////////////
void ReadSourceFile(long &lTextSize, char SourceTextBuffer[]);
//////////////////////////////////////////////////////////////////////////
int main()
{
	// Ensure the buffer is big enough to hold
	// whole file content.
	const int SourceTextBufferSize = 300000;
	char SourceTextBuffer[SourceTextBufferSize];

	long lTextSize;

    ReadSourceFile(lTextSize, SourceTextBuffer);

    try
	{
		const DWORD DataBlockSize = 100;
		const DWORD PartitionSize = 10000000L;

		CFileSystem::CreatePartition(
			"FileSysPartition",
			DataBlockSize,
			PartitionSize);

		CFileSystem *fs = new CFileSystem();

		fs->MountPartition("FileSysPartition");

		if (fs->FileExists("File1.txt"))
			fs->DeleteFile("File1.txt");
		if (fs->FileExists("File2.txt"))
			fs->DeleteFile("File2.txt");

		fs->CreateFile("File1.txt");
		CFile *pFile1 = fs->OpenFile("File1.txt");

		fs->CreateFile("File2.txt");
		CFile *pFile2 = fs->OpenFile("File2.txt");

		const int CopyBlockSize = 1000;

		long lPartialBlockSize = lTextSize % CopyBlockSize;
		long lFullBlocksTotalSize = lTextSize - lPartialBlockSize;

		// Write to two files in turn by blocks
		// to simulate concurrent file system use
		// and test that blocks are allocated
		// correctly for each file.
		// First write all blocks full blocks.
		for (long offset = 0; offset < lFullBlocksTotalSize; offset += CopyBlockSize)
		{
			pFile1->Write(&SourceTextBuffer[offset], CopyBlockSize);
			pFile2->Write(&SourceTextBuffer[offset], CopyBlockSize);
		}

		// Now write the leftover which is less than the block size.
		pFile1->Write(&SourceTextBuffer[lTextSize - lPartialBlockSize], lPartialBlockSize);
		pFile2->Write(&SourceTextBuffer[lTextSize - lPartialBlockSize], lPartialBlockSize);

		fs->CloseFile(pFile1);
		fs->CloseFile(pFile2);

		delete fs;

		fs = new CFileSystem();

		fs->MountPartition("FileSysPartition");

		pFile1 = fs->OpenFile("File1.txt");
		pFile2 = fs->OpenFile("File2.txt");

		// Now copy the files from our virtual file system
		// to the real file system so we can
		// check if they are not corrupted and have the same content
		// as original file.
		ofstream out1("out1.txt", ios::binary);
		ofstream out2("out2.txt", ios::binary);
	
		memset(SourceTextBuffer, 0, SourceTextBufferSize);
		pFile1->Read(SourceTextBuffer, pFile1->Size());
		out1.write(SourceTextBuffer, pFile1->Size());

		memset(SourceTextBuffer, 0, SourceTextBufferSize);
		pFile2->Read(SourceTextBuffer, pFile2->Size());
		out2.write(SourceTextBuffer, pFile2->Size());

		fs->CloseFile(pFile1);
		fs->CloseFile(pFile2);

		// Also write files list to the output.
		vector<string> vFileList;
		fs->GetFileList(&vFileList);

		cout << "File list:" << endl;
		for (DWORD i = 0; i < vFileList.size(); i++)
		{
			cout << vFileList[i] << endl;
		}

		delete fs;
	}
	catch (string &e)
	{
		cout << e << endl;
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
void ReadSourceFile(long &lTextSize, char SourceTextBuffer[])
{
    ifstream in;
    in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    in.open("assets/Alice in Wonderland.txt", ios::binary);
    in.seekg(0, ios::end);
    lTextSize = in.tellg();
    in.seekg(0, ios::beg);
    in.read(SourceTextBuffer, lTextSize);
    in.close();
}
//////////////////////////////////////////////////////////////////////////