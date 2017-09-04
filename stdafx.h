
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考MSDN。
#ifndef WINVER				// 允许使用特定于Windows XP 或更高版本的功能。
#define WINVER 0x0600		// 将此值更改为相应的值，以适用于Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0600	// 将此值更改为相应的值，以适用于Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将它更改为适合Windows Me 或更高版本的相应值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0700	// 将此值更改为相应的值，以适用于IE 的其他版本。值。
#endif
					

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些CString 构造函数将是显式的

// 关闭MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS



#include <afxwin.h>         // MFC 核心组件和标准组件



#include <afxext.h>         // MFC 扩展

#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxdhtml.h>
#include <atlimage.h>
#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
//#include <afxpopupmenu.h>
//#define CWinAppEx CWinApp

#include "WeiBoEngine/Data/tinystr.h"
#include "WeiBoEngine/Data/tinyxml.h"
#include "WeiBoEngine/Data/WeiBoStruct.h"
#include "WeiBoEngine/WeiBoData.h"
#include "WeiBoEngine/WeiBoEngine.h"
#include "WeiBoEngine/WeiBoHttp.h"
#include "WeiBoEngine/ThreadLock.h"

#include <sstream>
#include "WeiBoEngine/Data/Stream.h"

#include "control/dsError.h"
#include "control/DSkinDef.h"
#include "control/DSkinLite.h"

#include "Attach/Lock.h"




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


//#define _WTL_NO_AUTOMATIC_NAMESPACE
//#define _WTL_USE_CSTRING // 使用WTL的CString
//#define _CSTRING_NS // 强制使用命名空间定义CString
//#include <atlbase.h>
//#include <atlapp.h>

