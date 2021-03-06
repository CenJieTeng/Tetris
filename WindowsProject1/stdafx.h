// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 在此处引用程序需要的其他头文件
#include <time.h>
#include <strsafe.h>
#include <fstream>
#include <string>
#include <set>
#include <memory>
#include <boost/asio.hpp>
#include <mmsystem.h>
#include "block.h"
#include "block_O.h"
#include "block_J.h"
#include "block_L.h"
#include "block_I.h"
#include "block_T.h"
#include "block_S.h"
#include "block_Z.h"
#include "message.h"
#include "message.pb.h"