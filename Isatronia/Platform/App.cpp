//***************************************************************************************
// App.cpp by Aaron Jsc (C) 2020 All Rights Reserved.
// licenced under CC BY-NC-SA License.
//***************************************************************************************



#include "App.h"
#include "../Exceptions/Exception.h"
#include "../../libs/dxerr.cpp"



namespace Isatronia::Windows {
	using namespace Exception;

	LRESULT CALLBACK
		AppMainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// Forward hwnd on because we can get messages (e.g., WM_CREATE)
		// before CreateWindow returns, and thus before mhMainWnd is valid.
		return App::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
	}

	App* App::mApp = nullptr;

	App::App(HINSTANCE hInstance)
	{
		mhAppInst = hInstance;
		mhMainWnd = NULL;

		// member Init
		mAppPaused = false;
		mMinimized = false;
		mMaximized = false;
		mResizeing = false;

		mMainWndCaption	=	WINDOW_CAPTION;
		mClientWidth	=	CLIENT_WIDTH;
		mClientHeight	=	CLIENT_HEIGHT;
		mClientBPP		=	CLIENT_BPP;
		assert(mApp == nullptr);
		mApp = this;
		return;
	}

	App::~App()
	{
		return;
	}

	LRESULT App::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// TODO: �ڴ˴����ʵ�ִ���.
		switch (msg)
		{
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				// when the Window is inactive, pause the game.
				mAppPaused = true;
				mTimer.Stop();
			}
			else
			{
				mAppPaused = false;
				mTimer.Start();
			}
			return 0;
			break;
		case WM_COMMAND:
		{
		}
		break;
		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_F2:
				mAppPaused = !mAppPaused;
				break;
			default:
				break;
			}
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
			EndPaint(hwnd, &ps);
		}
		break;
		case WM_MOVE:
		{
			OnResize();
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}

	App* App::GetApp()
	{
		// TODO: �ڴ˴����ʵ�ִ���.
		return mApp;
	}

	HINSTANCE App::getAppInst()
	{
		// TODO: �ڴ˴����ʵ�ִ���.
		return mhAppInst;
	}

	HWND App::getMainWnd()
	{
		// TODO: �ڴ˴����ʵ�ִ���.
		return mhMainWnd;
	}

	float App::getAspectRatio()
	{
		// TODO: �ڴ˴����ʵ�ִ���.
		return (float)mClientWidth / mClientHeight;
	}

	int App::Run()
	{
		// TODO: �ڴ˴����ʵ�ִ���.
		MSG msg = { 0 };

		mTimer.Reset();

		ULONGLONG t1 = GetTickCount64(), t2 = GetTickCount64();

		while (msg.message != WM_QUIT)
		{
			// If there are Window messages then process them.
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			// Otherwise, do animation/game stuff.
			else
			{
				t1 = GetTickCount64();
				if (t1 - t2 > 33)
				{
					if (!mAppPaused)
					{
						Update(mTimer);
					}
					else
					{
						Sleep(100);
					}
					t2 = GetTickCount64();
				}
			}
		}

		return 0;
	}

	void App::Initialize()
	{
		if (!InitMainWindow())
		{
			MessageBoxA(0, "InitWindow Failed.", "", MB_OK);
			throw(new RuntimeException(L"InitWindow Failed."));
			return;
		}
		OnResize();

		return;
	}

	void App::OnResize()
	{
		if (mhMainWnd != nullptr)
		{
			GetWindowRect(mhMainWnd, &mClientRect);
		}
		return;
	}

	void App::Update(const Timer& Timer)
	{
		
	}

	//void App::Draw(const UvcTimer& Timer)
	//{
	//	// TODO: �ڴ˴����ʵ�ִ���.
	//}

	

	bool App::InitMainWindow()
	{
		// TODO: �ڴ˴����ʵ�ִ���.
		WNDCLASSEX wc;
		if (mhAppInst == nullptr)
		{
			MessageBoxW(0, L"Null instance.", 0, MB_OK);
			return false;
		}

		memset(&wc, 0, sizeof(wc));

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_DBLCLKS | CS_OWNDC |
			CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = AppMainWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = mhAppInst;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"MainWnd";
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wc))
		{
			MessageBoxW(0, L"RegisterClass Failed.", 0, MB_OK);
			return false;
		}

		RECT r = { 0,0,mClientWidth, mClientHeight };
		AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
		int width = r.right - r.left;
		int height = r.bottom - r.top;

		if (!(mhMainWnd = CreateWindowEx(

			NULL,														// extrened style
			L"MainWnd",													// class
			mMainWndCaption.c_str(),									//title
			WS_OVERLAPPEDWINDOW,										// windows style //| WS_VISIBLE
			0,															// initial x, y
			0,
			mClientWidth,												// initial width, height
			mClientHeight,
			NULL,														// handle to parent
			NULL,														// handle to menu
			mhAppInst,													// instance of this application
			NULL
		)))
		{
			MessageBoxW(0, L"CreateWindow Failed.", 0, MB_OK);
			return false;
		}

		ShowWindow(mhMainWnd, SW_SHOW);
		UpdateWindow(mhMainWnd);

		return true;
	}

	

	bool App::CreatCommandObjects()
	{
		// TODO: �ڴ˴����ʵ�ִ���.
		return true;
	}

	

	void App::FlushCommandQueue()
	{
		// TODO: �ڴ˴����ʵ�ִ���.
	}
	
}
/*

// ��������dxerr�ͱ������ĺ���

// ------------------------------
// DXTraceW����
// ------------------------------
// �ڵ�����������������ʽ��������Ϣ����ѡ�Ĵ��󴰿ڵ���(�Ѻ���)
// [In]strFile			��ǰ�ļ�����ͨ�����ݺ�__FILEW__
// [In]hlslFileName     ��ǰ�кţ�ͨ�����ݺ�__LINE__
// [In]hr				����ִ�г�������ʱ���ص�HRESULTֵ
// [In]strMsg			���ڰ������Զ�λ���ַ�����ͨ������L#x(����ΪNULL)
// [In]bPopMsgBox       ���ΪTRUE���򵯳�һ����Ϣ������֪������Ϣ
// ����ֵ: �β�hr
HRESULT WINAPI DXTraceW(_In_z_ const WCHAR* strFile, _In_ DWORD dwLine, _In_ HRESULT hr,
	_In_opt_ const WCHAR* strMsg, _In_ bool bPopMsgBox)
{
	WCHAR strBufferFile[MAX_PATH];
	WCHAR strBufferLine[128];
	WCHAR strBufferError[300];
	WCHAR strBufferMsg[1024];
	WCHAR strBufferHR[40];
	WCHAR strBuffer[3000];

	swprintf_s(strBufferLine, 128, L"%lu", dwLine);
	if (strFile)
	{
		swprintf_s(strBuffer, 3000, L"%ls(%ls): ", strFile, strBufferLine);
		OutputDebugStringW(strBuffer);
	}

	size_t nMsgLen = (strMsg) ? wcsnlen_s(strMsg, 1024) : 0;
	if (nMsgLen > 0)
	{
		OutputDebugStringW(strMsg);
		OutputDebugStringW(L" ");
	}
	// Windows SDK 8.0��DirectX�Ĵ�����Ϣ�Ѿ����ɽ��������У�����ͨ��FormatMessageW��ȡ������Ϣ�ַ���
	// ����Ҫ�����ַ����ڴ�
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		strBufferError, 256, nullptr);

	WCHAR* errorStr = wcsrchr(strBufferError, L'\r');
	if (errorStr)
	{
		errorStr[0] = L'\0';	// ����FormatMessageW�����Ļ��з�(��\r\n��\r�û�Ϊ\0����)
	}

	swprintf_s(strBufferHR, 40, L" (0x%0.8x)", hr);
	wcscat_s(strBufferError, strBufferHR);
	swprintf_s(strBuffer, 3000, L"�����뺬�壺%ls", strBufferError);
	OutputDebugStringW(strBuffer);

	OutputDebugStringW(L"\n");

	if (bPopMsgBox)
	{
		wcscpy_s(strBufferFile, MAX_PATH, L"");
		if (strFile)
			wcscpy_s(strBufferFile, MAX_PATH, strFile);

		wcscpy_s(strBufferMsg, 1024, L"");
		if (nMsgLen > 0)
			swprintf_s(strBufferMsg, 1024, L"��ǰ���ã�%ls\n", strMsg);

		swprintf_s(strBuffer, 3000, L"�ļ�����%ls\n�кţ�%ls\n�����뺬�壺%ls\n%ls����Ҫ���Ե�ǰӦ�ó�����",
			strBufferFile, strBufferLine, strBufferError, strBufferMsg);

		int nResult = MessageBoxW(GetForegroundWindow(), strBuffer, L"����", MB_YESNO | MB_ICONERROR);
		if (nResult == IDYES)
			DebugBreak();
	}

	return hr;
}

#endif

// �Ͼɵı������ı��洴��������

	// TODO: �ڴ˴����ʵ�ִ���.
	memset(&mDDSD, 0, sizeof(mDDSD));
	mDDSD.dwSize = sizeof(mDDSD);
	mDDSD.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	mDDSD.dwBackBufferCount = mCurBackBuffer;
	mDDSD.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;	//primary surface

	HRESULT debug = 0;

	if (FAILED(debug = mlpDD7->CreateSurface(&mDDSD, &mlpDDSurefacePrimary, NULL)))
	{
		__POSTERR(L"Primery Surface Creat Failed");
#ifdef DEBUG
		DXTraceW(__FILEW__, __LINE__, debug, L"Primery Surface Creat Failed", TRUE);
#endif // DEBUG
		return false;
	}

	mDDSD.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

	UvcDXInit_s(
		mlpDDSurefacePrimary->GetAttachedSurface(&mDDSD.ddsCaps, &mlpDDSurefaceBackBuffer),
		L"Back Buffer Surface Creat Failed"
	);

*/