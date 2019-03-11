
// Kolokvijum2014View.cpp : implementation of the CKolokvijum2014View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kolokvijum2014.h"
#endif

#include "Kolokvijum2014Doc.h"
#include "Kolokvijum2014View.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKolokvijum2014View

IMPLEMENT_DYNCREATE(CKolokvijum2014View, CView)

BEGIN_MESSAGE_MAP(CKolokvijum2014View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKolokvijum2014View construction/destruction


CKolokvijum2014View::CKolokvijum2014View() noexcept
{
	// TODO: add construction code here


	imgWheel = new DImage();
	imgWheel->Load(CString("Wheel.png"));

	mfClio = GetEnhMetaFile(CString("clio.emf"));	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	drive = 0;

	angleDelta = 0.0;
}

CKolokvijum2014View::~CKolokvijum2014View()
{
	delete imgWheel;
	DeleteEnhMetaFile(mfClio);
}

BOOL CKolokvijum2014View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKolokvijum2014View drawing

void CKolokvijum2014View::OnDraw(CDC* /*pDC*/)
{
	CKolokvijum2014Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	//pDC->SetMapMode(MM_ISOTROPIC);
	//pDC->SetWindowExt(1600, 800);
	//pDC->SetViewportExt(rect.right, rect.bottom);

	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);
	
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pMemDC->SelectObject(&bmp);

	XFORM transOld;
	int gm = pMemDC->SetGraphicsMode(GM_ADVANCED);
	pMemDC->GetWorldTransform(&transOld);
	
	float angle = 0.0 + angleDelta;
	
	DrawGround(pMemDC, angle);

	int xCar = drive;
	int r = 38;
	Translation(pMemDC, 450 / 2.0, -450.0 / 5.0 - r / 2, true);
	Rotate(pMemDC, -angle, true);
	Translation(pMemDC, 0, rect.Height(), true);

	DrawCar(pMemDC, xCar, 0, 450, 450 / 2.5, 0);

	DrawWheel(pMemDC, xCar - 155, 70, 38, 30);
	DrawWheel(pMemDC, xCar + 135, 70, 38, 30);

	pMemDC->SetWorldTransform(&transOld);
	pMemDC->SetGraphicsMode(gm);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
	delete pMemDC;

}


void CKolokvijum2014View::DrawGround(CDC* pDC, float angle) {
	float rad = 3.14159 * angle / 180.0;
	CRect rect;
	GetClientRect(&rect);

	CBrush* brushBack = new CBrush(RGB(128, 235, 247));
	CBrush* oldBrush = pDC->SelectObject(brushBack);
	pDC->Rectangle(0, 0, rect.right, rect.bottom);

	CPen* pen = new CPen(PS_SOLID, 3, RGB(79, 45, 47));
	CPen* oldPen = pDC->SelectObject(pen);

	CBrush* brush = new CBrush(HS_FDIAGONAL, RGB(125, 225, 26));
	pDC->SelectObject(brush);

	CPoint points[] = { CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom), CPoint(rect.right, rect.Height() - (rect.right * rad) / (3.14159 / 2 - rad)) };

	COLORREF oldBk = pDC->SetBkColor(RGB(82, 188, 3));

	pDC->Polygon(points, 3);

	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(&oldBk);
}

void CKolokvijum2014View::Translation(CDC* pDC, float dX, float dY, bool right) {
	XFORM trans;
	trans.eM11 = 1.0;
	trans.eM12 = 0.0;
	trans.eM21 = 0.0;
	trans.eM22 = 1.0;
	trans.eDx = dX;
	trans.eDy = dY;

	if (right)
		pDC->ModifyWorldTransform(&trans, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&trans, MWT_LEFTMULTIPLY);
}

void CKolokvijum2014View::Rotate(CDC* pDC, float angle, bool right) {
	float rad = 3.14159 * angle / 180.0;
	XFORM trans;
	trans.eM11 = cos(rad);
	trans.eM12 = sin(rad);
	trans.eM21 = -sin(rad);
	trans.eM22 = cos(rad);
	trans.eDx = 0.0;
	trans.eDy = 0.0;

	if (right)
		pDC->ModifyWorldTransform(&trans, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&trans, MWT_LEFTMULTIPLY);
}

void CKolokvijum2014View::Scale(CDC* pDC, float scaleX, float scaleY, bool right) {
	XFORM trans;
	trans.eM11 = scaleX;
	trans.eM12 = 0.0;
	trans.eM21 = 0.0;
	trans.eM22 = scaleY;
	trans.eDx = 0.0;
	trans.eDy = 0.0;

	if (right)
		pDC->ModifyWorldTransform(&trans, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&trans, MWT_LEFTMULTIPLY);
}

void CKolokvijum2014View::DrawCar(CDC* pDC, int x, int y, int w, int h, float angle) {
	
	pDC->PlayMetaFile(mfClio, CRect(x + w / 2, y - h / 2, x - w / 2, y + h / 2));

}

void CKolokvijum2014View::DrawWheel(CDC* pDC1, int x, int y, int r, float angle) {
	
	XFORM transOld;
	int prevMode = SetGraphicsMode(pDC1->m_hDC, GM_ADVANCED);
	pDC1->GetWorldTransform(&transOld);
	Translation(pDC1, x, y, false);
	Rotate(pDC1, angle, false);
	

	CRect srcRect(52, 15, 52 + 376, 15 + 376);
	CRect dstRect(-r, -r, r, r);
	imgWheel->DrawTransparent(pDC1, srcRect, dstRect, RGB(255, 255, 255));

	pDC1->SetWorldTransform(&transOld);
	SetGraphicsMode(pDC1->m_hDC, prevMode);
}

// CKolokvijum2014View printing

BOOL CKolokvijum2014View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKolokvijum2014View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKolokvijum2014View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKolokvijum2014View diagnostics

#ifdef _DEBUG
void CKolokvijum2014View::AssertValid() const
{
	CView::AssertValid();
}

void CKolokvijum2014View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKolokvijum2014Doc* CKolokvijum2014View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKolokvijum2014Doc)));
	return (CKolokvijum2014Doc*)m_pDocument;
}
#endif //_DEBUG


// CKolokvijum2014View message handlers


BOOL CKolokvijum2014View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
	return true;
}


void CKolokvijum2014View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_RIGHT) {
		drive += 10;
		Invalidate(false);
	}
	if (nChar == VK_LEFT) {
		drive -= 10;
		Invalidate(false);
	}

	if (nChar == VK_UP) {
		angleDelta += 5;
		Invalidate(false);
	}

	if (nChar == VK_DOWN) {
		angleDelta -= 5;
		Invalidate(false);
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
