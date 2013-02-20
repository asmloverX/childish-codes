#include <kwinui/kwin.h>
#include <kwinui/kapp.h>

using namespace kwinui;

class KMainWindow : public KWindowBase<KMainWindow>{
public:
	KMainWindow():KWindowBase<KMainWindow>(_T("transparent")){}

	K_BEGIN_MSG_MAP
		K_MSG_HANDLER(WM_CREATE,OnCreate)
	K_END_MSG_MAP(KWindowBase<KMainWindow>)
	
	bool PreCreateWindow(CREATESTRUCT& cs){
		cs.dwExStyle|=WS_EX_LAYERED;
		return true;
	}

	LRESULT OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		SetLayeredWindowAttributes();
		return 0;
	}
};

class KHelloApp : public KWinApp<KHelloApp>{
public:
	bool InitInstance(){
		m_pMainWindow=new KMainWindow();
		m_pMainWindow->CreateOverlappedWindow(_T("transparent window"));
		m_pMainWindow->ShowWindow(m_nCmdShow);
		m_pMainWindow->UpdateWindow();
		
		return true;
	}
	void ExitInstance(){
		SAFE_DEL_PTR(m_pMainWindow);
	}
	
private:
	KMainWindow* m_pMainWindow;
};

KHelloApp hello;
