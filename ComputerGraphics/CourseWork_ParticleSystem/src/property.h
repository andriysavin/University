#pragma once

template<class OuterType,class ValType>
class property
{
public:
	typedef ValType (OuterType::*PGET)(const ValType&/* Val*/);
	typedef void (OuterType::*PSET)(ValType&/* ValToSet*/,ValType/* Val*/);

	property(const property<OuterType,ValType>& refProp)
	{
		*this = ((ValType)refProp);
	}

	property() : 
		m_Val(),
		m_pGet(NULL),
		m_pSet(NULL),
		m_pThis(NULL)
	{
	}

	void Init(PGET pGet,PSET pSet,OuterType* pThis )
	{
		m_pGet	= pGet;
		m_pSet	= pSet;
		m_pThis = pThis;
	}

	operator ValType()
	{
		return (m_pThis->*m_pGet)(m_Val);
	}

	property<OuterType,ValType>&  operator = (const property<OuterType,ValType>& refProp)
	{
		*this = (refProp.m_pThis->*m_pGet)(refProp.m_Val);
		return *this;
	}

	property<OuterType,ValType>* operator &()
	{
		return NULL;
	}

	ValType& operator = (const ValType& Val)
	{
		(m_pThis->*m_pSet)(m_Val,Val);
		return m_Val;
	}


private:
	ValType		m_Val;
	PGET		m_pGet;
	PSET		m_pSet;
	OuterType*	m_pThis ;
};
