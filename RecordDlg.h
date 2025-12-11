#pragma once
#include <afxwin.h>
#include "Record.h"

class CRecordDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CRecordDlg)
public:
    CRecordDlg(bool isIncome = true, CWnd* pParent = nullptr);
    virtual ~CRecordDlg();

    void SetRecord(const Record& r);
    Record GetRecord();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_RECORD_DLG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

private:
    COleDateTime m_dtDate;
    Record m_rec;
    bool m_isIncome;
    CString m_sCategory;
    CString m_sNotes;
    double m_amount = 0.0;
};