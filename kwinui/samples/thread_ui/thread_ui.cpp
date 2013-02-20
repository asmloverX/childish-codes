#include <kwinui/kwin.h>
#include <kwinui/kctrls.h>
#include <kwinui/kapp.h>

using namespace kwinui;

class KMainWindow : public KWindowBase<KMainWindow>{
public:
	KMainWindow():KWindowBase<KMainWindow>(_T("MyClassName")){}
	K_BEGIN_MSG_MAP
		K_MSG_HANDLER(WM_CREATE,OnCreate)
		K_COMMAND_ID_HANDLER(1000,OnExit)
	K_END_MSG_MAP(KWindowBase<KMainWindow>)

	LRESULT OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		RECT rect1={30,30,90,60};
		m_btnExit.Create(*this,_T("exit"),WS_CHILD|WS_VISIBLE|WS_TABSTOP,rect1,1000);
		return 0;
	}
	LRESULT OnExit(WORD wID,WORD wNotifyCode,HWND hWndCtrl,bool& bHandled){
		m_nRetCode=wID;
		SendMessage(WM_CLOSE);
		return 0;
	}
private:
    int     m_nRetCode;
	KButton	m_btnExit;
};

class KWorkThread : public KThreadImpl<KWorkThread>{
public:
	KWorkThread():KThreadImpl<KWorkThread>(CREATE_SUSPENDED){}
	bool InitInstance(){
		m_pMainWindow=new KMainWindow();
		m_pMainWindow->CreateOverlappedWindow(_T("work thread's main window"));
		return true;
	}
	void ExitInstance(){
		SAFE_DEL_PTR(m_pMainWindow);
	}
public:
	KMainWindow* m_pMainWindow;
};

class KThreadUIApp : public KWinApp<KThreadUIApp>{
public:
	KThreadUIApp():m_pMainWindow(0),m_pWorkThread(0){}
	bool InitInstance(){
		m_pWorkThread=new KWorkThread();
		m_pWorkThread->ResumeThread();

		m_pMainWindow=new KMainWindow();
		m_pMainWindow->CreateOverlappedWindow(_T("main thread's main window"));

		return true;
	}
	void ExitInstance(){
		SAFE_DEL_PTR(m_pMainWindow);
		SAFE_DEL_PTR(m_pWorkThread);
	}
	
private:
	KMainWindow* 	m_pMainWindow;
	KWorkThread*	m_pWorkThread;
};

KThreadUIApp theApp;
