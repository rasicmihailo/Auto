
// Kolokvijum2014View.h : interface of the CKolokvijum2014View class
//

#pragma once
#include "DImage.h"

class CKolokvijum2014View : public CView
{
protected: // create from serialization only
	CKolokvijum2014View() noexcept;
	DECLARE_DYNCREATE(CKolokvijum2014View)

// Attributes
public:
	CKolokvijum2014Doc* GetDocument() const;
	DImage* imgWheel;
	HENHMETAFILE mfClio;
	int drive;
	float angleDelta;

// Operations
public:
	void DrawGround(CDC* pDC, float angle);
	void Translation(CDC* pDC, float dX, float dY, bool right);
	void Rotate(CDC* pDC, float angle, bool right);
	void Scale(CDC* pDC, float scaleX, float scaleY, bool right);
	void DrawCar(CDC* pDC, int x, int y, int w, int h, float angle);
	void DrawWheel(CDC* pDC, int x, int y, int r, float angle);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKolokvijum2014View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in Kolokvijum2014View.cpp
inline CKolokvijum2014Doc* CKolokvijum2014View::GetDocument() const
   { return reinterpret_cast<CKolokvijum2014Doc*>(m_pDocument); }
#endif

