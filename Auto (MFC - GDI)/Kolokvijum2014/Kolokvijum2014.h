
// Kolokvijum2014.h : main header file for the Kolokvijum2014 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKolokvijum2014App:
// See Kolokvijum2014.cpp for the implementation of this class
//

class CKolokvijum2014App : public CWinApp
{
public:
	CKolokvijum2014App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKolokvijum2014App theApp;
