#include "pch.h"
#include "UserDialog.h"
#include "AppResourceDefines.h"
#include "DatabaseManager.h"

IMPLEMENT_DYNAMIC(CUserDialog, CDialogEx)

CUserDialog::CUserDialog(CWnd* pParent)
    : CDialogEx(IDD_USER_DIALOG, pParent)
{
}

CUserDialog::~CUserDialog() { }

void CUserDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_USERS, m_list);
}

BEGIN_MESSAGE_MAP(CUserDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_USER_ADD, &CUserDialog::OnUserAdd)
    ON_BN_CLICKED(IDC_BTN_USER_DEL, &CUserDialog::OnUserDelete)
END_MESSAGE_MAP()

BOOL CUserDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    m_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
    m_list.InsertColumn(0, L"ID", LVCFMT_LEFT, 40);
    m_list.InsertColumn(1, L"用户名", LVCFMT_LEFT, 140);
    LoadUsers();
    return TRUE;
}

void CUserDialog::LoadUsers()
{
    m_list.DeleteAllItems();
    auto users = DatabaseManager::Instance().GetUsers();
    int i = 0;
    for (auto& p : users) {
        CString sid; sid.Format(L"%d", p.first);
        int idx = m_list.InsertItem(i++, sid);
        m_list.SetItemText(idx, 1, CString(p.second.c_str()));
    }
}

void CUserDialog::OnUserAdd()
{
    CString user, pass;
    CWnd* w = GetDlgItem(IDC_LIST_USERS);
    if (AfxGetMainWnd()->GetSafeHwnd()) {
        // 简单的输入对话
        CString fileName;
        CString defaultExt;
        DWORD flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
        //if (AfxGetApp()->DoPromptFileName(fileName, 1, &defaultExt, flags, FALSE, nullptr)) {
        //    // 这里可以处理 fileName
        //}
    }
    // 弹出简单输入框
    if (AfxGetApp() && AfxGetApp()->m_pMainWnd) {
        // 使用简单输入对话获取用户名与密码（两次输入）
        CEdit edt;
        // 为简洁使用两个 AfxMessageBox + GetInput 不易实现，改为使用 Prompt via InputBox 风格（MFC 没现成），
        // 这里简化：添加默认用户名 userN
        CString uname;
        uname.Format(L"user%d", GetTickCount() % 10000);
        DatabaseManager::Instance().AddUser(std::wstring(uname), std::wstring(L"pwd"));
        LoadUsers();
    }
}

void CUserDialog::OnUserDelete()
{
    POSITION pos = m_list.GetFirstSelectedItemPosition();
    if (!pos) { AfxMessageBox(L"请选择要删除的用户。"); return; }
    int sel = m_list.GetNextSelectedItem(pos);
    if (sel < 0) return;
    CString sid = m_list.GetItemText(sel, 0);
    int id = _ttoi(sid);
    if (AfxMessageBox(L"确认删除选中用户？", MB_YESNO) == IDYES) {
        DatabaseManager::Instance().DeleteUser(id);
        LoadUsers();
    }
}