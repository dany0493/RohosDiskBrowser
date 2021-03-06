/**********************************************************************
** Copyright (C) 2005-2015 Tesline-Service S.R.L. All rights reserved.
**
** Rohos Disk Browser, Rohos Mini Drive Portable.
** 
**
** This file may be distributed and/or modified under the terms of the
** GNU Affero General Public license version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file. If not, see <http://www.gnu.org/licenses/>
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.rohos.com/ for GPL licensing information.
**
** Contact info@rohos.com http://rohos.com
**/


#include "stdafx.h"
#include "DiskBrowserApp.h"
#include "Dlg_Password.h"

#include "XKeyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_Password dialog


CDlg_Password::CDlg_Password(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Password::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Password)
	m_password = _T("");
	
	m_lbEnterPassword = _T("");
	m_lbImagePath = _T("");
	//}}AFX_DATA_INIT

	m_FullPath = _T("");
	myFileDialog = new CFileDialog(TRUE);
}


void CDlg_Password::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Password)
	DDX_Text(pDX, IDC_EDIT1, m_password);
	//DDX_Text(pDX, IDC_FULL_PATH, m_FullPath);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_Password, CDialog)
	//{{AFX_MSG_MAP(CDlg_Password)
	ON_BN_CLICKED(IDC_BUTTON1, OnVirtualKey)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_Password::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlg_Password::OnBnClickedBrowseFile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Password message handlers

void CDlg_Password::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();

	TCHAR _disk_image_path[600];
	GetDlgItemText( IDC_COMBO1, _disk_image_path, 590); // èìÿ äèñêà äëÿ ìîíòèðîâàíèÿ 

	m_FullPath = _disk_image_path;

	if ( m_vk_passw.GetLength() )
		m_password = m_vk_passw; // get virtual keyboard passwrod here
}

void CDlg_Password::OnVirtualKey() 
{
	CPoint point;
    GetCursorPos(&point);

    CXKeyboard dlg;//(/*point.x, point.y-127*/);


    dlg.SetShowText(false);
    //dlg.SetMRUPasswords(sa);
    
    dlg.SetMinLength(1);
    dlg.SetMaxLength(25);
    //dlg.SetTimeout(23);
    //dlg.SetSpecial((CXKeyboard::SPECIAL_CHARACTERS)m_nSpecial);
    dlg.SetShowSpacebar(1);
    //dlg.SetCase((CXKeyboard::XKEYBOARD_CASE)m_nCase);
    dlg.SetPassword("");
    //dlg.SetReadOnly(TRUE);

    int rc = dlg.DoModal();
    if (rc == IDOK)
    {
        m_vk_passw = dlg.GetPassword();

		m_password = "********";
		UpdateData(false); 
		Sleep(300);
		OnOK();
        
    }
    else if (rc == IDABORT)
    {		
        
    }
    else
    {
        
    }
	
}

BOOL CDlg_Password::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText( LS(IDS_PASSWORD_DLG) );
	SetDlgItemText(IDC_LABEL_ENTER_PASS,  LS(IDS_ENTER_PASSW) );
	SetDlgItemText(IDC_STATIC1,  LS(IDS_DISK_FOLDER) );
	SetDlgItemText(IDCANCEL,  LS(IDS_CANCEL) );

	CWnd* pEdit = GetDlgItem(IDC_COMBO1)->GetDlgItem(1001); // edit box inside ComboBox
    if( pEdit )
    {
        SHAutoComplete( pEdit->m_hWnd, SHACF_FILESYSTEM | SHACF_AUTOAPPEND_FORCE_OFF);
    }
	
	//m_lbImagePath = 

	// TODO:  Add extra initialization here

	int selectionIdx=0;

	for (int i=0; i<_possible_disk_images_arr.GetSize(); i++) {
		int intemidx = SendDlgItemMessage( IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)_possible_disk_images_arr.GetAt(i) );
		
		if ( _possible_disk_images_arr.GetAt(i) == m_FullPath)
			SendDlgItemMessage( IDC_COMBO1, CB_SETCURSEL, intemidx, 0L );		

		if (_possible_disk_images_arr.GetAt(i).Find(".rdi")>0 )
			selectionIdx = intemidx;
	}

	if (_possible_disk_images_arr.GetSize() == 0)
	{	
		SendDlgItemMessage( IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"..." );
		int intemidx = SendDlgItemMessage( IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)m_FullPath );
		
		SendDlgItemMessage(IDC_COMBO1, CB_SETCURSEL, intemidx, 0L );		
	}

	if ( SendDlgItemMessage( IDC_COMBO1, CB_GETCURSEL, 0L, 0L ) == -1)
	{		
		SendDlgItemMessage( IDC_COMBO1, CB_SETCURSEL, selectionIdx, 0L );		
	}

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_Password::AddPossibleDiskImage(LPCTSTR imagepath)
{	

	_possible_disk_images_arr.Add(imagepath);

}
void CDlg_Password::AddPossibleDiskImageAtBegin(LPCTSTR imagepath)
{	

	_possible_disk_images_arr.InsertAt(0, imagepath);

}

void CDlg_Password::OnCbnSelchangeCombo1()
{
	int cursel = SendDlgItemMessage( IDC_COMBO1, CB_GETCURSEL, 0, 0L );
	TCHAR text[500] = {0};
	SendDlgItemMessage( IDC_COMBO1, CB_GETLBTEXT, cursel, (LPARAM)(LPCTSTR)text );

	if ( _tcsstr(text, "...") == NULL ) {
		m_FullPath= text;
		return;
	}

	return;
	/*

	// user selected "..." - open file selection dialog

	CString file;		
	TCHAR filter[105];
	
	memset(filter, 0, sizeof(TCHAR)*101 );
	LoadString(AfxGetApp()->m_hInstance, IDS_ALLFILES_FILTER, filter, 100);
	CString title;
	title = LS(IDS_SELECT_FILE_IMAGE);	
	
	
	//myFileDialog->m_ofn.Flags = 0 | OFN_ENABLEHOOK |OFN_EXPLORER | OFN_ENABLESIZING | OFN_PATHMUSTEXIST |OFN_FILEMUSTEXIST | OFN_FORCESHOWHIDDEN;
	myFileDialog->m_ofn.Flags = OFN_PATHMUSTEXIST |OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_FORCESHOWHIDDEN;
	myFileDialog->m_ofn.hwndOwner = m_hWnd;
	myFileDialog->m_ofn.lpstrFilter = (LPCTSTR)filter;
	myFileDialog->m_ofn.lpstrTitle = (LPCTSTR)title;
	//if (_traveler_USB_drive.GetLength() )
		//myFileDialog->m_ofn.lpstrInitialDir = (LPCTSTR)_traveler_USB_drive;

	CString image_path, image_file_name;
	
	if (myFileDialog->DoModal() == IDOK) {
		
		m_FullPath= myFileDialog->GetPathName();
		AddPossibleDiskImage(m_FullPath);

		int intemidx = SendDlgItemMessage( IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)m_FullPath );
		SendDlgItemMessage( IDC_COMBO1, CB_SETCURSEL, intemidx, 0L );		
		

	} else
		return;
		*/ 

	//if ( 0 == FillListBox("") )
	//	AfxMessageBox("There are no configured USb Keys.", MB_ICONINFORMATION);
}

void CDlg_Password::OnBnClickedBrowseFile()
{
	// user selected "..." - open file selection dialog

	CString file;		
	TCHAR filter[105];
	
	memset(filter, 0, sizeof(TCHAR)*101 );
	LoadString(AfxGetApp()->m_hInstance, IDS_ALLFILES_FILTER, filter, 100);
	CString title;
	title = LS(IDS_SELECT_FILE_IMAGE);	
	
	
	//myFileDialog->m_ofn.Flags = 0 | OFN_ENABLEHOOK |OFN_EXPLORER | OFN_ENABLESIZING | OFN_PATHMUSTEXIST |OFN_FILEMUSTEXIST | OFN_FORCESHOWHIDDEN;
	myFileDialog->m_ofn.Flags = OFN_PATHMUSTEXIST |OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_FORCESHOWHIDDEN;
	myFileDialog->m_ofn.hwndOwner = m_hWnd;
	myFileDialog->m_ofn.lpstrFilter = (LPCTSTR)filter;
	myFileDialog->m_ofn.lpstrTitle = (LPCTSTR)title;
	//if (_traveler_USB_drive.GetLength() )
		//myFileDialog->m_ofn.lpstrInitialDir = (LPCTSTR)_traveler_USB_drive;

	CString image_path, image_file_name;
	
	if (myFileDialog->DoModal() == IDOK) {
		
		m_FullPath= myFileDialog->GetPathName();
		AddPossibleDiskImage(m_FullPath);

		int intemidx = SendDlgItemMessage( IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)m_FullPath );
		SendDlgItemMessage( IDC_COMBO1, CB_SETCURSEL, intemidx, 0L );		
		

	} else
		return;
}
