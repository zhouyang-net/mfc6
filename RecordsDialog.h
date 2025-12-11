#pragma once
#include <afxwin.h>
#include <vector>
#include "Record.h"

class CRecordsDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CRecordsDialog)
public:
    CRecordsDialog(bool isIncome = true, CWnd* pParent = nullptr);
    virtual ~CRecordsDialog();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_RECORDS_DIALOG };
#endif

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()

    afx_msg void OnAdd();
    afx_msg void OnEdit();
    afx_msg void OnDelete();
    afx_msg void OnPrint();

private:
    void LoadList();
    bool m_isIncome;
    CListCtrl m_list;
    std::vector<Record> m_records;
};