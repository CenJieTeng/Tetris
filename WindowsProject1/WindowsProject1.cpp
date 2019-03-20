//WindowsProject1.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "client.h"
#include "WindowsProject1.h"
#pragma comment(lib, "WINMM.LIB")
#define WIDTH  12
#define HEIGHT 15
#define TIMER_ID1 1
#define HALF(x) (x/2)
#define HOME_IDB_ONE 1001 //单人游戏
#define HOME_IDB_TWO 1002 //多人游戏
#define HOME_IDB_THREE 1003 //游戏说明
#define SINGLE_IDB_ONE 2001 //退出按钮
#define MULT_IDB_ONE 3001 //刷新
#define MULT_IDB_TWO 3002 //创建房间
#define MULT_IDB_THREE 3003 //加入房间
#define MULT_IDB_FOUR 3004 //离开房间
#define MULT_IDB_FIVE 3005 //开始游戏

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
boost::asio::io_service io_service_;
boost::asio::ip::tcp::resolver resolver_(io_service_);
boost::asio::ip::tcp::resolver::query query_("127.0.0.1", "9999");
client client_(io_service_, resolver_.resolve(query_)); //客户端类
static std::thread thread_client([] { io_service_.run(); }); //创建一个线程，用于跑网络相关的服务

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。
	//随机数种子
	srand(static_cast<unsigned int>(time(NULL)));

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_EX_LAYERED | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
      200, 50, WIDTH * 50 + 200, HEIGHT * 50, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//设置一组按钮的可见性
void SetTotalButtonVisible(HWND hWnd[9], bool status[9]) {
	for (int i = 0; i < 9; ++i) {
		if (status[i]) {
			ShowWindow(hWnd[i], SW_SHOW);
		}
		else{
			ShowWindow(hWnd[i], SW_HIDE);
		}
	}
}

void ShowCurRoom(HWND hWnd, const HFONT &hFont)
{
	//显示当前房间号
	HDC hdc = GetDC(hWnd);
	std::wstring wstr = L"当前所在房间号：";
	wstr += std::to_wstring((int)client_.roomId());
	wstr += L" ";
	SelectObject(hdc, hFont);
	SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	TextOut(hdc, 0, 0, wstr.c_str(), wstr.size());
	SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
	DeleteDC(hdc);
}

//绘制地图
void DrawMap(HDC hdc, int apart, int col, int colRange, int row, int rowRange)
{
	SelectObject(hdc, GetStockObject(WHITE_PEN));

	//画竖线
	for (int i = 0; i <= col; i++)
	{
		if (i == col)
			SelectObject(hdc, GetStockObject(BLACK_PEN));

		MoveToEx(hdc, apart * i, 0, nullptr);
		LineTo(hdc, apart * i, colRange);
	}

	SelectObject(hdc, GetStockObject(WHITE_PEN));

	//画横线
	for (int i = 0; i <= row; i++)
	{
		MoveToEx(hdc, 0, apart * i, nullptr);
		LineTo(hdc, rowRange, apart * i);
	}
}

//随机产生方块
Block* GetRandBlock() {
	Block *bp = nullptr;

	switch (rand() % 12)
	{
	case 0: 
	case 1: bp = new Block_O(); break;
	case 2: 
	case 3: bp = new Block_J(); break;
	case 4: 
	case 5: bp = new Block_L(); break;
	case 6: 
	case 7: bp = new Block_I(); break;
	case 8: 
	case 9: bp = new Block_T(); break;
	case 10: bp = new Block_S(); break;
	case 11: bp = new Block_Z(); break;
	}

	return bp;
}

//制作方块
void CreateBlock(Block *b, int map[WIDTH][HEIGHT], int brCur)
{
	for (int i = 0; i < 4; i++)
	{
		if (map[b->Shape[i].x][b->Shape[i].y] == 0)
			map[b->Shape[i].x][b->Shape[i].y] = brCur;
	}
}

//调整方块出生点
void SetBlockBirthPoint(Block *b, int map[WIDTH][HEIGHT])
{
	for (int i = 0; i < 1; i++)
		b->MoveRightBlovk(map);
}

//绘制方块
void DrawBlock(HDC hdc, int apart, int map[WIDTH][HEIGHT], HBRUSH br[9])
{
	RECT rect;

	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (map[i][j])
			{
				rect.left   = i * apart + 1;
				rect.top    = j * apart + 1;
				rect.right  = rect.left + apart - 1;
				rect.bottom = rect.top + apart - 1;
				
				FillRect(hdc, &rect, br[map[i][j]]);
			}
		}
	}
}

//方块移动
BOOL MoveBlock(Block *b, int map[WIDTH][HEIGHT])
{
	if (!b->MoveDownBlovk(map))
		return FALSE;

	return TRUE;
}

//消除方块
void ReMoveBlock(Block *b, int map[WIDTH][HEIGHT])
{
	for (int i = 0; i < 4; ++i)
	{
		map[b->Shape[i].x][b->Shape[i].y] = 0;
	}
}

//消除行
void Judge(int map[WIDTH][HEIGHT], int &score, int &gradeOfDifficult, int &badRowCount)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		int j = 0;
		for (; j < WIDTH; j++)
		{
			//当前行有空缺，直接跳过
			if (!map[j][i])
				break;
 
			//当前行是badRow,跳过
			if (map[j][i] == 8)
				break;
		}

		//如果检查完一行没有空缺，消除该行
		if (j == WIDTH)
		{
			PlaySound(L"./src/得分.wav", NULL, SND_FILENAME | SND_ASYNC);

			//加分
			score += 100;
			//增加难度
			if (score >= gradeOfDifficult * 500)
			{
				gradeOfDifficult += 1;
			}

			j = 0;
			for (; j < WIDTH; j++)
			{
				map[j][i] = 0;
			}

			//方块下降
			for (int k = i; k > 0; k--)
			{
				j = 0;
				for (; j < WIDTH; j++)
				{
					map[j][k] = map[j][k - 1];
				}
			}

			//如果有badRow则消除一行
			if (badRowCount > 0)
			{
				//方块下降
				for (int i = 0; i < WIDTH; ++i)
					for (int j = HEIGHT - 1; j > 0; --j)
						map[i][j] = map[i][j - 1];

				--badRowCount;
			}
				

			//攻击其他对手
			GameMsg gameMsg;
			gameMsg.set_msgtype(GameMessageType::ATK_FOE_MSG);
			client_.write(MessageType::GAME_MSG, gameMsg);
		}
	}
}

//绘制得分&难度
void DrawScore(HDC hdc, int apart, int score, int gradeOfDifficult)
{
	const int    kBufferMaxSize = 30;
	TCHAR        szBuffer[kBufferMaxSize];
	const size_t kBufferByteCount = kBufferMaxSize * sizeof(TCHAR);
	LPCWSTR pszFormat = L"%s %d        %s %d"; //文本输出格式
	LPCWSTR str1 = L"得分:";
	LPCWSTR str2 = L"难度等级: ";

	StringCbPrintf(szBuffer, kBufferByteCount, pszFormat, str1, score, str2, gradeOfDifficult);
	TextOut(hdc, WIDTH * apart + 40, 10, szBuffer, (int)wcslen(szBuffer));
}


//绘制函数
void Draw(HWND hWnd, HDC hdc, Block *b, const int &cyClient, int map[WIDTH][HEIGHT], HBRUSH br[9], int brCur, int score, int gradeOfDifficult, bool isCreateBlock)
{
	static int apart = cyClient / HEIGHT;
	static int col = WIDTH;
	static int row = HEIGHT;
	static int colRange = row * apart;
	static int rowRange = col * apart;

	DrawMap(hdc, apart, col, colRange, row, rowRange); //绘制地图
	DrawScore(hdc, apart, score, gradeOfDifficult); //绘制分数

	CreateBlock(b, map, brCur);

	DrawBlock(hdc, apart, map, br);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int map[WIDTH][HEIGHT]; //地图
	static HBRUSH br[9]; //画刷
	static int brCur = rand() % 7; //当前画刷
	static int cxClient, cyClient;
	static int elapse = 800; //定时器触发间隔
	static int score = 0; //分数
	static int gradeOfDifficult = 1; //难度等级
	static int badRowCount = 0; //计算badRow有多少
	static bool isCreateBlock = false;
	static enum class EnumInterface{ //标识当前所在界面
		HOME_INTERFACE, //家
		SINGLE_INTERFACE, //单人
		MULT_INTERFACE //多人
	}curInterface = EnumInterface::HOME_INTERFACE; //当前所在界面，默认在 家
	PAINTSTRUCT ps;
	HDC hdc, mdc;

	//字体
	static HFONT hFont = CreateFont(20, 20, 0, 0, FW_THIN, false, false, false,
		CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
		FF_MODERN, L"宋体");

	//图片资源
	static HBITMAP bg_bmp = (HBITMAP)LoadImage(NULL,
		L"./src/天空.bmp", 
		IMAGE_BITMAP, 120, 80, LR_LOADFROMFILE);
	static HBITMAP explain_bmp = (HBITMAP)LoadImage(NULL,
		L"./src/游戏说明.bmp",
		IMAGE_BITMAP, 1152, 648, LR_LOADFROMFILE);
	//图片像素
	POINT bg_size{ 120, 80 };
	POINT button_size{ 93, 21 };

	static HWND buttonHwnd[9]; //按钮句柄
	static HWND roomListHwnd; //listBox句柄

	auto errCode = GetLastError();
	static Block *block = nullptr;

	//优化点2///////////////////////////////////
	//static std::shared_ptr<Block*> b = &bo;
	////////////////////////////////////////////

    switch (message)
    {
	case WM_CREATE: 
	{
		//初始化画刷数组
		br[1] = CreateSolidBrush(RGB(255, 0, 0));
		br[2] = CreateSolidBrush(RGB(255, 125, 0));
	    br[3] = CreateSolidBrush(RGB(255, 255, 0));
		br[4] = CreateSolidBrush(RGB(0, 255, 0));
		br[5] = CreateSolidBrush(RGB(0, 0, 255));
		br[6] = CreateSolidBrush(RGB(0, 255, 255));
		br[7] = CreateSolidBrush(RGB(255, 0, 255));
		br[8] = CreateSolidBrush(RGB(105, 105, 105)); //badRow颜色
	}
	break;
	case WM_SIZE:
	{
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		//子窗口所在的矩形
		RECT rt1{ HALF(cxClient) - button_size.x, cyClient * 2 / 3,
			button_size.x * 2, button_size.y * 2 }; //单人游戏
		RECT rt2{ HALF(cxClient) - button_size.x, cyClient * 2 / 3 + (button_size.y * 3),
			button_size.x * 2, button_size.y * 2, }; //多人游戏
		RECT rt3{ HALF(cxClient) - button_size.x, cyClient * 2 / 3 + (button_size.y * 6),
			button_size.x * 2, button_size.y * 2, }; //游戏说明
		RECT rt4{ cxClient - (button_size.x * 2), cyClient - (button_size.y * 2), 
			button_size.x * 2, button_size.y * 2 }; //退出按钮
		RECT rt5{ cxClient - (button_size.x * 2), cyClient - (button_size.y * 8),
			button_size.x * 2, button_size.y * 2 }; //刷新
		RECT rt6{ cxClient - (button_size.x * 2), cyClient - (button_size.y * 6),
			button_size.x * 2, button_size.y * 2 }; //创建房间 & 离开房间
		RECT rt7{ cxClient - (button_size.x * 2), cyClient - (button_size.y * 4),
			button_size.x * 2, button_size.y * 2 }; //加入房间
		RECT rt8{ 0, cyClient - (button_size.y * 10), 
			cxClient - (button_size.x * 3), cyClient }; //房间列表
		RECT rt9{ cxClient - (button_size.x * 2), cyClient - (button_size.y * 12),
			button_size.x * 2, button_size.y * 2 }; //开始游戏

		buttonHwnd[0] = CreateWindow(L"Button", L"单人游戏", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			rt1.left, rt1.top, button_size.x * 2, button_size.y * 2, hWnd, (HMENU)HOME_IDB_ONE, hInst, NULL);

		buttonHwnd[1] = CreateWindow(L"Button", L"多人游戏", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			rt2.left, rt2.top, button_size.x * 2, button_size.y * 2, hWnd, (HMENU)HOME_IDB_TWO, hInst, NULL);

		buttonHwnd[2] = CreateWindow(L"Button", L"游戏说明", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			rt3.left, rt3.top, button_size.x * 2, button_size.y * 2, hWnd, (HMENU)HOME_IDB_THREE, hInst, NULL);

		buttonHwnd[3] = CreateWindow(L"Button", L"退出", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			rt4.left, rt4.top, button_size.x * 2, button_size.y * 2, hWnd, (HMENU)SINGLE_IDB_ONE, hInst, NULL);

		buttonHwnd[4] = CreateWindow(L"Button", L"刷新", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			rt5.left, rt5.top, button_size.x * 2, button_size.y * 2, hWnd, (HMENU)MULT_IDB_ONE, hInst, NULL);

		buttonHwnd[5] = CreateWindow(L"Button", L"创建房间", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			rt6.left, rt6.top, button_size.x * 2, button_size.y * 2, hWnd, (HMENU)MULT_IDB_TWO, hInst, NULL);

		buttonHwnd[6] = CreateWindow(L"Button", L"加入房间", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			rt7.left, rt7.top, button_size.x * 2, button_size.y * 2, hWnd, (HMENU)MULT_IDB_THREE, hInst, NULL);

		buttonHwnd[7] = CreateWindow(L"Button", L"离开房间", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			rt6.left, rt6.top, button_size.x * 2, button_size.y * 2, hWnd, (HMENU)MULT_IDB_FOUR, hInst, NULL);

		buttonHwnd[8] = CreateWindow(L"Button", L"开始游戏", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			rt9.left, rt9.top, button_size.x * 2, button_size.y * 2, hWnd, (HMENU)MULT_IDB_FIVE, hInst, NULL);

		roomListHwnd = CreateWindow(L"ListBox", NULL, WS_VISIBLE | WS_CHILD | WS_VSCROLL,
			rt8.left, rt8.top, rt8.right - rt8.left, button_size.y * 10, hWnd, NULL, hInst, NULL);

		//设置游戏开始函数
		client_.SetStartGameFun([hWnd] {
			if (block)
				delete block;
			block = GetRandBlock();

			//设置按钮可见性
			bool status[9] = { 0, 0, 0, 1, 0, 0, 0, 0, 0};
			SetTotalButtonVisible(buttonHwnd, status);

			//隐藏房间列表
			ShowWindow(roomListHwnd, SW_HIDE);

			curInterface = EnumInterface::MULT_INTERFACE;
			SetTimer(hWnd, TIMER_ID1, elapse - gradeOfDifficult * 50, NULL);
		});

		//设置受到伤害函数
		client_.SetHurtFun([hWnd] {
			//方块上升
			for (int i = 0; i < WIDTH; ++i)
			{
				for (int j = 0; j < HEIGHT - 1; j++)
				{
					//对与正在下落中的方块忽略
					if (i == block->Shape[0].x && j+1 == block->Shape[0].y
						|| i == block->Shape[1].x && j+1 == block->Shape[1].y
						|| i == block->Shape[2].x && j+1 == block->Shape[2].y
						|| i == block->Shape[3].x && j+1 == block->Shape[3].y)
						continue;

					map[i][j] = map[i][j + 1];
				}
			}

			//产生badRow
			for (int i = 0; i < WIDTH; ++i)
				map[i][(HEIGHT -1) - badRowCount] = 8;
			badRowCount += 1;
		});

		//设置刷新函数
		client_.SetRenovateFun([hWnd] {
			SendMessage(hWnd, WM_COMMAND, WPARAM(MULT_IDB_ONE), NULL);
		});

		//设置按钮可见性
		bool status[9] = { 1, 1, 1, 0, 0, 0, 0, 0, 0 };
		SetTotalButtonVisible(buttonHwnd, status);

		//隐藏房间列表
		ShowWindow(roomListHwnd, SW_HIDE);
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case HOME_IDB_ONE: //单人游戏
			{
				//初始化
				elapse = 800;
				score = 0;
				gradeOfDifficult = 1;
				badRowCount = 0;

				if (block)
					delete block;
				block = GetRandBlock(); //产生随机方块
				assert(block != nullptr);

				for (int i = 0; i < WIDTH; ++i)
					for (int j = 0; j < HEIGHT; ++j) {
						map[i][j] = 0;
					}

				//设置按钮可见性
				bool status[9] = { 0, 0, 0, 1, 0, 0, 0, 0, 0 };
				SetTotalButtonVisible(buttonHwnd, status);

				curInterface = EnumInterface::SINGLE_INTERFACE;
				SetTimer(hWnd, TIMER_ID1, elapse - gradeOfDifficult * 50, NULL);
			}
			break;
			case HOME_IDB_TWO: //多人游戏
			{
				//设置按钮可见性
				bool status[9] = { 0, 0, 0, 1, 1, 1, 1, 0, 0 };
				SetTotalButtonVisible(buttonHwnd, status);

				//显示房间列表
				ShowWindow(roomListHwnd, SW_SHOW);

				//刷新房间列表
				SendMessage(hWnd, WM_COMMAND, WPARAM(MULT_IDB_ONE), NULL);

				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
			case HOME_IDB_THREE: //游戏说明
			{
				//设置按钮可见性
				bool status[9] = { 0, 0, 0, 1, 0, 0, 0, 0, 0 };
				SetTotalButtonVisible(buttonHwnd, status);
				
				hdc = GetDC(hWnd);

				mdc = CreateCompatibleDC(hdc);  //创建协调的设备上下文
				SelectObject(mdc, explain_bmp); //选入环境
				StretchBlt(hdc, 0, 0, cxClient, cyClient, mdc, 0, 0, 1152, 648, SRCAND); //显示拉伸的图片
				DeleteDC(mdc); //坑爹

				DeleteDC(hdc);
			}
			break;
			case SINGLE_IDB_ONE: //退出
			{
				//通知服务器退出房间
				ServerMsg serverMsg;
				serverMsg.set_msgtype(ServerMessageType::LEAVE_SERVER_MSG);
				client_.write(MessageType::SERVER_MSG, serverMsg);

				//设置按钮可见性
				bool status[9] = { 1, 1, 1, 0, 0, 0, 0, 0, 0 };
				SetTotalButtonVisible(buttonHwnd, status);

				//隐藏房间列表
				ShowWindow(roomListHwnd, SW_HIDE);

				curInterface = EnumInterface::HOME_INTERFACE;
				KillTimer(hWnd, TIMER_ID1); //干掉定时器
				InvalidateRect(hWnd, NULL, TRUE);
			}
            break;
			case MULT_IDB_ONE: //刷新
			{
				//初始化client_的房间信息
				client_.InitRoomInfo();

				//向服务器请求获得房间列表
				ServerMsg serverMsg;
				serverMsg.set_msgtype(ServerMessageType::GET_ROOM_LIST_MSG);
				client_.write(MessageType::SERVER_MSG, serverMsg, [] {
					//等到服务器传来需要的信息后才开始后面的工作
					//listbox 删除列表框原有的数据
					SendMessage(roomListHwnd, LB_RESETCONTENT, NULL, NULL);

					//重新添加列表框数据
					auto people_num_iter = client_.room_people_num_vec().cbegin();
					auto room_status_iter = client_.room_status_vec().cbegin();
					for (const auto &id : client_.room_set()) {
						std::wstring buf(L"房间号：<");
						buf += std::to_wstring(id);
						buf += L">";
						buf += L"         房间人数：";
						assert(people_num_iter != client_.room_people_num_vec().cend());
						buf += std::to_wstring(*people_num_iter++);
						buf += L"/4";
						assert(room_status_iter != client_.room_status_vec().cend());
						if (*room_status_iter)
							buf += L"         房间状态：等待中";
						else
							buf += L"         房间状态：游戏中";
						++room_status_iter;


						SendMessage(roomListHwnd, LB_ADDSTRING, NULL,
							(LPARAM)buf.c_str());
					}
				});
			}
			break;
			case MULT_IDB_TWO: //创建房间
			{
				ServerMsg serverMsg;
				serverMsg.set_msgtype(ServerMessageType::CREATE_SERVER_MSG);
				client_.write(MessageType::SERVER_MSG, serverMsg, [hWnd] {
					ShowCurRoom(hWnd, hFont);
				});

				//设置按钮可见性
				bool status[9] = { 0, 0, 0, 1, 1, 0, 0, 1, 1 };
				SetTotalButtonVisible(buttonHwnd, status);
			}
			break;
			case MULT_IDB_THREE: //加入房间
			{
				//当前选中的list box项
				auto result = SendMessage(roomListHwnd, LB_GETCURSEL, NULL, NULL);
				if (result != LB_ERR) {
					wchar_t buf[50];
					SendMessage(roomListHwnd, LB_GETTEXT, result, (LPARAM)buf); //获取选择项的内容
					std::wstring tmp(buf);
					std::wstring tmp2 = tmp.substr(tmp.find_first_of(L'<') + 1, 
						tmp.find_first_of(L'>') - tmp.find_first_of(L'<'));

					_int32 targetId = std::stoi(tmp2.c_str());
					client_.roomId(targetId); //设置当前所在的房间
					ShowCurRoom(hWnd, hFont);

					ServerMsg serverMsg;
					serverMsg.set_msgtype(ServerMessageType::JOIN_SERVER_MSG);
					serverMsg.set_serverid(targetId); //设置要加入的房间

					client_.write(MessageType::SERVER_MSG, serverMsg, [&] {
						client_.roomId(0);
						ShowCurRoom(hWnd, hFont);

						//设置按钮可见性
						bool status[9] = { 0, 0, 0, 1, 1, 1, 1, 0, 0 };
						SetTotalButtonVisible(buttonHwnd, status);

						MessageBox(NULL, L"加入房间失败", L"Message", MB_OK);
					});

					//设置按钮可见性
					bool status[9] = { 0, 0, 0, 1, 1, 0, 0, 1, 0 };
					SetTotalButtonVisible(buttonHwnd, status);
				}
				else {
					//没有选择
					MessageBox(NULL, L"请选要加入的房间!", L"Message", MB_OK);
				}
			}
			break;
			case MULT_IDB_FOUR: //离开房间
			{
				client_.roomId(0);
				ShowCurRoom(hWnd, hFont);

				//通知服务器离开房间
				ServerMsg serverMsg;
				serverMsg.set_msgtype(ServerMessageType::LEAVE_SERVER_MSG);
				client_.write(MessageType::SERVER_MSG, serverMsg);

				//设置按钮可见性
				bool status[9] = { 0, 0, 0, 1, 1, 1, 1, 0, 0 };
				SetTotalButtonVisible(buttonHwnd, status);
			}
			break;
			case MULT_IDB_FIVE: //开始游戏
			{
				//给服务器发 GAME_START 请求
				GameMsg gameMsg;
				gameMsg.set_msgtype(GameMessageType::GAME_START_MSG);
				client_.write(MessageType::GAME_MSG, gameMsg);
			}
			break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
		break;

	break;
	case WM_TIMER:
	{
		ReMoveBlock(block, map);

		//如果方块没有移动，产生新的方块
		if (!MoveBlock(block, map))
		{
			PlaySound(L"./src/方块落地.wav", NULL, SND_FILENAME | SND_ASYNC);

			isCreateBlock = true;

			for (int i = 0; i < 4; i++)
				map[block->Shape[i].x][block->Shape[i].y] = brCur;

			//随机颜色
			brCur = rand() % 7 + 1;

			if (block)
				delete block;
			block = GetRandBlock(); //产生随机方块
			assert(block != nullptr); 

			//判断游戏是否结束
			if (map[block->Shape[0].x][block->Shape[0].y] != 0
				|| map[block->Shape[1].x][block->Shape[1].y] != 0
				|| map[block->Shape[2].x][block->Shape[2].y] != 0
				|| map[block->Shape[3].x][block->Shape[3].y] != 0)
			{
				PlaySound(L"./src/失败.wav", NULL, SND_FILENAME | SND_ASYNC);
				KillTimer(hWnd, TIMER_ID1);
				MessageBox(NULL, L"Game Over!!!", L"Message", MB_OK);

				//通知服务器游戏结束
				GameMsg gameMsg;
				gameMsg.set_msgtype(GameMessageType::GAME_OVER_MSG);
				client_.write(MessageType::GAME_MSG, gameMsg);
			}

			Judge(map, score, gradeOfDifficult, badRowCount); //判断是否需要消除
		}
		else{
			isCreateBlock = false;
		}

		//RECT rt = { 0, 0, ,cyClient };
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_KEYDOWN:
	{
		if (elapse == 30)
			break;

		switch (wParam)
		{
		case VK_SPACE:
			elapse = 30; 
			KillTimer(hWnd, TIMER_ID1);
			SetTimer(hWnd, TIMER_ID1, elapse, NULL);
			break;
		}
	}
	break;
	case WM_CHAR:
	{
		switch (wParam)
		{
		case 'a':
		case 'A':
		{
			ReMoveBlock(block, map);
			if (block->MoveLeftBlovk(map))
				InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		case 'd':
		case 'D':
			ReMoveBlock(block, map);
			if (block->MoveRightBlovk(map))
				InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 13:
		case '5':
			ReMoveBlock(block, map);
			if (block->ChangeBlock(map))
				InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	}
	break;
	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_SPACE:
			elapse = 800;
			KillTimer(hWnd, TIMER_ID1);
			SetTimer(hWnd, TIMER_ID1, elapse - gradeOfDifficult * 50, NULL);
		}
	}
	break;
    case WM_PAINT:{
            hdc = BeginPaint(hWnd, &ps);

			switch (curInterface)
			{
			case EnumInterface::HOME_INTERFACE:
				mdc = CreateCompatibleDC(hdc);  //创建协调的设备上下文
				SelectObject(mdc, bg_bmp); //选入环境
				StretchBlt(hdc, 0, 0, cxClient, cyClient, mdc, 0, 0, bg_size.x, bg_size.y, SRCAND); //显示拉伸的图片
				DeleteDC(mdc); //坑爹
				break;
			case EnumInterface::SINGLE_INTERFACE:
				Draw(hWnd, hdc, block, cyClient,
					map, br, brCur, score, gradeOfDifficult, isCreateBlock);
				break;
			case EnumInterface::MULT_INTERFACE:
				Draw(hWnd, hdc, block, cyClient,
					map, br, brCur, score, gradeOfDifficult, isCreateBlock);
				break;
			}

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		client_.close();
		thread_client.join();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}