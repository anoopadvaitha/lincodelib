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
enum KDxMouseAction
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

/*
	键盘动作类型
*/
enum KDxKeyAction
{
	kaDown,						// 键盘击下
	kaChar,						// 键盘字符
	kaUp						// 键盘弹起
};

/*
	Shift状态
*/
enum KDxShiftState
{
	ssShift,					// Shift键按下
	ssAlt,						// Alt键按下
	ssCtrl						// Ctrl键按下
};

/*
	通知事件类型, 0~NT_USER-1由框架保留，用户可以使用其他的值
*/
typedef DWORD KDxNotifyType;
#define NT_USER						0x10000
// 尺寸正在改变，param: SIZE*=将要改变的尺寸，可以改变这个值，影响最终的尺寸
#define ntSizeChanging				1
// 尺寸改变，param: NULL
#define ntSizeChanged				2
// 位置正在改变，param: POINT*=将要改变的尺寸，可以改变这个值，影响最终的位置
#define ntPosChanging				3
// 位置改变，param: NULL
#define ntPosChanged				4
// 激活改变	 param:  0=反激活, !0=激活
#define ntActiveChanged				5
// 焦点改变, param:　0=失去焦点, !0=获得焦点
#define ntFocusChanged				6
// 鼠标捕获视图改变，param:　0=失去捕获, !0=获得捕获
#define ntCaptureChnaged			7
// 可见改变，param:　0=隐藏, !0=可见
#define ntVisibleChanged			8
// 可用改变，param:　0=禁用, !0=可用
#define ntEnableChanged				9
// 环境(右键)菜单事件，param：NULL=由键盘引起的, PPOINT=由鼠标引起的，PPOINT指定鼠标在VScreen的位置
#define ntContextMenu				10
// 开始ShowModal, param: NULL
#define ntBeginModal				11
// 结束ShowModal, param: NULL
#define ntEndModal					12
// 鼠标进入视图, param: NULL
#define ntMouseEnter				13
// 鼠标退出视图, param: NULL
#define ntMouseLeave				14
// 文本改变， param: NULL
#define ntTextChanged				15

/*
	请求事件类型, 0~QT_USER-1由框架保留，用户可以使用其他的值
*/
typedef DWORD KDxQueryType;
#define QT_USER						0x10000
// 是否要处理TAB，返回值：0=不处理，!0=处理
#define qtWantTab					1
// 是否要处理方向键，返回值：0=不处理，!0=处理 
#define qtWantArrows				2
// 是否要处理ESC键，返回值：0=不处理，!0=处理 
#define qtWantEscape				3
// 窗口关闭请求，返回值：CloseAction
#define qtClose						4
// 设置光标，参数： param=点击测试值；返回值：!0=设置成功
#define qtSetCursor					5

/*
	视图点击测试值0~oxHT_USER-1由框架保留，用户可以使用其他值
*/
typedef DWORD KDxHitTest;
#define HT_USER					0x10000
#define htNone						0	  // 什么也没有，透明
#define htCient						1	  // 点击到客户区
#define htBorderLeft				2	  // 边缘左
#define htBorderTop					3	  // 边缘顶
#define htBorderRight				4	  // 边缘右
#define htBorderBottom				5	  // 边缘底
#define htBorderLeftTop				6	  // 边缘左顶
#define htBorderLeftBottom			7	  // 边缘左底
#define htBorderRightTop			8	  // 边缘右顶
#define htBorderRightBottom			9	  // 边缘右底
#define htMoveRegion				10	  // 可移动区域

/*
	光标类型, 0~CT_USER-1由框架保留，用户可以使用其他值
*/
typedef DWORD KDxCursorType;
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

/*
	视图风格
*/
typedef DWORD KDxViewStyle;
#define vsTabStop			0x01		// Tab停止，与焦点关系紧密

/*
	窗口风格
*/
typedef DWORD KDxWindowStyle;
#define wsMovable			0x01   // 可移动
#define wsSizable			0x02   // 可拖动
#define wsTopMost			0x04   // 顶层窗口
#define wsEscHide			0x08   // ESC关闭

/*
	窗口状态
*/
typedef DWORD KDxWndState;
#define wsModal				0x01	// 模态状态

enum KDxButtonState
{
	bsNormal,						// 普通状态
	bsOver,							// 鼠标经过时的状态
	bsDown							// 按下时的状态
};

/*
	窗口关闭动作
*/
enum KDxCloseAction
{
	caHide,							// 隐藏
	caFree,							// 释放
	caNone							// 什么事也不做
};

/*
	快捷键值								  
*/
typedef DWORD Shortcut;

/*
	投递事件ID
*/
typedef DWORD KDxPostId;
#define piRelease			1		// 释放自己，无参数

class KDxScreen;
class KDxWindow;
class KDxView;

/*
	视图事件接口
*/
interface IDxViewEvent
{
	/*
		鼠标事件，点下，弹起，移动
	*/
	virtual void OnMouse(KDxView* view, KDxMouseAction action, KDxShiftState shift, const POINT& pt) 
	{

	}

	/*
		键盘事件，按下，弹起，字符
	*/
	virtual void OnKeyboard(KDxView* view, KDxKeyAction action, WORD& key, KDxShiftState shift) 
	{

	}

	/*
		通用的通知事件，KDxNotifyType指明通知的类型, param为附带的参数
	*/
	virtual void OnNotify(KDxView* view, KDxNotifyType type, DWORD param) 
	{

	}

	/*
		通用的请求事件，KDxQueryType指明请求的类型, param为附带的参数
	*/
	virtual LRESULT OnQuery(KDxView* view, KDxQueryType type, DWORD param) 
	{ 
		return 0; 
	}

	/*
		更新事件
	*/
	virtual void OnUpdate(KDxView* view) 
	{

	}

	/*
		绘制事件， 
		rcPaint	要绘制的区域，为屏幕坐标
		rcScreen 视图在屏幕中的区域
	*/
	virtual void OnPaint(KDxView* view, const RECT& rcPaint, const RECT& rcScreen) 
	{

	}
};

/*
	视图列表
*/
typedef std::vector<KDxView*>	KDxViewVector;

/*
	投递的动作信息
*/
struct KDxPostInfo 
{
	KDxView*		view;		// 动作所属视图
	KDxPostId		id;			// 投递ID
	DWORD			param1;		// 附带参数
	DWORD			param2;		// 附带参数2
};

/*
	必须使用该宏创建一个视图类
	theclass 要创建的视图类
	parenview 父视图，可以为NULL
	ownerscreen 所属的屏幕类：KDxScreen
*/
#define NEW_VIEW(theclass, parenview, ownerscreen)\
	(theclass*)((new theclass)->Initialize(parenview, ownerscreen));

/*
	必须该宏释放一个视图
*/
#define FREE_VIEW(theobject)\
	do{\
		theobject->Finalize();\
		delete theobject;\
	}while(0)


/*
	视图类
*/
class KDxView: public KObject
{
	DECLARE_RUNTIMEINFO(KDxView)
public:
	KDxView():	
		mOwnerScreen(NULL),
		mOwnerWindow(NULL),
		mParentView(NULL), 
		mViewEvent(NULL),
		mViewStyle(vsTabStop),
		mUserData(0), 
		mLeft(0), 
		mTop(0), 
		mWidth(100), 
		mHeight(600), 
		mIsVisible(TRUE), 
		mIsEnable(TRUE)
	{
		mMinSize.cx = 0;
		mMinSize.cy = 0;
		mMaxSize.cx = 0;
		mMaxSize.cy = 0;
	}

	/*
		初始化控件，不要直接调用它，应该由NEW_VIEW调用, 子类可以覆盖它，作一些初始化
		parentView 父视图，可以为NULL，如果不为NULL，父视图将管理本视图的生命周期，不必手动去释放视图
		screen 所属的屏幕
	*/
	virtual KDxView* Initialize(KDxView* parentView, KDxScreen* screen);

	/*
		结束清理资源，不要直接调用它，应该由FREE_VIEW自动调用
		子类可以覆盖它，作一些初始化
	*/
	virtual void Finalize();

	/*
		取得所属屏幕
	*/
	KDxScreen* OwnerScreen() 
	{ 
		return mOwnerScreen; 
	}

	/*
		取得所属窗口
	*/
	KDxWindow* OwnerWindow() 
	{ 
		return mOwnerWindow; 
	}

	/*
		取父视图
	*/
	KDxView* ParentView() 
	{ 
		return mParentView; 
	}

	/*
		设置父视图
	*/
	void SetParentView(KDxView* parentView);

	/*
		增加子视图
		childView 要增加的子视图
		isCheck 是否检查子视图已经存在，如果为FALSE，可以显著提高效率，但须承担重复添加的风险
	*/
	BOOL AddChild(KDxView* childView, BOOL isCheck = TRUE);

	/*
		插入子视图
		childView 要增加的子视图
		pos 插入的位置
		isCheck 是否检查子视图已经存在，如果为FALSE，可以显著提高效率，但须承担重复添加的风险
	*/
	virtual BOOL InsertChild(KDxView* childView, int pos , BOOL isCheck = TRUE);

	/*
		删除子视图，注意不会释放子视图
	*/
	void RemoveChild(KDxView* childView);

	/*
		删除子视图，注意不会释放子视图
	*/
	void DeleteChild(int pos);

	/*
		子视图数量
	*/
	int	ChildCount() 
	{ 
		return (int)mChildViews.size(); 
	}

	/*
		取子视图，为加快速度，不判断边界
	*/
	KDxView* ChildView(int idx) 
	{ 
		return mChildViews[idx]; 
	}

	/*
		取最后一个子视图，如果没有子视图，返回NULL
	*/
	KDxView* LastChild();

	/*
		取第一个子视图， 如果没有子视图，返回NULL
	*/
	KDxView* FirstChild();

	/*
		取得子视图的索引，如果失败返回-1
	*/
	int IndexOf(KDxView* childView);

	/*
		调整子视图的索引位置
	*/
	BOOL MoveChildPos(KDxView* childView, int newPos);

	/*
		把自己置于父视图的最顶层
	*/
	void BringToFront();

	/*
		把自己置于父视图的最底层
	*/
	void SendToBack();

	/*
		用户数据
	*/
	int	 UserData() 
	{ 
		return mUserData; 
	}

	/*
		设用户数据
	*/
	void SetUserData(int data) 
	{ 
		mUserData = data; 
	}

	/*
		左
	*/
	int  Left() 
	{ 
		return mLeft; 
	}

	/*
		顶
	*/
	int  Top() 
	{ 
		return mTop; 
	}

	/*
		宽
	*/
	int  Width() 
	{ 
		return  mWidth; 
	}

	/*
		高
	*/
	int  Height() 
	{ 
		return mHeight; 
	}

	/*
		设左
	*/
	void SetLeft(int left);

	/*
		设顶
	*/
	void SetTop(int top);

	/*
		设宽
	*/
	void SetWidth(int width);

	/*
		设高
	*/
	void SetHeight(int height);

	/*
		设尺寸
	*/
	void SetSize(int width, int height);

	/*
		设位置
	*/
	void SetPos(int left, int top);

	/*
		取最小尺寸
	*/
	SIZE MinSize() 
	{ 
		return mMinSize; 
	}

	/*
		取最大尺寸
	*/
	SIZE MaxSize() 
	{ 
		return mMaxSize; 
	}

	/*
		设最小尺寸
	*/
	void SetMinSize(int cx, int cy);

	/*
		设最大尺寸
	*/
	void SetMaxSize(int cx, int cy);

	/*
		取得视图的客户区域
	*/
	void ClientRect(RECT& rc);

	/*
		取得视图所占的屏幕区域
	*/
	void ScreenRect(RECT& rc);

	/*
		屏幕坐标转客户坐标
	*/
	POINT ScreenToClient(const POINT& pt);

	/*
		客户坐标转屏幕坐标
	*/
	POINT ClientToScreen(const POINT& pt);

	/*
		取得坐标下的视图
		x, y 相对于父视图的坐标
		allowDisabled 是否允许取Disabled的视图
	*/
	virtual KDxView* GetViewAtPos(POINT pt, BOOL allowDisabled = FALSE);

	/*
		点击测试，子类可覆盖返回自定义值 
	*/
	virtual KDxHitTest HitTestView(POINT pt);

	/*
		Tab键定位
	*/
	BOOL IsTabStop() 
	{ 
		return HAS_FLAG(mViewStyle, vsTabStop); 
	}

	/*
		Tab键定位
	*/
	void SetTabStop(BOOL isTabStop);

	/*
		是否为焦点控件，isInWnd指定是在窗口级别判断，还是在屏幕级别判断
		注意屏幕级别只有一个焦点视图，这个视图是接收键盘事件的
	*/
	BOOL IsFocused(BOOL isInWnd = FALSE);

	/*
		是否可以获得焦点
	*/
	BOOL CanFocus();

	/*
		设焦点
	*/
	BOOL SetFocus();

	/*
		是否鼠标捕获
	*/
	BOOL IsCaptured();

	/*
		是否能被捕获
	*/
	BOOL CanCapture();

	/*
		设置鼠标捕获
	*/
	BOOL SetCapture();

	/*
		释放鼠标捕获
	*/
	void ReleaseCapture();

	/*
		是否可见
	*/
	BOOL IsVisible() 
	{ 
		return mIsVisible; 
	}

	/*
		设可见性
	*/
	void SetVisible(BOOL isVisible);

	/*
		是否可用
	*/
	BOOL IsEnable() 
	{ 
		return mIsEnable; 
	}

	/*
		设置可用性
	*/
	void SetEnable(BOOL isEnable);

	/*
		取视图事件接口
	*/
	IDxViewEvent* isEnable() { return mViewEvent; }

	/*
		设视图事件接口，并返回前一个事件接口
	*/
	IDxViewEvent* SetViewEvent(IDxViewEvent* event);

public:
	/*
		鼠标事件
	*/
	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt);

	/*
		键盘事件
	*/
	virtual void DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift);

	/*
		通知事件
	*/
	virtual void DoNotify(KDxNotifyType type, DWORD param);

	/*
		请求事件
	*/
	virtual LRESULT DoQuery(KDxQueryType type, DWORD param);

	/*
		更新
	*/
	virtual void DoUpdate();

	/*
		绘制，具体用什么绘制
	*/
	virtual void DoPaint(const RECT& rcPaint, const RECT& rcScreen);

	/*
		处理投递事件
	*/
	virtual void HandlePostEvent(KDxPostId id, DWORD param1, DWORD param2);


protected:
	/*
		更新可见状态
	*/
	void UpdateVisible(BOOL isVisible);

	/*
		更新可用状态
	*/
	void UpdateEnable(BOOL isEnable);

	/*
		可见可用，遍历父视图
	*/
	BOOL VisibleAndEnable();

	/*
		消毁所有子视图
	*/
	void FreeChilds();

	/*
		重置鼠标盘旋
	*/
	void ResetScreenHoverView();

	/*
		删除投递消息
	*/
	void DelScreenPostAction();

protected:
	KDxScreen*			mOwnerScreen;		// 所属屏幕
	KDxWindow*			mOwnerWindow;		// 所拥有的窗口，如果View为窗口，则mOwnerWindow为自己
	KDxView*			mParentView;		// 父视图
	IDxViewEvent*		mViewEvent;			// 事件接口
	KDxViewVector		mChildViews;		// 子视图列表
	KDxViewStyle		mViewStyle;			// 视图风格
	int					mUserData;			// 可以挂任意数据
	int					mLeft;				// 左
	int					mTop;				// 顶
	int					mWidth;				// 宽
	int					mHeight;			// 高
	SIZE				mMinSize;			// 最小尺寸，为0表示无限制
	SIZE				mMaxSize;			// 最大尺寸，为0表示无限制
	BOOL				mIsVisible;			// 可见
	BOOL				mIsEnable;			// 可用
};
IMPLEMENT_RUNTIMEINFO(KDxView, KObject)

}

#endif // __KAMA_KMDXVIEWS_H__