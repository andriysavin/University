//////////////////////////////////////////////////////////////////////////
#include "BitArray.h"
//////////////////////////////////////////////////////////////////////////
CBitArray::CBitArray()
{
	m_dwSizeInBits = 0;
	m_dwSizeInBytes = 0;
	m_pBits = NULL;
}
//////////////////////////////////////////////////////////////////////////
CBitArray::CBitArray(DWORD dwSize)
{
	m_dwSizeInBits = dwSize;
	m_dwSizeInBytes = dwSize / 8 + ((dwSize % 8) ? 1 : 0);

	m_pBits = new uint8_t[m_dwSizeInBytes];

	memset(m_pBits, 0, m_dwSizeInBytes);
}
//////////////////////////////////////////////////////////////////////////
CBitArray::~CBitArray()
{
	delete[] m_pBits;
}
//////////////////////////////////////////////////////////////////////////
bool CBitArray::GetBit(DWORD dwPos) const
{
	if (dwPos >= m_dwSizeInBits) throw string("Bit position out of range");

	return (m_pBits[dwPos / 8] & (0x01 << (dwPos % 8))) != 0;
}
//////////////////////////////////////////////////////////////////////////
void CBitArray::SetBit(DWORD dwPos, bool blBit)
{
	if (dwPos >= m_dwSizeInBits) throw string("Bit position out of range");

	if (blBit)  m_pBits[dwPos / 8] |= (0x01 << (dwPos % 8));
	else         m_pBits[dwPos / 8] &= ~(0x01 << (dwPos % 8));
}
//////////////////////////////////////////////////////////////////////////
DWORD CBitArray::FindFirst(bool blVal, DWORD dwStartPos /* = 0 */) const
{
	if (dwStartPos >= m_dwSizeInBits) throw string("Bit position out of range");

	for (DWORD i = dwStartPos; i < m_dwSizeInBits; i++)
	{
		if (((m_pBits[i / 8] & (0x01 << (i % 8))) != 0) == blVal) return i;
	}

	// no bits with the specified value were found
	return m_dwSizeInBits;
}
//////////////////////////////////////////////////////////////////////////
void CBitArray::SaveToStream(ostream& out) const
{
	out.write((char*)&m_dwSizeInBits, sizeof(DWORD));
	out.write((char*)m_pBits, m_dwSizeInBytes);
}
//////////////////////////////////////////////////////////////////////////
void CBitArray::LoadFromStream(istream& in)
{
	if (m_pBits) delete[] m_pBits; m_pBits = NULL;

	in.read((char*)&m_dwSizeInBits, sizeof(DWORD));

	m_dwSizeInBytes = m_dwSizeInBits / 8 + ((m_dwSizeInBits % 8) ? 1 : 0);
	m_pBits = new uint8_t[m_dwSizeInBytes];

	in.read((char*)m_pBits, m_dwSizeInBytes);
}
//////////////////////////////////////////////////////////////////////////
