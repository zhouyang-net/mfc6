#include "pch.h"
#include "RecordDlg.h"
#include "AppResourceDefines.h"
#include <atlstr.h>

IMPLEMENT_DYNAMIC(CRecordDlg, CDialogEx)

CRecordDlg::CRecordDlg(bool isIncome, CWnd* pParent)
    : CDialogEx(IDD_RECORD_DLG, pParent), m_isIncome(isIncome)
{
}

CRecordDlg::~CRecordDlg() { }

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_CATEGORY, m_sCategory);
    DDX_Text(pDX, IDC_EDIT_NOTES, m_sNotes);
    DDX_Text(pDX, IDC_EDIT_AMOUNT, m_amount);
    DDX_DateTimeCtrl(pDX, IDC_DATETIME_DATE, m_dtDate); // 使用 COleDateTime 类型
}

BEGIN_MESSAGE_MAP(CRecordDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CRecordDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (!m_rec.category.empty()) {
        m_sCategory = m_rec.category.c_str();
        m_sNotes = m_rec.notes.c_str();
        m_amount = m_rec.amount;

        // 将 m_rec.date (std::wstring, 格式 YYYY-MM-DD) 转换为 COleDateTime
        if (m_rec.date.size() >= 10) {
            int y = _wtoi(m_rec.date.substr(0,4).c_str());
            int m = _wtoi(m_rec.date.substr(5,2).c_str());
            int d = _wtoi(m_rec.date.substr(8,2).c_str());
            m_dtDate.SetDate(y, m, d);
        } else {
            m_dtDate = COleDateTime::GetCurrentTime();
        }

        UpdateData(FALSE);
    }
    return TRUE;
}

void CRecordDlg::SetRecord(const Record& r)
{
    m_rec = r;
    // 这里可以直接在 OnInitDialog 里处理日期转换
}

Record CRecordDlg::GetRecord()
{
    Record out = m_rec;
    UpdateData(TRUE);

    // COleDateTime -> std::wstring
    CString strDate;
    strDate.Format(L"%04d-%02d-%02d", m_dtDate.GetYear(), m_dtDate.GetMonth(), m_dtDate.GetDay());
    out.date = strDate.GetString();

    out.category = m_sCategory;
    out.notes = m_sNotes;
    out.amount = m_amount;
    return out;
}