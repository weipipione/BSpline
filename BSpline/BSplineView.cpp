
// BSplineView.cpp: CBSplineView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "BSpline.h"
#endif

#include "BSplineDoc.h"
#include "BSplineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBSplineView

IMPLEMENT_DYNCREATE(CBSplineView, CView)

BEGIN_MESSAGE_MAP(CBSplineView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBSplineView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBSplineView 构造/析构
//
CBSplineView::CBSplineView() noexcept
{
	// TODO: 在此处添加构造代码
}

CBSplineView::~CBSplineView()
{
}

BOOL CBSplineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CBSplineView 绘图

void CBSplineView::OnDraw(CDC*  /*pDC*/)
{
	CBSplineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    // TODO: 在此处为本机数据添加绘制代码
    DrawTwoBSpline();
	DrawThreeBSpline();
}


// CBSplineView 打印


void CBSplineView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBSplineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CBSplineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CBSplineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CBSplineView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBSplineView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBSplineView 诊断

#ifdef _DEBUG
void CBSplineView::AssertValid() const
{
	CView::AssertValid();
}

void CBSplineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBSplineDoc* CBSplineView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBSplineDoc)));
	return (CBSplineDoc*)m_pDocument;
}
#endif //_DEBUG


// CBSplineView 消息处理程序
//1. 将调和函数定义为成员函数,二次B样条曲线，函数形式如下：
double CBSplineView::B02(double t)
{
	return ((pow(t, 2) - 2 * t + 1) / 2);// pow()函数:求x的y次方
}

double CBSplineView::B12(double t)
{
	return ((-2 * pow(t, 2) + 2 * t + 1) / 2);
}

double CBSplineView::B22(double t)
{
	return (pow(t, 2) / 2);
}

//2. 将调和函数定义为成员函数,三次B样条曲线，函数形式如下：
double CBSplineView::B03(double t)
{
	return ((-pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) / 6);
}

double CBSplineView::B13(double t)
{
	return ((3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6);
}

double CBSplineView::B23(double t)
{
	return ((-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6);
}

double CBSplineView::B33(double t)
{
	return (pow(t, 3) / 6);
}

//绘制两次B样条曲线
void CBSplineView::DrawTwoBSpline()
{
	double t = 0.0, dt = 0.01;
	int  x[10], y[10];
	double dx = 0.0, dy = 0.0;
	CDC* pDC = GetDC();
	int n = 2, pointnum = 5; //pointnum为顶点数，这里有5个顶点，绘制（5-2）=3段B样条曲线
	int m = pointnum - n; //m为绘制的样条数
	CPen PenRed(PS_SOLID, 5, RGB(255, 0, 0));//定义红色实线笔
	CPen PenBlue(PS_SOLID, 5, RGB(0, 255, 0));//定义蓝色实线笔
	x[0] = 50; y[0] = 100;
	x[1] = 70; y[1] = 60;
	x[2] = 150; y[2] = 80;
	x[3] = 300; y[3] = 120;
	x[4] = 200; y[4] = 300;

	//绘出特征多边形
	pDC->SelectObject(&PenRed);
	pDC->MoveTo(x[0], y[0]);
	for (int i = 1; i < pointnum; i++)
		pDC->LineTo(x[i], y[i]);
	//绘制B样条曲线
	pDC->SelectObject(&PenBlue);
	for (int i = 0; i < m; i++)    //绘制m条（段）B样条曲线
	{
		t = 0;
		while (t <= 1)
		{
			dx = B02(t) * x[i] + B12(t) * x[i + 1] + B22(t) * x[i + 2];
			dy = B02(t) * y[i] + B12(t) * y[i + 1] + B22(t) * y[i + 2];
			if (t == 0)
			{
				pDC->MoveTo(int(dx), int(dy));
				pDC->LineTo(int(dx), int(dy));
				t = t + dt;
			}
			else {
				pDC->LineTo(int(dx), int(dy));
				t = t + dt;
			}
		}
	}
}

//绘制三次B样条曲线
void CBSplineView::DrawThreeBSpline(void)
{
	double t = 0.0, dt = 0.01;
	int  x[10], y[10];
	double dx = 0.0, dy = 0.0;
	CDC* pDC = GetDC();
	int n = 3, pointnum = 5; //pointnum为顶点数，这里有5个顶点，绘制（5-3）=2段B样条曲线
	int m = pointnum - n; //m为绘制的样条数
	CPen PenRed(PS_SOLID, 5, RGB(255, 0, 0));//定义红色实线笔
	CPen PenBlue(PS_SOLID, 5, RGB(0, 0, 255));//定义蓝色实线笔
	x[0] = 450; y[0] = 100;
	x[1] = 470; y[1] = 60;
	x[2] = 550; y[2] = 80;
	x[3] = 700; y[3] = 120;
	x[4] = 600; y[4] = 300;

	//绘出特征多边形
	pDC->SelectObject(&PenRed);
	pDC->MoveTo(x[0], y[0]);
	for (int i = 1; i < pointnum; i++)
		pDC->LineTo(x[i], y[i]);
	//绘制B样条曲线
	pDC->SelectObject(&PenBlue);
	for (int i = 0; i < m; i++)    //绘制m条（段）B样条曲线
	{
		t = 0;
		while (t <= 1)
		{
			dx = B03(t) * x[i] + B13(t) * x[i + 1] + B23(t) * x[i + 2] + B33(t) * x[i + 3];
			dy = B03(t) * y[i] + B13(t) * y[i + 1] + B23(t) * y[i + 2] + B33(t) * y[i + 3];
			if (t == 0)
			{
				pDC->MoveTo(int(dx),int(dy));
				pDC->LineTo(int(dx), int(dy));
				t = t + dt;
			}else {
				pDC->LineTo(int(dx), int(dy));
				t = t + dt;
			}
		}
	}
}




