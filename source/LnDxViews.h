/*******************************************************************************
  Filename:		LnDxViews.h
  Author:		colin
  Email:		linzhenqun@gmail.com
  Date:			2009/12/19

  Brief:    	这是lincode代码库的一部分，由colin创建并维护!
 -------------------------------------------------------------------------------
  Description:
	Dx视图框架
*******************************************************************************/
#ifndef __LIN_DXVIEWS_H__
#define __LIN_DXVIEWS_H__
#include "LnCommons.h"
#include "LnDxRender.h"

namespace lin
{

//////////////////////////////////////////////////////////////////////////
// declare

/*
	鼠标动作类型
*/
enum DxMouseAction
{
	maMouseMove,						// 移动
	maLButtonDown,						// 左键点下
	maRButtonDown,						// 右键点下
	maMButtonDown,						// 中键点下
	maLButtonUp,						// 左键弹起
	maRButtonUp,						// 右键弹起
	maMButtonUp,						// 中键弹起
	maLButtonDblClk,					// 左键双击
	maRButtonDblClk,					// 右键双击
	maMButtonDblClk,					// 中键双击
	maMouseClick,						// 点击
	maMouseWheelDown,					// 滚轮向下滚动, 如果处理了该消息, DoMouse应该返回TRUE
	maMouseWheelUp,						// 滚轮向上滚动, 如果处理了该消息, DoMouse应该返回TRUE
};					

/*
	键盘动作类型
*/
enum DxKeyAction
{
	kaKeyDown,							// 键盘击下
	kaChar,								// 键盘字符
	kaKeyUp								// 键盘弹起
};

/*
	Shift状态
*/
typedef DWORD DxShiftState;
#define ssShift					0x01	// Shift键按下
#define ssAlt					0x02	// Alt键按下
#define ssCtrl					0x04	// Ctrl键按下
#define ssLeft					0x08	// 鼠标左键点下	
#define ssRight					0x08	// 鼠标右键点下
#define ssMiddle				0x10	// 鼠标中键点下


// 用户ID起始值, 自定义控件可以使用这个ID以上的值
#define ID_USER					0x10000
// 程序ID起始值, 外部程序应该使用这个ID以上的值
#define ID_APP					0x20000

/*
	通知事件ID, 0~NTD_USER-1由框架保留，用户可以使用其他的值
*/
typedef DWORD DxNotifyId;
#define NID_USER					ID_USER
// 尺寸正在改变，param: SIZE*=将要改变的尺寸，可以改变这个值，影响最终的尺寸
#define NID_SIZECHANGING			1
// 尺寸改变，param: NULL
#define NID_SIZECHANGED				2
// 位置正在改变，param: POINT*=将要改变的尺寸，可以改变这个值，影响最终的位置
#define NID_POSCHANGING				3
// 位置改变，param: NULL
#define NID_POSCHANGED				4
// 激活改变	 param:  0=反激活, !0=激活
#define NID_ACTIVECHANGED			5
// 焦点改变, param:　0=失去焦点, !0=获得焦点
#define NID_FOCUSCHANGED			6
// 鼠标捕获视图改变，param:　0=失去捕获, !0=获得捕获
#define NID_CAPTURECHANGED			7
// 可见改变，param:　0=隐藏, !0=可见
#define NID_VISIBLECHANGED			8
// 可用改变，param:　0=禁用, !0=可用
#define NID_ENABLECHANGED			9
// 环境(右键)菜单事件，param：NULL=由键盘引起的, PPOINT=由鼠标引起的，PPOINT指定鼠标在screen的位置
#define NID_CONTEXATMENU			10
// 开始ShowModal, param: NULL
#define NID_BEGINMODAL				11
// 结束ShowModal, param: NULL
#define NID_ENDMODAL				12
// 鼠标进入视图, param: NULL
#define NID_MOUSEENTER				13
// 鼠标退出视图, param: NULL
#define NID_MOUSELEAVE				14
// 文本改变， param: NULL
#define NID_TEXTCHANGED				15
// 字体改变， param: NULL
#define NID_FONTCHANGED				16
// 定时器事件，param: 定时器ID
#define NID_TIMER					17

/*
	请求事件ID, 0~QID_USER-1由框架保留，用户可以使用其他的值
*/
typedef DWORD DxQueryId;
#define QID_USER					ID_USER
// 是否要处理TAB，返回值：0=不处理，!0=处理
#define QID_WANTTAB					1
// 是否要处理方向键，返回值：0=不处理，!0=处理 
#define QID_WANTARROWS				2
// 是否要处理ESC键，返回值：0=不处理，!0=处理 
#define QID_WANTESCAPE				3
// 窗口关闭请求，返回值：KDxCloseMode
#define QID_CLOSE					4
// 设置光标，参数： param=点击测试值；返回值：!0=设置成功
#define QID_SETCURSOR				5


/*
	视图点击测试值0~HT_USER-1由框架保留，用户可以使用其他值
*/
typedef DWORD DxHitTest;
#define HT_USER						ID_USER
#define HT_NONE						0		// 什么也没有，透明
#define HT_CLIENT					1		// 点击到客户区
#define HT_BORDERLEFT				2		// 边缘左
#define HT_BORDERTOP				3		// 边缘顶
#define HT_BORDERRIGHT				4		// 边缘右
#define HT_BORDERBOTTOM				5		// 边缘底
#define HT_BORDERLEFTTOP			6		// 边缘左顶
#define HT_BORDERLEFTBOTTOM			7		// 边缘左底
#define HT_BORDERRIGHTTOP			8		// 边缘右顶
#define HT_BORDERRIGHTBOTTOM		9		// 边缘右底
#define HT_MOVEREION				10		// 可移动区域

/*
	光标类型, 0~CID_USER-1由框架保留，用户可以使用其他值
*/
typedef DWORD DxCursorId;
#define CID_USER					ID_USER
#define CID_ARROW					1		// 默认光标
#define CID_IBEAM					2		// 光束, 移到编辑框时的那个形状
#define CID_WAIT					3		// 沙漏, 等待光标
#define CID_CROSS					4		// 十字光标
#define CID_UPARROW					5		// 向上箭头
#define CID_SIZENWSE				6		// 西北--东南 方向箭头  "\"
#define CID_SIZENESW				7		// 东北--西南 方向箭头	"/"
#define CID_SIZEWE					8		// 东西 方向箭头 "-"
#define CID_SIZENS					9		// 南北 方向箭头 "|"
#define CID_SIZEALL					10		// 移动, 十字箭头
#define CID_NO						11		// 圆圈带斜杠, 表示拒绝
#define CID_APPSTARTING				12		// 光标带沙漏, 表示程序正在启动
#define CID_HELP					13		// 光标带问号, 表示帮助
#define CID_HAND					14		// 手指光标

/*
	视图风格
*/
typedef DWORD DxViewStyle;
#define VS_TABSTOP					0x01	// Tab停止，与焦点关系紧密
#define VS_FOCUSABLE				0x02	// 是否可获得焦点

/*
	窗口风格
*/
typedef DWORD DxWndStyle;
#define WS_MOVABLE					0x01	// 可移动
#define WS_SIZABLE					0x02	// 可拖动
#define WS_TOPMOST					0x04	// 顶层窗口
#define WS_ESCCLOSE					0x08	// ESC关闭
#define WS_ACTIVATABLE				0x10	// 可激活

/*
	窗口状态
*/
typedef DWORD DxWndState;
#define WS_MODAL					0x01	// 模态状态

/*
	窗口关闭模式
*/
enum DxCloseMode
{
	cmHide,									// 隐藏
	cmFree,									// 释放
	cmNone									// 什么事也不做
};

/*
	快捷键值								  
*/
typedef DWORD DxShortcut;
// 生成快捷键类型
#define MAKE_SHORTCUT(key, shift) (DxShortcut)MAKELONG(key, shift)
// 从快捷键取得虚键
#define VKEY_OF_SHORTCUT(shortcut) LOWORD(shortcut)
// 从快捷键取得Shift状态
#define SHIFT_OF_SHORTCUT(shortcut) (DxShiftState)HIWORD(shortcut)


/*
	投递事件ID, 0~PID_USER-1由框架保留，用户可以使用其他值
*/
typedef DWORD DxPostId;
#define PID_USER					ID_USER
#define PID_RELEASE					1		// 释放自己，无参数

class DxView;
class DxWindow;
class DxScreen;

/*
	视图事件接口
*/
struct DxViewListener
{
	/*
		视图初始化完毕
	*/
	virtual void OnInitialize()
	{

	}

	/*
		视图将要结束
	*/
	virtual void OnFinalize()
	{

	}

	/*
		鼠标事件，点下，弹起，移动
	*/
	virtual LRESULT OnMouse(DxView* view, DxMouseAction action, DxShiftState shift, const POINT& pt) 
	{
		return 0;
	}

	/*
		键盘事件，按下，弹起，字符
	*/
	virtual LRESULT OnKeyboard(DxView* view, DxKeyAction action, WORD& key, DxShiftState shift) 
	{
		return 0;
	}

	/*
		通用的通知事件，KDxNotifyId指明通知的类型, param为附带的参数
	*/
	virtual void OnNotify(DxView* view, DxNotifyId id, DWORD param) 
	{

	}

	/*
		通用的请求事件，KDxQueryId指明请求的类型, param为附带的参数
	*/
	virtual LRESULT OnQuery(DxView* view, DxQueryId id, DWORD param) 
	{ 
		return 0; 
	}

	/*
		更新事件
	*/
	virtual void OnUpdate(DxView* view, DWORD tick) 
	{

	}

	/*
		绘制事件， 注意: Render此时已经调整好绘制坐标的偏移, 
		因此调用Render的绘制函数时, 应该传入视图的客户坐标, 而非屏幕坐标
	*/
	virtual void OnPaint(DxView* view, DxRender* render) 
	{

	}
};

/*
	视图列表
*/
typedef std::vector<DxView*>	DxViewVector;

/*
	投递的动作信息
*/
struct DxPostInfo 
{
	DxView*		view;		// 动作所属视图
	DxPostId		id;			// 投递ID
	DWORD			param1;		// 附带参数
	DWORD			param2;		// 附带参数2
};

/*
	必须使用该宏创建一个视图类
	theclass 要创建的视图类，如KDxView，KDxWindow等
	parenview 父视图，可以为NULL
	ownerscreen 所属的屏幕类：KDxScreen
*/
#define NEW_VIEW(theclass, parenview, ownerscreen)\
	(theclass*)((new theclass)->Initialize(parenview, ownerscreen));

/*
	必须使用该宏释放一个视图
*/
#define FREE_VIEW(theobject)\
	do{\
		theobject->Finalize();\
		delete theobject;\
	}while(0)


/*
	视图类，大概用法如下: 
	*.  创建
		DxView* view = NEW_VIEW(DxView, wnd, screen);
	*.  设置各种属性
		view->Set... ...
	*.  释放
		FREE_VIEW(view);

	可继承自该类实现自定义视图，详见LnDxCtrls.h
*/
class DxView: public Object
{
	DECLARE_RUNTIMEINFO(DxView)
public:
	DxView():	
		mOwnerScreen(NULL),
		mOwnerWindow(NULL),
		mParentView(NULL), 
		mViewListener(NULL),
		mViewStyle(VS_TABSTOP | VS_FOCUSABLE),
		mUserData(0), 
		mLeft(0), 
		mTop(0), 
		mWidth(100), 
		mHeight(60), 
		mGroup(0),
		mIsVisible(TRUE), 
		mIsEnable(TRUE),
		mWheelAccumulator(0)
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
	virtual DxView* Initialize(DxView* parentView, DxScreen* screen);
	
	/*
		结束清理资源，不要直接调用它，应该由FREE_VIEW自动调用
		子类可以覆盖它，作一些初始化
	*/
	virtual void Finalize();

	/*
		取得所属屏幕
	*/
	DxScreen* GetOwnerScreen(); 

	/*
		取得所属窗口
	*/
	DxWindow* GetOwnerWindow(); 

	/*
		取父视图
	*/
	DxView* GetParentView(); 

	/*
		设置父视图
	*/
	void SetParentView(DxView* parentView);

	/*
		增加子视图
		childView 要增加的子视图
		isCheck 是否检查子视图已经存在，如果为FALSE，可以显著提高效率，但须承担重复添加的风险
	*/
	BOOL AddChild(DxView* childView, BOOL isCheck = TRUE);

	/*
		插入子视图
		childView 要增加的子视图
		pos 插入的位置
		isCheck 是否检查子视图已经存在，如果为FALSE，可以显著提高效率，但须承担重复添加的风险
	*/
	virtual BOOL InsertChild(DxView* childView, int pos , BOOL isCheck = TRUE);

	/*
		删除子视图，注意不会释放子视图
	*/
	void RemoveChild(DxView* childView);

	/*
		删除子视图，注意不会释放子视图
	*/
	void DeleteChild(int pos);

	/*
		子视图数量
	*/
	int	GetChildCount();

	/*
		取子视图，为加快速度，不判断边界
	*/
	DxView* GetChildView(int idx);

	/*
		取最后一个子视图，如果没有子视图，返回NULL
	*/
	DxView* GetLastChild();

	/*
		取第一个子视图， 如果没有子视图，返回NULL
	*/
	DxView* GetFirstChild();

	/*
		取得子视图的索引，如果失败返回-1
	*/
	int IndexOf(DxView* childView);

	/*
		调整子视图的索引位置
	*/
	BOOL MoveChildPos(DxView* childView, int newPos);

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
	int	 GetUserData();

	/*
		设用户数据
	*/
	void SetUserData(int data);

	/*
		左
	*/
	int  GetLeft();

	/*
		顶
	*/
	int  GetTop();

	/*
		宽
	*/
	int  GetWidth();

	/*
		高
	*/
	int  GetHeight();

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
		设置位置尺寸
	*/
	void SetBound(int left, int top, int width, int height);
	void SetBound(const RECT& rc);

	/*
		取最小尺寸
	*/
	SIZE GetMinSize();

	/*
		取最大尺寸
	*/
	SIZE GetMaxSize();

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
	void GetClientRect(RECT& rc);

	/*
		取得相对于父视图的区域
	*/
	void GetViewRect(RECT& rc);

	/*
		取得视图所占的屏幕区域
	*/
	void GetScreenRect(RECT& rc);

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
	virtual DxView* GetViewAtPos(POINT pt, BOOL allowDisabled = FALSE);

	/*
		点击测试，子类可覆盖返回自定义值 
	*/
	virtual DxHitTest HitTestView(const POINT& pt);

	/*
		Tab键定位
	*/
	BOOL IsTabStop();

	/*
		Tab键定位
	*/
	void SetTabStop(BOOL isTabStop);

	/*
		是否可获得焦点
	*/
	BOOL IsFocusable();

	/*
		设置是否可获得焦点
	*/
	void SetFocusable(BOOL isFocusable);


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
	BOOL IsVisible();

	/*
		设可见性
	*/
	void SetVisible(BOOL isVisible);

	/*
		是否可用
	*/
	BOOL IsEnable();

	/*
		设置可用性
	*/
	void SetEnable(BOOL isEnable);

	/*
		是否当前鼠标盘旋的视图
	*/
	BOOL IsHoverView();

	/*
		组
	*/
	int GetGroup();

	/*
		设组
	*/
	void SetGroup(int group);

	/*
		取视图事件接口
	*/
	DxViewListener* GetViewListener(); 

	/*
		设视图事件接口，并返回前一个事件接口
	*/
	DxViewListener* SetViewListener(DxViewListener* listener);

	/*
		处理滚轮
	*/
	void HandleMouseWheel(DxShiftState shift, short delta, const POINT& pt);

public:
	/*
		鼠标事件
	*/
	virtual LRESULT DoMouse(DxMouseAction action, DxShiftState shift, const POINT& pt);

	/*
		键盘事件
	*/
	virtual LRESULT DoKeyboard(DxKeyAction action, WORD& key, DxShiftState shift);

	/*
		通知事件
	*/
	virtual void DoNotify(DxNotifyId id, DWORD param);

	/*
		请求事件
	*/
	virtual LRESULT DoQuery(DxQueryId id, DWORD param);

	/*
		更新
	*/
	virtual void DoUpdate(DWORD tick);

	/*
		绘制，注意: Render此时已经调整好绘制坐标的偏移, 
		因此调用Render的绘制函数时, 应该传入视图的客户坐标, 而非屏幕坐标
	*/
	virtual void DoPaint(DxRender* render);

	/*
		处理投递事件
	*/
	virtual void HandlePostEvent(DxPostId id, DWORD param1, DWORD param2);

	/*
		处理滚轮事件
	*/
	virtual BOOL DoMouseWheel(DxShiftState shift, short delta, const POINT& pt);

protected:
	/*
		视图初始化好，子类可以在这里初始化数据
	*/
	virtual void DoInitialize();

	/*
		视图将结束，子类可以在这里清理数据
	*/
	virtual void DoFinalize();

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
	void DelScreenPostEvent();

protected:
	DxScreen*			mOwnerScreen;		// 所属屏幕
	DxWindow*			mOwnerWindow;		// 所拥有的窗口，如果View为窗口，则mOwnerWindow为自己
	DxView*				mParentView;		// 父视图
	DxViewListener*		mViewListener;		// 事件接口
	DxViewVector		mChildViews;		// 子视图列表
	DxViewStyle			mViewStyle;			// 视图风格
	int					mUserData;			// 可以挂任意数据
	int					mLeft;				// 左
	int					mTop;				// 顶
	int					mWidth;				// 宽
	int					mHeight;			// 高
	int					mGroup;				// 组
	SIZE				mMinSize;			// 最小尺寸，为0表示无限制
	SIZE				mMaxSize;			// 最大尺寸，为0表示无限制
	BOOL				mIsVisible;			// 可见
	BOOL				mIsEnable;			// 可用
	int					mWheelAccumulator;	// 鼠标滚轮累加器
};

class DxShortcutMgr;

/*
	快捷键事件
*/
struct DxShortcutListener
{
	/*
		快捷键事件 DxShortcut为相应快捷键
	*/
	virtual void OnShortcut(DxShortcutMgr* mgr, DxShortcut shortcut) 
	{
	}
};

/*
	快捷键管理器
*/
class DxShortcutMgr
{
	typedef std::map<DxShortcut, DxShortcutListener*> DxShortcutMap;

public:
	/*
		增加快捷键以及相应事件
	*/
	BOOL AddShortcut(DxShortcut shortcut, DxShortcutListener* listener)
	{
		DxShortcutMap::iterator itr = mShortcutMap.find(shortcut);
		if (itr != mShortcutMap.end())
			return FALSE;

		mShortcutMap.insert(std::make_pair(shortcut, listener));
		return TRUE;
	}

	/*
		删除快捷键
	*/
	BOOL DelShortcut(DxShortcut shortcut)
	{
		DxShortcutMap::iterator itr = mShortcutMap.find(shortcut);
		if (itr == mShortcutMap.end())
			return FALSE;

		mShortcutMap.erase(itr);

		return TRUE;
	}

protected:
	/*
		触发事件 成功返回TRUE 没有相应快捷键则返回FALSE
	*/
	virtual BOOL HandleShortcut(DxShortcut shortcut)
	{
		DxShortcutMap::iterator itr = mShortcutMap.find(shortcut);
		if (itr == mShortcutMap.end())
			return FALSE;

		if (itr->second)
		{
			itr->second->OnShortcut(this, shortcut);
			return TRUE;
		}
		return FALSE;
	}

private:
	DxShortcutMap mShortcutMap;
};

/*
	窗口列表
*/
typedef std::list<DxWindow*> DxWindowList;

/*
	创建视图窗口
*/
#define NEW_WINDOW(theclass, ownerscreen) NEW_VIEW(theclass, ownerscreen, ownerscreen)
/*
	释放窗口
*/
#define FREE_WINDOW(theobject) FREE_VIEW(theobject)

/*
	窗口类，大概用法如下：
	*.  创建
		DxWindow* wnd = NEW_WINDOW(DxWindow, screen);
	*.  设置各种属性并显示
		... ...
		wnd->Show();
	*.  释放
		FREE_WINDOW(wnd);

	可以继承自该类实现自定义窗口，详见LnDxCtrls.h
*/
class DxWindow: public DxView, public DxShortcutMgr
{
	DECLARE_RUNTIMEINFO(DxWindow)
public:
	DxWindow():
		mWndStyle(WS_MOVABLE | WS_SIZABLE | WS_ACTIVATABLE),
		mFrameSize(3),
		mFocusedView(NULL),
		mWndState(0)
	{
		// 必须等Show以后才显示出来
		mIsVisible = FALSE;
		mMinSize.cx = 2 * mFrameSize;
		mMinSize.cy = 2 * mFrameSize;
	}

	/*
		检查可移动状态
	*/
	BOOL IsMovable();

	/*
		设置可移动状态
	*/
	void SetMovable(BOOL isMovable);

	/*
		检查可拖动状态
	*/
	BOOL IsSizable();

	/*
		设置可改变大小状态
	*/
	void SetSizable(BOOL isSizable);

	/*
		ESC关闭
	*/
	BOOL IsEscClose();

	/*
		设置ESC关闭
	*/
	void SetEscClose(BOOL isEscClose);

	/*
		是否为顶层窗口
	*/
	BOOL IsTopMost();

	/*
		设置顶层窗口
	*/
	void SetTopMost(BOOL isTopMose);

	/*
		是否可激活
	*/
	BOOL IsActivatable();

	/*
		设置可激活性
	*/
	void SetActivatable(BOOL activatable);

	/*
		激活自己
	*/
	BOOL Active();

	/*
		是否激活
	*/
	BOOL IsActive();

	/*
		能否激活
	*/
	BOOL CanActive();

	/*
		取得焦点视图
	*/
	DxView* GetFocusedView();

	/*
		设焦点视图
	*/
	BOOL SetFocusedView(DxView* view);

	/*
		寻找下一个Tab视图，startView指定开始寻找的起始点
	*/
	DxView* FindNexTabtView(DxView* startView, BOOL isForward);

	/*
		寻找下一个Arrow视图，startView指定开始寻找的起始点
	*/
	DxView* FindNextArrowView(DxView* startView, BOOL isForward);

	/*
		显示
	*/
	void Show(BOOL isActive = TRUE);

	/*
		关闭
	*/
	void Close();

	/*
		隐藏
	*/
	void Hide();

	/*
		模态显示
	*/
	int	ShowModal();

	/*
		关闭模态, ret指定ShowModal返回的值
	*/
	BOOL CloseModal(int ret);

	/*
		是否模态
	*/
	BOOL IsModal();

	/*
		调整位置到屏幕中间
	*/
	void ScreenCenter();

	/*
		是否正在拖动
	*/
	virtual BOOL IsDraging();

	/*
		开始拖动
	*/
	virtual void BeginDrag(const POINT& pt);

	/*
		拖动
	*/
	virtual void DragTo(const POINT& pt);

	/*
		结束拖动
	*/
	virtual void EndDrag(const POINT& pt);

	/*
		取消拖动
	*/
	virtual void CancelDrag();

public:
	virtual void Finalize();

	virtual LRESULT DoMouse(DxMouseAction action, DxShiftState state, const POINT& pt);

	virtual void DoNotify(DxNotifyId id, DWORD param);

	virtual LRESULT DoQuery(DxQueryId id, DWORD param);

	virtual DxHitTest HitTestView(const POINT& pt);

	virtual BOOL PreKeyHandle(WORD& key, DxShiftState shift);

	virtual void HandlePostAction(DxPostId paId, DWORD param1, DWORD param2);

protected:
	/*
		生成Tab顺序列表
	*/
	void GenTabList(DxViewVector& viewVector, DxView* parentView);

protected:
	DxWndStyle		mWndStyle;			// 窗口风格
	DxWndState		mWndState;			// 窗口状态
	int				mModalResult;		// 模态返回结束
	int				mFrameSize;			// 外框的厚度，影响拖动大小
	DxView*			mFocusedView;		// 获得焦点的视图
};

/*
	创建屏幕类
*/
#define NEW_SCREEN(theclass) NEW_VIEW(theclass, NULL, NULL)
/*
	释放屏幕
*/
#define FREE_SCREEN(theobject) FREE_VIEW(theobject)

/*
	屏幕类，大概用法如下:
	*.  创建
		DxScreen* screen = NEW_SCREEN(DxScreen);
	*.  设宿主窗口
		screen->SetHostWnd(hwnd);
	*.  设渲染器
		screen->SetRender(GetRender);
	*.  定时更新和绘制，一般在消息循环的Idle时做
		screen->Update();
		// 这里可能要限制帧数
		screen->Screen();
	*.  释放
		FREE_SCREEN(screen);
		
*/
class DxScreen: public DxView, public DxShortcutMgr
{
	typedef std::map<DxCursorId, HCURSOR> DxCursorMap;
	typedef std::list<DxPostInfo> DxPostInfoList;
	typedef std::vector<DxView*> DxTimerVector;
	DECLARE_RUNTIMEINFO(DxScreen)
public:
	DxScreen(): 
		mDefHostWndProc(NULL),
		mHostWnd(NULL), 
		mCaptureView(NULL),
		mActiveWindow(NULL),
		mMouseX(0),
		mMouseY(0),
		mModalLevel(0),
		mMsgLooper(NULL),
		mHoverView(NULL),
		mRender(NULL),
		mCanClip(FALSE),
		mTimerVector(512)
	{
	}

	/*
		设置宿主窗口，虚拟屏幕将挂接窗口的键盘鼠标消息
	*/
	void SetHostWnd(HWND hwnd);

	/*
		取宿主窗口
	*/
	HWND GetHostWnd(); 

	/*
		设渲染器
	*/
	void SetRender(DxRender* render);

	/*
		取渲染器
	*/
	DxRender* GetRender();

	/*
		是否支持剪裁
	*/
	void SetClip(BOOL canClip);

	/*
		是否支持剪裁
	*/
	BOOL CanClip();

	/*
		取某个子窗口
	*/
	DxWindow* GetChildWindow(int idx);

	/*
		取活动窗口
	*/
	DxWindow* GetActiveWindow(); 

	/*
		取焦点视图, 整个屏幕只有一个视图可以接收键盘输入, 这个视图就是焦点视图
		焦点视图必须在活动窗口中
	*/
	DxView* GetFocusedView();

	/*
		设置活动窗口
	*/
	BOOL SetActiveWindow(DxWindow* wnd);

	/*
		激活下一个窗口
	*/
	void ActiveNextWindow(DxWindow* wnd);

	/*
		取得鼠标捕获的视图
	*/
	DxView* GetCaptureView(); 

	/*
		设置鼠标捕获的视图
	*/
	BOOL SetCaptureView(DxView* view);

	/*
		取id类型的光标
	*/
	HCURSOR GetCursor(DxCursorId id);

	/*
		添加自定义光标
	*/
	BOOL AddCursor(DxCursorId id, HCURSOR cursor);

	/*
		当前光标位置
	*/
	int GetMouseX();
	int GetMouseY(); 

	/*
		投递动作，效果类型于PostMessage
		ActionID指明动作ID，param1和param2是动作附带的参数，
	*/
	BOOL PostEvent(DxView* view, DxPostId id, DWORD param1, DWORD param2);

	/*
		移除所有关于该视图的投递动作
	*/
	void DelPostEvents(DxView* view);

	/*
		开始模态窗口
	*/
	void BeginModal(DxWindow* wnd, DxWindowList& wndList);

	/*
		结束模态窗口
	*/
	void EndModal(DxWindow* wnd, DxWindowList& wndList);

	/*
		取得消息循环接口
	*/
	MsgLooper* GetMsgLooper();

	/*
		设消息循环接口
	*/
	void SetMsgLooper(MsgLooper* msgLooper);

	/*
		取鼠标盘旋在上面的视图
	*/
	DxView* GetHoverView(); 

	/*
		重置鼠标盘旋视图
	*/
	void ResetHoverView(); 

	/*
		绘制
	*/
	virtual void Paint();

	/*
		更新
	*/
	virtual void Update(DWORD tick);

	/*
		设置定时器, 返回定时器ID，如果为NULL，表现函数失败
	*/
	UINT SetTimer(DxView* view, UINT elapse);

	/*
		取消定时器，id为SetTimer返回的值
	*/
	BOOL KillTimer(DxView* view, UINT id);

	/*
		清除视图相关的定时器
	*/
	void ClearTimer(DxView* view);

public:
	/*
		对于屏幕要作一些特定的处理
	*/
	virtual DxView* Initialize(DxView* parentView, DxScreen* screen);

	/*
		清理全局资源
	*/
	virtual void Finalize();

	/*
		子视图必须是窗口
	*/
	virtual BOOL InsertChild(DxView* childView, int pos , BOOL isCheck = TRUE);

	/*
		处理顶层窗口的情况
	*/
	virtual DxView* GetViewAtPos(const POINT& pt, BOOL allowDisabled  = FALSE);

	/*
		键盘消息预处理
	*/
	virtual BOOL PreKeyHandle(WORD& key, DxShiftState shift);

protected:
	/*
		初始化光标表
	*/
	void InitCursorTable();

	/*
		消毁所有光标
	*/
	void DestroyAllCursor();

	/*
		绘制子视图
	*/
	void PaintChilds(DxView* parentView, const RECT& rcParentPaint, const RECT& rcParentScreen);

	/*
		更新子视图
	*/
	void UpdateChilds(DxView* parentView, DWORD tick);

	/*
		定时器函数
	*/
	BOOL RestoreTimer(DxView* view, UINT id);
	UINT AllocTimer(DxView* view);

protected:
	/*
		子类化宿主窗口
	*/
	void SubclassWindow(HWND hwnd);

	/*
		反子类化宿主窗口
	*/
	void UnsubclassWindow(HWND hwnd);

	/*
		宿主窗口过程
	*/
	static LRESULT CALLBACK	StdWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/*
		返回TRUE将不再交给mDefHostWndProc处理，lRet即是处理过程的返回值
	*/
	virtual BOOL WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& lRet);

	/*
		宿主窗口默认处理过程
	*/
	LRESULT DefWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/*
		具体窗口消息处理
	*/
	void WMMouseMove(WPARAM wparam, LPARAM lparam);
	void WMSize(WPARAM wparam, LPARAM lparam);
	void WMLButtonDblClk(WPARAM wparam, LPARAM lparam);
	void WMLButtonDown(WPARAM wparam, LPARAM lparam);
	void WMLButtonUp(WPARAM wparam, LPARAM lparam);
	void WMCancelMode(WPARAM wparam, LPARAM lparam);
	void WMOtherMouse(DxMouseAction action, WPARAM wparam, LPARAM lparam);
	void WMMouseWheel(WPARAM wparam, LPARAM lparam);
	void WMKeyDown(WPARAM wparam, LPARAM lparam);
	void WMKeyMsg(DxKeyAction action, WPARAM wparam, LPARAM lparam);
	void WMContextMenu(WPARAM wparam, LPARAM lparam);
	void WMPostAction(WPARAM wparam, LPARAM lparam);
	BOOL WMSetCursor(WPARAM wparam, LPARAM lparam);
	BOOL WMTimer(WPARAM wparam, LPARAM lparam);

protected:
	WNDPROC				mDefHostWndProc;		// 宿主窗口默认的窗口过程
	HWND				mHostWnd;				// 宿主窗口
	DxView*				mCaptureView;			// 捕获鼠标的视图
	DxWindow*			mActiveWindow;			// 当前活动的窗口
	DxCursorMap			mCursorMap;				// 光标与光标类型的对应表
	DxPostInfoList		mPostActionList;		// 投递动作列表
	int					mMouseX;				// 当前光标位置X
	int					mMouseY;				// 当前光标位置Y
	int					mModalLevel;			// 模态层次
	MsgLooper*			mMsgLooper;				// 消息循环
	DxView*				mHoverView;				// 鼠标盘旋的视图
	DxRender*			mRender;				// 渲染器
	BOOL				mCanClip;				// 支持剪裁，可能会明显降低性能，慎用
	DxTimerVector		mTimerVector;			// 定时器列表
};


//////////////////////////////////////////////////////////////////////////
// implement

//------------------------------------------------------------------------------
// Helpers

/*
	取得一个视图的父窗口，如果pView本身是窗口，则返回自己
*/
inline DxWindow* GetParentWindow(DxView* view)
{
	DxView* parentView = view;
	while (parentView && (!OBJECT_DERIVEDFROM(parentView, DxWindow)))
		parentView = parentView->GetParentView();

	return (DxWindow*)parentView;
}

/*
	将virtual keys 转换为ShiftState
*/
inline DxShiftState VKeyToShiftState(WORD key)
{
	DxShiftState shift = 0;
	if (HAS_FLAG(key, MK_SHIFT))	ADD_FLAG(shift, ssShift);
	if (HAS_FLAG(key, MK_CONTROL))  ADD_FLAG(shift, ssCtrl);
	if (::GetKeyState(VK_MENU) < 0) ADD_FLAG(shift, ssAlt);
	if (HAS_FLAG(key, MK_LBUTTON))	ADD_FLAG(shift, ssLeft);
	if (HAS_FLAG(key, MK_RBUTTON))	ADD_FLAG(shift, ssRight);
	if (HAS_FLAG(key, MK_MBUTTON))	ADD_FLAG(shift, ssMiddle);
	return shift;
}

/*
	将Key data 转换为ShiftState
*/
inline DxShiftState KeyDataToShiftState(long keyData)
{
	#define ALT_MASK 0x20000000
	DxShiftState shift = 0;
	if (::GetKeyState(VK_SHIFT) < 0)	ADD_FLAG(shift, ssShift);
	if (::GetKeyState(VK_CONTROL) < 0)	ADD_FLAG(shift, ssCtrl);
	if (HAS_FLAG(keyData, ALT_MASK))	ADD_FLAG(shift, ssAlt);
	return shift;
}

//------------------------------------------------------------------------------
// DxView

IMPLEMENT_RUNTIMEINFO(DxView, Object)

inline DxView* DxView::Initialize(DxView* parentView, DxScreen* screen)
{
	LN_ASSERT(screen != NULL);
	mOwnerScreen = screen;

	if (parentView)
		parentView->AddChild(this, FALSE);

	DoInitialize();

	return this;
}

inline void DxView::Finalize()
{
	DoFinalize();

	FreeChilds();
	SetParentView(NULL);

	// 去除焦点
	if (mOwnerWindow && mOwnerWindow->GetFocusedView() == this)
		mOwnerWindow->SetFocusedView(NULL);

	// 去除鼠标捕获
	ReleaseCapture();

	// 重置鼠标盘旋视图
	ResetScreenHoverView();

	// 删除属于该视图的投递动作
	DelScreenPostEvent();

	// 清除定时器
	if (mOwnerScreen)
		mOwnerScreen->ClearTimer(this);
}

inline void DxView::DoInitialize()
{

}

inline void DxView::DoFinalize()
{

}

inline DxScreen* DxView::GetOwnerScreen() 
{ 
	return mOwnerScreen; 
}

inline DxWindow* DxView::GetOwnerWindow() 
{ 
	return mOwnerWindow; 
}

inline DxView* DxView::GetParentView() 
{ 
	return mParentView; 
}

inline void DxView::SetParentView(DxView* parentView)
{
	if (parentView != mParentView)
	{
		if (mParentView)
			mParentView->RemoveChild(this);
		if (parentView)
			parentView->AddChild(this);
	}
}

inline BOOL DxView::AddChild(DxView* childView, BOOL isCheck)
{
	return InsertChild(childView, (int)mChildViews.size(), isCheck);
}

inline BOOL DxView::InsertChild(DxView* childView, int pos , BOOL isCheck)
{
	LN_ASSERT(childView);

	if (isCheck)
	{
		if (std::find(mChildViews.begin(), mChildViews.end(), childView) != mChildViews.end())
			return FALSE;
	}

	if (pos < 0) 
		pos = 0;
	if (pos > (int)mChildViews.size()) 
		pos = (int)mChildViews.size();

	if (pos == (int)mChildViews.size())
		mChildViews.push_back(childView);
	else
		mChildViews.insert((mChildViews.begin() + pos), childView);

	childView->mParentView = this;
	childView->mOwnerWindow = GetParentWindow(childView);
	return TRUE;
}

inline void DxView::RemoveChild(DxView* childView)
{
	DxViewVector::iterator itr = std::find(mChildViews.begin(), mChildViews.end(), childView);
	if (itr != mChildViews.end())
		mChildViews.erase(itr);
	childView->mParentView = NULL;
}

inline void DxView::DeleteChild(int pos)
{
	if ((pos < 0) || (pos >= (int)mChildViews.size()))
		return;

	DxView* childView = mChildViews[pos];
	mChildViews.erase(mChildViews.begin() + pos);
	childView->mParentView = NULL;
}

inline int DxView::GetChildCount() 
{ 
	return (int)mChildViews.size(); 
}

inline DxView* DxView::GetChildView(int idx) 
{ 
	return mChildViews[idx]; 
}

inline DxView* DxView::GetLastChild()
{
	DxViewVector::reverse_iterator revItr = mChildViews.rbegin();
	if (revItr == mChildViews.rend())
		return NULL;
	else
		return (*revItr);
}

inline DxView* DxView::GetFirstChild()
{
	DxViewVector::iterator revItr = mChildViews.begin();
	if (revItr == mChildViews.end())
		return NULL;
	else
		return (*revItr);

}

inline int DxView::IndexOf(DxView* childView)
{
	if (NULL == childView)
		return -1;

	DxViewVector::iterator Itr = std::find(mChildViews.begin(), mChildViews.end(), childView);
	if (Itr == mChildViews.end())
		return -1;
	else 
		return (int)(Itr - mChildViews.begin());
}
inline BOOL DxView::MoveChildPos(DxView* childView, int newPos)
{
	LN_ASSERT(childView);

	if (0 == mChildViews.size())
		return FALSE;

	int oldPos = IndexOf(childView);
	if (oldPos < 0)
		return FALSE;

	if (newPos < 0) 
		newPos = 0;
	if (oldPos == newPos) 
		return TRUE;

	mChildViews.erase(mChildViews.begin() + oldPos);
	if (newPos >= (int)mChildViews.size())
		mChildViews.push_back(childView);
	else
		mChildViews.insert(mChildViews.begin() + newPos, childView);

	return TRUE;
}

inline void DxView::BringToFront()
{
	if (mParentView)
		mParentView->MoveChildPos(this, mParentView->GetChildCount() - 1);
}


inline void DxView::SendToBack()
{
	if (mParentView)
		mParentView->MoveChildPos(this, 0);
}

inline int DxView::GetUserData() 
{ 
	return mUserData; 
}

inline void DxView::SetUserData(int data) 
{ 
	mUserData = data; 
}

inline int DxView::GetLeft() 
{ 
	return mLeft; 
}

inline int DxView::GetTop() 
{ 
	return mTop; 
}

inline int DxView::GetWidth() 
{ 
	return  mWidth; 
}

inline int DxView::GetHeight() 
{ 
	return mHeight; 
}

inline void DxView::SetLeft(int left)
{
	SetPos(left, mTop);
}

inline void DxView::SetTop(int top)
{
	SetPos(mLeft, top);
}

inline void DxView::SetWidth(int width)
{
	SetSize(width, mHeight);
}

inline void DxView::SetHeight(int height)
{
	SetSize(mWidth, height);
}

inline void DxView::SetSize(int width, int height)
{
	if ((width < 0) || (height < 0))
		return;
	if ((width == mWidth) && (height == mHeight))
		return;

	SIZE sz;
	sz.cx = width;
	sz.cy = height;
	DoNotify(NID_SIZECHANGING, (DWORD)(&sz));

	if (mMinSize.cx != 0)
	{
		if (mMinSize.cx > sz.cx)
			sz.cx = mMinSize.cx;
	}
	if (mMinSize.cy != 0)
	{
		if (mMinSize.cy > sz.cy)
			sz.cx = mMinSize.cy;
	}
	if (mMaxSize.cx != 0)
	{
		if (mMaxSize.cx < sz.cx)
			sz.cx = mMaxSize.cx;
	}
	if (mMaxSize.cy != 0)
	{
		if (mMaxSize.cy < sz.cy)
			sz.cy = mMaxSize.cy;
	}

	mWidth = sz.cx;
	mHeight = sz.cy;
	DoNotify(NID_SIZECHANGED, NULL);
}

inline void DxView::SetPos(int left, int top)
{
	if ((left == mLeft) && (top == mTop))
		return;

	POINT pt;
	pt.x = left;
	pt.y = top;
	DoNotify(NID_POSCHANGING, (DWORD)(&pt));

	mLeft = pt.x;
	mTop = pt.y;
	DoNotify(NID_POSCHANGED, NULL);
}

inline void DxView::SetBound(const RECT& rc)
{
	SetBound(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

inline void DxView::SetBound(int left, int top, int width, int height)
{
	SetPos(left, top);
	SetSize(width, height);
}

inline SIZE DxView::GetMinSize() 
{ 
	return mMinSize; 
}

inline SIZE DxView::GetMaxSize() 
{ 
	return mMaxSize; 
}

inline void DxView::SetMinSize(int cx, int cy)
{
	if (cx < 0 || cy < 0)
		return;

	mMinSize.cx = cx;
	mMinSize.cy = cy;

	// 重设尺寸
	SetSize(
		mWidth < cx ? cx : mWidth, 
		mHeight < cy ? cy : mHeight);
}

inline void DxView::SetMaxSize(int cx, int cy)
{
	if (cx < 0 || cy < 0)
		return;

	mMaxSize.cx = cx;
	mMaxSize.cy = cy;

	// 重设尺寸
	SetSize(
		mWidth > cx ? cx : mWidth, 
		mHeight > cy ? cy : mHeight);
}

inline void DxView::GetClientRect(RECT& rc)
{
	SetRect(&rc, 0, 0, mWidth, mHeight);
}

inline void DxView::GetViewRect(RECT& rc)
{
	SetRect(&rc, mLeft, mTop, mLeft + mWidth, mTop + mHeight);
}

inline void DxView::GetScreenRect(RECT& rc)
{
	int Left = 0;
	int Top = 0;
	DxView* view = this;

	do
	{
		Left += view->GetLeft();
		Top += view->GetTop();
		view = view->GetParentView();
	}  
	while (view && !OBJECT_DERIVEDFROM(view, DxScreen));

	SetRect(&rc, Left, Top, Left + mWidth, Top + mHeight);
}

inline POINT DxView::ScreenToClient(const POINT& pt)
{
	RECT rc;
	GetScreenRect(rc);
	POINT ptRet;
	ptRet.x = pt.x - rc.left;
	ptRet.y = pt.y - rc.top;
	return ptRet;
}

inline POINT DxView::ClientToScreen(const POINT& pt)
{
	RECT rc;
	GetScreenRect(rc);
	POINT ptRet;
	ptRet.x = pt.x + rc.left;
	ptRet.y = pt.y + rc.top;
	return ptRet;
}

inline DxView* DxView::GetViewAtPos(POINT pt, BOOL allowDisabled)
{
	if (HitTestView(pt) && mIsVisible && (mIsEnable || allowDisabled))
	{
		POINT tmpPt;
		DxViewVector::reverse_iterator revItr = mChildViews.rbegin();
		while (revItr != mChildViews.rend())
		{
			tmpPt = pt;
			DxView* view = (*revItr);
			tmpPt.x -= view->mLeft;
			tmpPt.y -= view->mTop;
			view = view->GetViewAtPos(tmpPt, allowDisabled);
			if (NULL != view)
				return view;

			++revItr;
		}

		return this;
	}

	return NULL;
}

inline DxHitTest DxView::HitTestView(const POINT& pt)
{
	RECT rc;
	GetClientRect(rc);
	return PtInRect(&rc, pt) ? HT_CLIENT : HT_NONE;
}

inline BOOL DxView::IsTabStop() 
{ 
	return HAS_FLAG(mViewStyle, VS_TABSTOP); 
}

inline void DxView::SetTabStop(BOOL isTabStop)
{
	if (isTabStop)
		ADD_FLAG(mViewStyle, VS_TABSTOP);
	else
		DEL_FLAG(mViewStyle, VS_TABSTOP);
}

inline BOOL DxView::IsFocusable()
{
	return HAS_FLAG(mViewStyle, VS_FOCUSABLE);
}

inline void DxView::SetFocusable(BOOL isFocusable)
{
	if (isFocusable)
		ADD_FLAG(mViewStyle, VS_FOCUSABLE);
	else
		DEL_FLAG(mViewStyle, VS_FOCUSABLE);
}

inline BOOL DxView::IsFocused(BOOL isInWnd)
{
	if ((NULL == mOwnerWindow) || (mOwnerWindow->GetFocusedView() != this))
		return FALSE;

	if (!isInWnd)
		return mOwnerWindow->IsActive();
	else
		return TRUE;
}

inline BOOL DxView::CanFocus()
{
	return IsFocusable() && VisibleAndEnable();
}

inline BOOL DxView::SetFocus()
{
	if (NULL == mOwnerWindow)
		return FALSE;

	return mOwnerWindow->SetFocusedView(this);
}

inline BOOL DxView::IsCaptured()
{
	if (OBJECT_DERIVEDFROM(this, DxScreen))
		return ((DxScreen*)this)->GetCaptureView() == this;
	else if (mOwnerScreen)
		return mOwnerScreen->GetCaptureView() == this;
	else
		return FALSE;
}

inline BOOL DxView::CanCapture()
{
	return VisibleAndEnable();
}

inline BOOL DxView::SetCapture()
{
	if (OBJECT_DERIVEDFROM(this, DxScreen))
		return ((DxScreen*)this)->SetCaptureView(this);
	else if (mOwnerScreen)		
		return mOwnerScreen->SetCaptureView(this);
	else
		return FALSE;
}

inline void DxView::ReleaseCapture()
{
	if (IsCaptured())
	{
		if (OBJECT_DERIVEDFROM(this, DxScreen))
			((DxScreen*)this)->SetCaptureView(NULL);
		else if (mOwnerScreen)
			mOwnerScreen->SetCaptureView(NULL);
	}
}

inline BOOL DxView::IsVisible() 
{ 
	return mIsVisible; 
}

inline void DxView::SetVisible(BOOL isVisible)
{
	if (mIsVisible != isVisible)
	{
		mIsVisible = isVisible;
		UpdateVisible(isVisible);
		DoNotify(NID_VISIBLECHANGED, DWORD(mIsVisible));
	}
}

inline BOOL DxView::IsEnable() 
{ 
	return mIsEnable; 
}

inline void DxView::SetEnable(BOOL isEnable)
{
	if (mIsEnable != isEnable)
	{
		mIsEnable = isEnable;
		UpdateEnable(isEnable);
		DoNotify(NID_ENABLECHANGED, DWORD(mIsEnable));
	}
}

inline BOOL DxView::IsHoverView()
{
	LN_ASSERT(mOwnerScreen);
	return (mOwnerScreen->GetHoverView() == this);
}

inline int DxView::GetGroup()
{
	return mGroup;
}

inline void DxView::SetGroup(int group)
{
	mGroup = group;
}

inline DxViewListener* DxView::GetViewListener() 
{ 
	return mViewListener; 
}

inline DxViewListener* DxView::SetViewListener(DxViewListener* listener)
{
	DxViewListener* prelistener = mViewListener;
	mViewListener = listener;
	return prelistener;
}

inline void DxView::HandleMouseWheel(DxShiftState shift, short delta, const POINT& pt)
{
	if (!DoMouseWheel(shift, delta, pt))
	{
		if (mParentView)
			mParentView->HandleMouseWheel(shift, delta, pt);
	}
}

inline LRESULT DxView::DoMouse(DxMouseAction action, DxShiftState shift, const POINT& pt)
{
	if (mViewListener)
		return mViewListener->OnMouse(this, action, shift, pt);
	else
		return 0;
}

inline LRESULT DxView::DoKeyboard(DxKeyAction action, WORD& key, DxShiftState shift)
{
	if (mViewListener)
		return mViewListener->OnKeyboard(this, action, key, shift);
	else
		return 0;
}

inline void DxView::DoNotify(DxNotifyId id, DWORD param)
{
	if (mViewListener)
		mViewListener->OnNotify(this, id, param);
}

inline LRESULT DxView::DoQuery(DxQueryId id, DWORD param)
{
	if (mViewListener)
		return mViewListener->OnQuery(this, id, param);
	return 0;
}

inline void DxView::DoUpdate(DWORD tick)
{
	if (mViewListener)
		mViewListener->OnUpdate(this, tick);
}

inline void DxView::DoPaint(DxRender* render)
{
	if (mViewListener)
		mViewListener->OnPaint(this, render);
}

inline void DxView::HandlePostEvent(DxPostId id, DWORD param1, DWORD param2)
{

}

inline BOOL DxView::DoMouseWheel(DxShiftState shift, short delta, const POINT& pt)
{
	BOOL bRet = FALSE;
	mWheelAccumulator += delta;
	while (abs(mWheelAccumulator) >= WHEEL_DELTA)
	{
		if (mWheelAccumulator < 0)
		{
			mWheelAccumulator += WHEEL_DELTA;
			bRet = DoMouse(maMouseWheelDown, shift, pt);
		}
		else
		{
			mWheelAccumulator -= WHEEL_DELTA;
			bRet = DoMouse(maMouseWheelUp, shift, pt);
		}
	}
	return bRet;
}

inline void DxView::UpdateVisible(BOOL isVisible)
{
	if (!isVisible)
	{
		ReleaseCapture();
		ResetScreenHoverView();
		if (IsFocused(TRUE))
			mOwnerWindow->SetFocusedView(NULL);
	}

	DxViewVector::iterator itr = mChildViews.begin();
	while (itr != mChildViews.end())
	{
		(*itr)->UpdateVisible(isVisible);
		++itr;
	}

}

inline void DxView::UpdateEnable(BOOL isEnable)
{
	if (!isEnable)
	{
		ReleaseCapture();
		ResetScreenHoverView();
		if (IsFocused(TRUE))
			mOwnerWindow->SetFocusedView(NULL);
	}

	DxViewVector::iterator itr = mChildViews.begin();
	while (itr != mChildViews.end())
	{
		(*itr)->UpdateEnable(isEnable);
		++itr;
	}
}

inline BOOL DxView::VisibleAndEnable()
{
	// 遍历父视图链，检查可用可见性
	DxView* view = this;
	do
	{
		if (!(view->mIsEnable && view->mIsVisible))
			return FALSE;
		view = view->mParentView;
	} while (view);

	return TRUE;
}

inline void DxView::FreeChilds()
{
	// 从最后一个视图往前释放
	while (!mChildViews.empty())
	{
		DxView* childView = mChildViews.back();
		mChildViews.pop_back();
		childView->mParentView = NULL;

		FREE_VIEW(childView);
	}
}

inline void DxView::ResetScreenHoverView()
{
	if (OBJECT_DERIVEDFROM(this, DxScreen))
	{
		DxScreen* screen = (DxScreen*)this;
		if (screen->GetHoverView() == this)
			screen->ResetHoverView();
	}
	else if (mOwnerScreen)
	{
		if (mOwnerScreen->GetHoverView() == this)
			mOwnerScreen->ResetHoverView();
	}
}

inline void DxView::DelScreenPostEvent()
{
	if (OBJECT_DERIVEDFROM(this, DxScreen))
	{
		DxScreen* screen = (DxScreen*)this;
		screen->DelPostEvents(this);
	}
	else if (mOwnerScreen)
	{
		mOwnerScreen->DelPostEvents(this);
	}
}

//------------------------------------------------------------------------------
// DxWindow

/*
	移动时用到的状态，这里假定同时只有一个窗口正在移动或拉大小
*/
_declspec(selectany) BOOL			gIsDraging = FALSE;	// 标志是否开始拖动
_declspec(selectany) DxHitTest		gDragHitTest;		// 保存拖动时的点击测试值
_declspec(selectany) POINT			gDragPoint;			// 拖动时的原始位置
_declspec(selectany) int			gOrgWidth;			// 拖动时的大小位置
_declspec(selectany) int			gOrgHeight;
_declspec(selectany) int			gOrgLeft;
_declspec(selectany) int			gOrgTop;

IMPLEMENT_RUNTIMEINFO(DxWindow, DxView)

inline BOOL DxWindow::IsMovable() 
{ 
	return HAS_FLAG(mWndStyle, WS_MOVABLE); 
}

inline void DxWindow::SetMovable(BOOL isMovable)
{ 
	if (isMovable) 
		ADD_FLAG(mWndStyle, WS_MOVABLE);
	else 
		DEL_FLAG(mWndStyle, WS_MOVABLE); 
}

inline BOOL DxWindow::IsSizable() 
{ 
	return HAS_FLAG(mWndStyle, WS_SIZABLE); 
}

inline void DxWindow::SetSizable(BOOL isSizable)
{
	if (isSizable)
		ADD_FLAG(mWndStyle, WS_SIZABLE);
	else
		DEL_FLAG(mWndStyle, WS_SIZABLE);
}

inline BOOL DxWindow::IsEscClose() 
{ 
	return HAS_FLAG(mWndStyle, WS_ESCCLOSE); 
}

inline void DxWindow::SetEscClose(BOOL isEscClose)
{
	if (isEscClose)
		ADD_FLAG(mWndStyle, WS_ESCCLOSE);
	else
		DEL_FLAG(mWndStyle, WS_ESCCLOSE);
}

inline BOOL DxWindow::IsTopMost() 
{ 
	return HAS_FLAG(mWndStyle, WS_TOPMOST); 
}

inline void DxWindow::SetTopMost(BOOL isTopMose)
{
	if (isTopMose)
		ADD_FLAG(mWndStyle, WS_TOPMOST);
	else
		DEL_FLAG(mWndStyle, WS_TOPMOST);
}

inline BOOL DxWindow::IsActivatable()
{
	return HAS_FLAG(mWndStyle, WS_ACTIVATABLE);
}

inline void DxWindow::SetActivatable(BOOL activatable)
{
	if (activatable)
		ADD_FLAG(mWndStyle, WS_ACTIVATABLE);
	else
		DEL_FLAG(mWndStyle, WS_ACTIVATABLE);
}

inline BOOL DxWindow::Active()
{
	return mOwnerScreen->SetActiveWindow(this);
}

inline BOOL DxWindow::IsActive()
{
	return mOwnerScreen->GetActiveWindow() == this;
}

inline BOOL DxWindow::CanActive()
{
	return VisibleAndEnable() && IsActivatable();
}

inline DxView* DxWindow::GetFocusedView() 
{ 
	return mFocusedView; 
}

inline BOOL DxWindow::SetFocusedView(DxView* view)
{
	if (mFocusedView == view)
		return TRUE;

	if (view && (!view->CanFocus()))
		return FALSE;

	DxView* oldView = mFocusedView;
	mFocusedView = view;
	if (NULL != oldView)
		oldView->DoNotify(NID_FOCUSCHANGED, FALSE);
	if (NULL != mFocusedView)
		mFocusedView->DoNotify(NID_FOCUSCHANGED, TRUE);
	return TRUE;
}

inline DxView* DxWindow::FindNexTabtView(DxView* startView, BOOL isForward)
{
	DxViewVector viewVector;
	GenTabList(viewVector, this);
	if (viewVector.empty())
		return NULL;

	DxViewVector::iterator itr = find(viewVector.begin(), viewVector.end(), startView);
	if (viewVector.end() == itr)
	{
		if (isForward)
			itr = viewVector.end() - 1;
		else
			itr = viewVector.begin();
	}

	DxViewVector::iterator curItr = itr;
	do 
	{
		if (isForward)
		{
			++curItr;
			if (curItr == viewVector.end())
				curItr = viewVector.begin();
		}
		else
		{
			if (curItr == viewVector.begin())
				curItr = viewVector.end();
			--curItr;
		}
		if ((*curItr)->CanFocus() && (*curItr)->IsTabStop())
			return (*curItr);
	} 
	while (itr != curItr);

	return NULL;
}

inline DxView* DxWindow::FindNextArrowView(DxView* startView, BOOL isForward)
{
	DxView* parentView;
	int pos;

	if (NULL == startView)
	{
		parentView = this;
		if (parentView->GetChildCount() == 0)
			return NULL;

		if (isForward)
			pos = parentView->GetChildCount() - 1;
		else
			pos = 0;
	}
	else
	{
		parentView = startView->GetParentView();
		if ((NULL == parentView) || OBJECT_DERIVEDFROM(parentView, DxScreen))
			return NULL;

		pos = parentView->IndexOf(startView);
	}

	int curPos = pos;
	DxView* view = NULL;
	do 
	{
		if (isForward)
		{
			++curPos;
			if (curPos >= parentView->GetChildCount())
				curPos = 0;
		}
		else
		{
			if (curPos == 0)
				curPos = parentView->GetChildCount();
			--curPos;
		}
		view = parentView->GetChildView(curPos);
		if (view->CanFocus() && (view->GetGroup() == startView->GetGroup()))
			return view;
	} while (curPos != pos);

	return NULL;
}

inline void DxWindow::Show(BOOL isActive)
{
	SetVisible(TRUE);
	if (isActive)
		Active();
}

inline void DxWindow::Close()
{
	if (HAS_FLAG(mWndState, WS_MODAL))
	{
		CloseModal(0);
	}
	else 
	{
		DxCloseMode action = (DxCloseMode)DoQuery(QID_CLOSE, NULL);
		if (cmHide == action)
			SetVisible(FALSE);
		else if (cmFree == action)
			mOwnerScreen->PostEvent(this, PID_RELEASE, 0, 0);
	}
}

inline void DxWindow::Hide()
{
	SetVisible(FALSE);
}

inline int DxWindow::ShowModal()
{
	if (IsVisible() || !IsEnable() || HAS_FLAG(mWndState, WS_MODAL))
		return 0;

	gIsDraging = FALSE;
	mOwnerScreen->SetCaptureView(NULL);
	DoNotify(NID_BEGINMODAL, 0);

	DxWindowList disableList;
	mOwnerScreen->BeginModal(this, disableList);

	ADD_FLAG(mWndState, WS_MODAL);
	Show();
	MsgLooper* msgLooper = mOwnerScreen->GetMsgLooper();
	while (IsModal() && !msgLooper->IsTerm())
		msgLooper->HandleMsg();

	mOwnerScreen->EndModal(this, disableList);
	Hide();
	DoNotify(NID_ENDMODAL, 0);
	return mModalResult;
}

inline BOOL DxWindow::CloseModal(int ret)
{
	if (!IsModal())
		return FALSE;

	DxCloseMode action = (DxCloseMode)DoQuery(QID_CLOSE, NULL);
	if ((cmHide == action) || (cmFree == action))
	{
		DEL_FLAG(mWndState, WS_MODAL);
		mModalResult = ret;
		if (cmFree == action)
			mOwnerScreen->PostEvent(this, PID_RELEASE, 0, 0);
	}

	return TRUE;
}

inline BOOL DxWindow::IsModal()
{ 
	return HAS_FLAG(mWndState, WS_MODAL); 
}

inline void DxWindow::ScreenCenter()
{
	int x = (mOwnerScreen->GetWidth() - mWidth) / 2;
	int y = (mOwnerScreen->GetHeight() - mHeight) / 2;
	SetPos(x, y);
}

inline BOOL DxWindow::IsDraging()
{
	return gIsDraging;
}

inline void DxWindow::BeginDrag(const POINT& pt)
{
	gIsDraging = TRUE;
	gDragPoint.x = mOwnerScreen->GetMouseX();
	gDragPoint.y = mOwnerScreen->GetMouseY();
	gOrgLeft = mLeft;
	gOrgTop = mTop;
	gOrgWidth = mWidth;
	gOrgHeight = mHeight;	
}

inline void DxWindow::DragTo(const POINT& pt)
{
	if (!gIsDraging)
		return;

	int dx = mOwnerScreen->GetMouseX() - gDragPoint.x;
	int dy = mOwnerScreen->GetMouseY() - gDragPoint.y;
	if (gDragHitTest == HT_MOVEREION)
	{
		SetPos(dx + gOrgLeft, dy + gOrgTop);
	}
	else if (gDragHitTest == HT_BORDERLEFT)
	{	
		if ((mMaxSize.cx != 0) && (gOrgWidth - dx > mMaxSize.cx))
		{
			SetSize(mMaxSize.cx, mHeight);
			SetPos(gOrgLeft - (mMaxSize.cx - gOrgWidth), mTop);
		}
		else if ((mMinSize.cx != 0) && (gOrgWidth - dx < mMinSize.cx))
		{
			SetSize(mMinSize.cx, mHeight);
			SetPos(gOrgLeft + (gOrgWidth - mMinSize.cx), mTop);
		}
		else
		{
			SetSize(gOrgWidth - dx, mHeight);
			SetPos(gOrgLeft + dx, mTop);
		}
	}
	else if (gDragHitTest == HT_BORDERTOP)
	{
		if ((mMaxSize.cy != 0) && (gOrgHeight - dy > mMaxSize.cy))
		{
			SetSize(mWidth, mMaxSize.cy);
			SetPos(mLeft, gOrgTop - (mMaxSize.cy - gOrgHeight));
		}
		else if ((mMinSize.cy != 0) && (gOrgHeight - dy < mMinSize.cy))
		{
			SetSize(mWidth, mMinSize.cy);
			SetPos(mLeft, gOrgTop + (gOrgHeight - mMinSize.cy));
		}
		else
		{
			SetSize(mWidth, gOrgHeight - dy);
			SetPos(mLeft, gOrgTop + dy);
		}
	}
	else if (gDragHitTest == HT_BORDERLEFTTOP)
	{
		int w, h, l, t;

		if ((mMaxSize.cx != 0) && (gOrgWidth - dx > mMaxSize.cx))
		{
			w = mMaxSize.cx;
			l = gOrgLeft - (mMaxSize.cx - gOrgWidth);
		}
		else if ((mMinSize.cx != 0) && (gOrgWidth - dx < mMinSize.cx))
		{
			w = mMinSize.cx;
			l = gOrgLeft + (gOrgWidth - mMinSize.cx);
		}
		else
		{
			w = gOrgWidth - dx;
			l = gOrgLeft + dx;
		}

		if ((mMaxSize.cy != 0) && (gOrgHeight - dy > mMaxSize.cy))
		{
			h = mMaxSize.cy;
			t = gOrgTop - (mMaxSize.cy - gOrgHeight);
		}
		else if ((mMinSize.cy != 0) && (gOrgHeight - dy < mMinSize.cy))
		{
			h = mMinSize.cy;
			t = gOrgTop + (gOrgHeight - mMinSize.cy);
		}
		else
		{
			h = gOrgHeight - dy;
			t = gOrgTop + dy;
		}

		SetSize(w, h);
		SetPos(l, t);
	}
	else if (gDragHitTest == HT_BORDERRIGHT)
	{
		if ((mMaxSize.cx != 0) && (gOrgWidth + dx > mMaxSize.cx))
			SetSize(mMaxSize.cx, mHeight);
		else if ((mMinSize.cx != 0) && (gOrgWidth + dx < mMinSize.cx))
			SetSize(mMinSize.cx, mHeight);
		else
			SetSize(gOrgWidth + dx, mHeight);
	}
	else if (gDragHitTest == HT_BORDERRIGHTTOP)
	{
		int w, h, t;
		if ((mMaxSize.cx != 0) && (gOrgWidth + dx > mMaxSize.cx))
			w = mMaxSize.cx;
		else if ((mMinSize.cx != 0) && (gOrgWidth + dx < mMinSize.cx))
			w = mMinSize.cx;
		else
			w = gOrgWidth + dx;

		if ((mMaxSize.cy != 0) && (gOrgHeight - dy > mMaxSize.cy))
		{
			h = mMaxSize.cy;
			t = gOrgTop - (mMaxSize.cy - gOrgHeight);
		}
		else if ((mMinSize.cy != 0) && (gOrgHeight - dy < mMinSize.cy))
		{
			h = mMinSize.cy;
			t = gOrgTop + (gOrgHeight - mMinSize.cy);
		}
		else
		{
			h = gOrgHeight - dy;
			t = gOrgTop + dy;
		}

		SetSize(w, h);
		SetPos(mLeft, t);
	}
	else if (gDragHitTest == HT_BORDERBOTTOM)
	{
		if ((mMaxSize.cy != 0) && (gOrgHeight + dy > mMaxSize.cy))
			SetSize(mWidth, mMaxSize.cy);
		else if ((mMinSize.cy != 0) && (gOrgHeight + dy < mMinSize.cy))
			SetSize(mWidth, mMinSize.cy);
		else
			SetSize(mWidth, gOrgHeight + dy);
	}
	else if (gDragHitTest == HT_BORDERLEFTBOTTOM)
	{
		int w, h, l;

		if ((mMaxSize.cx != 0) && (gOrgWidth - dx > mMaxSize.cx))
		{
			w = mMaxSize.cx;
			l = gOrgLeft - (mMaxSize.cx - gOrgWidth);
		}
		else if ((mMinSize.cx != 0) && (gOrgWidth - dx < mMinSize.cx))
		{
			w = mMinSize.cx;
			l = gOrgLeft + (gOrgWidth - mMinSize.cx);
		}
		else
		{
			w = gOrgWidth - dx;
			l = gOrgLeft + dx;
		}

		if ((mMaxSize.cy != 0) && (gOrgHeight + dy > mMaxSize.cy))
			h = mMaxSize.cy;
		else if ((mMinSize.cy != 0) && (gOrgHeight + dy < mMinSize.cy))
			h = mMinSize.cy;
		else
			h = gOrgHeight + dy;

		SetSize(w, h);
		SetPos(l, mTop);
	}
	else if (gDragHitTest == HT_BORDERRIGHTBOTTOM)
	{
		int w, h;
		if ((mMaxSize.cx != 0) && (gOrgWidth + dx > mMaxSize.cx))
			w = mMaxSize.cx;
		else if ((mMinSize.cx != 0) && (gOrgWidth + dx < mMinSize.cx))
			w = mMinSize.cx;
		else
			w = gOrgWidth + dx; 

		if ((mMaxSize.cy != 0) && (gOrgHeight + dy > mMaxSize.cy))
			h = mMaxSize.cy;
		else if ((mMinSize.cy != 0) && (gOrgHeight + dy < mMinSize.cy))
			h = mMinSize.cy;
		else
			h = gOrgHeight + dy;

		SetSize(w, h);
	}
}

inline void DxWindow::EndDrag(const POINT& pt)
{
	gIsDraging = FALSE;
}

inline void DxWindow::CancelDrag()
{
	if(gIsDraging)
	{
		gIsDraging = FALSE;
		SetPos(gOrgLeft, gOrgTop);
		SetSize(gOrgWidth, gOrgHeight);
	}
}

inline void DxWindow::Finalize()
{
	if (IsActive())
		mOwnerScreen->ActiveNextWindow(this);

	DxView::Finalize();
}

inline LRESULT DxWindow::DoMouse(DxMouseAction action, DxShiftState state, const POINT& pt)
{
	// 下面代码处理拖动
	if (action == maMouseMove)	
	{
		// 鼠标移动，窗口拖动进行中
		if (IsDraging())
			DragTo(pt);
	}
	else if (action == maLButtonDown)
	{
		// 鼠标点下，拖动开始
		gDragHitTest = HitTestView(pt);
		if ((gDragHitTest == HT_MOVEREION) || 
			((gDragHitTest >= HT_BORDERLEFT) && (gDragHitTest <= HT_BORDERRIGHTBOTTOM)))
			BeginDrag(pt);
	}
	else if (action = maLButtonUp)
	{
		// 鼠标弹起，拖动结束
		EndDrag(pt);
	}

	return DxView::DoMouse(action, state, pt);
}

inline void DxWindow::DoNotify(DxNotifyId id, DWORD param)
{
	if (id == NID_VISIBLECHANGED)
	{
		// 隐藏激活下一个窗口
		if (!param)
		{
			if (IsActive())
				mOwnerScreen->ActiveNextWindow(this);
		}
	}

	DxView::DoNotify(id, param);
}

inline LRESULT DxWindow::DoQuery(DxQueryId id, DWORD param)
{
	if (QID_SETCURSOR == id)
	{
		HCURSOR hc = NULL;
		if (param == HT_BORDERLEFT)
			hc = mOwnerScreen->GetCursor(CID_SIZEWE);
		else if (param == HT_BORDERRIGHT)
			hc = mOwnerScreen->GetCursor(CID_SIZEWE);
		else if (param == HT_BORDERTOP)
			hc = mOwnerScreen->GetCursor(CID_SIZENS);
		else if (param == HT_BORDERBOTTOM)
			hc = mOwnerScreen->GetCursor(CID_SIZENS);
		else if (param == HT_BORDERLEFTTOP)
			hc = mOwnerScreen->GetCursor(CID_SIZENWSE);
		else if (param == HT_BORDERRIGHTBOTTOM)
			hc = mOwnerScreen->GetCursor(CID_SIZENWSE);
		else if (param == HT_BORDERRIGHTTOP)
			hc = mOwnerScreen->GetCursor(CID_SIZENESW);
		else if (param == HT_BORDERLEFTBOTTOM)
			hc = mOwnerScreen->GetCursor(CID_SIZENESW);
		else if (param == HT_MOVEREION)
			hc = mOwnerScreen->GetCursor(CID_ARROW);

		if (hc)
		{
			::SetCursor(hc);
			return TRUE;
		}
	}

	return DxView::DoQuery(id, param);
}

inline DxHitTest DxWindow::HitTestView(const POINT& pt)
{
	if (gIsDraging)
		return gDragHitTest;

	RECT rc;
	if (IsSizable())
	{
		::SetRect(&rc, mWidth - mFrameSize - 4, mHeight - mFrameSize - 4, mWidth, mHeight);
		if (::PtInRect(&rc, pt))
			return HT_BORDERRIGHTBOTTOM;

		::SetRect(&rc, 0, mHeight - mFrameSize - 4, mFrameSize + 4, mHeight);
		if (::PtInRect(&rc, pt))
			return HT_BORDERLEFTBOTTOM;

		::SetRect(&rc, mWidth - mFrameSize - 4, 0, mWidth, mFrameSize + 4);
		if (::PtInRect(&rc, pt))
			return HT_BORDERRIGHTTOP;

		::SetRect(&rc, 0, 0, mFrameSize + 4, mFrameSize + 4);
		if (::PtInRect(&rc, pt))
			return HT_BORDERLEFTTOP;

		::SetRect(&rc, mWidth - mFrameSize, mFrameSize, mWidth, mHeight - mFrameSize);
		if (::PtInRect(&rc, pt))
			return HT_BORDERRIGHT;

		::SetRect(&rc, 0, mFrameSize, mFrameSize, mHeight - mFrameSize);
		if (::PtInRect(&rc, pt))
			return HT_BORDERLEFT;

		::SetRect(&rc, mFrameSize, mHeight - mFrameSize, mWidth - mFrameSize, mHeight);
		if (::PtInRect(&rc, pt))
			return HT_BORDERBOTTOM;

		::SetRect(&rc, mFrameSize, 0, mWidth - mFrameSize, mFrameSize);
		if (::PtInRect(&rc, pt))
			return HT_BORDERTOP;
	}

	if (IsMovable())
	{
		GetClientRect(rc);
		return ::PtInRect(&rc, pt) ? HT_MOVEREION : HT_NONE;
	}

	return DxView::HitTestView(pt);
}

inline BOOL DxWindow::PreKeyHandle(WORD& key, DxShiftState shift)
{
	// 窗口快捷键
	DxShortcut shortcut = MAKE_SHORTCUT(key, shift);
	if (HandleShortcut(shortcut))
		return TRUE;

	if (!HAS_FLAG(shift, ssAlt))
	{
		if (VK_ESCAPE == key)
		{
			// 取消拖动
			if (IsDraging())
			{
				CancelDrag();
				ReleaseCapture();		
				return TRUE;
			}
			else if (IsEscClose())
			{
				if (NULL != mFocusedView)
					if (mFocusedView->DoQuery(QID_WANTESCAPE, NULL))
						return FALSE;

				Close();
				return TRUE;
			}
		}
		if ((VK_TAB == key) && (!HAS_FLAG(shift, ssCtrl)))
		{
			if (mFocusedView && mFocusedView->DoQuery(QID_WANTTAB, NULL))
				return FALSE;

			DxView* pNextView = FindNexTabtView(mFocusedView, !HAS_FLAG(shift, ssShift));
			if (NULL != pNextView)
			{
				pNextView->SetFocus();
				return TRUE;
			}
		}
		else if ((VK_LEFT <= key) && (key <= VK_DOWN))
		{
			if (mFocusedView && mFocusedView->DoQuery(QID_WANTARROWS, NULL))
				return FALSE;

			DxView* pNextView = FindNextArrowView(mFocusedView, (VK_RIGHT == key) || (VK_DOWN == key));
			if (NULL != pNextView)
			{
				pNextView->SetFocus();
				return TRUE;
			}			
		}
	}

	return FALSE;
}

inline void DxWindow::HandlePostAction(DxPostId paId, DWORD param1, DWORD param2)
{
	if (PID_RELEASE == paId)
	{
		FREE_VIEW(this);
	}
}

inline void DxWindow::GenTabList(DxViewVector& viewVector, DxView* parentView)
{
	if (NULL == parentView)
		return;

	DxView* view;
	for (int i = 0; i < parentView->GetChildCount(); ++i)
	{
		view = parentView->GetChildView(i);
		viewVector.push_back(view);
		GenTabList(viewVector, view);
	}
}

//------------------------------------------------------------------------------
// DxScreen

#define LIN_DXSCREEN_ATOM  L"Kama.DxScreen.Atom"
#define LIN_DXSCREEN_POST L"Kama.DxScreen.PostEvent"
_declspec(selectany) UINT WM_POSTEVENT = ::RegisterWindowMessageW(LIN_DXSCREEN_POST);

IMPLEMENT_RUNTIMEINFO(DxScreen, DxView)

inline void DxScreen::SetHostWnd(HWND hwnd)
{
	if (hwnd == mHostWnd)
		return;

	if (IsWindow(mHostWnd))
	{
		UnsubclassWindow(mHostWnd);
		mDefHostWndProc = NULL;
		mHostWnd = NULL;
	}

	if (IsWindow(hwnd))
	{
		mHostWnd = hwnd;
		SubclassWindow(mHostWnd);
		RECT rc;
		::GetClientRect(mHostWnd, &rc);
		SetPos(0, 0);
		SetSize(rc.right, rc.bottom);
	}
}

inline HWND DxScreen::GetHostWnd() 
{ 
	return mHostWnd; 
}

inline void DxScreen::SetRender(DxRender* render)
{
	mRender = render;
}

inline DxRender* DxScreen::GetRender()
{
	return mRender;
}

inline void DxScreen::SetClip(BOOL canClip)
{
	mCanClip = canClip;
}

inline BOOL DxScreen::CanClip()
{
	return mCanClip;
}

inline DxWindow* DxScreen::GetChildWindow(int idx)
{
	DxView* view = GetChildView(idx);
	if (view && OBJECT_DERIVEDFROM(view, DxWindow))
		return (DxWindow*)view;

	return NULL;
}

inline DxWindow* DxScreen::GetActiveWindow() 
{ 
	return mActiveWindow; 
}

inline DxView* DxScreen::GetFocusedView()
{
	if (!mActiveWindow)
		return NULL;
	return mActiveWindow->GetFocusedView();
}

inline BOOL DxScreen::SetActiveWindow(DxWindow* wnd)
{
	if (wnd == mActiveWindow)
		return TRUE;

	if ((NULL != wnd) && (!wnd->CanActive()))
		return FALSE;

	DxWindow* oldWnd = mActiveWindow;
	mActiveWindow = wnd;
	if (NULL != oldWnd)
		oldWnd->DoNotify(NID_ACTIVECHANGED, FALSE);

	if (NULL != mActiveWindow)
	{
		mActiveWindow->BringToFront();
		mActiveWindow->DoNotify(NID_ACTIVECHANGED, TRUE);

		// 寻找第一个可以获得焦点的视图，有就设焦点给它，如果没有就设给自己
		if (!mActiveWindow->GetFocusedView() || (mActiveWindow->GetFocusedView() == mActiveWindow))
		{
			DxView* view = mActiveWindow->FindNexTabtView(NULL, TRUE);
			if (NULL != view)
				view->SetFocus();
			else
				mActiveWindow->SetFocus();
		}
	}

	return TRUE;
}

inline void DxScreen::ActiveNextWindow(DxWindow* wnd)
{
	int idx = IndexOf(wnd);
	DxWindow* nextWnd;
	while ((idx--) >= 0)
	{
		nextWnd = GetChildWindow(idx);
		if (nextWnd->Active())
			break;
	}
}

inline DxView* DxScreen::GetCaptureView() 
{ 
	return mCaptureView; 
}

inline BOOL DxScreen::SetCaptureView(DxView* view)
{
	if (view == mCaptureView)
		return TRUE;

	if ((NULL != view) && (!view->CanCapture()))
		return FALSE;

	DxView* oldView = mCaptureView;
	mCaptureView = view;
	if (NULL != oldView)
		oldView->DoNotify(NID_CAPTURECHANGED, FALSE);

	if (NULL != mCaptureView)
		mCaptureView->DoNotify(NID_CAPTURECHANGED, TRUE);
	return TRUE;
}

inline HCURSOR DxScreen::GetCursor(DxCursorId id)
{
	DxCursorMap::iterator itr = mCursorMap.find(id);
	if (itr == mCursorMap.end())
		return NULL;

	return itr->second;

}

inline BOOL DxScreen::AddCursor(DxCursorId id, HCURSOR cursor)
{
	if (id < CID_USER)
		return FALSE;

	DxCursorMap::iterator itr = mCursorMap.find(id);
	if (itr != mCursorMap.end())
		return FALSE;

	mCursorMap.insert(std::make_pair(id, cursor));
	return TRUE;

}

inline int DxScreen::GetMouseX() 
{ 
	return mMouseX; 
}

inline int DxScreen::GetMouseY() 
{ 
	return mMouseY; 
}

inline BOOL DxScreen::PostEvent(DxView* view, DxPostId id, DWORD param1, DWORD param2)
{
	if (!IsWindow(mHostWnd))
		return FALSE;

	if (NULL == view)
		return FALSE;

	if (0 == WM_POSTEVENT)
		return FALSE;

	DxPostInfo Info;
	Info.view = view;
	Info.id = id;
	Info.param1 = param1;
	Info.param2 = param2;
	mPostActionList.push_back(Info);

	PostMessageW(mHostWnd, WM_POSTEVENT, 0, 0);
	return TRUE;
}

inline void DxScreen::DelPostEvents(DxView* view)
{
	DxPostInfoList::iterator itr = mPostActionList.begin();
	while (itr != mPostActionList.end())
	{
		if (itr->view == view)
			itr = mPostActionList.erase(itr);
		else
			++itr;
	}
}

inline void DxScreen::BeginModal(DxWindow* wnd, DxWindowList& wndList)
{
	++mModalLevel;	

	DxWindow* childWnd;
	for (int i = 0; i < GetChildCount(); ++i)
	{
		childWnd = GetChildWindow(i);
		if ((wnd != childWnd) && childWnd->IsVisible() && childWnd->IsEnable())
		{
			wndList.push_back(childWnd);
			childWnd->SetEnable(FALSE);
		}
	}
}

inline void DxScreen::EndModal(DxWindow* wnd, DxWindowList& wndList)
{
	--mModalLevel;

	DxWindowList::iterator itr = wndList.begin();
	while (itr != wndList.end())
	{
		(*itr)->SetEnable(TRUE);
		++itr;
	}
	wndList.clear();

	if (0 == mModalLevel)
	{
		if (GetMsgLooper()->IsTerm())
			::PostQuitMessage(0);
	}
}

inline MsgLooper* DxScreen::GetMsgLooper()
{
	LN_ASSERT(mMsgLooper != NULL);
	return mMsgLooper;
}

inline void DxScreen::SetMsgLooper(MsgLooper* msgLooper)
{
	mMsgLooper = msgLooper;
}

inline DxView* DxScreen::GetHoverView() 
{ 
	return mHoverView; 
}

inline void DxScreen::ResetHoverView() 
{ 
	mHoverView = NULL; 
}

inline void DxScreen::Paint()
{
	if(IsVisible())
	{
		LN_ASSERT(mRender != NULL);

		DoPaint(mRender);

		RECT rcPaint;
		GetScreenRect(rcPaint);
		PaintChilds(this, rcPaint, rcPaint);
	}
}

inline void DxScreen::Update(DWORD tick)
{
	// 鼠标移进移出
	DxView* view = GetViewAtPos(MakePoint(mMouseX, mMouseY));
	if (view != mHoverView)
	{
		if(mHoverView)
			mHoverView->DoNotify(NID_MOUSELEAVE, NULL);
		mHoverView = view;
		if (mHoverView)
			mHoverView->DoNotify(NID_MOUSEENTER, NULL);
	}

	if (IsVisible())
	{
		DoUpdate(tick);
		UpdateChilds(this, tick);
	}
}

inline DxView* DxScreen::Initialize(DxView* parentView, DxScreen* screen)
{
	LN_ASSERT(NULL == parentView);
	LN_ASSERT(NULL == screen);

	InitCursorTable();

	DoInitialize();

	return this;
}

inline void DxScreen::Finalize()
{
	DxView::Finalize();

	DestroyAllCursor();

	SetHostWnd(NULL);
	mRender = NULL;
}

inline UINT DxScreen::AllocTimer(DxView* view)
{
	DxTimerVector::iterator itr;
	for (itr = mTimerVector.begin(); itr != mTimerVector.end(); ++itr)
	{
		if (NULL == *itr)
		{
			*itr = view;
			return (itr - mTimerVector.begin() + 1);
		}
	}
	mTimerVector.push_back(view);
	return (int)mTimerVector.size();
}

inline BOOL DxScreen::RestoreTimer(DxView* view, UINT id)
{
	if ((id > mTimerVector.size()) || (id <= 0))
		return FALSE;

	DxView* timeView = mTimerVector[id-1];
	if (!timeView || (timeView != view))
		return FALSE;

	mTimerVector[id-1] = NULL;
	return TRUE;
}

inline void DxScreen::ClearTimer(DxView* view)
{	
	if (!mHostWnd)
		return;

	DxView* timeView;
	for (UINT i = 0; i < mTimerVector.size(); ++i)
	{
		timeView = mTimerVector[i];
		if (timeView == view)
		{
			::KillTimer(mHostWnd, i+1);
			mTimerVector[i] = NULL;
		}
	}
}

inline UINT DxScreen::SetTimer(DxView* view, UINT elapse)
{
	if (!mHostWnd)
		return 0;

	UINT id = AllocTimer(view);
	if (::SetTimer(mHostWnd, id, elapse, NULL))
		return id;
	else
		return 0;
}

inline BOOL DxScreen::KillTimer(DxView* view, UINT id)
{
	if (!mHostWnd || !RestoreTimer(view, id))
		return FALSE;
	else
		return ::KillTimer(mHostWnd, id);
}

inline BOOL DxScreen::InsertChild(DxView* childView, int pos , BOOL isCheck)
{
	// 屏幕的子视图一定是窗口
	LN_ASSERT(NULL != childView);
	LN_ASSERT(OBJECT_DERIVEDFROM(childView, DxWindow));

	return DxView::InsertChild(childView, pos, isCheck);
}

inline DxView* DxScreen::GetViewAtPos(const POINT& pt, BOOL allowDisabled)
{
	if (HitTestView(pt) && mIsVisible && (mIsEnable || allowDisabled))
	{
		POINT tmpPt;
		DxWindow* wnd;
		DxView* view;

		// 先遍历顶层窗口
		DxViewVector::reverse_iterator revItr = mChildViews.rbegin();
		while (revItr != mChildViews.rend())
		{	wnd = OBJECT_DERIVEDFROM((*revItr), DxWindow) ? (DxWindow*)(*revItr) : NULL;
			if (wnd && wnd->IsTopMost())
			{
				tmpPt = pt;
				tmpPt.x -= wnd->GetLeft();
				tmpPt.y -= wnd->GetTop();
				view = wnd->GetViewAtPos(tmpPt, allowDisabled);
				if (NULL != view)
					return view;
			}
			++revItr;
		}

		// 再遍历非顶层窗口
		revItr = mChildViews.rbegin();
		while (revItr != mChildViews.rend())
		{
			wnd = OBJECT_DERIVEDFROM((*revItr), DxWindow) ? (DxWindow*)(*revItr) : NULL;
			if (wnd && (!wnd->IsTopMost()))
			{
				tmpPt = pt;
				tmpPt.x -= wnd->GetLeft();
				tmpPt.y -= wnd->GetTop();
				view = wnd->GetViewAtPos(tmpPt, allowDisabled);
				if (NULL != view)
					return view;
			}
			++revItr;
		}

		if (mModalLevel > 0)
			return NULL;
		else
			return this;
	}

	return NULL;
}

inline BOOL DxScreen::PreKeyHandle(WORD& key, DxShiftState shift)
{
	// 全局快捷键
	DxShortcut shortcut = MAKE_SHORTCUT(key, shift);

	if (HandleShortcut(shortcut))
		return TRUE;

	// 窗口快捷键
	if (NULL != mActiveWindow)
		return mActiveWindow->PreKeyHandle(key, shift);

	return FALSE;
}

inline void DxScreen::InitCursorTable()
{
	mCursorMap.insert(std::make_pair(CID_ARROW,		::LoadCursor(NULL, IDC_ARROW)));
	mCursorMap.insert(std::make_pair(CID_IBEAM,		::LoadCursor(NULL, IDC_IBEAM)));
	mCursorMap.insert(std::make_pair(CID_WAIT,		::LoadCursor(NULL, IDC_WAIT)));
	mCursorMap.insert(std::make_pair(CID_CROSS,		::LoadCursor(NULL, IDC_CROSS)));
	mCursorMap.insert(std::make_pair(CID_UPARROW,		::LoadCursor(NULL, IDC_UPARROW)));
	mCursorMap.insert(std::make_pair(CID_SIZENWSE,	::LoadCursor(NULL, IDC_SIZENWSE)));
	mCursorMap.insert(std::make_pair(CID_SIZENESW,	::LoadCursor(NULL, IDC_SIZENESW)));
	mCursorMap.insert(std::make_pair(CID_SIZEWE,		::LoadCursor(NULL, IDC_SIZEWE)));
	mCursorMap.insert(std::make_pair(CID_SIZENS,		::LoadCursor(NULL, IDC_SIZENS)));
	mCursorMap.insert(std::make_pair(CID_SIZEALL,		::LoadCursor(NULL, IDC_SIZEALL)));
	mCursorMap.insert(std::make_pair(CID_NO,			::LoadCursor(NULL, IDC_NO)));
	mCursorMap.insert(std::make_pair(CID_APPSTARTING,	::LoadCursor(NULL, IDC_APPSTARTING)));
	mCursorMap.insert(std::make_pair(CID_HELP,		::LoadCursor(NULL, IDC_HELP)));
	mCursorMap.insert(std::make_pair(CID_HAND,		::LoadCursor(NULL, IDC_HAND)));
}

inline void DxScreen::DestroyAllCursor()
{
	DxCursorMap::iterator itr;

	for(itr = mCursorMap.begin(); itr != mCursorMap.end(); ++itr)
	{
		if (itr->first >= CID_USER)
			::DestroyCursor(itr->second);
	}
	mCursorMap.clear();
}

inline void DxScreen::PaintChilds(DxView* parentView, const RECT& rcParentPaint, const RECT& rcParentScreen)
{
	RECT rcChildScreen, rcChildPaint;
	int orgOffsetX, orgOffsetY;
	orgOffsetX = mRender->GetPaintOffsetX();
	orgOffsetY = mRender->GetPaintOffsetY();

	if (OBJECT_DERIVEDFROM(parentView, DxScreen))
	{
		DxWindow* wnd;
		DxView* view;
		// 先绘制非顶层窗口
		for (int i = 0; i < parentView->GetChildCount(); ++i)
		{
			view = parentView->GetChildView(i);
			wnd = OBJECT_DERIVEDFROM(view, DxWindow) ? (DxWindow*)view : NULL;
			if (wnd->IsVisible() && (!wnd->IsTopMost()))
			{
				rcChildScreen.left = rcParentScreen.left + wnd->GetLeft();
				rcChildScreen.top = rcParentScreen.top + wnd->GetTop();
				rcChildScreen.right = rcChildScreen.left + wnd->GetWidth();
				rcChildScreen.bottom = rcChildScreen.top + wnd->GetHeight();
				::IntersectRect(&rcChildPaint, &rcParentPaint, &rcChildScreen);
				if (!::IsRectEmpty(&rcChildPaint))
				{
					BOOL isClip = FALSE;
					if (mCanClip && mRender->BeginClip(rcChildPaint))
						isClip = TRUE;

					mRender->SetPaintOffsetX(rcChildScreen.left);
					mRender->SetPaintOffsetY(rcChildScreen.top);
					wnd->DoPaint(mRender);

					if (isClip) 
						mRender->EndClip();

					PaintChilds(wnd, rcChildPaint, rcChildScreen);
				}
			}
		}

		// 再绘制顶层窗
		for (int i = 0; i < parentView->GetChildCount(); ++i)
		{
			view = parentView->GetChildView(i);
			wnd = OBJECT_DERIVEDFROM(view, DxWindow) ? (DxWindow*)view : NULL;
			if (wnd->IsVisible() && (wnd->IsTopMost()))
			{
				rcChildScreen.left = rcParentScreen.left + wnd->GetLeft();
				rcChildScreen.top = rcParentScreen.top + wnd->GetTop();
				rcChildScreen.right = rcChildScreen.left + wnd->GetWidth();
				rcChildScreen.bottom = rcChildScreen.top + wnd->GetHeight();
				::IntersectRect(&rcChildPaint, &rcParentPaint, &rcChildScreen);
				if (!::IsRectEmpty(&rcChildPaint))
				{
					BOOL isClip = FALSE;
					if (mCanClip && mRender->BeginClip(rcChildPaint))
						isClip = TRUE;

					mRender->SetPaintOffsetX(rcChildScreen.left);
					mRender->SetPaintOffsetY(rcChildScreen.top);
					wnd->DoPaint(mRender);

					if (isClip) 
						mRender->EndClip();

					PaintChilds(wnd, rcChildPaint, rcChildScreen);
				}
			}
		}
	}
	else
	{
		DxView* view;
		for (int i = 0; i < parentView->GetChildCount(); ++i)
		{
			view = parentView->GetChildView(i);
			if (view->IsVisible())
			{
				rcChildScreen.left = rcParentScreen.left + view->GetLeft();
				rcChildScreen.top = rcParentScreen.top + view->GetTop();
				rcChildScreen.right = rcChildScreen.left + view->GetWidth();
				rcChildScreen.bottom = rcChildScreen.top + view->GetHeight();
				::IntersectRect(&rcChildPaint, &rcParentPaint, &rcChildScreen);
				if (!::IsRectEmpty(&rcChildPaint))
				{
					BOOL isClip = FALSE;
					if (mCanClip && mRender->BeginClip(rcChildPaint))
						isClip = TRUE;

					mRender->SetPaintOffsetX(rcChildScreen.left);
					mRender->SetPaintOffsetY(rcChildScreen.top);
					view->DoPaint(mRender);

					if (isClip)
						mRender->EndClip();
					
					PaintChilds(view, rcChildPaint, rcChildScreen);
				}
			}
		}
	}

	mRender->SetPaintOffsetX(orgOffsetX);
	mRender->SetPaintOffsetY(orgOffsetY);
}

inline void DxScreen::UpdateChilds(DxView* parentView, DWORD tick)
{
	DxView* childView;
	for (int i = 0; i < parentView->GetChildCount(); ++i)
	{
		childView = parentView->GetChildView(i);
		if (childView->IsVisible())
		{
			childView->DoUpdate(tick);
			UpdateChilds(childView, tick);
		}
	}
}

inline void DxScreen::SubclassWindow(HWND hwnd)
{
	// 一个窗口只能与一个Screen关联
	HANDLE handle = GetPropW(hwnd, LIN_DXSCREEN_ATOM);
	LN_ASSERT(handle == NULL);

	mDefHostWndProc = (WNDPROC)::SetWindowLongW(hwnd, GWL_WNDPROC, (LONG)&StdWndProc);
	LN_ASSERT(NULL != mDefHostWndProc);

	::SetPropW(hwnd, LIN_DXSCREEN_ATOM, (HANDLE)this);
}

inline void DxScreen::UnsubclassWindow(HWND hwnd)
{
	LN_ASSERT(NULL != mDefHostWndProc);

	::RemovePropW(hwnd, LIN_DXSCREEN_ATOM);
	::SetWindowLongW(hwnd, GWL_WNDPROC, (LONG)mDefHostWndProc);
}

inline LRESULT CALLBACK	DxScreen::StdWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT lRet = 0;
	BOOL bDone = FALSE;
	DxScreen* screen = NULL;

	screen = (DxScreen*)::GetPropW(hwnd, LIN_DXSCREEN_ATOM);
	if (NULL != screen)
		bDone = screen->WndProc(hwnd, msg, wparam, lparam, lRet);

	if (bDone)
		return lRet;
	else
		return CallWindowProcW(screen->mDefHostWndProc, hwnd, msg, wparam, lparam);
}

inline BOOL DxScreen::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& lRet)
{
	if ((WM_KEYDOWN == msg) || (WM_SYSKEYDOWN == msg))
	{
		WMKeyDown(wparam, lparam);
	}
	else if ((WM_KEYUP == msg) || (WM_SYSKEYUP == msg))
	{
		WMKeyMsg(kaKeyUp, wparam, lparam);
	}
	else if (WM_CHAR == msg)
	{
		WMKeyMsg(kaChar, wparam, lparam);
	}
	else if (WM_MOUSEMOVE == msg)
	{
		WMMouseMove(wparam, lparam);
	}
	else if (WM_LBUTTONDBLCLK == msg)
	{
		WMLButtonDblClk(wparam, lparam);
	}
	else if (WM_LBUTTONDOWN == msg)
	{
		WMLButtonDown(wparam, lparam);
	}
	else if (WM_LBUTTONUP == msg)
	{
		WMLButtonUp(wparam, lparam);
	}
	else if (WM_CANCELMODE == msg)
	{
		WMCancelMode(wparam, lparam);
	}
	else if (WM_LBUTTONDBLCLK == msg)
	{
		WMOtherMouse(maLButtonDblClk, wparam, lparam);
	}
	else if (WM_RBUTTONDBLCLK == msg)
	{
		WMOtherMouse(maRButtonDblClk, wparam, lparam);
	}
	else if (WM_MBUTTONDBLCLK == msg)
	{
		WMOtherMouse(maMButtonDblClk, wparam, lparam);
	}
	else if (WM_RBUTTONDOWN == msg)
	{
		WMOtherMouse(maRButtonDown, wparam, lparam);
	}
	else if (WM_RBUTTONUP == msg)
	{
		WMOtherMouse(maRButtonUp, wparam, lparam);
	}
	else if (WM_MBUTTONDOWN == msg)
	{
		WMOtherMouse(maMButtonDown, wparam, lparam);
	}
	else if (WM_MBUTTONUP == msg)
	{
		WMOtherMouse(maMButtonUp, wparam, lparam);
	}
	else if (WM_MOUSEWHEEL == msg)
	{
		WMMouseWheel(wparam, lparam);
		return TRUE;
	}
	else if (WM_SIZE == msg)
	{
		WMSize(wparam, lparam);
	}
	else if (WM_CONTEXTMENU == msg)
	{
		WMContextMenu(wparam, lparam);
	}
	else if (WM_POSTEVENT == msg)
	{
		WMPostAction(wparam, lparam);
	}
	else if (WM_SETCURSOR == msg)
	{
		if (WMSetCursor(wparam, lparam))
		{
			lRet = 1;
			return TRUE;
		}
	}
	else if (WM_ERASEBKGND == msg)
	{
		lRet = 1;
		return TRUE;
	}
	else if (WM_DESTROY == msg)
	{
		SetHostWnd(NULL);
	}
	else if (WM_TIMER == msg)
	{
		if (WMTimer(wparam, lparam))
			return TRUE;
	}

	return FALSE;
}

inline LRESULT DxScreen::DefWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{ 
	return ::CallWindowProc(mDefHostWndProc, hwnd, msg, wparam, lparam); 
}

inline void DxScreen::WMMouseMove(WPARAM wparam, LPARAM lparam)
{
	mMouseX = X_OF_LPARAM(lparam);
	mMouseY = Y_OF_LPARAM(lparam);

	DxShiftState shift = VKeyToShiftState((WORD)wparam);
	POINT pt;

	// 捕获鼠标的视图优先，注意：CaptureView一定是可见的，可用的
	if (NULL != mCaptureView)
	{
		pt = mCaptureView->ScreenToClient(SmallPtToPoint(MAKEPOINTS(lparam)));
		mCaptureView->DoMouse(maMouseMove, shift, pt);
	}
	else
	{
		pt = SmallPtToPoint(MAKEPOINTS(lparam));
		DxView* view = GetViewAtPos(pt, FALSE);
		if (NULL != view)
		{
			pt = view->ScreenToClient(pt);
			view->DoMouse(maMouseMove, shift, pt);
		}
	}								   
}

inline void DxScreen::WMSize(WPARAM wparam, LPARAM lparam)
{
	SetPos(0, 0);
	SetSize(X_OF_LPARAM(lparam), Y_OF_LPARAM(lparam));
}

inline void DxScreen::WMLButtonDblClk(WPARAM wparam, LPARAM lparam)
{
	DxShiftState shift = VKeyToShiftState((WORD)wparam);
	POINT pt;

	pt = SmallPtToPoint(MAKEPOINTS(lparam));
	DxView* view = GetViewAtPos(pt, FALSE);

	if (NULL != view)	
	{
		// 先激活父窗口
		SetActiveWindow(view->GetOwnerWindow());

		// 再捕获鼠标以及传送鼠标事件
		SetCaptureView(view);
		if (NULL != mCaptureView)
		{	
			pt = mCaptureView->ScreenToClient(pt);
			mCaptureView->DoMouse(maLButtonDblClk, shift, pt);
			mCaptureView->DoMouse(maLButtonDown, shift, pt);
			// 焦点
			if (!OBJECT_DERIVEDFROM(mCaptureView, DxWindow))
				mCaptureView->SetFocus();
		}
	}
}

inline void DxScreen::WMLButtonDown(WPARAM wparam, LPARAM lparam)
{
	DxShiftState shift = VKeyToShiftState((WORD)wparam);
	POINT pt;

	pt = SmallPtToPoint(MAKEPOINTS(lparam));
	DxView* view = GetViewAtPos(pt, FALSE);

	if (NULL != view)	
	{
		// 先激活父窗口
		SetActiveWindow(view->GetOwnerWindow());

		// 再捕获鼠标以及传送鼠标事件
		SetCaptureView(view);
		if (NULL != mCaptureView)
		{	
			pt = mCaptureView->ScreenToClient(pt);
			mCaptureView->DoMouse(maLButtonDown, shift, pt);
			// 焦点
			if (!OBJECT_DERIVEDFROM(mCaptureView, DxWindow))
				mCaptureView->SetFocus();
		}
	}
}

inline void DxScreen::WMLButtonUp(WPARAM wparam, LPARAM lparam)
{
	DxShiftState shift = VKeyToShiftState((WORD)wparam);
	POINT pt;
	pt = SmallPtToPoint(MAKEPOINTS(lparam));

	DxView* view = mCaptureView;
	SetCaptureView(NULL);

	if (NULL != view)
	{
		pt = view->ScreenToClient(pt);

		// 点击
		RECT rc;
		view->GetClientRect(rc);
		if (::PtInRect(&rc, pt))
			view->DoMouse(maMouseClick, shift, pt);

		// 弹起
		view->DoMouse(maLButtonUp, shift, pt);
	}
	else
	{
		view = GetViewAtPos(pt, FALSE);
		if (NULL != view)
		{
			pt = view->ScreenToClient(pt);
			view->DoMouse(maLButtonUp, shift, pt);
		}
	}
}

inline void DxScreen::WMCancelMode(WPARAM wparam, LPARAM lparam)
{
	SetCaptureView(NULL);
	if (mActiveWindow)
		mActiveWindow->CancelDrag();
}

inline void DxScreen::WMOtherMouse(DxMouseAction action, WPARAM wparam, LPARAM lparam)
{
	DxShiftState shift = VKeyToShiftState((WORD)wparam);
	POINT pt;

	pt = SmallPtToPoint(MAKEPOINTS(lparam));
	DxView* view = GetViewAtPos(pt, FALSE);
	if (NULL != view)
	{	
		pt = view->ScreenToClient(pt);
		view->DoMouse(action, shift, pt);
	}
}

inline void DxScreen::WMKeyDown(WPARAM wparam, LPARAM lparam)
{
	DxShiftState shift = KeyDataToShiftState((WORD)lparam);
	WORD key = LOWORD(wparam);

	// 先处理快捷键，Tab，方向键等
	if (!PreKeyHandle(key, shift))
	{
		if (NULL != mActiveWindow)
		{
			DxView* view = mActiveWindow->GetFocusedView();
			if (NULL == view) 
				view = mActiveWindow;
			view->DoKeyboard(kaKeyDown, key, shift);
		}
		else
		{
			DoKeyboard(kaKeyDown, key, shift);
		}
	}
}

inline void DxScreen::WMKeyMsg(DxKeyAction action, WPARAM wparam, LPARAM lparam)
{
	DxShiftState shift = KeyDataToShiftState((WORD)lparam);
	WORD key = LOWORD(wparam);

	if (NULL != mActiveWindow)
	{
		DxView* view = mActiveWindow->GetFocusedView();
		if (NULL == view) 
			view = mActiveWindow;
		view->DoKeyboard(action, key, shift);
	}
	else
	{
		DoKeyboard(action, key, shift);
	}	
}

inline void DxScreen::WMMouseWheel(WPARAM wparam, LPARAM lparam)
{
	DxView* view = GetFocusedView();
	if (view)
	{
		short delta = (short)HIWORD(wparam);
		DxShiftState shift = VKeyToShiftState(LOWORD(wparam));
		POINT pt = SmallPtToPoint(MAKEPOINTS(lparam));
		::ScreenToClient(mHostWnd, &pt);
		pt = view->ScreenToClient(pt);
		view->HandleMouseWheel(shift, delta, pt);
	}
}

inline void DxScreen::WMContextMenu(WPARAM wparam, LPARAM lparam)
{
	POINTS pts = MAKEPOINTS(lparam);	
	if ((pts.x == -1) && (pts.y == -1))
	{
		// 是由键盘引起的环境菜单事件
		if (mActiveWindow)
		{
			DxView* view = mActiveWindow->GetFocusedView();
			if (NULL == view)
				view = mActiveWindow;
			view->DoNotify(NID_CONTEXATMENU, NULL);
		}
	}
	else
	{
		// 是由鼠标引起的环境菜单事件
		POINT pt = SmallPtToPoint(pts);
		::ScreenToClient(mHostWnd, &pt);
		DxView* view = GetViewAtPos(pt);
		if (NULL != view)
			view->DoNotify(NID_CONTEXATMENU, (DWORD)&pt);
	}
}

inline void DxScreen::WMPostAction(WPARAM wparam, LPARAM lparam)
{
	DxPostInfoList::iterator itr = mPostActionList.begin();
	if (itr != mPostActionList.end())
	{
		DxPostInfo Info = *itr;
		mPostActionList.pop_front();
		if (Info.view)
			Info.view->HandlePostEvent(Info.id, Info.param1, Info.param2);
	}
}

inline BOOL DxScreen::WMSetCursor(WPARAM wparam, LPARAM lparam)
{
	if (LOWORD(lparam) == HT_CLIENT)
	{
		POINT pt = MakePoint(mMouseX, mMouseY);
		DxView* view = GetViewAtPos(pt);
		if (view)
		{
			pt = view->ScreenToClient(pt);
			DxHitTest htValue = view->HitTestView(pt);
			if (view->DoQuery(QID_SETCURSOR, htValue))
				return TRUE;			
		}
	}
	return FALSE;
}

inline BOOL DxScreen::WMTimer(WPARAM wparam, LPARAM lparam)
{
	UINT id = wparam;
	if ((id <= 0) || (id > (int)mTimerVector.size()))
		return FALSE;

	DxView* view = mTimerVector[id-1];
	if (!view)
		return FALSE;

	view->DoNotify(NID_TIMER, id);
	return TRUE;
}

}

#endif // __LIN_DXVIEWS_H__