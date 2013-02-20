#include <kwinui/kcstcmnctrls.h>
#include <kwinui/kapp.h>
#include "resource.h"

using namespace kwinui;

class KRndSliderDemoDlg : public KDialogBase<KRndSliderDemoDlg>{
public:
	K_BEGIN_MSG_MAP
		K_MSG_HANDLER(WM_INITDIALOG,OnInitDialog)
		K_COMMAND_ID_HANDLER(IDOK,OnOK)
		K_COMMAND_ID_HANDLER(IDCANCEL,OnOK)
	K_END_MSG_MAP(KDialogBase<KRndSliderDemoDlg>)

	enum{IDD=IDD_DLG_ROUND_SLIDER};
	
	LRESULT OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,bool& bHandled){
		m_rscSlider1.SubclassDlgItem(IDC_SLIDER1,*this);
		m_rscSlider2.SubclassDlgItem(IDC_SLIDER2,*this);

		m_rscSlider1.SetRange(-179, 180, FALSE);
		m_rscSlider1.SetPos(42);
		m_rscSlider1.SetZero(90);
		m_rscSlider1.SetInverted();

		m_rscSlider1.SetDialColor(RGB(255, 255, 0));
		m_rscSlider1.SetKnobColor(RGB(0, 0, 255));

		m_rscSlider2.SetRange(875, 1080, FALSE);
		m_rscSlider2.SetPos(948);
		m_rscSlider2.SetZero(180);
		m_rscSlider2.SetRadioButtonStyle();	


		m_rscSlider2.SetFontName(_T("Comic Sans MS"));
		m_rscSlider2.SetFontSize(14);
		m_rscSlider2.SetFontItalic();
		m_rscSlider2.SetTextColor(RGB(0, 0, 255));

		CentralizeWindow();
		return TRUE;
	}
	LRESULT OnOK(WORD wID,WORD wNotifyCode,HWND hWndCtrl,bool& bHandled){
		EndDialog(wID);
		return 0;
	}
private:
	KRoundSlider	m_rscSlider1,m_rscSlider2;
};

class KRndSliderDemo : public KWinApp<KRndSliderDemo>{
public:
	bool InitInstance(){
		KRndSliderDemoDlg dlg;
		dlg.DoModal();
		return false;
	}
};

KRndSliderDemo theApp;
