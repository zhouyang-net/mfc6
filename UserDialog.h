#pragma once
#include <afxwin.h>

class CUserDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CUserDialog)
public:
    CUserDialog(CWnd* pParent = nullptr);
    virtual ~CUserDialog();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_USER_DIALOG };
#endif

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()

    afx_msg void OnUserAdd();
    afx_msg void OnUserDelete();

private:
    CListCtrl m_list;
    void LoadUsers();
};