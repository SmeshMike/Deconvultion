
// DeconvultionSmeshDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DeconvultionSmesh.h"
#include "DeconvultionSmeshDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define KOORD(x,y) (xp*((x)-xmin)),(yp*((y)-ymax)) // макрос перевода координат для графика 1
#define KOORD2(x,y) (xp2*((x)-xmin2)),(yp2*((y)-ymax2)) // макрос перевода координат для графика 2
#define KOORD3(x,y) (xp3*((x)-xmin3)),(yp3*((y)-ymax3)) // макрос перевода координат для графика 3



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDeconvultionSmeshDlg dialog



CDeconvultionSmeshDlg::CDeconvultionSmeshDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DECONVULTIONSMESH_DIALOG, pParent)
	, ampl1(1)
	, ampl2(3)
	, ampl3(3)
	, vert1(10)
	, vert2(28)
	, vert3(38)
	, width1(5)
	, width2(3)
	, width3(3)
	, size(45)
	, impulseAmpl(3)
	, impulseWidth(5)
	, functional(0)
	, sqrtSum(0)
	, epsilin(1e-5)
	, noisePerc(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeconvultionSmeshDlg::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_AMPL1, ampl1);
	DDX_Text(pDX, IDC_AMPL2, ampl2);
	DDX_Text(pDX, IDC_AMPL3, ampl3);
	DDX_Text(pDX, IDC_VERTEX1, vert1);
	DDX_Text(pDX, IDC_VERTEX2, vert2);
	DDX_Text(pDX, IDC_VERTEX3, vert3);
	DDX_Text(pDX, IDC_WIDTH1, width1);
	DDX_Text(pDX, IDC_WIDTH2, width2);
	DDX_Text(pDX, IDC_WIDTH3, width3);
	DDX_Text(pDX, IDC_COUNT, size);
	DDX_Text(pDX, IDC_IMPAMP, impulseAmpl);
	DDX_Text(pDX, IDC_IMPWIDTH, impulseWidth);
	DDX_Text(pDX, IDC_FUNC, functional);
	DDX_Text(pDX, IDC_SQRT, sqrtSum);
	DDX_Text(pDX, IDC_EPS, epsilin);
	DDX_Text(pDX, IDC_PERC, noisePerc);
}

BEGIN_MESSAGE_MAP(CDeconvultionSmeshDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUN, &CDeconvultionSmeshDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_RECOVER, &CDeconvultionSmeshDlg::OnBnClickedRecover)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDeconvultionSmeshDlg message handlers

BOOL CDeconvultionSmeshDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	check_x = 0;
	check_y = 0;
	check_xx = 0;
	srand(time(NULL));
	// перья
	setka_pen.CreatePen(		//для сетки
		PS_DOT,					//пунктирная
		1,						//толщина 1 пиксель
		RGB(0, 0, 255));		//цвет серый

	osi_pen.CreatePen(			//координатные оси
		PS_SOLID,				//сплошная линия
		3,						//толщина 3 пикселя
		RGB(0, 0, 0));			//цвет черный

	graf_pen.CreatePen(			//график
		PS_SOLID,				//сплошная линия
		3,						//толщина 4 пикселя
		RGB(0, 0, 0));			//цвет черный

	h_pen.CreatePen(		//аппроксимация
		PS_SOLID,				//плошная линия
		2,						//толщина 3 пиксель
		RGB(255, 0, 0));		//цвет красный

	xx_pen.CreatePen(		//аппроксимация
		PS_SOLID,				//плошная линия
		2,						//толщина 3 пиксель
		RGB(0, 0, 255));		//цвет красный

	pryamougol_with_overlap_pen.CreatePen(		//аппроксимация
		PS_SOLID,				//плошная линия
		2,						//толщина 3 пиксель
		RGB(50, 100, 255));		//цвет красный

	//для графика 1
	XWnd = GetDlgItem(IDC_SIGNAL);
	XDc = XWnd->GetDC();
	XWnd->GetClientRect(&XRect);

	//для графика 2
	YWnd = GetDlgItem(IDC_STACK);
	YDc = YWnd->GetDC();
	YWnd->GetClientRect(&YRect);

	//для графика 3
	XXWnd = GetDlgItem(IDC_REC);
	XXDc = XXWnd->GetDC();
	XXWnd->GetClientRect(&XXRect);


	translator_otklonenia_edit = GetDlgItem(IDC_SQRT);
	translator_functional_edit = GetDlgItem(IDC_FUNC);

	//начальная область построения для графика 1
	//начальная область построения для графика 1
	xmin = 0;			//минимальное значение х-
	xmax = size;			//максимальное значение х
	ymax = 10;
	ymin = -ymax / 14.;		//минимальное значение y
	//максимальное значение y

	//начальная область построения для графика 2
	xmin2 = 0;			//минимальное значение х-
	xmax2 = size;			//максимальное значение х
	ymax2 = 10;   //максимальное значение y
	ymin2 = -ymax2 / 14.;		//минимальное значение y

	//начальная область построения для графика 3
	xmin3 = 0;			//минимальное значение х-
	xmax3 = size;			//максимальное значение х
	ymax3 = 10;   //максимальное значение y
	ymin3 = -ymax3 / 14.;		//минимальное значение y

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDeconvultionSmeshDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDeconvultionSmeshDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CDC* XDcMemory = new CDC();
		CDC* YDcMemory = new CDC();
		CDC* XXDcMemory = new CDC();

		XDcMemory->CreateCompatibleDC(XDc);
		YDcMemory->CreateCompatibleDC(YDc);
		XXDcMemory->CreateCompatibleDC(XXDc);

		CBitmap x_bmp;
		CBitmap y_bmp;
		CBitmap xx_bmp;

		x_bmp.CreateCompatibleBitmap(XDc, XRect.Width(), XRect.Height());
		y_bmp.CreateCompatibleBitmap(YDc, YRect.Width(), YRect.Height());
		xx_bmp.CreateCompatibleBitmap(XXDc, XXRect.Width(), XXRect.Height());

		CBitmap* x_tmp = XDcMemory->SelectObject(&x_bmp);
		CBitmap* y_tmp = YDcMemory->SelectObject(&y_bmp);
		CBitmap* xx_tmp = XXDcMemory->SelectObject(&xx_bmp);
		//пересчёты для графика 1
		xp = ((double)(XRect.Width()) / (double)(xmax - xmin));			//Коэффициенты пересчёта координат по Х
		yp = -((double)(XRect.Height()) / (double)(ymax - ymin));		//Коэффициенты пересчёта координат по У

		//пересчёты для графика 2
		xp2 = ((double)(YRect.Width()) / (double)(xmax2 - xmin2));			//Коэффициенты пересчёта координат по Х
		yp2 = -((double)(YRect.Height()) / (double)(ymax2 - ymin2));		//Коэффициенты пересчёта координат по У

		//пересчёты для графика 3
		xp3 = ((double)(XXRect.Width()) / (double)(xmax3 - xmin3));			//Коэффициенты пересчёта координат по Х
		yp3 = -((double)(XXRect.Height()) / (double)(ymax3 - ymin3));		//Коэффициенты пересчёта координат по У

		///////////////////////////////////////////////РАБОТА С ГРАФИКОМ ФУНКЦИИ/////////////////////////////////////////////////////////////////////////
		XDcMemory->FillSolidRect(&XRect, RGB(255, 255, 255));			//закрашиваю фон 
		YDcMemory->FillSolidRect(&YRect, RGB(255, 255, 255));			//закрашиваю фон 
		XXDcMemory->FillSolidRect(&XXRect, RGB(255, 255, 255));			//закрашиваю фон

		XDcMemory->SelectObject(&osi_pen);		//выбираем перо
		YDcMemory->SelectObject(&osi_pen);		//выбираем перо
		XXDcMemory->SelectObject(&osi_pen);		//выбираем перо

		//создаём Ось Y
		XDcMemory->MoveTo(KOORD(0, ymax));
		XDcMemory->LineTo(KOORD(0, ymin));
		//создаём Ось Х
		XDcMemory->MoveTo(KOORD(xmin, 0));
		XDcMemory->LineTo(KOORD(xmax, 0));

		//создаём Ось Y
		YDcMemory->MoveTo(KOORD2(0, ymax2));
		YDcMemory->LineTo(KOORD2(0, ymin2));
		//создаём Ось Х
		YDcMemory->MoveTo(KOORD2(xmin2, 0));
		YDcMemory->LineTo(KOORD2(xmax2, 0));

		//создаём Ось Y
		XXDcMemory->MoveTo(KOORD3(0, ymax3));
		XXDcMemory->LineTo(KOORD3(0, ymin3));
		//создаём Ось Х
		XXDcMemory->MoveTo(KOORD3(xmin3, 0));
		XXDcMemory->LineTo(KOORD3(xmax3, 0));

		XDcMemory->SelectObject(&setka_pen);
		YDcMemory->SelectObject(&setka_pen);
		XXDcMemory->SelectObject(&setka_pen);

		//отрисовка сетки по y
		for (double x = 0; x <= xmax; x += xmax / 8)
		{
			XDcMemory->MoveTo(KOORD(x, ymax));
			XDcMemory->LineTo(KOORD(x, ymin));
		}
		//отрисовка сетки по x
		for (double y = 0; y <= ymax; y += ymax / 4)
		{
			XDcMemory->MoveTo(KOORD(xmin, y));
			XDcMemory->LineTo(KOORD(xmax, y));
		}
		////////////////////////////////////////////////////////
		//отрисовка сетки по y
		for (double x = 0; x <= xmax2; x += xmax2 / 8)
		{
			YDcMemory->MoveTo(KOORD2(x, ymax2));
			YDcMemory->LineTo(KOORD2(x, ymin2));
		}
		//отрисовка сетки по x
		for (double y = 0; y <= ymax2; y += ymax2 / 4)
		{
			YDcMemory->MoveTo(KOORD2(xmin2, y));
			YDcMemory->LineTo(KOORD2(xmax2, y));
		}
		////////////////////////////////////////////////////
		//отрисовка сетки по y
		for (double x = 0; x <= xmax3; x += xmax3 / 8)
		{
			XXDcMemory->MoveTo(KOORD3(x, ymax3));
			XXDcMemory->LineTo(KOORD3(x, ymin3));
		}
		//отрисовка сетки по x
		for (double y = 0; y <= ymax3; y += ymax3 / 4)
		{
			XXDcMemory->MoveTo(KOORD3(xmin3, y));
			XXDcMemory->LineTo(KOORD3(xmax3, y));
		}
		////////////////////////////////////////////////////
		//подпись точек на оси
		CFont font;
		font.CreateFontW(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS || CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Times New Roman"));
		XDcMemory->SelectObject(font);
		//по Y
		for (double i = 0; i <= ymax; i += ymax / 4)
		{
			CString str;
			str.Format(_T("%.2f"), i);
			XDcMemory->TextOutW(KOORD(xmax / 60., i - (ymax - ymin) / 100.), str);
		}
		//по X
		for (double j = xmax / 8; j <= xmax; j += xmax / 8)
		{
			CString str;
			str.Format(_T("%.2f"), j);
			XDcMemory->TextOutW(KOORD(j, -ymax / 100.), str);
		}
		////////////////////////////////////////////////////////////

		YDcMemory->SelectObject(font);
		//по Y
		for (double i = 0; i <= ymax2; i += ymax2 / 4)
		{
			CString str;
			str.Format(_T("%.2f"), i);
			YDcMemory->TextOutW(KOORD2(xmax2 / 60., i - (ymax2 - ymin2) / 100.), str);
		}
		//по X
		for (double j = xmax2 / 8; j <= xmax2; j += xmax2 / 8)
		{
			CString str;
			str.Format(_T("%.3f"), j);
			YDcMemory->TextOutW(KOORD2(j, -ymax2 / 100.), str);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//подпись точек на оси

		XXDcMemory->SelectObject(font);
		//по Y с шагом 5
		for (double i = 0; i <= ymax3; i += ymax3 / 4)
		{
			CString str;
			str.Format(_T("%.2f"), i);
			XXDcMemory->TextOutW(KOORD3(xmax3 / 60., i - (ymax3 - ymin3) / 100.), str);
		}
		//по X 
		for (double j = xmax3 / 8; j <= xmax3; j += xmax3 / 8)
		{
			CString str;
			str.Format(_T("%.2f"), j);
			XXDcMemory->TextOutW(KOORD3(j, -ymax3 / 100), str);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (check_x)
		{
			XDcMemory->SelectObject(&graf_pen);
			XDcMemory->MoveTo(KOORD(0, x_pic[0]));
			for (int i = 0; i < size; i++)
			{
				XDcMemory->LineTo(KOORD(i, x_pic[i]));
			}

			XDcMemory->SelectObject(&h_pen);
			XDcMemory->MoveTo(KOORD(0, h_pic[0]));
			for (int i = 0; i < size; i++)
			{
				XDcMemory->LineTo(KOORD(i, h_pic[i]));
			}

		}


		if (check_y)
		{
			YDcMemory->SelectObject(&graf_pen);
			YDcMemory->MoveTo(KOORD2(0, y_pic[0]));
			for (int i = 0; i < size; i++)
			{
				YDcMemory->LineTo(KOORD2(i, y_pic[i]));
			}
		}

		if (check_xx)
		{
			XXDcMemory->SelectObject(&xx_pen);
			XXDcMemory->MoveTo(KOORD3(0, xx_pic[0]));
			for (int i = 0; i < size; i++)
			{
				XXDcMemory->LineTo(KOORD3(i, xx_pic[i]));
			}

			XXDcMemory->SelectObject(&graf_pen);
			XXDcMemory->MoveTo(KOORD3(0, x_pic[0]));
			for (int i = 0; i < size; i++)
			{
				XXDcMemory->LineTo(KOORD3(i, x_pic[i]));
			}
		}

		XDc->BitBlt(0, 0, XRect.Width(), XRect.Height(), XDcMemory, 0, 0, SRCCOPY);
		YDc->BitBlt(0, 0, YRect.Width(), YRect.Height(), YDcMemory, 0, 0, SRCCOPY);
		XXDc->BitBlt(0, 0, XXRect.Width(), XXRect.Height(), XXDcMemory, 0, 0, SRCCOPY);

		delete XDcMemory;
		delete XXDcMemory;
		delete YDcMemory;
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDeconvultionSmeshDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDeconvultionSmeshDlg::OnBnClickedRun()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	check_x = 1;
	check_y = 1;
	check_xx = 0;
	x_pic = new double[size];
	y_pic = new double[size];
	kupola();
	svertka();


	xmax = size;
	xmax2 = size;

	ymax = 0;
	ymax2 = 0;
	for (int i = 0; i < size; i++)
	{
		if (ymax < x_pic[i]) ymax = x_pic[i];
		if (ymax < h_pic[i]) ymax = h_pic[i];

	}

	for (int i = 0; i < size; i++)
	{
		if (ymax2 < y_pic[i]) ymax2 = y_pic[i];
	}

	ymin2 = -ymax2 / 14.;
	ymin = -ymax / 14.;
	Invalidate();
}

void CDeconvultionSmeshDlg::kupola()
{
	for (int i = 0; i < size; i++)
	{
		x_pic[i] = ampl1 * exp(-(i - vert1) * (i - vert1) / width1 / width1) + ampl2 * exp(-(i - vert2) * (i - vert2) / width2 / width2) + ampl3 * exp(-(i - vert3) * (i - vert3) / width3 / width3);
	}
}


void CDeconvultionSmeshDlg::svertka()
{
	h_pic = new double[size];
	for (int i = 0; i <= size / 2; i++)
	{
		h_pic[i] = impulseAmpl * exp(-i * i / impulseWidth / impulseWidth);
		h_pic[size - i - 1] = h_pic[i];
	}

	for (int i = 0; i < size; i++)
	{
		y_pic[i] = 0;
		for (int j = 0; j < size; j++)
		{
			if ((i - j) < 0) y_pic[i] += x_pic[j] * h_pic[size + (i - j) - 1];
			else y_pic[i] += x_pic[j] * h_pic[i - j];
		}
	}


	//////////создание шума/////////////////
	double* shum_n = new double[size];
	double alfa;
	for (int i = 0; i < size; i++)
	{
		shum_n[i] = 0;
	}

	double sum_signal = 0;
	double sum_shum = 0;

	for (int i = 0; i < size; i++)
	{
		sum_signal += y_pic[i] * y_pic[i];
	}


	for (int i = 0; i < size; i++)
	{
		double M, ksi;
		M = rand() % 9 + 12;
		ksi = 0;
		for (int k = 1; k <= M; k++)
		{
			ksi += (double)((rand() % 21 - 10) / 10.);
		}
		shum_n[i] = ksi / M;
	}

	for (int i = 0; i < size; i++)
	{
		sum_shum += shum_n[i] * shum_n[i];
	}

	alfa = sqrt(sum_signal / sum_shum * noisePerc / 100.f);

	for (int i = 0; i < size; i++)
	{
		y_pic[i] = y_pic[i] + alfa * shum_n[i];
	}

	for (int i = 0; i < size; i++)
	{
		if (y_pic[i] < 0) y_pic[i] = 0;
	}

	delete[]shum_n;

}

float CDeconvultionSmeshDlg::function(float* lamda)
{
	// Реализует оптимизируемую функцию
	// Возвращает значение функции
	// количество параметров является членом класса, в противном случае изменить сигнатуру функции
	//	........
	double functional = 0;

	double* buffer = new double[size];
	for (int k = 0; k < size; k++)
	{
		double sum = 0;
		for (int j = 0; j < size; j++)
		{
			if ((j - k) < 0) sum += lamda[j] * h_pic[size + (j - k) - 1];
			else sum += lamda[j] * h_pic[j - k];
		}
		buffer[k] = exp(-1 - sum);
	}

	for (int m = 0; m < size; m++)
	{
		double sum = 0;
		for (int k = 0; k < size; k++)
		{
			if ((m - k) < 0) sum += buffer[k] * h_pic[size + (m - k) - 1];
			else sum += buffer[k] * h_pic[m - k];
		}
		functional += (y_pic[m] - sum) * (y_pic[m] - sum);
	}
	delete[] buffer;
	return functional;
}

void CDeconvultionSmeshDlg::OnBnClickedRecover()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	check_xx = 1;

	b = new float[size];
	y = new float[size];
	p = new float[size];
	lamda = new float[size];
	xx_pic = new double[size];


	h = 1.;
	lamda[0] = 1.;
	for (int i = 1; i < size; i++)  lamda[i] = 0;// (float)rand() / RAND_MAX; // Задается начальное приближение

	k = h;
	for (int i = 0; i < size; i++)	y[i] = p[i] = b[i] = lamda[i];
	fi = function(lamda);
	ps = 0;   bs = 1;  fb = fi;

	j = 0;
	SetTimer(1, 20, NULL);
}


void CDeconvultionSmeshDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);
	while (j < size)
	{
		lamda[j] = y[j] + k;
		z = function(lamda);
		if (z >= fi)
		{
			lamda[j] = y[j] - k;
			z = function(lamda);
			if (z < fi)   y[j] = lamda[j];
			else  lamda[j] = y[j];
		}
		else  y[j] = lamda[j];
		fi = function(lamda);

		j++;
	}
	if (fi + 1e-8 >= fb)
	{
		if (ps == 1 && bs == 0)
		{
			for (i = 0; i < size; i++)
			{
				p[i] = y[i] = lamda[i] = b[i];
			}
			z = function(lamda);
			bs = 1;   ps = 0;   fi = z;   fb = z;   j = 0;
		}
		k /= 10.;
		if (k < epsilin)
		{
			KillTimer(1);
			//delete[] lamda;
			MessageBoxA(NULL, "Complete", "Message", MB_OK);
		}
		j = 0;

	}

	for (i = 0; i < size; i++)
	{
		p[i] = 2 * y[i] - b[i];
		b[i] = y[i];
		lamda[i] = p[i];

		y[i] = lamda[i];
	}
	z = function(lamda);
	fb = fi;   ps = 1;   bs = 0;   fi = z;   j = 0;


	for (int i = 0; i < size; i++)
	{
		double sum = 0;
		for (int j = 0; j < size; j++)
		{
			if ((j - i) < 0)	sum += lamda[j] * h_pic[size + (j - i) - 1];
			else	sum += lamda[j] * h_pic[j - i];
		}
		xx_pic[i] = exp(-1 - sum);
	}

	double translator_buffer = 0;
	for (int i = 0; i < size; i++)
	{
		translator_buffer += (x_pic[i] - xx_pic[i]) * (x_pic[i] - xx_pic[i]);
	}

	sqrtSum = translator_buffer / size;
	functional = fi;

	CString str;
	str.Format(_T("%.9f"), functional);
	translator_functional_edit->SetWindowTextW(str);

	str.Format(_T("%.9f"), sqrtSum);
	translator_otklonenia_edit->SetWindowTextW(str);

	xmax3 = size;
	ymax3 = 0;
	for (int i = 0; i < size; i++)
	{
		if (ymax3 < xx_pic[i]) ymax3 = xx_pic[i];
		if (ymax3 < x_pic[i]) ymax3 = x_pic[i];

	}
	ymin3 = -ymax3 / 14.;


	OnPaint();
	CDialogEx::OnTimer(nIDEvent);

}
