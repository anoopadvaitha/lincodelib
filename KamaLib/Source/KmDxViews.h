/***********************************************************
  Filename:	KmDxViews.h
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/19

  Brief:    这是KamaLib代码库的一部分，由Tramper创建并维护，
			版权没有，请自由使用！
***********************************************************/
#ifndef __KAMA_KMDXVIEWS_H__
#define __KAMA_KMDXVIEWS_H__
#include "KmCommons.h"

/*=======================================================================
  说明: Dx视图控件库
========================================================================*/
namespace kama
{

/*
	鼠标动作类型
*/
enum KMouseAction
{
	maMove,						// 移动
	maLDown,					// 左键点下
	maRDown,					// 右键点下
	maMDown,					// 中键点下
	maLUp,						// 左键弹起
	maRUp,						// 右键弹起
	maMUp,						// 中键弹起
	maLDblClk,					// 左键双击
	maRDblClk,					// 右键双击
	maMDblClk,					// 中键双击
	maClick						// 点击
};					
// #define MACTION_MOVE		1	// 移动
// #define MACTION_LDOWN		2	// 左键点下
// #define MACTION_RDOWN		3	// 右键点下
// #define MACTION_MDOWN		4	// 中键点下
// #define MACTION_LUP			5	// 左键弹起
// #define MACTION_RUP			6	// 右键弹起
// #define MACTION_MUP			7	// 中键弹起
// #define MACTION_LDBLCLK		8	// 左键双击
// #define MACTION_RDBLCLK		9	// 右键双击
// #define MACTION_MDBLCLK		10	// 中键双击
// #define MACTION_CLICK		11	// 点击

/*
	键盘动作类型
*/
enum KKeyAction
{
	kaDown,						// 键盘击下
	kaChar,						// 键盘字符
	kaUp						// 键盘弹起
};
// typedef DWORD KeyAction;
// #define KACTION_DOWN		1	// 键盘击下
// #define KACTION_CHAR		2	// 键盘字符
// #define KACTION_UP		3	// 键盘弹起

/*
	Shift状态
*/
enum KShiftState
{
	ssShift,					// Shift键按下
	ssAlt,						// Alt键按下
	ssCtrl						// Ctrl键按下
};
// typedef WORD ShiftState;
// #define SSTATE_SHIFT	0x01	// Shift键按下
// #define SSTATE_ALT		0x02	// Alt键按下
// #define SSTATE_CTRL		0x04	// Ctrl键按下

/*
	通知事件类型, 0~NT_USER-1由框架保留，用户可以使用其他的值
*/
typedef DWORD KNotifyType;
#define NT_USER						0x10000
// 尺寸正在改变，dwParam: SIZE*=将要改变的尺寸，可以改变这个值，影响最终的尺寸
#define ntSizeChanging				1
// 尺寸改变，dwParam: NULL
#define ntSizeChanged				2
// 位置正在改变，dwParam: POINT*=将要改变的尺寸，可以改变这个值，影响最终的位置
#define ntPosChanging				3
// 位置改变，dwParam: NULL
#define ntPosChanged				4
// 激活改变	 dwParam:  0=反激活, !0=激活
#define ntActiveChanged				5
// 焦点改变, dwParam:　0=失去焦点, !0=获得焦点
#define ntFocusChanged				6
// 鼠标捕获视图改变，dwParam:　0=失去捕获, !0=获得捕获
#define ntCaptureChnaged			7
// 可见改变，dwParam:　0=隐藏, !0=可见
#define ntVisibleChanged			8
// 可用改变，dwParam:　0=禁用, !0=可用
#define ntEnableChanged				9
// 环境(右键)菜单事件，dwParam：NULL=由键盘引起的, PPOINT=由鼠标引起的，PPOINT指定鼠标在VScreen的位置
#define ntContextMenu				10
// 开始ShowModal, dwParam: NULL
#define ntBeginModal				11
// 结束ShowModal, dwParam: NULL
#define ntEndModal					12
// 鼠标进入视图, dwParam: NULL
#define ntMouseEnter				13
// 鼠标退出视图, dwParam: NULL
#define ntMouseLeave				14
// 文本改变， dwParam: NULL
#define ntTextChanged				15
	
// typedef DWORD NotifyType;
// // 尺寸改变，dwParam: NULL
// #define NTYPE_SIZECHANGED			1	
// 	// 位置改变，dwParam: NULL
// #define NTYPE_POSCHANGED			2	
// 	// 激活改变	 dwParam:  0=反激活, !0=激活
// #define NTYPE_ACTIVECHANGED			3	
// 	// 焦点改变, dwParam:　0=失去焦点, !0=获得焦点
// #define NTYPE_FOCUSCHANGED			4	
// 	// 鼠标捕获视图改变，dwParam:　0=失去捕获, !0=获得捕获
// #define NTYPE_CAPTRURECHANGED		5   
// 	// 可见改变，dwParam:　0=隐藏, !0=可见
// #define NTYPE_VISIBLECHANGED		6	
// 	// 可用改变，dwParam:　0=禁用, !0=可用
// #define NTYPE_ENBALECHANGED			7	
// 	// 环境(右键)菜单事件，dwParam：NULL=由键盘引起的, PPOINT=由鼠标引起的，PPOINT指定鼠标在VScreen的位置
// #define NTYPE_CONTEXTMENU			8
// 	// 位置正在改变，dwParam: POINT*=将要改变的尺寸，可以改变这个值，影响最终的位置
// #define NTYPE_POSCHINGING			9
// 	// 尺寸正在改变，dwParam: SIZE*=将要改变的尺寸，可以改变这个值，影响最终的尺寸
// #define NTYPE_SIZECHANGING			10
// 	// 开始ShowModal, dwParam: NULL
// #define NTYPE_BEGINMODAL			11
// 	// 结束ShowModal, dwParam: NULL
// #define NTYPE_ENDMODAL				12
// 	// 鼠标进入视图, dwParam: NULL
// #define NTYPE_MOUSEENTER			13
// 	// 鼠标退出视图, dwParam: NULL
// #define NTYPE_MOUSELEAVE			14
// 	// 文本改变， dwParam: NULL
// #define NTYPE_TEXTCHANGED			15

/*
	请求事件类型, 0~QT_USER-1由框架保留，用户可以使用其他的值
*/
typedef DWORD KQueryType;
#define QT_USER						0x10000
// 是否要处理TAB，返回值：0=不处理，!0=处理
#define qtWantTab					1
// 是否要处理方向键，返回值：0=不处理，!0=处理 
#define qtWantArrows				2
// 是否要处理ESC键，返回值：0=不处理，!0=处理 
#define qtWantEscape				3
// 窗口关闭请求，返回值：CloseAction
#define qtClose						4
// 设置光标，参数： dwParam=点击测试值；返回值：!0=设置成功
#define qtSetCursor					5
// 请求事件类型
// typedef DWORD QueryType;
// 是否要处理TAB，返回值：0=不处理，!0=处理
// #define QTYPE_WANTTAB				1	
// 	// 是否要处理方向键，返回值：0=不处理，!0=处理 
// #define QTYPE_WANTARROWS			2	
// 	// 是否要处理ESC键，返回值：0=不处理，!0=处理 
// #define QTYPE_WANTESCAPE			3
// 	// 窗口关闭请求，返回值：CloseAction
// #define QTYPE_CLOSE					4
// 	// 设置光标，参数： dwParam=点击测试值；返回值：!0=设置成功
// #define QTYPE_SETCURSOR				5

/*
	视图点击测试值0~oxHTV_USER-1由框架保留，用户可以使用其他值
*/
typedef DWORD KHitTestValue;
#define HTV_USER					0x10000
#define htvNone						0	  // 什么也没有，透明
#define htvCient					1	  // 点击到客户区
#define htvBorderLeft				2	  // 边缘左
#define htvBorderTop				3	  // 边缘顶
#define htvBorderRight				4	  // 边缘右
#define htvBorderBottom				5	  // 边缘底
#define htvBorderLeftTop			6	  // 边缘左顶
#define htvBorderLeftBottom			7	  // 边缘左底
#define htvBorderRightTop			8	  // 边缘右顶
#define htvBorderRightBottom		9	  // 边缘右底
#define htvMoveRegion				10	  // 可移动区域

// 	// 视图点击测试值
// 	// 注意：0x0~HTVALUE_USER-1 段供框架使用，外部可以用 HTVALUE_USER~0xFFFFFFFF 来定义点击测试值
// 	typedef DWORD HitTestValue;
// #define HTVALUE_USER				0x10000
// #define HTVALUE_NONE				0x0000	// 什么也没有，透明
// #define HTVALUE_CLIENT				0x0001	// 点击到客户区
// #define HTVALUE_BORDER_LEFT			0x0002	// 边缘左
// #define HTVALUE_BORDER_TOP			0x0003	// 边缘顶
// #define HTVALUE_BORDER_RIGHT		0x0004	// 边缘右
// #define HTVALUE_BORDER_BOTTOM		0x0005	// 边缘底
// #define HTVALUE_BORDER_LEFTTOP		0x0006	// 边缘左顶
// #define HTVALUE_BORDER_LEFTBOTTOM	0x0007	// 边缘左底
// #define HTVALUE_BORDER_RIGHTTOP		0x0008	// 边缘右顶
// #define HTVALUE_BORDER_RIGHTBOTTOM	0x0009	// 边缘右底
// #define HTVALUE_MOVE_REGION			0x000A  // 可移动区域

/*
	光标类型, 0~CT_USER-1由框架保留，用户可以使用其他值
*/
typedef DWORD KCursorType;
#define ctUSER				0x10000
#define ctArrow				1		// 默认光标
#define ctIbeam				2		// 光束, 移到编辑框时的那个形状
#define ctWait				3		// 沙漏, 等待光标
#define ctCross				4		// 十字光标
#define ctUpArrow			5		// 向上箭头
#define ctSizeNWSE			6		// 西北--东南 方向箭头  "\"
#define ctSizeNESW			7		// 东北--西南 方向箭头	"/"
#define ctSIzeWE			8		// 东西 方向箭头 "-"
#define ctSizeNS			9		// 南北 方向箭头 "|"
#define ctSizeAll			10		// 移动, 十字箭头
#define ctNo				11		// 圆圈带斜杠, 表示拒绝
#define ctAppStarting		12		// 光标带沙漏, 表示程序正在启动
#define ctHelp				13		// 光标带问号, 表示帮助
#define ctHand				14		// 手指光标
// 	// 光标类型
// 	// 注意：0x0~VIDC_USER-1 段供框架使用，外部可以用 VIDC_USER~0xFFFFFFFF 来定义自己的光标类型
// 	typedef DWORD CursorType;
// #define VIDC_USER			0x10000
// #define VIDC_ARROW			0x0001	// 默认光标
// #define VIDC_IBEAM			0x0002	// 光束, 移到编辑框时的那个形状
// #define VIDC_WAIT           0x0003	// 沙漏, 等待光标
// #define VIDC_CROSS          0x0004	// 十字光标
// #define VIDC_UPARROW        0x0005	// 向上箭头
// #define VIDC_SIZENWSE       0x0006	// 西北--东南 方向箭头  "\"
// #define VIDC_SIZENESW       0x0007	// 东北--西南 方向箭头	"/"
// #define VIDC_SIZEWE         0x0008	// 东西 方向箭头 "-"
// #define VIDC_SIZENS         0x0009	// 南北 方向箭头 "|"
// #define VIDC_SIZEALL        0x000A	// 移动, 十字箭头
// #define VIDC_NO             0x000B	// 圆圈带斜杠, 表示拒绝
// #define VIDC_APPSTARTING    0x000C	// 光标带沙漏, 表示程序正在启动
// #define VIDC_HELP			0x000D	// 光标带问号, 表示帮助
// #define VIDC_HAND			0x000E	// 手指光标

/*
	视图风格
*/
typedef DWORD KViewStyle;
#define vtTopStop			0x01		// Tab停止，与焦点关系紧密
// 	// 视图风格
// typedef DWORD ViewStyle;
// #define VSTYLE_TABSTOP		0x1		// Tab停止，与焦点关系紧密

/*
	窗口风格
*/
typedef DWORD KWindowStyle;
#define wsMovable			0x01   // 可移动
#define wsSizable			0x02   // 可拖动
#define wsTopMost			0x04   // 顶层窗口
#define wsEscHide			0x08   // ESC关闭
// 	// 窗口风格
// 	typedef DWORD WindowStyle;
// #define WSTYLE_MOVABLE		0x1		// 可移动
// #define WSTYLE_SIZABLE		0x2		// 可拖动
// #define WSTYLE_TOPMOST		0x4		// 顶层窗口
// #define WSTYLE_ESCHIDE		0x8		// ESC关闭

/*
	窗口状态
*/
typedef DWORD KDxWndState;
#define wsModal				0x01	// 模态状态
// 	// 窗口状态
// 	typedef DWORD WindowState;
// #define WSTATE_MODAL		0x1		// 模态状态

	typedef DWORD ButtonState;
#define BS_NORMAL			0x1		// 普通状态
#define BS_OVER				0x2		// 鼠标经过时的状态
#define BS_DOWN				0x3		// 按下时的状态


	// 窗口关闭动作
	typedef DWORD CloseAction;
#define CACTION_HIDE		0x00	// 隐藏
#define CACTION_FREE		0x01	// 释放
#define CACTION_NONE		0x02	// 什么事也不做

	// 快捷键值								  
	typedef DWORD Shortcut;

	// 动作ID
	typedef DWORD PostActionID;
	// 释放自己，无参数
#define PAID_RELEASE		1

}
#endif // __KAMA_KMDXVIEWS_H__