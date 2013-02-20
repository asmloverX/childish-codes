#include "resource.h"
#include <kwinui/kapp.h>
#include <kwinui/kwin.h>

using namespace kwinui;

class KDlgMain : public KDialogBase<KDlgMain>{
public:
	typedef KDialogBase<KDlgMain> __base;
	enum{IDD=IDD_DLG_MAIN};

	K_BEGIN_MSG_MAP
		K_COMMAND_ID_HANDLER(IDOK,OnOK)
		K_COMMAND_ID_HANDLER(IDCANCEL,OnCancel)
	K_END_MSG_MAP(__base)

	LRESULT OnOK(WORD wID,WORD wNotifyCode,HWND hWndCtrl,bool& bHandled){
		ShowMessage(_T("KWinUI dialog based program sample!"));
		return TRUE;
	}

	LRESULT OnCancel(WORD wID,WORD wNotifyCode,HWND hWndCtrl,bool& bHandled){
		EndDialog(wID);
		::PostQuitMessage(0);
		return TRUE;
	}
};

class KDlgApp : public KWinApp<KDlgApp>{
public:
	bool InitInstance(){
		m_pMainWindow=new KDlgMain();
		m_pMainWindow->Create(NULL);
		return true;
	}
private:
	KDlgMain* m_pMainWindow;
};

KDlgApp theApp;
