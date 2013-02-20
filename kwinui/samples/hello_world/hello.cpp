#include <kwinui/kwin.h>
#include <kwinui/kapp.h>

using namespace kwinui;

class KMainWindow : public KWindowBase<KMainWindow>{
public:
	KMainWindow():KWindowBase<KMainWindow>(_T("MyClassName")){}

	K_BEGIN_MSG_MAP
		K_MSG_HANDLER(WM_PAINT,OnPaint)
    K_end_msg_MAP(KWindowBase<KMainWindow>)

	LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		PAINTSTRUCT ps;
		HDC hDC;

		RECT rect;
		::GetClientRect(m_hWnd,&rect);
		
		hDC=::BeginPaint(m_hWnd,&ps);
		::SetBkMode(hDC,TRANSPARENT);
		::DrawText(hDC,_T("Hell World!"),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		::EndPaint(m_hWnd,&ps);

		return 0;
	}
};

class KHelloApp : public KWinApp<KHelloApp>{
public:
	bool InitInstance(){
		m_pMainWindow=new KMainWindow();
		m_pMainWindow->CreateOverlappedWindow(_T("Hello World!"));
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

KHelloApp theApp;
