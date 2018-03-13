#ifndef IC_LOCALE_ENV_H
#define IC_LOCALE_ENV_H
#include <locale.h>
#include "mbctype.h"

class CEyeLocaleEnv
{
public:
	CEyeLocaleEnv::CEyeLocaleEnv(BOOL bSetThread, 
		bool bLocAsEngUS1252 = false, bool bSysDefault = true)//set it to be the system/user default en-us-ansi one that is independent with localization.
	{
		m_OrgLocale = NULL;
		m_bSetThread = FALSE;

	#ifdef _MSC_VER
		
		m_bSetThread = bSetThread;
		//Get Original Locale
		m_OrgLocale = _tcsdup( _tsetlocale( LC_ALL,  NULL ) );

		LCID Locale = GetUserDefaultLCID();

		if( m_bSetThread )
		{
			//Thread Locale
			m_OrgLCID = GetThreadLocale();
			//Get UserDefault LCID to thread locale
			SetThreadLocale( Locale );			
		}	
		
		if(bLocAsEngUS1252)
		{
			if(!bSysDefault)
				_tsetlocale( LC_ALL, _T("English_United States.1252") ); 
			else
				_tsetlocale( LC_ALL, _T("C") ); 
		}
		else
		{
			TCHAR cpname[32];
			int nbytes = 0;
			GetLocaleInfo (Locale, LOCALE_SENGLANGUAGE, cpname, 32);

			TCHAR cpchain1[128];

			nbytes = GetLocaleInfo (Locale, LOCALE_SENGCOUNTRY   , cpchain1, 128);	

			TCHAR cpchain[7];

			nbytes = GetLocaleInfo (Locale, LOCALE_IDEFAULTANSICODEPAGE, cpchain, 7);

			TCHAR NewLocal[256];

			_stprintf_s( NewLocal, 256, _T("%s_%s.%s"),cpname,cpchain1,cpchain);

			_tsetlocale( LC_ALL,  NewLocal );
		}

	#endif
	}



	
	CEyeLocaleEnv::~CEyeLocaleEnv()
	{
	#ifdef _MSC_VER
		if( m_OrgLocale )
		{
			_tsetlocale( LC_ALL,  m_OrgLocale );

			ASSERT( _tcscmp(m_OrgLocale, _tsetlocale(LC_ALL,NULL) ) == 0 );

			free(m_OrgLocale);
		}
		if( m_bSetThread )
		{
			SetThreadLocale( m_OrgLCID );	
			ASSERT( m_OrgLCID == GetThreadLocale() );
		}
	#endif
	}


protected:
	LPTSTR m_OrgLocale;

	BOOL m_bSetThread;
	LCID m_OrgLCID;
};

//fix using function ReadString of CStdioFile issue.
//if reading an ANSI text, and the local info is en-us, but mbcs info is zh-cn, the reading result is wrong.
//so we should set local info same as mbcs info temporarily.
class CMBCSLocalEnv
{
public:
	CMBCSLocalEnv()
	{
		m_OrgMBCSLoc = NULL;
#ifdef _MSC_VER
		m_OrgMBCSLoc = _tcsdup( _tsetlocale( LC_ALL,  NULL ) );
		CPINFOEX cpInfo;
		int nCP = _getmbcp();
		GetCPInfoEx(nCP, 0, &cpInfo);
		ILangSupport* piLangSprt = EyeGetLangSupport();

		ELangAreaID lID = piLangSprt->GetCurLanguageID();
		LCID mbcsID = 0;
		if (lID == LA_ZH_CN_NO_LANG_DLL)
		{
			mbcsID = 0x0804;
		}
		else if (lID == LA_ZH_TW_NO_LANG_DLL)
		{
			mbcsID = 0x0404;
		}
		else if (lID == LA_EN_US_NO_LANG_DLL)
		{
			mbcsID = 0x0409;
		}
		else if (lID == LA_UNKNOWN)
		{
			return ;
		}
		else
			mbcsID = (LCID)lID;

		TCHAR cpname[32];
		int nbytes = 0;
		GetLocaleInfo (mbcsID, LOCALE_SENGLANGUAGE, cpname, 32);

		TCHAR cpchain1[128];

		nbytes = GetLocaleInfo (mbcsID, LOCALE_SENGCOUNTRY   , cpchain1, 128);	

		TCHAR cpchain[7];

		nbytes = GetLocaleInfo (mbcsID, LOCALE_IDEFAULTANSICODEPAGE, cpchain, 7);

		TCHAR NewLocal[256];

		_stprintf_s( NewLocal, 256,  _T("%s_%s.%s"),cpname,cpchain1,cpchain);

		_tsetlocale( LC_ALL,  NewLocal );
#endif
	}

	~CMBCSLocalEnv()
	{
		if (m_OrgMBCSLoc == NULL)
		{
			return;
		}
#ifdef _MSC_VER
		_tsetlocale( LC_ALL,  m_OrgMBCSLoc );

		ASSERT( _tcscmp(m_OrgMBCSLoc, _tsetlocale(LC_ALL,NULL) ) == 0 );

		free(m_OrgMBCSLoc);
#endif
	}

private:
	LPTSTR m_OrgMBCSLoc;
};



#endif
