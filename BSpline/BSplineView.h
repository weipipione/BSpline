
// BSplineView.h: CBSplineView 类的接口
//

#pragma once


class CBSplineView : public CView
{
protected: // 仅从序列化创建
	CBSplineView() noexcept;
	DECLARE_DYNCREATE(CBSplineView)

// 特性
public:
	CBSplineDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CBSplineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	double B02(double t);
	double B12(double t);
	double B22(double t);
	double B03(double t);
	double B13(double t);
	double B23(double t);
	double B33(double t);
	void DrawTwoBSpline();
	void DrawThreeBSpline();
};

#ifndef _DEBUG  // BSplineView.cpp 中的调试版本
inline CBSplineDoc* CBSplineView::GetDocument() const
   { return reinterpret_cast<CBSplineDoc*>(m_pDocument); }
#endif

