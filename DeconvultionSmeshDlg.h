
// DeconvultionSmeshDlg.h : header file
//

#pragma once


// CDeconvultionSmeshDlg dialog
class CDeconvultionSmeshDlg : public CDialogEx
{
// Construction
public:
	CDeconvultionSmeshDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DECONVULTIONSMESH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedRecover();
	
	double ampl1;
	double ampl2;
	double ampl3;
	double vert1;
	double vert2;
	double vert3;
	double width1;
	double width2;
	double width3;
	int size;
	double impulseAmpl;
	double impulseWidth;
	double functional;
	double sqrtSum;
	double epsilin;
	double noisePerc;
	//Переменные для работы с масштабом
	double xp, ymin, ymax, yp, xmin, xmax, //для графика 
		xmin2, xmax2, xp2, ymin2, ymax2, yp2,//для графика 
		xp3, yp3, xmax3, xmin3, ymax3, ymin3;  //для 

	BOOL check_x, check_y, check_xx;
	double* x_pic;
	double* h_pic;
	double* y_pic;
	double* xx_pic;
	float* lamda;
	void svertka();	void kupola();
	float MHJ(int kk, float* x);
	float function(float* lam);
	//связки для модели
	CWnd* XWnd;
	CDC* XDc;
	CRect XRect;

	float z, h, k, fi, fb;
	float* b;
	float* y;
	float* p;
	int i, j, bs, ps;
	//связки для спектра
	CWnd* YWnd;
	CDC* YDc;
	CRect YRect;

	//связки для эксперимента
	CWnd* XXWnd;
	CDC* XXDc;
	CRect XXRect;

	CWnd* translator_otklonenia_edit;
	CWnd* translator_functional_edit;

		//объявление ручек
	CPen osi_pen;		// ручка для осей
	CPen setka_pen;		// для сетки
	CPen graf_pen;		// для графика функции
	CPen h_pen;	// 
	CPen xx_pen;
	CPen pryamougol_with_overlap_pen;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
