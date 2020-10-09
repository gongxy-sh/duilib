//
//
// DirectUI - UI Library
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2006-2007 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. These
// source files may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
////
// Acknowledgements :
// Bjarke Viksoe (http://www.viksoe.dk/code/windowless1.htm)
//
//
//
// Beware of bugs.
//
//
//
////////////////////////////////////////////////////////
#ifndef __UIUTIL_H__
#define __UIUTIL_H__

#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma once
#endif

#define CP_UTF16LE  65535
#define CP_UNICODE  CP_UTF16LE

//CP_TEXT,T¥Æ±‡¬Î
//CP_ASCII,A¥Æ±‡¬Î
#ifdef _UNICODE
#define CP_TEXT  CP_UNICODE //T¥Æ±‡¬ÎUTF16LE
#define CP_ASCII CP_UTF8    //A¥Æ±‡¬ÎUTF8
#else //!_UNICODE
#define CP_TEXT  CP_ACP     //T¥Æ±‡¬ÎÕ¨µ±«∞LCID
#define CP_ASCII CP_ACP     //A¥Æ±‡¬ÎÕ¨µ±«∞LCID
#endif //!_UNICODE

class CUiStringA;
class CUiStringW;

class CUiStringA : public std::string
{
public:
  CUiStringA();
  CUiStringA(LPCSTR that);
  CUiStringA(const std::string& that);
  CUiStringA(const CUiStringA& that);

  void Format(LPCSTR IN szFormat,...);
  void FormatV(LPCSTR IN szFormat,va_list IN vp);

  void Assign(const CUiStringA& strText,UINT cpi,UINT cpo);
  void Assign(const CUiStringW& strText,UINT cpi,UINT cpo);
  void Assign(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo);
  void Assign(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo);
  
  size_t Replace(char IN cOld,char IN cNew);
  size_t Replace(LPCSTR IN szOld,LPCSTR IN szNew);

  CUiStringA& operator=(char that);
  CUiStringA& operator=(LPCSTR that);
  CUiStringA& operator=(const std::string& that);
  CUiStringA& operator=(const CUiStringA& that);

  CUiStringA operator+(char that);
  CUiStringA operator+(LPCSTR that);
  CUiStringA operator+(const CUiStringA& that);

#if _MSC_VER <= 1200 
  friend CUiStringA operator+(LPCSTR s0,const CUiStringA& s1);
#endif //_MSC_VER
};

class CUiStringW : public std::wstring
{
public:
  CUiStringW();
  CUiStringW(LPCWSTR that);
  CUiStringW(const std::wstring& that);
  CUiStringW(const CUiStringW& that);

  void Format(LPCWSTR IN szFormat,...);
  void FormatV(LPCWSTR IN szFormat,va_list IN vp);

  void Assign(const CUiStringA& strText,UINT cpi,UINT cpo);
  void Assign(const CUiStringW& strText,UINT cpi,UINT cpo);
  void Assign(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo);
  void Assign(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo);
    
  size_t Replace(WCHAR IN cOld,WCHAR IN cNew);
  size_t Replace(LPCWSTR IN szOld,LPCWSTR IN szNew);

  CUiStringW& operator=(WCHAR that);
  CUiStringW& operator=(LPCWSTR that);
  CUiStringW& operator=(const std::wstring& that);
  CUiStringW& operator=(const CUiStringW& that);

  CUiStringW operator+(WCHAR that);
  CUiStringW operator+(LPCWSTR that);
  CUiStringW operator+(const CUiStringW& that);

#if _MSC_VER <= 1200 
  friend CUiStringW operator+(LPCSTR s0,const CUiStringW& s1);
#endif //_MSC_VER
};

#if _MSC_VER <= 1200
CUiStringA operator+(LPCSTR s0,const CUiStringA& s1);
CUiStringW operator+(LPCWSTR s0,const CUiStringW& s1);
#endif //_MSC_VER

//»Œ∫Œ¥Æ◊™A¥Æ
class CDuiX2A
{
public:
  CDuiX2A(void);
  CDuiX2A(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo);
  CDuiX2A(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo);

  CDuiX2A(const CUiStringA& strText,UINT cpi,UINT cpo);
  CDuiX2A(const CUiStringW& strText,UINT cpi,UINT cpo);
  CDuiX2A(BSTR strText,UINT cpi,UINT cpo);

  void Assign(const CUiStringA& strText,UINT cpi,UINT cpo);
  void Assign(const CUiStringW& strText,UINT cpi,UINT cpo);
  void Assign(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo);
  void Assign(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo);

  __forceinline operator LPCSTR() const
  {
	return m_szText;
  }
  __forceinline LPCSTR c_str() const
  {
	return m_szText;
  }
  __forceinline size_t length() const
  {
	return m_cbText;
  }

protected:  
  size_t      m_cbText;
  LPCSTR      m_szText;
  CUiStringA m_strText;
};

//»Œ∫Œ¥Æ◊™W¥Æ
class CDuiX2W
{
public:
  CDuiX2W(void);
  CDuiX2W(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo);
  CDuiX2W(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo);

  CDuiX2W(const CUiStringA& strText,UINT cpi,UINT cpo);
  CDuiX2W(const CUiStringW& strText,UINT cpi,UINT cpo);
  CDuiX2W(BSTR strText,UINT cpi,UINT cpo);

  void Assign(const CUiStringA& strText,UINT cpi,UINT cpo);
  void Assign(const CUiStringW& strText,UINT cpi,UINT cpo);
  void Assign(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo);
  void Assign(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo);
 
 __forceinline operator LPCWSTR() const
  {
	return m_szText;
  }
  __forceinline LPCWSTR c_str() const
  {
	return m_szText;
  }
  __forceinline size_t length() const
  {
	return m_cbText;
  }
protected:
  size_t      m_cbText;
  LPCWSTR     m_szText;
  CUiStringW m_strText;
};

#ifdef _UNICODE

//A¥Æ◊™T¥Æ
class CDuiA2T : public CDuiX2W
{
public:
  CDuiA2T(void)
  {
  }
  CDuiA2T(LPCSTR szText,size_t cbText = -1) : CDuiX2W(szText,cbText,CP_ASCII,CP_TEXT)
  {
  }
  CDuiA2T(const CUiStringA& strText) : CDuiX2W(strText,CP_ASCII,CP_TEXT)
  {
  }
};

//W¥Æ◊™T¥Æ
class CDuiW2T : public CDuiX2W
{
public:
  CDuiW2T(void)
  {
  }
  CDuiW2T(LPCWSTR szText,size_t cbText = -1) : CDuiX2W(szText,cbText,CP_UNICODE,CP_TEXT)
  {
  }
  CDuiW2T(const CUiStringW& strText) : CDuiX2W(strText.c_str(),strText.length(),CP_UNICODE,CP_TEXT)
  {
  }
  CDuiW2T(BSTR strText) : CDuiX2W(strText,SysStringLen(strText),CP_UNICODE,CP_TEXT)
  {
  }
};

//T¥Æ◊™A¥Æ
class CDuiT2A : public CDuiX2A
{
public:
  CDuiT2A(void)
  {
  }
  CDuiT2A(LPCWSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_TEXT,CP_ASCII)
  {
  }
  CDuiT2A(const CUiStringW& strText) : CDuiX2A(strText.c_str(),strText.length(),CP_TEXT,CP_ASCII)
  {
  }
  CDuiT2A(BSTR strText) : CDuiX2A(strText,SysStringLen(strText),CP_TEXT,CP_ASCII)
  {
  }
};

//T¥Æ◊™W¥Æ
class CDuiT2W : public CDuiX2W
{
public:
  CDuiT2W(void)
  {
  }
  CDuiT2W(LPCWSTR szText,size_t cbText = -1) : CDuiX2W(szText,cbText,CP_TEXT,CP_UNICODE)
  {
  }
  CDuiT2W(const CUiStringW& strText) : CDuiX2W(strText.c_str(),strText.length(),CP_TEXT,CP_UNICODE)
  {
  }
  CDuiT2W(BSTR strText) : CDuiX2W(strText,SysStringLen(strText),CP_TEXT,CP_UNICODE)
  {
  }
};

//U¥Æ◊™T¥Æ
class CDuiU2T : public CDuiX2W
{
public:
  CDuiU2T(void)
  {
  }
  CDuiU2T(LPCSTR szText,size_t cbText = -1) : CDuiX2W(szText,cbText,CP_UTF8,CP_UNICODE)
  {
  }
  CDuiU2T(const CUiStringA& strText) : CDuiX2W(strText.c_str(),strText.length(),CP_UTF8,CP_UNICODE)
  {
  }
  CDuiU2T(const CString& strText) : CDuiX2W(strText,strText.GetLength(),CP_UTF8,CP_UNICODE)
  {
  }
};

//T¥Æ◊™U¥Æ
class CDuiT2U : public CDuiX2A
{
public:
  CDuiT2U(void)
  {
  }
  CDuiT2U(LPCTSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_UNICODE,CP_UTF8)
  {
  }
  CDuiT2U(const CDuiString& strText) : CDuiX2A(strText.GetData(),strText.GetLength(),CP_UNICODE,CP_UTF8)
  {
  }
};

//U¥Æ◊™W¥Æ
class CDuiU2W : public CDuiX2W
{
public:
  CDuiU2W(void)
  {
  }
  CDuiU2W(LPCSTR szText,size_t cbText = -1) : CDuiX2W(szText,cbText,CP_UTF8,CP_UNICODE)
  {
  }
  CDuiU2W(const CUiStringA& strText) : CDuiX2W(strText.c_str(),strText.length(),CP_UTF8,CP_UNICODE)
  {
  }
};

//T¥Æ◊™CP_ACP A¥Æ
class CDuiT2ACP : public CDuiX2A
{
public:
  CDuiT2ACP(void)
  {
  }
  CDuiT2ACP(LPCTSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_TEXT,CP_ACP)
  {
  }
  CDuiT2ACP(const CDuiString& strText) : CDuiX2A(strText.GetData(),strText.GetLength(),CP_TEXT,CP_ACP)
  {
  }
};

#else //!_UNICODE

//A¥Æ◊™T¥Æ
class CDuiA2T : public CDuiX2A
{
public:
  CDuiA2T(void)
  {
  }
  CDuiA2T(LPCSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_ASCII,CP_TEXT)
  {
  }
  CDuiA2T(const CUiStringA& strText) : CDuiX2A(strText.c_str(),strText.length(),CP_ASCII,CP_TEXT)
  {
  }
};

//W¥Æ◊™T¥Æ
class CDuiW2T : public CDuiX2A
{
public:
  CDuiW2T(void)
  {
  }
  CDuiW2T(LPCWSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_UNICODE,CP_TEXT)
  {
  }
  CDuiW2T(const CUiStringW& strText) : CDuiX2A(strText.c_str(),strText.length(),CP_UNICODE,CP_TEXT)
  {
  }
  CDuiW2T(BSTR strText) : CDuiX2A(strText,SysStringLen(strText),CP_UNICODE,CP_TEXT)
  {
  }
};

//T¥Æ◊™A¥Æ
class CDuiT2A : public CDuiX2A
{
public:
  CDuiT2A(void)
  {
  }
  CDuiT2A(LPCTSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_TEXT,CP_ASCII)
  {
  }
  CDuiT2A(const CDuiString& strText) : CDuiX2A(strText.GetData(),strText.GetLength(),CP_TEXT,CP_ASCII)
  {
  }
  CDuiT2A(const CString& strText) : CDuiX2A(strText,strText.GetLength(),CP_TEXT,CP_ASCII)
  {
  }
};

//T¥Æ◊™W¥Æ
class CDuiT2W : public CDuiX2W
{
public:
  CDuiT2W(void)
  {
  }
  CDuiT2W(LPCTSTR szText,size_t cbText = -1) : CDuiX2W(szText,cbText,CP_TEXT,CP_UNICODE)
  {
  }
  CDuiT2W(const CDuiString& strText) : CDuiX2W(strText.GetData(),strText.GetLength(),CP_TEXT,CP_UNICODE)
  {
  }
};

//U¥Æ◊™T¥Æ
class CDuiU2T : public CDuiX2A
{
public:
  CDuiU2T(void)
  {
  }
  CDuiU2T(LPCSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_UTF8,CP_ASCII)
  {
  }
  CDuiU2T(const CUiStringA& strText) : CDuiX2A(strText.c_str(),strText.length(),CP_UTF8,CP_ASCII)
  {
  }
};

//T¥Æ◊™U¥Æ
class CDuiT2U : public CDuiX2A
{
public:
  CDuiT2U(void)
  {
  }
  CDuiT2U(LPCTSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_UTF8,CP_ASCII)
  {
  }
  CDuiT2U(const CDuiString& strText) : CDuiX2A(strText.GetData(),strText.GetLength(),CP_UTF8,CP_ASCII)
  {
  }
  CDuiT2U(const CString& strText) : CDuiX2A(strText,strText.GetLength(),CP_UTF8,CP_ASCII)
  {
  }
};

//U¥Æ◊™W¥Æ
class CDuiU2W : public CDuiX2W
{
public:
  CDuiU2W(void)
  {
  }
  CDuiU2W(LPCSTR szText,size_t cbText = -1) : CDuiX2W(szText,cbText,CP_UTF8,CP_UNICODE)
  {
  }
  CDuiU2W(const CUiStringA& strText) : CDuiX2W(strText.c_str(),strText.length(),CP_UTF8,CP_UNICODE)
  {
  }
};

//T¥Æ◊™CP_ACP A¥Æ
class CDuiT2ACP : public CDuiX2A
{
public:
  CDuiT2ACP(void)
  {
  }
  CDuiT2ACP(LPCTSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_TEXT,CP_ACP)
  {
  }
  CDuiT2ACP(const CDuiString& strText) : CDuiX2A(strText.GetData(),strText.GetLength(),CP_TEXT,CP_ACP)
  {
  }
};

#endif //!_UNICODE

//A¥Æ◊™W¥Æ
class CDuiA2W : public CDuiX2W
{
public:
  CDuiA2W(void)
  {
  }
  CDuiA2W(LPCSTR szText,size_t cbText = -1) : CDuiX2W(szText,cbText,CP_ASCII,CP_UNICODE)
  {
  }
  CDuiA2W(const CUiStringA& strText) : CDuiX2W(strText.c_str(),strText.length(),CP_ASCII,CP_UNICODE)
  {
  }
};

//W¥Æ◊™A¥Æ
class CDuiW2A : public CDuiX2A
{
public:
  CDuiW2A(void)
  {
  }
  CDuiW2A(LPCWSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_UNICODE,CP_ASCII)
  {
  }
  CDuiW2A(const CUiStringW& strText) : CDuiX2A(strText.c_str(),strText.length(),CP_UNICODE,CP_ASCII)
  {
  }
  CDuiW2A(BSTR strText) : CDuiX2A(strText,SysStringLen(strText),CP_UNICODE,CP_ASCII)
  {
  }
};

//A¥Æ◊™A¥Æ
class CDuiA2A : public CDuiX2A
{
public:
  CDuiA2A(void)
  {
  }
  CDuiA2A(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo) : CDuiX2A(szText,cbText,cpi,cpo)
  {
  }
  CDuiA2A(const CUiStringA& strText,UINT cpi,UINT cpo) : CDuiX2A(strText.c_str(),strText.length(),cpi,cpo)
  {
  }
};

//U¥Æ◊™A¥Æ
class CDuiU2A : public CDuiA2A
{
public:
  CDuiU2A(void)
  {
  }
  CDuiU2A(LPCSTR szText,size_t cbText = -1) : CDuiA2A(szText,cbText,CP_UTF8,CP_ASCII)
  {
  }
  CDuiU2A(const CUiStringA& strText) : CDuiA2A(strText.c_str(),strText.length(),CP_UTF8,CP_ASCII)
  {
  }
};

//A¥Æ◊™U¥Æ
class CDuiA2U : public CDuiX2A
{
public:
  CDuiA2U(void)
  {
  }
  CDuiA2U(LPCSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_ASCII,CP_UTF8)
  {
  }
  CDuiA2U(const CUiStringA& strText) : CDuiX2A(strText.c_str(),strText.length(),CP_ASCII,CP_UTF8)
  {
  }
  CDuiA2U(const CString& strText) : CDuiX2A(strText,strText.GetLength(),CP_ASCII,CP_UTF8)
  {
  }
};

//W¥Æ◊™U¥Æ
class CDuiW2U : public CDuiX2A
{
public:
  CDuiW2U(void)
  {
  }
  CDuiW2U(LPCWSTR szText,size_t cbText = -1) : CDuiX2A(szText,cbText,CP_UNICODE,CP_UTF8)
  {
  }
};

#endif //__UIUTIL_H__
