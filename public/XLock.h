#pragma once
class XLock
{
public:
	XLock(CRITICAL_SECTION  *Lock){m_Lock = Lock;EnterCriticalSection(m_Lock);}
		
	~XLock(void){LeaveCriticalSection(m_Lock);};
public:
	CRITICAL_SECTION  * m_Lock;
};

