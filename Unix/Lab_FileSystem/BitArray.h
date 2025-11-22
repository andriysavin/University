//////////////////////////////////////////////////////////////////////////
#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__
//////////////////////////////////////////////////////////////////////////
#include "Utils.h"
#include <iostream>
using namespace std;
//////////////////////////////////////////////////////////////////////////
class CBitArray
{
public:
	CBitArray();

	CBitArray(DWORD dwSize);

	~CBitArray();

	bool    GetBit(DWORD dwPos) const;
	void    SetBit(DWORD dwPos, bool blBit);

	DWORD   GetSizeInBits() const { return m_dwSizeInBits; }
	DWORD   GetSizeInBytes() const { return m_dwSizeInBytes; }

	DWORD   GetDataSize() const {
		return m_dwSizeInBytes +
			sizeof(m_dwSizeInBits) + sizeof(m_dwSizeInBytes);
	}

	DWORD   FindFirst(bool blVal, DWORD dwStartPos = 0) const;

	void    SaveToStream(ostream& out) const;
	void    LoadFromStream(istream& in);

private:
	uint8_t* m_pBits;
	DWORD   m_dwSizeInBits;
	DWORD   m_dwSizeInBytes;
};
//////////////////////////////////////////////////////////////////////////
#endif //__BIT_ARRAY_H__
//////////////////////////////////////////////////////////////////////////



