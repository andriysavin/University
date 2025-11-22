//////////////////////////////////////////////////////////////////////////
#include "Utils.h"
//////////////////////////////////////////////////////////////////////////
bool CreateSizedFile(const string& sName, DWORD dwSize)
{
	ofstream f(sName.c_str(), ios::out | ios::binary);

	if (!f) return false;

	const int BufferSize = 1024;

	uint8_t* pTrash = new uint8_t[BufferSize];
	memset(pTrash, 0, BufferSize);

	// Write whole buffer blocks.
	for (DWORD i = 0; i < dwSize / BufferSize; i++)
	{
		if (!f.write((char*)pTrash, BufferSize))
		{
			f.close();
			remove(sName.c_str());
			delete[] pTrash;
			return false;
		}
		f.flush();
	}

	// Write leftover partial buffer block.
	if (!f.write((char*)pTrash, dwSize % BufferSize))
	{
		f.close();
		remove(sName.c_str());
		delete[] pTrash;
		return false;
	}

	f.close();
	delete[] pTrash;
	return true;
}
//////////////////////////////////////////////////////////////////////////
