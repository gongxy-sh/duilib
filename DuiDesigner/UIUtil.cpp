#include "stdafx.h"
#include "UIUtil.h"

//---CUiStringA
CUiStringA::CUiStringA()
{
}

CUiStringA::CUiStringA(LPCSTR that)
{
  assign(that);
}

CUiStringA::CUiStringA(const std::string& that)
{
  assign(that.c_str(),that.length());
}

CUiStringA::CUiStringA(const CUiStringA& that)
{
  assign(that.c_str(),that.length());
}

CUiStringA& CUiStringA::operator=(char that)
{
  assign(&that,1);
  return *this;
}

CUiStringA& CUiStringA::operator=(LPCSTR that)
{
  assign(that);
  return *this;
}

CUiStringA& CUiStringA::operator=(const std::string& that)
{
  assign(that.c_str(),that.length());
  return *this;
}

CUiStringA& CUiStringA::operator=(const CUiStringA& that)
{
  assign(that.c_str(),that.length());
  return *this;
}

CUiStringA CUiStringA::operator+(char that)
{
  CUiStringA strTemp(*this);

  strTemp.append(&that,1);
  return strTemp;
}

CUiStringA CUiStringA::operator+(LPCSTR that)
{
  CUiStringA strTemp(*this);

  strTemp.append(that);
  return strTemp;
}

CUiStringA CUiStringA::operator+(const CUiStringA& that)
{
  CUiStringA strTemp(*this);

  strTemp.append(that.c_str(),that.length());
  return strTemp;
}

#if _MSC_VER <= 1200
CUiStringA operator+(LPCSTR s0,const CUiStringA& s1)
{
  CUiStringA strTemp(s0);

  strTemp += s1;
  return strTemp;
}
#endif //_MSC_VER

#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000

void CUiStringA::FormatV(LPCSTR szFormat,va_list vp)
{
  va_list vpSave = vp;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCSTR lpsz = szFormat; *lpsz != '\0'; lpsz++)
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != _T('%') || *(++lpsz) == '%')
		{
			nMaxLen += strlen(lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; ++lpsz)
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(vp, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = atoi(lpsz);
			for (; *lpsz != '\0' && isdigit(*lpsz); ++lpsz)
				;
		}
		DEBUG_BREAK_IF(nWidth < 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			++lpsz;

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(vp, int);
				++lpsz;
			}
			else
			{
				nPrecision = atoi(lpsz);
				for (; *lpsz != '\0' && isdigit(*lpsz); ++lpsz)
					;
			}
			DEBUG_BREAK_IF(nPrecision < 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (strncmp(lpsz, "I64", 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
			// __int64 is only available on X86 and ALPHA platforms
#endif
		}
		else
		{
			switch (*lpsz)
			{
			// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				++lpsz;
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				++lpsz;
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				++lpsz;
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
		// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(vp, int);
			break;
		case 'c'|FORCE_ANSI:
		case 'C'|FORCE_ANSI:
			nItemLen = 2;
			va_arg(vp, int);
			break;
		case 'c'|FORCE_UNICODE:
		case 'C'|FORCE_UNICODE:
			nItemLen = 2;
			va_arg(vp, int);
			break;

		// strings
		case 's':
			{
				LPCSTR pstrNextArg = va_arg(vp, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = strlen(pstrNextArg);
				   nItemLen = MaxOf(1, nItemLen);
				}
			}
			break;

		case 'S':
			{
				LPCSTR pstrNextArg = va_arg(vp, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = strlen(pstrNextArg);
				   nItemLen = MaxOf(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(vp, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = strlen(pstrNextArg);
				   nItemLen = MaxOf(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_UNICODE:
		case 'S'|FORCE_UNICODE:
			{
				LPCWSTR pstrNextArg = va_arg(vp, LPCWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = MaxOf(1, nItemLen);
				}
			}
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = MinOf(nItemLen, nPrecision);
			nItemLen = MaxOf(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(vp, __int64);
				else
					va_arg(vp, int);
				nItemLen = 32;
				nItemLen = MaxOf(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(vp, double);
				nItemLen = 128;
				nItemLen = MaxOf(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
				{
					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == MaxOf precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					va_arg(vp, double);
					nItemLen = 312+nPrecision+6;
				}
				break;

			case 'p':
				va_arg(vp, void*);
				nItemLen = 32;
				nItemLen = MaxOf(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(vp, int*);
				break;

			default:
				DEBUG_BREAK();
                break;
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

  resize(nMaxLen + 1);
  nMaxLen = _vsnprintf(&(*this)[0],nMaxLen + 1,szFormat,vpSave);
  resize(nMaxLen);
}

void CUiStringA::Format(LPCSTR szFormat,...)
{
  va_list vp;

  va_start(vp,szFormat);
  FormatV(szFormat,vp);
  va_end(vp);
}

void CUiStringA::Assign(const CUiStringA& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}

void CUiStringA::Assign(const CUiStringW& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}

void CUiStringA::Assign(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  if (cpi == cpo)
  {
    assign(szText,cbText);
  }
  else
  {
    CDuiX2W x2w(szText,cbText,cpi,CP_UNICODE);

    Assign(x2w.c_str(),x2w.length(),CP_UNICODE,cpo);
  }
}

void CUiStringA::Assign(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  size_t cbDest;

  DEBUG_BREAK_IF(CP_UNICODE != cpi);
  if (SIZE_T_NEG_1 == cbText) cbText = wcslen(szText);
  cbDest = WideCharToMultiByte(cpo,0,szText,cbText,NULL,0,NULL,NULL);
  resize(cbDest + 1);
  cbDest = WideCharToMultiByte(cpo,0,szText,cbText,&(*this)[0],cbDest + 1,NULL,NULL);
  resize(cbDest);
}

size_t CUiStringA::Replace(char cOld,char cNew)
{
  size_t i,numReplace;

  for(i=numReplace=0;i<length();i++)
  {
    if (cOld == (*this)[i])
    {
      (*this)[i] = cNew;
	  numReplace++;
    }
  }
  return numReplace;
}

size_t CUiStringA::Replace(LPCSTR szOld,LPCSTR szNew)
{
  size_t curPos,numReplace,oldSize,newSize;

  numReplace = 0;
  oldSize = strlen(szOld);
  newSize = strlen(szNew);
  for(curPos = find(szOld,0);curPos != npos;curPos = find(szOld,curPos + newSize)) 
  { 
    replace(curPos,oldSize,szNew,newSize); 
    numReplace++; 
  }
  return numReplace; 
}
//+++CUiStringA

//---CUiStringW
CUiStringW::CUiStringW()
{
}

CUiStringW::CUiStringW(LPCWSTR that)
{
  assign(that);
}

CUiStringW::CUiStringW(const std::wstring& that)
{
  assign(that.c_str(),that.length());
}

CUiStringW::CUiStringW(const CUiStringW& that)
{
  assign(that.c_str(),that.length());
}

CUiStringW& CUiStringW::operator=(WCHAR that)
{
  assign(&that,1);
  return *this;
}

CUiStringW& CUiStringW::operator=(LPCWSTR that)
{
  assign(that);
  return *this;
}

CUiStringW& CUiStringW::operator=(const std::wstring& that)
{
  assign(that.c_str(),that.length());
  return *this;
}

CUiStringW& CUiStringW::operator=(const CUiStringW& that)
{
  assign(that.c_str(),that.length());
  return *this;
}

CUiStringW CUiStringW::operator+(WCHAR that)
{
  CUiStringW strTemp(*this);

  strTemp.append(&that,1);
  return strTemp;
}

CUiStringW CUiStringW::operator+(LPCWSTR that)
{
  CUiStringW strTemp(*this);

  strTemp.append(that);
  return strTemp;
}

CUiStringW CUiStringW::operator+(const CUiStringW& that)
{
  CUiStringW strTemp(*this);

  strTemp.append(that.c_str(),that.length());
  return strTemp;
}

#if _MSC_VER <= 1200
CUiStringW operator+(LPCWSTR s0,const CUiStringW& s1)
{
  CUiStringW strTemp(s0);

  strTemp += s1;
  return strTemp;
}
#endif //_MSC_VER

#ifndef WIN32
long _wtoi(LPCWSTR szText)
{
  return 0;
}

size_t _vsnwprintf(PWSTR szDest,size_t cbDest,LPCWSTR szFMT,va_list vp)
{
  return 0;
}
#endif //WIN32

void CUiStringW::FormatV(LPCWSTR szFormat,va_list vp)
{
  va_list vpSave = vp;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCWSTR lpsz = szFormat; *lpsz != '\0'; lpsz++)
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != _T('%') || *(++lpsz) == _T('%'))
		{
			nMaxLen += wcslen(lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; ++lpsz)
		{
			// check for valid flags
			if (*lpsz == _T('#'))
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == _T('*'))
				nWidth = va_arg(vp, int);
			else if (*lpsz == _T('-') || *lpsz == _T('+') || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _wtoi(lpsz);
			for (; *lpsz != '\0' && iswdigit(*lpsz); ++lpsz)
				;
		}
		DEBUG_BREAK_IF(nWidth < 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			++lpsz;

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(vp, int);
				++lpsz;
			}
			else
			{
				nPrecision = _wtoi(lpsz);
				for (; *lpsz != '\0' && iswdigit(*lpsz); ++lpsz)
					;
			}
			DEBUG_BREAK_IF(nPrecision < 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (wcsncmp(lpsz, L"I64", 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
			// __int64 is only available on X86 and ALPHA platforms
#endif
		}
		else
		{
			switch (*lpsz)
			{
			// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				++lpsz;
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				++lpsz;
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				++lpsz;
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
		// single characters
		case _T('c'):
		case _T('C'):
			nItemLen = 2;
			va_arg(vp, int);
			break;
		case _T('c')|FORCE_ANSI:
		case _T('C')|FORCE_ANSI:
			nItemLen = 2;
			va_arg(vp, int);
			break;
		case _T('c')|FORCE_UNICODE:
		case _T('C')|FORCE_UNICODE:
			nItemLen = 2;
			va_arg(vp, int);
			break;

		// strings
		case 's':
			{
				LPCWSTR pstrNextArg = va_arg(vp, LPCWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = MaxOf(1, nItemLen);
				}
			}
			break;

		case 'S':
			{
				LPCWSTR pstrNextArg = va_arg(vp, LPCWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = MaxOf(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(vp, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = strlen(pstrNextArg);
				   nItemLen = MaxOf(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_UNICODE:
		case 'S'|FORCE_UNICODE:
			{
				LPCWSTR pstrNextArg = va_arg(vp, LPCWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = MaxOf(1, nItemLen);
				}
			}
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = MinOf(nItemLen, nPrecision);
			nItemLen = MaxOf(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(vp, __int64);
				else
					va_arg(vp, int);
				nItemLen = 32;
				nItemLen = MaxOf(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(vp, double);
				nItemLen = 128;
				nItemLen = MaxOf(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
				{
					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == MaxOf precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					va_arg(vp, double);
					nItemLen = 312+nPrecision+6;
				}
				break;

			case 'p':
				va_arg(vp, void*);
				nItemLen = 32;
				nItemLen = MaxOf(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(vp, int*);
				break;

			default:
				DEBUG_BREAK();
                break;
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

  resize(nMaxLen + 1);
  nMaxLen = _vsnwprintf(&(*this)[0],nMaxLen + 1,szFormat,vpSave);
  resize(nMaxLen);
}

void CUiStringW::Format(LPCWSTR szFormat,...)
{
  va_list vp;

  va_start(vp,szFormat);
  FormatV(szFormat,vp);
  va_end(vp);
}

void CUiStringW::Assign(const CUiStringA& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}

void CUiStringW::Assign(const CUiStringW& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}

void CUiStringW::Assign(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  size_t cbDest;

  DEBUG_BREAK_IF(CP_UNICODE != cpo);
  if (SIZE_T_NEG_1 == cbText) cbText = strlen(szText);
  cbDest = MultiByteToWideChar(cpi,0,szText,cbText,NULL,0);
  resize(cbDest + 1);
  cbDest = MultiByteToWideChar(cpi,0,szText,cbText,&(*this)[0],cbDest + 1);
  resize(cbDest);
}

void CUiStringW::Assign(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  DEBUG_BREAK_IF(cpi != cpo);
  assign(szText,cbText);
}

size_t CUiStringW::Replace(WCHAR cOld,WCHAR cNew)
{
  size_t i,numReplace;

  for(i=numReplace=0;i<length();i++)
  {
    if (cOld == (*this)[i])
    {
      (*this)[i] = cNew;
	  numReplace++;
    }
  }
  return numReplace;
}

size_t CUiStringW::Replace(LPCWSTR szOld,LPCWSTR szNew)
{
  size_t curPos,numReplace,oldSize,newSize;

  numReplace = 0;
  oldSize = wcslen(szOld);
  newSize = wcslen(szNew);
  for(curPos = find(szOld,0);curPos != npos;curPos = find(szOld,curPos + newSize)) 
  { 
    replace(curPos,oldSize,szNew,newSize); 
    numReplace++; 
  }
  return numReplace; 
}
//+++CUiStringW

//---CDuiX2A
CDuiX2A::CDuiX2A(void)
{
  m_szText = NULL;
  m_cbText = 0;
}

CDuiX2A::CDuiX2A(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  Assign(szText,cbText,cpi,cpo);
}

CDuiX2A::CDuiX2A(const CUiStringA& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}

CDuiX2A::CDuiX2A(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  Assign(szText,cbText,cpi,cpo);
}

CDuiX2A::CDuiX2A(const CUiStringW& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}  

void CDuiX2A::Assign(const CUiStringA& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}

void CDuiX2A::Assign(const CUiStringW& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}

void CDuiX2A::Assign(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  if (NULL == szText)
  {
    m_szText = NULL;
    m_cbText = 0;
  }
  else
  {    
    if (cpi == cpo)
    {
      m_szText = szText;
      m_cbText = cbText;
    }
    else
    {
      m_strText.Assign(szText,cbText,cpi,cpo);
      m_szText = m_strText.c_str();
      m_cbText = m_strText.length();
    }
  }
}

void CDuiX2A::Assign(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  //DEBUG_BREAK_IF(CP_UNICODE != cpi);
  if (NULL == szText)
  {
    m_szText = NULL;
    m_cbText = 0;
  }
  else
  {
    m_strText.Assign(szText,cbText,cpi,cpo);
    m_szText = m_strText.c_str();
    m_cbText = m_strText.length();
  }
}
//+++CDuiX2A

//---CDuiX2W
CDuiX2W::CDuiX2W(void)
{
  m_szText = NULL;
  m_cbText = 0;
}

CDuiX2W::CDuiX2W(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  Assign(szText,cbText,cpi,cpo);
}

CDuiX2W::CDuiX2W(const CUiStringA& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}

CDuiX2W::CDuiX2W(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  Assign(szText,cbText,cpi,cpo);
}

CDuiX2W::CDuiX2W(const CUiStringW& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}  

void CDuiX2W::Assign(const CUiStringA& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}

void CDuiX2W::Assign(const CUiStringW& strText,UINT cpi,UINT cpo)
{
  Assign(strText.c_str(),strText.length(),cpi,cpo);
}

void CDuiX2W::Assign(LPCSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  //DEBUG_BREAK_IF(CP_UNICODE != cpo);
  if (NULL == szText)
  {
    m_szText = NULL;
    m_cbText = 0;
  }
  else
  {
    m_strText.Assign(szText,cbText,cpi,cpo);
    m_szText = m_strText.c_str();
    m_cbText = m_strText.length();
  }
}

void CDuiX2W::Assign(LPCWSTR szText,size_t cbText,UINT cpi,UINT cpo)
{
  //DEBUG_BREAK_IF(cpi != cpo);
  if (NULL == szText)
  {
    m_szText = NULL;
    m_cbText = 0;
  }
  else
  {
    m_szText = szText;
    m_cbText = cbText;
  }  
}
//+++CDuiX2W
