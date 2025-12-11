
// mfc6View.cpp: Cmfc6View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "mfc6.h"
#endif

#include "mfc6Doc.h"
#include "mfc6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cmfc6View

IMPLEMENT_DYNCREATE(Cmfc6View, CView)

BEGIN_MESSAGE_MAP(Cmfc6View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Cmfc6View 构造/析构

Cmfc6View::Cmfc6View() noexcept
{
	// TODO: 在此处添加构造代码

}

Cmfc6View::~Cmfc6View()
{
}

BOOL Cmfc6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Cmfc6View 绘图

void Cmfc6View::OnDraw(CDC* /*pDC*/)
{
	Cmfc6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// Cmfc6View 打印

BOOL Cmfc6View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Cmfc6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Cmfc6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Cmfc6View 诊断

#ifdef _DEBUG
void Cmfc6View::AssertValid() const
{
	CView::AssertValid();
}

void Cmfc6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cmfc6Doc* Cmfc6View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cmfc6Doc)));
	return (Cmfc6Doc*)m_pDocument;
}
#endif //_DEBUG


// Cmfc6View 消息处理程序
