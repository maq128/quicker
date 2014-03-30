// HKHandler.h : interface of the CHKHandler class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(_HKHandler_H_)
#define _HKHandler_H_

#pragma once

class CHKHandlerBase
{
    static int seq;

public:
    CHKHandlerBase();
    ATOM m_hkID;
    UINT m_hkMod;
    UINT m_hkVK;

    BOOL parseHotKeySpec( LPCTSTR str );
    virtual void Process() = 0;
};

class CHKManager : public CMessageFilter
{
protected:
    CSimpleArray<CHKHandlerBase*> m_handlerList;

public:
    void RegisterAll();
    void UnregisterAll();
    BOOL PreTranslateMessage(MSG* pMsg);
};

#endif // !defined(_HKHandler_H_)
