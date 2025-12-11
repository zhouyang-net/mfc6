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
}

BEGIN_MESSAGE_MAP(CRecordDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CRecordDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 若已有记录，填充
    if (!m_rec.category.empty()) {
        m_sCategory = m_rec.category.c_str();
        m_sNotes = m_rec.notes.c_str();
        m_amount = m_rec.amount;
        UpdateData(FALSE);
        // 设置日期
        CDateTimeCtrl* pDate = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_DATE);
        if (pDate) {
            SYSTEMTIME st = {0};
            // 期望 date 格式 YYYY-MM-DD
            std::wstring d = m_rec.date;
            if (d.size() >= 10) {
                int y = _wtoi(d.substr(0,4).c_str());
                int m = _wtoi(d.substr(5,2).c_str());
                int day = _wtoi(d.substr(8,2).c_str());
                st.wYear = y; st.wMonth = m; st.wDay = day;
                pDate->SetTime(&st);
            }
        }
    }
    return TRUE;
}

void CRecordDlg::SetRecord(const Record& r)
{
    m_rec = r;
}

Record CRecordDlg::GetRecord()
{
    Record r = m_rec;
    Record out = m_rec;
    // 读取控件数据
    //CRecordDlg* self = const_cast<CRecordDlg*>(this);
    //CString sCat, sNotes;
    //double amt = 0.0;
    //self->GetDlgItemText(IDC_EDIT_CATEGORY, sCat);
    //self->GetDlgItemText(IDC_EDIT_NOTES, sNotes);
    //CString sAmt;
    //self->GetDlgItemText(IDC_EDIT_AMOUNT, sAmt);
    //amt = _ttof(sAmt);
    CString sCat, sNotes;
    GetDlgItemText(IDC_EDIT_CATEGORY, sCat);
    GetDlgItemText(IDC_EDIT_NOTES, sNotes);
    CString sAmt;
    GetDlgItemText(IDC_EDIT_AMOUNT, sAmt);
    double amt = _ttof(sAmt);
    CDateTimeCtrl* pDate = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_DATE);
    SYSTEMTIME st = {0};
    std::wstring datew = out.date;
    if (pDate && pDate->GetTime(&st) == GDT_VALID) {
        WCHAR buf[16];
        swprintf_s(buf, L"%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
        datew = buf;
    }
    out.category = std::wstring((LPCTSTR)sCat);
    out.notes = std::wstring((LPCTSTR)sNotes);
    out.amount = amt;
    out.date = datew;
    return out;
}