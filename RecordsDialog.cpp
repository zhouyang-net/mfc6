#include "pch.h"
#include "RecordsDialog.h"
#include "AppResourceDefines.h"
#include "DatabaseManager.h"
#include "RecordDlg.h"
#include <afxdlgs.h>

IMPLEMENT_DYNAMIC(CRecordsDialog, CDialogEx)

CRecordsDialog::CRecordsDialog(bool isIncome, CWnd* pParent)
    : CDialogEx(IDD_RECORDS_DIALOG, pParent), m_isIncome(isIncome)
{
}

CRecordsDialog::~CRecordsDialog() { }

void CRecordsDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_RECORDS, m_list);
}

BEGIN_MESSAGE_MAP(CRecordsDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_ADD, &CRecordsDialog::OnAdd)
    ON_BN_CLICKED(IDC_BTN_EDIT, &CRecordsDialog::OnEdit)
    ON_BN_CLICKED(IDC_BTN_DELETE, &CRecordsDialog::OnDelete)
    ON_BN_CLICKED(IDC_BTN_PRINT, &CRecordsDialog::OnPrint)
END_MESSAGE_MAP()

BOOL CRecordsDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 初始化列表
    m_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
    m_list.InsertColumn(0, L"ID", LVCFMT_LEFT, 40);
    m_list.InsertColumn(1, L"分类", LVCFMT_LEFT, 100);
    m_list.InsertColumn(2, L"金额", LVCFMT_RIGHT, 80);
    m_list.InsertColumn(3, L"日期", LVCFMT_LEFT, 80);
    m_list.InsertColumn(4, L"备注", LVCFMT_LEFT, 200);

    LoadList();
    return TRUE;
}

void CRecordsDialog::LoadList()
{
    m_list.DeleteAllItems();
    m_records = DatabaseManager::Instance().GetRecords(m_isIncome);
    for (size_t i = 0; i < m_records.size(); ++i) {
        const Record& r = m_records[i];
        int idx = m_list.InsertItem((int)i, CString(std::to_wstring(r.id).c_str()));
        m_list.SetItemText(idx, 1, CString(r.category.c_str()));
        CString sAmt;
        sAmt.Format(L"%.2f", r.amount);
        m_list.SetItemText(idx, 2, sAmt);
        m_list.SetItemText(idx, 3, CString(r.date.c_str()));
        m_list.SetItemText(idx, 4, CString(r.notes.c_str()));
    }
}

void CRecordsDialog::OnAdd()
{
    CRecordDlg dlg(m_isIncome, this);
    Record r;
    r.isIncome = m_isIncome;
    dlg.SetRecord(r);
    if (dlg.DoModal() == IDOK) {
        Record newr = dlg.GetRecord();
        newr.isIncome = m_isIncome;
        DatabaseManager::Instance().AddRecord(newr);
        LoadList();
    }
}

void CRecordsDialog::OnEdit()
{
    POSITION pos = m_list.GetFirstSelectedItemPosition();
    if (!pos) { AfxMessageBox(L"请选择要编辑的记录。"); return; }
    int sel = m_list.GetNextSelectedItem(pos);
    if (sel < 0 || sel >= (int)m_records.size()) return;
    Record r = m_records[sel];
    CRecordDlg dlg(m_isIncome, this);
    dlg.SetRecord(r);
    if (dlg.DoModal() == IDOK) {
        Record upd = dlg.GetRecord();
        upd.id = r.id;
        upd.isIncome = m_isIncome;
        DatabaseManager::Instance().UpdateRecord(upd);
        LoadList();
    }
}

void CRecordsDialog::OnDelete()
{
    POSITION pos = m_list.GetFirstSelectedItemPosition();
    if (!pos) { AfxMessageBox(L"请选择要删除的记录。"); return; }
    int sel = m_list.GetNextSelectedItem(pos);
    if (sel < 0 || sel >= (int)m_records.size()) return;
    if (AfxMessageBox(L"确认删除选中记录？", MB_YESNO) == IDYES) {
        DatabaseManager::Instance().DeleteRecord(m_records[sel].id);
        LoadList();
    }
}

void CRecordsDialog::OnPrint()
{
    CPrintDialog pd(FALSE);
    if (pd.DoModal() == IDOK) {
        CDC dc;
        dc.Attach(pd.GetPrinterDC());
        DOCINFO di = { sizeof(DOCINFO) };
        di.lpszDocName = m_isIncome ? L"收入记录" : L"支出记录";
        dc.StartDoc(&di);
        dc.StartPage();

        CString title = m_isIncome ? L"收入记录" : L"支出记录";
        int y = 100;
        dc.TextOut(100, y, title); y += 40;
        dc.TextOut(80, y, L"ID");
        dc.TextOut(140, y, L"分类");
        dc.TextOut(260, y, L"金额");
        dc.TextOut(340, y, L"日期");
        y += 20;

        for (auto& r : m_records) {
            CString sid; sid.Format(L"%d", r.id);
            dc.TextOut(80, y, sid);
            dc.TextOut(140, y, CString(r.category.c_str()));
            CString sAmt; sAmt.Format(L"%.2f", r.amount);
            dc.TextOut(260, y, sAmt);
            dc.TextOut(340, y, CString(r.date.c_str()));
            y += 20;
            if (y > 1000) { dc.EndPage(); dc.StartPage(); y = 100; }
        }

        dc.EndPage();
        dc.EndDoc();
        dc.Detach();
    }
}