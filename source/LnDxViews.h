/*******************************************************************************
  Filename:		LnDxViews.h
  Author:		lingo
  Email:		lingoooooooooo@gmail.com
  Date:			2009/12/19

  Brief:    	����lincode������һ���֣���lingo������ά��!
 -------------------------------------------------------------------------------
  Description:
	Dx��ͼ���
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
	��궯������
*/
enum DxMouseAction
{
	maMouseMove,						// �ƶ�
	maLButtonDown,						// �������
	maRButtonDown,						// �Ҽ�����
	maMButtonDown,						// �м�����
	maLButtonUp,						// �������
	maRButtonUp,						// �Ҽ�����
	maMButtonUp,						// �м�����
	maLButtonDblClk,					// ���˫��
	maRButtonDblClk,					// �Ҽ�˫��
	maMButtonDblClk,					// �м�˫��
	maMouseClick,						// ���
	maMouseWheelDown,					// �������¹���, ��������˸���Ϣ, DoMouseӦ�÷���TRUE
	maMouseWheelUp,						// �������Ϲ���, ��������˸���Ϣ, DoMouseӦ�÷���TRUE
};					

/*
	���̶�������
*/
enum DxKeyAction
{
	kaKeyDown,							// ���̻���
	kaChar,								// �����ַ�
	kaKeyUp								// ���̵���
};

/*
	Shift״̬
*/
typedef DWORD DxShiftState;
#define ssShift					0x01	// Shift������
#define ssAlt					0x02	// Alt������
#define ssCtrl					0x04	// Ctrl������
#define ssLeft					0x08	// ����������	
#define ssRight					0x08	// ����Ҽ�����
#define ssMiddle				0x10	// ����м�����


// �û�ID��ʼֵ, �Զ���ؼ�����ʹ�����ID���ϵ�ֵ
#define ID_USER					0x10000
// ����ID��ʼֵ, �ⲿ����Ӧ��ʹ�����ID���ϵ�ֵ
#define ID_APP					0x20000

/*
	֪ͨ�¼�ID, 0~NTD_USER-1�ɿ�ܱ������û�����ʹ��������ֵ
*/
typedef DWORD DxNotifyId;
#define NID_USER					ID_USER
// �ߴ����ڸı䣬param: SIZE*=��Ҫ�ı�ĳߴ磬���Ըı����ֵ��Ӱ�����յĳߴ�
#define NID_SIZECHANGING			1
// �ߴ�ı䣬param: NULL
#define NID_SIZECHANGED				2
// λ�����ڸı䣬param: POINT*=��Ҫ�ı�ĳߴ磬���Ըı����ֵ��Ӱ�����յ�λ��
#define NID_POSCHANGING				3
// λ�øı䣬param: NULL
#define NID_POSCHANGED				4
// ����ı�	 param:  0=������, !0=����
#define NID_ACTIVECHANGED			5
// ����ı�, param:��0=ʧȥ����, !0=��ý���
#define NID_FOCUSCHANGED			6
// ��겶����ͼ�ı䣬param:��0=ʧȥ����, !0=��ò���
#define NID_CAPTURECHANGED			7
// �ɼ��ı䣬param:��0=����, !0=�ɼ�
#define NID_VISIBLECHANGED			8
// ���øı䣬param:��0=����, !0=����
#define NID_ENABLECHANGED			9
// ����(�Ҽ�)�˵��¼���param��NULL=�ɼ��������, PPOINT=���������ģ�PPOINTָ�������screen��λ��
#define NID_CONTEXATMENU			10
// ��ʼShowModal, param: NULL
#define NID_BEGINMODAL				11
// ����ShowModal, param: NULL
#define NID_ENDMODAL				12
// ��������ͼ, param: NULL
#define NID_MOUSEENTER				13
// ����˳���ͼ, param: NULL
#define NID_MOUSELEAVE				14
// �ı��ı䣬 param: NULL
#define NID_TEXTCHANGED				15
// ����ı䣬 param: NULL
#define NID_FONTCHANGED				16
// ��ʱ���¼���param: ��ʱ��ID
#define NID_TIMER					17

/*
	�����¼�ID, 0~QID_USER-1�ɿ�ܱ������û�����ʹ��������ֵ
*/
typedef DWORD DxQueryId;
#define QID_USER					ID_USER
// �Ƿ�Ҫ����TAB������ֵ��0=������!0=����
#define QID_WANTTAB					1
// �Ƿ�Ҫ�������������ֵ��0=������!0=���� 
#define QID_WANTARROWS				2
// �Ƿ�Ҫ����ESC��������ֵ��0=������!0=���� 
#define QID_WANTESCAPE				3
// ���ڹر����󣬷���ֵ��KDxCloseMode
#define QID_CLOSE					4
// ���ù�꣬������ param=�������ֵ������ֵ��!0=���óɹ�
#define QID_SETCURSOR				5


/*
	��ͼ�������ֵ0~HT_USER-1�ɿ�ܱ������û�����ʹ������ֵ
*/
typedef DWORD DxHitTest;
#define HT_USER						ID_USER
#define HT_NONE						0		// ʲôҲû�У�͸��
#define HT_CLIENT					1		// ������ͻ���
#define HT_BORDERLEFT				2		// ��Ե��
#define HT_BORDERTOP				3		// ��Ե��
#define HT_BORDERRIGHT				4		// ��Ե��
#define HT_BORDERBOTTOM				5		// ��Ե��
#define HT_BORDERLEFTTOP			6		// ��Ե��
#define HT_BORDERLEFTBOTTOM			7		// ��Ե���
#define HT_BORDERRIGHTTOP			8		// ��Ե�Ҷ�
#define HT_BORDERRIGHTBOTTOM		9		// ��Ե�ҵ�
#define HT_MOVEREION				10		// ���ƶ�����

/*
	�������, 0~CID_USER-1�ɿ�ܱ������û�����ʹ������ֵ
*/
typedef DWORD DxCursorId;
#define CID_USER					ID_USER
#define CID_ARROW					1		// Ĭ�Ϲ��
#define CID_IBEAM					2		// ����, �Ƶ��༭��ʱ���Ǹ���״
#define CID_WAIT					3		// ɳ©, �ȴ����
#define CID_CROSS					4		// ʮ�ֹ��
#define CID_UPARROW					5		// ���ϼ�ͷ
#define CID_SIZENWSE				6		// ����--���� �����ͷ  "\"
#define CID_SIZENESW				7		// ����--���� �����ͷ	"/"
#define CID_SIZEWE					8		// ���� �����ͷ "-"
#define CID_SIZENS					9		// �ϱ� �����ͷ "|"
#define CID_SIZEALL					10		// �ƶ�, ʮ�ּ�ͷ
#define CID_NO						11		// ԲȦ��б��, ��ʾ�ܾ�
#define CID_APPSTARTING				12		// ����ɳ©, ��ʾ������������
#define CID_HELP					13		// �����ʺ�, ��ʾ����
#define CID_HAND					14		// ��ָ���

/*
	��ͼ���
*/
typedef DWORD DxViewStyle;
#define VS_TABSTOP					0x01	// Tabֹͣ���뽹���ϵ����
#define VS_FOCUSABLE				0x02	// �Ƿ�ɻ�ý���

/*
	���ڷ��
*/
typedef DWORD DxWndStyle;
#define WS_MOVABLE					0x01	// ���ƶ�
#define WS_SIZABLE					0x02	// ���϶�
#define WS_TOPMOST					0x04	// ���㴰��
#define WS_ESCCLOSE					0x08	// ESC�ر�
#define WS_ACTIVATABLE				0x10	// �ɼ���

/*
	����״̬
*/
typedef DWORD DxWndState;
#define WS_MODAL					0x01	// ģ̬״̬

/*
	���ڹر�ģʽ
*/
enum DxCloseMode
{
	cmHide,									// ����
	cmFree,									// �ͷ�
	cmNone									// ʲô��Ҳ����
};

/*
	��ݼ�ֵ								  
*/
typedef DWORD DxShortcut;
// ���ɿ�ݼ�����
#define MAKE_SHORTCUT(key, shift) (DxShortcut)MAKELONG(key, shift)
// �ӿ�ݼ�ȡ�����
#define VKEY_OF_SHORTCUT(shortcut) LOWORD(shortcut)
// �ӿ�ݼ�ȡ��Shift״̬
#define SHIFT_OF_SHORTCUT(shortcut) (DxShiftState)HIWORD(shortcut)


/*
	Ͷ���¼�ID, 0~PID_USER-1�ɿ�ܱ������û�����ʹ������ֵ
*/
typedef DWORD DxPostId;
#define PID_USER					ID_USER
#define PID_RELEASE					1		// �ͷ��Լ����޲���

class DxView;
class DxWindow;
class DxScreen;

/*
	��ͼ�¼��ӿ�
*/
struct DxViewListener
{
	/*
		��ͼ��ʼ�����
	*/
	virtual void OnInitialize()
	{

	}

	/*
		��ͼ��Ҫ����
	*/
	virtual void OnFinalize()
	{

	}

	/*
		����¼������£������ƶ�
	*/
	virtual LRESULT OnMouse(DxView* view, DxMouseAction action, DxShiftState shift, const POINT& pt) 
	{
		return 0;
	}

	/*
		�����¼������£������ַ�
	*/
	virtual LRESULT OnKeyboard(DxView* view, DxKeyAction action, WORD& key, DxShiftState shift) 
	{
		return 0;
	}

	/*
		ͨ�õ�֪ͨ�¼���KDxNotifyIdָ��֪ͨ������, paramΪ�����Ĳ���
	*/
	virtual void OnNotify(DxView* view, DxNotifyId id, DWORD param) 
	{

	}

	/*
		ͨ�õ������¼���KDxQueryIdָ�����������, paramΪ�����Ĳ���
	*/
	virtual LRESULT OnQuery(DxView* view, DxQueryId id, DWORD param) 
	{ 
		return 0; 
	}

	/*
		�����¼�
	*/
	virtual void OnUpdate(DxView* view, DWORD tick) 
	{

	}

	/*
		�����¼��� ע��: Render��ʱ�Ѿ������û��������ƫ��, 
		��˵���Render�Ļ��ƺ���ʱ, Ӧ�ô�����ͼ�Ŀͻ�����, ������Ļ����
	*/
	virtual void OnPaint(DxView* view, DxRender* render) 
	{

	}
};

/*
	��ͼ�б�
*/
typedef std::vector<DxView*>	DxViewVector;

/*
	Ͷ�ݵĶ�����Ϣ
*/
struct DxPostInfo 
{
	DxView*		view;		// ����������ͼ
	DxPostId		id;			// Ͷ��ID
	DWORD			param1;		// ��������
	DWORD			param2;		// ��������2
};

/*
	����ʹ�øú괴��һ����ͼ��
	theclass Ҫ��������ͼ�࣬��KDxView��KDxWindow��
	parenview ����ͼ������ΪNULL
	ownerscreen ��������Ļ�ࣺKDxScreen
*/
#define NEW_VIEW(theclass, parenview, ownerscreen)\
	(theclass*)((new theclass)->Initialize(parenview, ownerscreen));

/*
	����ʹ�øú��ͷ�һ����ͼ
*/
#define FREE_VIEW(theobject)\
	do{\
		theobject->Finalize();\
		delete theobject;\
	}while(0)


/*
	��ͼ�࣬����÷�����: 
	*.  ����
		DxView* view = NEW_VIEW(DxView, wnd, screen);
	*.  ���ø�������
		view->Set... ...
	*.  �ͷ�
		FREE_VIEW(view);

	�ɼ̳��Ը���ʵ���Զ�����ͼ�����LnDxCtrls.h
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
		��ʼ���ؼ�����Ҫֱ�ӵ�������Ӧ����NEW_VIEW����, ������Ը���������һЩ��ʼ��
		parentView ����ͼ������ΪNULL�������ΪNULL������ͼ��������ͼ���������ڣ������ֶ�ȥ�ͷ���ͼ
		screen ��������Ļ
	*/
	virtual DxView* Initialize(DxView* parentView, DxScreen* screen);
	
	/*
		����������Դ����Ҫֱ�ӵ�������Ӧ����FREE_VIEW�Զ�����
		������Ը���������һЩ��ʼ��
	*/
	virtual void Finalize();

	/*
		ȡ��������Ļ
	*/
	DxScreen* GetOwnerScreen(); 

	/*
		ȡ����������
	*/
	DxWindow* GetOwnerWindow(); 

	/*
		ȡ����ͼ
	*/
	DxView* GetParentView(); 

	/*
		���ø���ͼ
	*/
	void SetParentView(DxView* parentView);

	/*
		��������ͼ
		childView Ҫ���ӵ�����ͼ
		isCheck �Ƿ�������ͼ�Ѿ����ڣ����ΪFALSE�������������Ч�ʣ�����е��ظ���ӵķ���
	*/
	BOOL AddChild(DxView* childView, BOOL isCheck = TRUE);

	/*
		��������ͼ
		childView Ҫ���ӵ�����ͼ
		pos �����λ��
		isCheck �Ƿ�������ͼ�Ѿ����ڣ����ΪFALSE�������������Ч�ʣ�����е��ظ���ӵķ���
	*/
	virtual BOOL InsertChild(DxView* childView, int pos , BOOL isCheck = TRUE);

	/*
		ɾ������ͼ��ע�ⲻ���ͷ�����ͼ
	*/
	void RemoveChild(DxView* childView);

	/*
		ɾ������ͼ��ע�ⲻ���ͷ�����ͼ
	*/
	void DeleteChild(int pos);

	/*
		����ͼ����
	*/
	int	GetChildCount();

	/*
		ȡ����ͼ��Ϊ�ӿ��ٶȣ����жϱ߽�
	*/
	DxView* GetChildView(int idx);

	/*
		ȡ���һ������ͼ�����û������ͼ������NULL
	*/
	DxView* GetLastChild();

	/*
		ȡ��һ������ͼ�� ���û������ͼ������NULL
	*/
	DxView* GetFirstChild();

	/*
		ȡ������ͼ�����������ʧ�ܷ���-1
	*/
	int IndexOf(DxView* childView);

	/*
		��������ͼ������λ��
	*/
	BOOL MoveChildPos(DxView* childView, int newPos);

	/*
		���Լ����ڸ���ͼ�����
	*/
	void BringToFront();

	/*
		���Լ����ڸ���ͼ����ײ�
	*/
	void SendToBack();

	/*
		�û�����
	*/
	int	 GetUserData();

	/*
		���û�����
	*/
	void SetUserData(int data);

	/*
		��
	*/
	int  GetLeft();

	/*
		��
	*/
	int  GetTop();

	/*
		��
	*/
	int  GetWidth();

	/*
		��
	*/
	int  GetHeight();

	/*
		����
	*/
	void SetLeft(int left);

	/*
		�趥
	*/
	void SetTop(int top);

	/*
		���
	*/
	void SetWidth(int width);

	/*
		���
	*/
	void SetHeight(int height);

	/*
		��ߴ�
	*/
	void SetSize(int width, int height);

	/*
		��λ��
	*/
	void SetPos(int left, int top);

	/*
		����λ�óߴ�
	*/
	void SetBound(int left, int top, int width, int height);
	void SetBound(const RECT& rc);

	/*
		ȡ��С�ߴ�
	*/
	SIZE GetMinSize();

	/*
		ȡ���ߴ�
	*/
	SIZE GetMaxSize();

	/*
		����С�ߴ�
	*/
	void SetMinSize(int cx, int cy);

	/*
		�����ߴ�
	*/
	void SetMaxSize(int cx, int cy);

	/*
		ȡ����ͼ�Ŀͻ�����
	*/
	void GetClientRect(RECT& rc);

	/*
		ȡ������ڸ���ͼ������
	*/
	void GetViewRect(RECT& rc);

	/*
		ȡ����ͼ��ռ����Ļ����
	*/
	void GetScreenRect(RECT& rc);

	/*
		��Ļ����ת�ͻ�����
	*/
	POINT ScreenToClient(const POINT& pt);

	/*
		�ͻ�����ת��Ļ����
	*/
	POINT ClientToScreen(const POINT& pt);

	/*
		ȡ�������µ���ͼ
		x, y ����ڸ���ͼ������
		allowDisabled �Ƿ�����ȡDisabled����ͼ
	*/
	virtual DxView* GetViewAtPos(POINT pt, BOOL allowDisabled = FALSE);

	/*
		������ԣ�����ɸ��Ƿ����Զ���ֵ 
	*/
	virtual DxHitTest HitTestView(const POINT& pt);

	/*
		Tab����λ
	*/
	BOOL IsTabStop();

	/*
		Tab����λ
	*/
	void SetTabStop(BOOL isTabStop);

	/*
		�Ƿ�ɻ�ý���
	*/
	BOOL IsFocusable();

	/*
		�����Ƿ�ɻ�ý���
	*/
	void SetFocusable(BOOL isFocusable);


	/*
		�Ƿ�Ϊ����ؼ���isInWndָ�����ڴ��ڼ����жϣ���������Ļ�����ж�
		ע����Ļ����ֻ��һ��������ͼ�������ͼ�ǽ��ռ����¼���
	*/
	BOOL IsFocused(BOOL isInWnd = FALSE);

	/*
		�Ƿ���Ի�ý���
	*/
	BOOL CanFocus();

	/*
		�轹��
	*/
	BOOL SetFocus();

	/*
		�Ƿ���겶��
	*/
	BOOL IsCaptured();

	/*
		�Ƿ��ܱ�����
	*/
	BOOL CanCapture();

	/*
		������겶��
	*/
	BOOL SetCapture();

	/*
		�ͷ���겶��
	*/
	void ReleaseCapture();

	/*
		�Ƿ�ɼ�
	*/
	BOOL IsVisible();

	/*
		��ɼ���
	*/
	void SetVisible(BOOL isVisible);

	/*
		�Ƿ����
	*/
	BOOL IsEnable();

	/*
		���ÿ�����
	*/
	void SetEnable(BOOL isEnable);

	/*
		�Ƿ�ǰ�����������ͼ
	*/
	BOOL IsHoverView();

	/*
		��
	*/
	int GetGroup();

	/*
		����
	*/
	void SetGroup(int group);

	/*
		ȡ��ͼ�¼��ӿ�
	*/
	DxViewListener* GetViewListener(); 

	/*
		����ͼ�¼��ӿڣ�������ǰһ���¼��ӿ�
	*/
	DxViewListener* SetViewListener(DxViewListener* listener);

	/*
		�������
	*/
	void HandleMouseWheel(DxShiftState shift, short delta, const POINT& pt);

public:
	/*
		����¼�
	*/
	virtual LRESULT DoMouse(DxMouseAction action, DxShiftState shift, const POINT& pt);

	/*
		�����¼�
	*/
	virtual LRESULT DoKeyboard(DxKeyAction action, WORD& key, DxShiftState shift);

	/*
		֪ͨ�¼�
	*/
	virtual void DoNotify(DxNotifyId id, DWORD param);

	/*
		�����¼�
	*/
	virtual LRESULT DoQuery(DxQueryId id, DWORD param);

	/*
		����
	*/
	virtual void DoUpdate(DWORD tick);

	/*
		���ƣ�ע��: Render��ʱ�Ѿ������û��������ƫ��, 
		��˵���Render�Ļ��ƺ���ʱ, Ӧ�ô�����ͼ�Ŀͻ�����, ������Ļ����
	*/
	virtual void DoPaint(DxRender* render);

	/*
		����Ͷ���¼�
	*/
	virtual void HandlePostEvent(DxPostId id, DWORD param1, DWORD param2);

	/*
		��������¼�
	*/
	virtual BOOL DoMouseWheel(DxShiftState shift, short delta, const POINT& pt);

protected:
	/*
		��ͼ��ʼ���ã���������������ʼ������
	*/
	virtual void DoInitialize();

	/*
		��ͼ�����������������������������
	*/
	virtual void DoFinalize();

protected:
	/*
		���¿ɼ�״̬
	*/
	void UpdateVisible(BOOL isVisible);

	/*
		���¿���״̬
	*/
	void UpdateEnable(BOOL isEnable);

	/*
		�ɼ����ã���������ͼ
	*/
	BOOL VisibleAndEnable();

	/*
		������������ͼ
	*/
	void FreeChilds();

	/*
		�����������
	*/
	void ResetScreenHoverView();

	/*
		ɾ��Ͷ����Ϣ
	*/
	void DelScreenPostEvent();

protected:
	DxScreen*			mOwnerScreen;		// ������Ļ
	DxWindow*			mOwnerWindow;		// ��ӵ�еĴ��ڣ����ViewΪ���ڣ���mOwnerWindowΪ�Լ�
	DxView*				mParentView;		// ����ͼ
	DxViewListener*		mViewListener;		// �¼��ӿ�
	DxViewVector		mChildViews;		// ����ͼ�б�
	DxViewStyle			mViewStyle;			// ��ͼ���
	int					mUserData;			// ���Թ���������
	int					mLeft;				// ��
	int					mTop;				// ��
	int					mWidth;				// ��
	int					mHeight;			// ��
	int					mGroup;				// ��
	SIZE				mMinSize;			// ��С�ߴ磬Ϊ0��ʾ������
	SIZE				mMaxSize;			// ���ߴ磬Ϊ0��ʾ������
	BOOL				mIsVisible;			// �ɼ�
	BOOL				mIsEnable;			// ����
	int					mWheelAccumulator;	// �������ۼ���
};

class DxShortcutMgr;

/*
	��ݼ��¼�
*/
struct DxShortcutListener
{
	/*
		��ݼ��¼� DxShortcutΪ��Ӧ��ݼ�
	*/
	virtual void OnShortcut(DxShortcutMgr* mgr, DxShortcut shortcut) 
	{
	}
};

/*
	��ݼ�������
*/
class DxShortcutMgr
{
	typedef std::map<DxShortcut, DxShortcutListener*> DxShortcutMap;

public:
	/*
		���ӿ�ݼ��Լ���Ӧ�¼�
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
		ɾ����ݼ�
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
		�����¼� �ɹ�����TRUE û����Ӧ��ݼ��򷵻�FALSE
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
	�����б�
*/
typedef std::list<DxWindow*> DxWindowList;

/*
	������ͼ����
*/
#define NEW_WINDOW(theclass, ownerscreen) NEW_VIEW(theclass, ownerscreen, ownerscreen)
/*
	�ͷŴ���
*/
#define FREE_WINDOW(theobject) FREE_VIEW(theobject)

/*
	�����࣬����÷����£�
	*.  ����
		DxWindow* wnd = NEW_WINDOW(DxWindow, screen);
	*.  ���ø������Բ���ʾ
		... ...
		wnd->Show();
	*.  �ͷ�
		FREE_WINDOW(wnd);

	���Լ̳��Ը���ʵ���Զ��崰�ڣ����LnDxCtrls.h
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
		// �����Show�Ժ����ʾ����
		mIsVisible = FALSE;
		mMinSize.cx = 2 * mFrameSize;
		mMinSize.cy = 2 * mFrameSize;
	}

	/*
		�����ƶ�״̬
	*/
	BOOL IsMovable();

	/*
		���ÿ��ƶ�״̬
	*/
	void SetMovable(BOOL isMovable);

	/*
		�����϶�״̬
	*/
	BOOL IsSizable();

	/*
		���ÿɸı��С״̬
	*/
	void SetSizable(BOOL isSizable);

	/*
		ESC�ر�
	*/
	BOOL IsEscClose();

	/*
		����ESC�ر�
	*/
	void SetEscClose(BOOL isEscClose);

	/*
		�Ƿ�Ϊ���㴰��
	*/
	BOOL IsTopMost();

	/*
		���ö��㴰��
	*/
	void SetTopMost(BOOL isTopMose);

	/*
		�Ƿ�ɼ���
	*/
	BOOL IsActivatable();

	/*
		���ÿɼ�����
	*/
	void SetActivatable(BOOL activatable);

	/*
		�����Լ�
	*/
	BOOL Active();

	/*
		�Ƿ񼤻�
	*/
	BOOL IsActive();

	/*
		�ܷ񼤻�
	*/
	BOOL CanActive();

	/*
		ȡ�ý�����ͼ
	*/
	DxView* GetFocusedView();

	/*
		�轹����ͼ
	*/
	BOOL SetFocusedView(DxView* view);

	/*
		Ѱ����һ��Tab��ͼ��startViewָ����ʼѰ�ҵ���ʼ��
	*/
	DxView* FindNexTabtView(DxView* startView, BOOL isForward);

	/*
		Ѱ����һ��Arrow��ͼ��startViewָ����ʼѰ�ҵ���ʼ��
	*/
	DxView* FindNextArrowView(DxView* startView, BOOL isForward);

	/*
		��ʾ
	*/
	void Show(BOOL isActive = TRUE);

	/*
		�ر�
	*/
	void Close();

	/*
		����
	*/
	void Hide();

	/*
		ģ̬��ʾ
	*/
	int	ShowModal();

	/*
		�ر�ģ̬, retָ��ShowModal���ص�ֵ
	*/
	BOOL CloseModal(int ret);

	/*
		�Ƿ�ģ̬
	*/
	BOOL IsModal();

	/*
		����λ�õ���Ļ�м�
	*/
	void ScreenCenter();

	/*
		�Ƿ������϶�
	*/
	virtual BOOL IsDraging();

	/*
		��ʼ�϶�
	*/
	virtual void BeginDrag(const POINT& pt);

	/*
		�϶�
	*/
	virtual void DragTo(const POINT& pt);

	/*
		�����϶�
	*/
	virtual void EndDrag(const POINT& pt);

	/*
		ȡ���϶�
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
		����Tab˳���б�
	*/
	void GenTabList(DxViewVector& viewVector, DxView* parentView);

protected:
	DxWndStyle		mWndStyle;			// ���ڷ��
	DxWndState		mWndState;			// ����״̬
	int				mModalResult;		// ģ̬���ؽ���
	int				mFrameSize;			// ���ĺ�ȣ�Ӱ���϶���С
	DxView*			mFocusedView;		// ��ý������ͼ
};

/*
	������Ļ��
*/
#define NEW_SCREEN(theclass) NEW_VIEW(theclass, NULL, NULL)
/*
	�ͷ���Ļ
*/
#define FREE_SCREEN(theobject) FREE_VIEW(theobject)

/*
	��Ļ�࣬����÷�����:
	*.  ����
		DxScreen* screen = NEW_SCREEN(DxScreen);
	*.  ����������
		screen->SetHostWnd(hwnd);
	*.  ����Ⱦ��
		screen->SetRender(GetRender);
	*.  ��ʱ���ºͻ��ƣ�һ������Ϣѭ����Idleʱ��
		screen->Update();
		// �������Ҫ����֡��
		screen->Screen();
	*.  �ͷ�
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
		�����������ڣ�������Ļ���ҽӴ��ڵļ��������Ϣ
	*/
	void SetHostWnd(HWND hwnd);

	/*
		ȡ��������
	*/
	HWND GetHostWnd(); 

	/*
		����Ⱦ��
	*/
	void SetRender(DxRender* render);

	/*
		ȡ��Ⱦ��
	*/
	DxRender* GetRender();

	/*
		�Ƿ�֧�ּ���
	*/
	void SetClip(BOOL canClip);

	/*
		�Ƿ�֧�ּ���
	*/
	BOOL CanClip();

	/*
		ȡĳ���Ӵ���
	*/
	DxWindow* GetChildWindow(int idx);

	/*
		ȡ�����
	*/
	DxWindow* GetActiveWindow(); 

	/*
		ȡ������ͼ, ������Ļֻ��һ����ͼ���Խ��ռ�������, �����ͼ���ǽ�����ͼ
		������ͼ�����ڻ������
	*/
	DxView* GetFocusedView();

	/*
		���û����
	*/
	BOOL SetActiveWindow(DxWindow* wnd);

	/*
		������һ������
	*/
	void ActiveNextWindow(DxWindow* wnd);

	/*
		ȡ����겶�����ͼ
	*/
	DxView* GetCaptureView(); 

	/*
		������겶�����ͼ
	*/
	BOOL SetCaptureView(DxView* view);

	/*
		ȡid���͵Ĺ��
	*/
	HCURSOR GetCursor(DxCursorId id);

	/*
		����Զ�����
	*/
	BOOL AddCursor(DxCursorId id, HCURSOR cursor);

	/*
		��ǰ���λ��
	*/
	int GetMouseX();
	int GetMouseY(); 

	/*
		Ͷ�ݶ�����Ч��������PostMessage
		ActionIDָ������ID��param1��param2�Ƕ��������Ĳ�����
	*/
	BOOL PostEvent(DxView* view, DxPostId id, DWORD param1, DWORD param2);

	/*
		�Ƴ����й��ڸ���ͼ��Ͷ�ݶ���
	*/
	void DelPostEvents(DxView* view);

	/*
		��ʼģ̬����
	*/
	void BeginModal(DxWindow* wnd, DxWindowList& wndList);

	/*
		����ģ̬����
	*/
	void EndModal(DxWindow* wnd, DxWindowList& wndList);

	/*
		ȡ����Ϣѭ���ӿ�
	*/
	MsgLooper* GetMsgLooper();

	/*
		����Ϣѭ���ӿ�
	*/
	void SetMsgLooper(MsgLooper* msgLooper);

	/*
		ȡ����������������ͼ
	*/
	DxView* GetHoverView(); 

	/*
		�������������ͼ
	*/
	void ResetHoverView(); 

	/*
		����
	*/
	virtual void Paint();

	/*
		����
	*/
	virtual void Update(DWORD tick);

	/*
		���ö�ʱ��, ���ض�ʱ��ID�����ΪNULL�����ֺ���ʧ��
	*/
	UINT SetTimer(DxView* view, UINT elapse);

	/*
		ȡ����ʱ����idΪSetTimer���ص�ֵ
	*/
	BOOL KillTimer(DxView* view, UINT id);

	/*
		�����ͼ��صĶ�ʱ��
	*/
	void ClearTimer(DxView* view);

public:
	/*
		������ĻҪ��һЩ�ض��Ĵ���
	*/
	virtual DxView* Initialize(DxView* parentView, DxScreen* screen);

	/*
		����ȫ����Դ
	*/
	virtual void Finalize();

	/*
		����ͼ�����Ǵ���
	*/
	virtual BOOL InsertChild(DxView* childView, int pos , BOOL isCheck = TRUE);

	/*
		�����㴰�ڵ����
	*/
	virtual DxView* GetViewAtPos(const POINT& pt, BOOL allowDisabled  = FALSE);

	/*
		������ϢԤ����
	*/
	virtual BOOL PreKeyHandle(WORD& key, DxShiftState shift);

protected:
	/*
		��ʼ������
	*/
	void InitCursorTable();

	/*
		�������й��
	*/
	void DestroyAllCursor();

	/*
		��������ͼ
	*/
	void PaintChilds(DxView* parentView, const RECT& rcParentPaint, const RECT& rcParentScreen);

	/*
		��������ͼ
	*/
	void UpdateChilds(DxView* parentView, DWORD tick);

	/*
		��ʱ������
	*/
	BOOL RestoreTimer(DxView* view, UINT id);
	UINT AllocTimer(DxView* view);

protected:
	/*
		���໯��������
	*/
	void SubclassWindow(HWND hwnd);

	/*
		�����໯��������
	*/
	void UnsubclassWindow(HWND hwnd);

	/*
		�������ڹ���
	*/
	static LRESULT CALLBACK	StdWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/*
		����TRUE�����ٽ���mDefHostWndProc����lRet���Ǵ�����̵ķ���ֵ
	*/
	virtual BOOL WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& lRet);

	/*
		��������Ĭ�ϴ������
	*/
	LRESULT DefWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/*
		���崰����Ϣ����
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
	WNDPROC				mDefHostWndProc;		// ��������Ĭ�ϵĴ��ڹ���
	HWND				mHostWnd;				// ��������
	DxView*				mCaptureView;			// ����������ͼ
	DxWindow*			mActiveWindow;			// ��ǰ��Ĵ���
	DxCursorMap			mCursorMap;				// ����������͵Ķ�Ӧ��
	DxPostInfoList		mPostActionList;		// Ͷ�ݶ����б�
	int					mMouseX;				// ��ǰ���λ��X
	int					mMouseY;				// ��ǰ���λ��Y
	int					mModalLevel;			// ģ̬���
	MsgLooper*			mMsgLooper;				// ��Ϣѭ��
	DxView*				mHoverView;				// �����������ͼ
	DxRender*			mRender;				// ��Ⱦ��
	BOOL				mCanClip;				// ֧�ּ��ã����ܻ����Խ������ܣ�����
	DxTimerVector		mTimerVector;			// ��ʱ���б�
};


//////////////////////////////////////////////////////////////////////////
// implement

//------------------------------------------------------------------------------
// Helpers

/*
	ȡ��һ����ͼ�ĸ����ڣ����pView�����Ǵ��ڣ��򷵻��Լ�
*/
inline DxWindow* GetParentWindow(DxView* view)
{
	DxView* parentView = view;
	while (parentView && (!OBJECT_DERIVEDFROM(parentView, DxWindow)))
		parentView = parentView->GetParentView();

	return (DxWindow*)parentView;
}

/*
	��virtual keys ת��ΪShiftState
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
	��Key data ת��ΪShiftState
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

	// ȥ������
	if (mOwnerWindow && mOwnerWindow->GetFocusedView() == this)
		mOwnerWindow->SetFocusedView(NULL);

	// ȥ����겶��
	ReleaseCapture();

	// �������������ͼ
	ResetScreenHoverView();

	// ɾ�����ڸ���ͼ��Ͷ�ݶ���
	DelScreenPostEvent();

	// �����ʱ��
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

	// ����ߴ�
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

	// ����ߴ�
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
	// ��������ͼ���������ÿɼ���
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
	// �����һ����ͼ��ǰ�ͷ�
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
	�ƶ�ʱ�õ���״̬������ٶ�ͬʱֻ��һ�����������ƶ�������С
*/
_declspec(selectany) BOOL			gIsDraging = FALSE;	// ��־�Ƿ�ʼ�϶�
_declspec(selectany) DxHitTest		gDragHitTest;		// �����϶�ʱ�ĵ������ֵ
_declspec(selectany) POINT			gDragPoint;			// �϶�ʱ��ԭʼλ��
_declspec(selectany) int			gOrgWidth;			// �϶�ʱ�Ĵ�Сλ��
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
	// ������봦���϶�
	if (action == maMouseMove)	
	{
		// ����ƶ��������϶�������
		if (IsDraging())
			DragTo(pt);
	}
	else if (action == maLButtonDown)
	{
		// �����£��϶���ʼ
		gDragHitTest = HitTestView(pt);
		if ((gDragHitTest == HT_MOVEREION) || 
			((gDragHitTest >= HT_BORDERLEFT) && (gDragHitTest <= HT_BORDERRIGHTBOTTOM)))
			BeginDrag(pt);
	}
	else if (action = maLButtonUp)
	{
		// ��굯���϶�����
		EndDrag(pt);
	}

	return DxView::DoMouse(action, state, pt);
}

inline void DxWindow::DoNotify(DxNotifyId id, DWORD param)
{
	if (id == NID_VISIBLECHANGED)
	{
		// ���ؼ�����һ������
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
	// ���ڿ�ݼ�
	DxShortcut shortcut = MAKE_SHORTCUT(key, shift);
	if (HandleShortcut(shortcut))
		return TRUE;

	if (!HAS_FLAG(shift, ssAlt))
	{
		if (VK_ESCAPE == key)
		{
			// ȡ���϶�
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

		// Ѱ�ҵ�һ�����Ի�ý������ͼ���о��轹����������û�о�����Լ�
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
	// ����ƽ��Ƴ�
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
	// ��Ļ������ͼһ���Ǵ���
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

		// �ȱ������㴰��
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

		// �ٱ����Ƕ��㴰��
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
	// ȫ�ֿ�ݼ�
	DxShortcut shortcut = MAKE_SHORTCUT(key, shift);

	if (HandleShortcut(shortcut))
		return TRUE;

	// ���ڿ�ݼ�
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
		// �Ȼ��ƷǶ��㴰��
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

		// �ٻ��ƶ��㴰
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
	// һ������ֻ����һ��Screen����
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

	// ����������ͼ���ȣ�ע�⣺CaptureViewһ���ǿɼ��ģ����õ�
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
		// �ȼ������
		SetActiveWindow(view->GetOwnerWindow());

		// �ٲ�������Լ���������¼�
		SetCaptureView(view);
		if (NULL != mCaptureView)
		{	
			pt = mCaptureView->ScreenToClient(pt);
			mCaptureView->DoMouse(maLButtonDblClk, shift, pt);
			mCaptureView->DoMouse(maLButtonDown, shift, pt);
			// ����
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
		// �ȼ������
		SetActiveWindow(view->GetOwnerWindow());

		// �ٲ�������Լ���������¼�
		SetCaptureView(view);
		if (NULL != mCaptureView)
		{	
			pt = mCaptureView->ScreenToClient(pt);
			mCaptureView->DoMouse(maLButtonDown, shift, pt);
			// ����
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

		// ���
		RECT rc;
		view->GetClientRect(rc);
		if (::PtInRect(&rc, pt))
			view->DoMouse(maMouseClick, shift, pt);

		// ����
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

	// �ȴ����ݼ���Tab���������
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
		// ���ɼ�������Ļ����˵��¼�
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
		// �����������Ļ����˵��¼�
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