// mfc6.h: mfc6 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号


// Cmfc6App:
// 有关此类的实现，请参阅 mfc6.cpp
//

class Cmfc6App : public CWinApp
{
public:
	Cmfc6App() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	void OnManageIncome();
	void OnManageExpense();
	void OnManageUsers();

	DECLARE_MESSAGE_MAP()
};

extern Cmfc6App theApp;
