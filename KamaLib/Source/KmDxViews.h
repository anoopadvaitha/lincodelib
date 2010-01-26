/*******************************************************************************
  Filename:		KmDxViews.h
  Author:		Tramper
  Email:		linzhenqun@gmail.com
  Date:			2009/12/19

  Brief:    	����KamaLib������һ���֣���Tramper������ά������Ȩû�У�
				������ʹ�ã�
 -------------------------------------------------------------------------------
  Description:
	Dx��ͼ���
*******************************************************************************/
#ifndef __KAMA_KMDXVIEWS_H__
#define __KAMA_KMDXVIEWS_H__
#include "KmCommons.h"
#include "KmDxRender.h"

namespace kama
{

//////////////////////////////////////////////////////////////////////////
// declare

/*
	��궯������
*/
enum KDxMouseAction
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
enum KDxKeyAction
{
	kaKeyDown,							// ���̻���
	kaChar,								// �����ַ�
	kaKeyUp								// ���̵���
};

/*
	Shift״̬
*/
typedef DWORD KDxShiftState;
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
typedef DWORD KDxNotifyId;
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

/*
	�����¼�ID, 0~QID_USER-1�ɿ�ܱ������û�����ʹ��������ֵ
*/
typedef DWORD KDxQueryId;
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
typedef DWORD KDxHitTest;
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
typedef DWORD KDxCursorId;
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
typedef DWORD KDxViewStyle;
#define VS_TABSTOP					0x01	// Tabֹͣ���뽹���ϵ����
#define VS_FOCUSABLE				0x02	// �Ƿ�ɻ�ý���

/*
	���ڷ��
*/
typedef DWORD KDxWndStyle;
#define WS_MOVABLE					0x01	// ���ƶ�
#define WS_SIZABLE					0x02	// ���϶�
#define WS_TOPMOST					0x04	// ���㴰��
#define WS_ESCCLOSE					0x08	// ESC�ر�
#define WS_ACTIVATABLE				0x10	// �ɼ���

/*
	����״̬
*/
typedef DWORD KDxWndState;
#define WS_MODAL					0x01	// ģ̬״̬

/*
	���ڹر�ģʽ
*/
enum KDxCloseMode
{
	cmHide,									// ����
	cmFree,									// �ͷ�
	cmNone									// ʲô��Ҳ����
};

/*
	��ݼ�ֵ								  
*/
typedef DWORD KDxShortcut;
// ���ɿ�ݼ�����
#define MAKE_SHORTCUT(key, shift) (KDxShortcut)MAKELONG(key, shift)
// �ӿ�ݼ�ȡ�����
#define VKEY_OF_SHORTCUT(shortcut) LOWORD(shortcut)
// �ӿ�ݼ�ȡ��Shift״̬
#define SHIFT_OF_SHORTCUT(shortcut) (KDxShiftState)HIWORD(shortcut)


/*
	Ͷ���¼�ID, 0~PID_USER-1�ɿ�ܱ������û�����ʹ������ֵ
*/
typedef DWORD KDxPostId;
#define PID_USER					ID_USER
#define PID_RELEASE					1		// �ͷ��Լ����޲���

class KDxView;
class KDxWindow;
class KDxScreen;

/*
	��ͼ�¼��ӿ�
*/
interface IDxViewEvent
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
	virtual LRESULT OnMouse(KDxView* view, KDxMouseAction action, KDxShiftState shift, const POINT& pt) 
	{
		return 0;
	}

	/*
		�����¼������£������ַ�
	*/
	virtual LRESULT OnKeyboard(KDxView* view, KDxKeyAction action, WORD& key, KDxShiftState shift) 
	{
		return 0;
	}

	/*
		ͨ�õ�֪ͨ�¼���KDxNotifyIdָ��֪ͨ������, paramΪ�����Ĳ���
	*/
	virtual void OnNotify(KDxView* view, KDxNotifyId id, DWORD param) 
	{

	}

	/*
		ͨ�õ������¼���KDxQueryIdָ�����������, paramΪ�����Ĳ���
	*/
	virtual LRESULT OnQuery(KDxView* view, KDxQueryId id, DWORD param) 
	{ 
		return 0; 
	}

	/*
		�����¼�
	*/
	virtual void OnUpdate(KDxView* view, DWORD tick) 
	{

	}

	/*
		�����¼��� ע��: Render��ʱ�Ѿ������û��������ƫ��, 
		��˵���Render�Ļ��ƺ���ʱ, Ӧ�ô�����ͼ�Ŀͻ�����, ������Ļ����
	*/
	virtual void OnPaint(KDxView* view, KDxRender* render) 
	{

	}
};

/*
	��ͼ�б�
*/
typedef std::vector<KDxView*>	KDxViewVector;

/*
	Ͷ�ݵĶ�����Ϣ
*/
struct KDxPostInfo 
{
	KDxView*		view;		// ����������ͼ
	KDxPostId		id;			// Ͷ��ID
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
		KDxView* view = NEW_VIEW(KDxView, wnd, screen);
	*.  ���ø�������
		view->Set... ...
	*.  �ͷ�
		FREE_VIEW(view);

	�ɼ̳��Ը���ʵ���Զ�����ͼ�����KmDxCtrls.h
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
	virtual KDxView* Initialize(KDxView* parentView, KDxScreen* screen);
	
	/*
		����������Դ����Ҫֱ�ӵ�������Ӧ����FREE_VIEW�Զ�����
		������Ը���������һЩ��ʼ��
	*/
	virtual void Finalize();

	/*
		ȡ��������Ļ
	*/
	KDxScreen* OwnerScreen(); 

	/*
		ȡ����������
	*/
	KDxWindow* OwnerWindow(); 

	/*
		ȡ����ͼ
	*/
	KDxView* ParentView(); 

	/*
		���ø���ͼ
	*/
	void SetParentView(KDxView* parentView);

	/*
		��������ͼ
		childView Ҫ���ӵ�����ͼ
		isCheck �Ƿ�������ͼ�Ѿ����ڣ����ΪFALSE�������������Ч�ʣ�����е��ظ���ӵķ���
	*/
	BOOL AddChild(KDxView* childView, BOOL isCheck = TRUE);

	/*
		��������ͼ
		childView Ҫ���ӵ�����ͼ
		pos �����λ��
		isCheck �Ƿ�������ͼ�Ѿ����ڣ����ΪFALSE�������������Ч�ʣ�����е��ظ���ӵķ���
	*/
	virtual BOOL InsertChild(KDxView* childView, int pos , BOOL isCheck = TRUE);

	/*
		ɾ������ͼ��ע�ⲻ���ͷ�����ͼ
	*/
	void RemoveChild(KDxView* childView);

	/*
		ɾ������ͼ��ע�ⲻ���ͷ�����ͼ
	*/
	void DeleteChild(int pos);

	/*
		����ͼ����
	*/
	int	ChildCount();

	/*
		ȡ����ͼ��Ϊ�ӿ��ٶȣ����жϱ߽�
	*/
	KDxView* ChildView(int idx);

	/*
		ȡ���һ������ͼ�����û������ͼ������NULL
	*/
	KDxView* LastChild();

	/*
		ȡ��һ������ͼ�� ���û������ͼ������NULL
	*/
	KDxView* FirstChild();

	/*
		ȡ������ͼ�����������ʧ�ܷ���-1
	*/
	int IndexOf(KDxView* childView);

	/*
		��������ͼ������λ��
	*/
	BOOL MoveChildPos(KDxView* childView, int newPos);

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
	int	 UserData();

	/*
		���û�����
	*/
	void SetUserData(int data);

	/*
		��
	*/
	int  Left();

	/*
		��
	*/
	int  Top();

	/*
		��
	*/
	int  Width();

	/*
		��
	*/
	int  Height();

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
	SIZE MinSize();

	/*
		ȡ���ߴ�
	*/
	SIZE MaxSize();

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
	virtual KDxView* GetViewAtPos(POINT pt, BOOL allowDisabled = FALSE);

	/*
		������ԣ�����ɸ��Ƿ����Զ���ֵ 
	*/
	virtual KDxHitTest HitTestView(const POINT& pt);

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
	int Group();

	/*
		����
	*/
	void SetGroup(int group);

	/*
		ȡ��ͼ�¼��ӿ�
	*/
	IDxViewEvent* ViewEvent(); 

	/*
		����ͼ�¼��ӿڣ�������ǰһ���¼��ӿ�
	*/
	IDxViewEvent* SetViewEvent(IDxViewEvent* event);

	/*
		�������
	*/
	void HandleMouseWheel(KDxShiftState shift, short delta, const POINT& pt);

public:
	/*
		����¼�
	*/
	virtual LRESULT DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt);

	/*
		�����¼�
	*/
	virtual LRESULT DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift);

	/*
		֪ͨ�¼�
	*/
	virtual void DoNotify(KDxNotifyId id, DWORD param);

	/*
		�����¼�
	*/
	virtual LRESULT DoQuery(KDxQueryId id, DWORD param);

	/*
		����
	*/
	virtual void DoUpdate(DWORD tick);

	/*
		���ƣ�ע��: Render��ʱ�Ѿ������û��������ƫ��, 
		��˵���Render�Ļ��ƺ���ʱ, Ӧ�ô�����ͼ�Ŀͻ�����, ������Ļ����
	*/
	virtual void DoPaint(KDxRender* render);

	/*
		����Ͷ���¼�
	*/
	virtual void HandlePostEvent(KDxPostId id, DWORD param1, DWORD param2);

	/*
		��������¼�
	*/
	virtual BOOL DoMouseWheel(KDxShiftState shift, short delta, const POINT& pt);

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
	KDxScreen*			mOwnerScreen;		// ������Ļ
	KDxWindow*			mOwnerWindow;		// ��ӵ�еĴ��ڣ����ViewΪ���ڣ���mOwnerWindowΪ�Լ�
	KDxView*			mParentView;		// ����ͼ
	IDxViewEvent*		mViewEvent;			// �¼��ӿ�
	KDxViewVector		mChildViews;		// ����ͼ�б�
	KDxViewStyle		mViewStyle;			// ��ͼ���
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

class KDxShortcutMgr;

/*
	��ݼ��¼�
*/
interface IDxShortcutEvent
{
	/*
		��ݼ��¼� KDxShortcutΪ��Ӧ��ݼ�
	*/
	virtual void OnShortcut(KDxShortcutMgr* mgr, KDxShortcut shortcut) 
	{
	}
};

/*
	��ݼ�������
*/
class KDxShortcutMgr
{
	typedef std::map<KDxShortcut, IDxShortcutEvent*> KDxShortcutMap;

public:
	/*
		���ӿ�ݼ��Լ���Ӧ�¼�
	*/
	BOOL AddShortcut(KDxShortcut shortcut, IDxShortcutEvent* event)
	{
		KDxShortcutMap::iterator itr = mShortcutMap.find(shortcut);
		if (itr != mShortcutMap.end())
			return FALSE;

		mShortcutMap.insert(std::make_pair(shortcut, event));
		return TRUE;
	}

	/*
		ɾ����ݼ�
	*/
	BOOL DelShortcut(KDxShortcut shortcut)
	{
		KDxShortcutMap::iterator itr = mShortcutMap.find(shortcut);
		if (itr == mShortcutMap.end())
			return FALSE;

		mShortcutMap.erase(itr);

		return TRUE;
	}

protected:
	/*
		�����¼� �ɹ�����TRUE û����Ӧ��ݼ��򷵻�FALSE
	*/
	virtual BOOL HandleShortcut(KDxShortcut shortcut)
	{
		KDxShortcutMap::iterator itr = mShortcutMap.find(shortcut);
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
	KDxShortcutMap mShortcutMap;
};

/*
	�����б�
*/
typedef std::list<KDxWindow*> KDxWindowList;

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
		KDxWindow* wnd = NEW_WINDOW(KDxWindow, screen);
	*.  ���ø������Բ���ʾ
		... ...
		wnd->Show();
	*.  �ͷ�
		FREE_WINDOW(wnd);

	���Լ̳��Ը���ʵ���Զ��崰�ڣ����KmDxCtrls.h
*/
class KDxWindow: public KDxView, public KDxShortcutMgr
{
	DECLARE_RUNTIMEINFO(KDxWindow)
public:
	KDxWindow():
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
	KDxView* FocusedView();

	/*
		�轹����ͼ
	*/
	BOOL SetFocusedView(KDxView* view);

	/*
		Ѱ����һ��Tab��ͼ��startViewָ����ʼѰ�ҵ���ʼ��
	*/
	KDxView* FindNexTabtView(KDxView* startView, BOOL isForward);

	/*
		Ѱ����һ��Arrow��ͼ��startViewָ����ʼѰ�ҵ���ʼ��
	*/
	KDxView* FindNextArrowView(KDxView* startView, BOOL isForward);

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

	virtual LRESULT DoMouse(KDxMouseAction action, KDxShiftState state, const POINT& pt);

	virtual void DoNotify(KDxNotifyId id, DWORD param);

	virtual LRESULT DoQuery(KDxQueryId id, DWORD param);

	virtual KDxHitTest HitTestView(const POINT& pt);

	virtual BOOL PreKeyHandle(WORD& key, KDxShiftState shift);

	virtual void HandlePostAction(KDxPostId paId, DWORD param1, DWORD param2);

protected:
	/*
		����Tab˳���б�
	*/
	void GenTabList(KDxViewVector& viewVector, KDxView* parentView);

protected:
	KDxWndStyle		mWndStyle;			// ���ڷ��
	KDxWndState		mWndState;			// ����״̬
	int				mModalResult;		// ģ̬���ؽ���
	int				mFrameSize;			// ���ĺ�ȣ�Ӱ���϶���С
	KDxView*		mFocusedView;		// ��ý������ͼ
};

/*
	Ĭ�ϵ���Ϣѭ��
*/
class KDxMsgLooper: public KMsgLooper
{
public:
	KDxMsgLooper(KDxScreen* screen): mScreen(screen)
	{
		mTick = KGetTickCount();
	}

	virtual void DoIdle(BOOL& isDone);

private:
	DWORD		mTick;
	KDxScreen*	mScreen;
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
		KDxScreen* screen = NEW_SCREEN(KDxScreen);
	*.  ����������
		screen->SetHostWnd(hwnd);
	*.  ����Ⱦ��
		screen->SetRender(Render);
	*.  ��ʱ���ºͻ��ƣ�һ������Ϣѭ����Idleʱ��
		screen->Update();
		// �������Ҫ����֡��
		screen->Screen();
	*.  �ͷ�
		FREE_SCREEN(screen);
		
*/
class KDxScreen: public KDxView, public KDxShortcutMgr
{
	typedef std::map<KDxCursorId, HCURSOR> KDxCursorMap;
	typedef std::list<KDxPostInfo> KDxPostInfoList;
	DECLARE_RUNTIMEINFO(KDxScreen)
public:
	KDxScreen(): 
		mDefHostWndProc(NULL),
		mHostWnd(NULL), 
		mCaptureView(NULL),
		mActiveWindow(NULL),
		mMouseX(0),
		mMouseY(0),
		mModalLevel(0),
		mMsgLooper(NULL),
		mDefMsgLooper(NULL),
		mFrameTime(30),
		mHoverView(NULL),
		mRender(NULL),
		mCanClip(FALSE)
	{
	}

	/*
		�����������ڣ�������Ļ���ҽӴ��ڵļ��������Ϣ
	*/
	void SetHostWnd(HWND hwnd);

	/*
		ȡ��������
	*/
	HWND HostWnd(); 

	/*
		����Ⱦ��
	*/
	void SetRender(KDxRender* render);

	/*
		ȡ��Ⱦ��
	*/
	KDxRender* Render();

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
	KDxWindow* ChildWindow(int idx);

	/*
		ȡ�����
	*/
	KDxWindow* ActiveWindow(); 

	/*
		ȡ������ͼ, ������Ļֻ��һ����ͼ���Խ��ռ�������, �����ͼ���ǽ�����ͼ
		������ͼ�����ڻ������
	*/
	KDxView* FocusedView();

	/*
		���û����
	*/
	BOOL SetActiveWindow(KDxWindow* wnd);

	/*
		������һ������
	*/
	void ActiveNextWindow(KDxWindow* wnd);

	/*
		ȡ����겶�����ͼ
	*/
	KDxView* CaptureView(); 

	/*
		������겶�����ͼ
	*/
	BOOL SetCaptureView(KDxView* view);

	/*
		ȡid���͵Ĺ��
	*/
	HCURSOR GetCursor(KDxCursorId id);

	/*
		����Զ�����
	*/
	BOOL AddCursor(KDxCursorId id, HCURSOR cursor);

	/*
		��ǰ���λ��
	*/
	int MouseX();
	int MouseY(); 

	/*
		Ͷ�ݶ�����Ч��������PostMessage
		ActionIDָ������ID��param1��param2�Ƕ��������Ĳ�����
	*/
	BOOL PostEvent(KDxView* view, KDxPostId id, DWORD param1, DWORD param2);

	/*
		�Ƴ����й��ڸ���ͼ��Ͷ�ݶ���
	*/
	void DelPostEvents(KDxView* view);

	/*
		��ʼģ̬����
	*/
	void BeginModal(KDxWindow* wnd, KDxWindowList& wndList);

	/*
		����ģ̬����
	*/
	void EndModal(KDxWindow* wnd, KDxWindowList& wndList);

	/*
		ȡ����Ϣѭ���ӿ�
	*/
	KMsgLooper* MsgLooper();

	/*
		����Ϣѭ���ӿ�
	*/
	void SetMsgLooper(KMsgLooper* msgLooper);

	/*
		ȡ���ºͻ��Ƶ�֡���
	*/
	DWORD FrameTime();

	/*
		����ºͻ��Ƶ�֡���
	*/
	void SetFrameTime(DWORD time);

	/*
		ȡ����������������ͼ
	*/
	KDxView* HoverView(); 

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

public:
	/*
		������ĻҪ��һЩ�ض��Ĵ���
	*/
	virtual KDxView* Initialize(KDxView* parentView, KDxScreen* screen);

	/*
		����ȫ����Դ
	*/
	virtual void Finalize();

	/*
		����ͼ�����Ǵ���
	*/
	virtual BOOL InsertChild(KDxView* childView, int pos , BOOL isCheck = TRUE);

	/*
		�����㴰�ڵ����
	*/
	virtual KDxView* GetViewAtPos(const POINT& pt, BOOL allowDisabled  = FALSE);

	/*
		������ϢԤ����
	*/
	virtual BOOL PreKeyHandle(WORD& key, KDxShiftState shift);

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
	void PaintChilds(KDxView* parentView, const RECT& rcParentPaint, const RECT& rcParentScreen);

	/*
		��������ͼ
	*/
	void UpdateChilds(KDxView* parentView, DWORD tick);

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
	void WMOtherMouse(KDxMouseAction action, WPARAM wparam, LPARAM lparam);
	void WMMouseWheel(WPARAM wparam, LPARAM lparam);
	void WMKeyDown(WPARAM wparam, LPARAM lparam);
	void WMKeyMsg(KDxKeyAction action, WPARAM wparam, LPARAM lparam);
	void WMContextMenu(WPARAM wparam, LPARAM lparam);
	void WMPostAction(WPARAM wparam, LPARAM lparam);
	BOOL WMSetCursor(WPARAM wparam, LPARAM lparam);

protected:
	WNDPROC				mDefHostWndProc;		// ��������Ĭ�ϵĴ��ڹ���
	HWND				mHostWnd;				// ��������
	KDxView*			mCaptureView;			// ����������ͼ
	KDxWindow*			mActiveWindow;			// ��ǰ��Ĵ���
	KDxCursorMap		mCursorMap;				// ����������͵Ķ�Ӧ��
	KDxPostInfoList		mPostActionList;		// Ͷ�ݶ����б�
	int					mMouseX;				// ��ǰ���λ��X
	int					mMouseY;				// ��ǰ���λ��Y
	int					mModalLevel;			// ģ̬���
	KMsgLooper*			mMsgLooper;				// ��Ϣѭ��
	KDxMsgLooper*		mDefMsgLooper;			// Ĭ����Ϣѭ��
	DWORD				mFrameTime;				// ÿ���»����һ֡��ʱ��(ms)
	KDxView*			mHoverView;				// �����������ͼ
	KDxRender*			mRender;				// ��Ⱦ��
	BOOL				mCanClip;			// ֧�ּ��ã����ܻ����Խ������ܣ�����
};


//////////////////////////////////////////////////////////////////////////
// implement

//------------------------------------------------------------------------------
// Helpers

/*
	ȡ��һ����ͼ�ĸ����ڣ����pView�����Ǵ��ڣ��򷵻��Լ�
*/
inline KDxWindow* GetParentWindow(KDxView* view)
{
	KDxView* parentView = view;
	while (parentView && (!OBJECT_DERIVEDFROM(parentView, KDxWindow)))
		parentView = parentView->ParentView();

	return (KDxWindow*)parentView;
}

/*
	��virtual keys ת��ΪShiftState
*/
inline KDxShiftState VKeyToShiftState(WORD key)
{
	KDxShiftState shift = 0;
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
inline KDxShiftState KeyDataToShiftState(long keyData)
{
	#define ALT_MASK 0x20000000
	KDxShiftState shift = 0;
	if (::GetKeyState(VK_SHIFT) < 0)	ADD_FLAG(shift, ssShift);
	if (::GetKeyState(VK_CONTROL) < 0)	ADD_FLAG(shift, ssCtrl);
	if (HAS_FLAG(keyData, ALT_MASK))	ADD_FLAG(shift, ssAlt);
	return shift;
}

//------------------------------------------------------------------------------
// KDxView

IMPLEMENT_RUNTIMEINFO(KDxView, KObject)

inline KDxView* KDxView::Initialize(KDxView* parentView, KDxScreen* screen)
{
	KASSERT(screen != NULL);
	mOwnerScreen = screen;

	if (parentView)
		parentView->AddChild(this, FALSE);

	DoInitialize();

	return this;
}

inline void KDxView::Finalize()
{
	DoFinalize();

	FreeChilds();
	SetParentView(NULL);

	// ȥ������
	if (mOwnerWindow && mOwnerWindow->FocusedView() == this)
		mOwnerWindow->SetFocusedView(NULL);

	// ȥ����겶��
	ReleaseCapture();

	// �������������ͼ
	ResetScreenHoverView();

	// ɾ�����ڸ���ͼ��Ͷ�ݶ���
	DelScreenPostEvent();
}

inline void KDxView::DoInitialize()
{

}

inline void KDxView::DoFinalize()
{

}

inline KDxScreen* KDxView::OwnerScreen() 
{ 
	return mOwnerScreen; 
}

inline KDxWindow* KDxView::OwnerWindow() 
{ 
	return mOwnerWindow; 
}

inline KDxView* KDxView::ParentView() 
{ 
	return mParentView; 
}

inline void KDxView::SetParentView(KDxView* parentView)
{
	if (parentView != mParentView)
	{
		if (mParentView)
			mParentView->RemoveChild(this);
		if (parentView)
			parentView->AddChild(this);
	}
}

inline BOOL KDxView::AddChild(KDxView* childView, BOOL isCheck)
{
	return InsertChild(childView, (int)mChildViews.size(), isCheck);
}

inline BOOL KDxView::InsertChild(KDxView* childView, int pos , BOOL isCheck)
{
	KASSERT(childView);

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

inline void KDxView::RemoveChild(KDxView* childView)
{
	KDxViewVector::iterator itr = std::find(mChildViews.begin(), mChildViews.end(), childView);
	if (itr != mChildViews.end())
		mChildViews.erase(itr);
	childView->mParentView = NULL;
}

inline void KDxView::DeleteChild(int pos)
{
	if ((pos < 0) || (pos >= (int)mChildViews.size()))
		return;

	KDxView* childView = mChildViews[pos];
	mChildViews.erase(mChildViews.begin() + pos);
	childView->mParentView = NULL;
}

inline int KDxView::ChildCount() 
{ 
	return (int)mChildViews.size(); 
}

inline KDxView* KDxView::ChildView(int idx) 
{ 
	return mChildViews[idx]; 
}

inline KDxView* KDxView::LastChild()
{
	KDxViewVector::reverse_iterator revItr = mChildViews.rbegin();
	if (revItr == mChildViews.rend())
		return NULL;
	else
		return (*revItr);
}

inline KDxView* KDxView::FirstChild()
{
	KDxViewVector::iterator revItr = mChildViews.begin();
	if (revItr == mChildViews.end())
		return NULL;
	else
		return (*revItr);

}

inline int KDxView::IndexOf(KDxView* childView)
{
	if (NULL == childView)
		return -1;

	KDxViewVector::iterator Itr = std::find(mChildViews.begin(), mChildViews.end(), childView);
	if (Itr == mChildViews.end())
		return -1;
	else 
		return (int)(Itr - mChildViews.begin());
}
inline BOOL KDxView::MoveChildPos(KDxView* childView, int newPos)
{
	KASSERT(childView);

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

inline void KDxView::BringToFront()
{
	if (mParentView)
		mParentView->MoveChildPos(this, mParentView->ChildCount() - 1);
}


inline void KDxView::SendToBack()
{
	if (mParentView)
		mParentView->MoveChildPos(this, 0);
}

inline int KDxView::UserData() 
{ 
	return mUserData; 
}

inline void KDxView::SetUserData(int data) 
{ 
	mUserData = data; 
}

inline int KDxView::Left() 
{ 
	return mLeft; 
}

inline int KDxView::Top() 
{ 
	return mTop; 
}

inline int KDxView::Width() 
{ 
	return  mWidth; 
}

inline int KDxView::Height() 
{ 
	return mHeight; 
}

inline void KDxView::SetLeft(int left)
{
	SetPos(left, mTop);
}

inline void KDxView::SetTop(int top)
{
	SetPos(mLeft, top);
}

inline void KDxView::SetWidth(int width)
{
	SetSize(width, mHeight);
}

inline void KDxView::SetHeight(int height)
{
	SetSize(mWidth, height);
}

inline void KDxView::SetSize(int width, int height)
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

inline void KDxView::SetPos(int left, int top)
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

inline void KDxView::SetBound(const RECT& rc)
{
	SetBound(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

inline void KDxView::SetBound(int left, int top, int width, int height)
{
	SetPos(left, top);
	SetSize(width, height);
}

inline SIZE KDxView::MinSize() 
{ 
	return mMinSize; 
}

inline SIZE KDxView::MaxSize() 
{ 
	return mMaxSize; 
}

inline void KDxView::SetMinSize(int cx, int cy)
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

inline void KDxView::SetMaxSize(int cx, int cy)
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

inline void KDxView::GetClientRect(RECT& rc)
{
	SetRect(&rc, 0, 0, mWidth, mHeight);
}

inline void KDxView::GetViewRect(RECT& rc)
{
	SetRect(&rc, mLeft, mTop, mLeft + mWidth, mTop + mHeight);
}

inline void KDxView::GetScreenRect(RECT& rc)
{
	int Left = 0;
	int Top = 0;
	KDxView* view = this;

	do
	{
		Left += view->Left();
		Top += view->Top();
		view = view->ParentView();
	}  
	while (view && !OBJECT_DERIVEDFROM(view, KDxScreen));

	SetRect(&rc, Left, Top, Left + mWidth, Top + mHeight);
}

inline POINT KDxView::ScreenToClient(const POINT& pt)
{
	RECT rc;
	GetScreenRect(rc);
	POINT ptRet;
	ptRet.x = pt.x - rc.left;
	ptRet.y = pt.y - rc.top;
	return ptRet;
}

inline POINT KDxView::ClientToScreen(const POINT& pt)
{
	RECT rc;
	GetScreenRect(rc);
	POINT ptRet;
	ptRet.x = pt.x + rc.left;
	ptRet.y = pt.y + rc.top;
	return ptRet;
}

inline KDxView* KDxView::GetViewAtPos(POINT pt, BOOL allowDisabled)
{
	if (HitTestView(pt) && mIsVisible && (mIsEnable || allowDisabled))
	{
		POINT tmpPt;
		KDxViewVector::reverse_iterator revItr = mChildViews.rbegin();
		while (revItr != mChildViews.rend())
		{
			tmpPt = pt;
			KDxView* view = (*revItr);
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

inline KDxHitTest KDxView::HitTestView(const POINT& pt)
{
	RECT rc;
	GetClientRect(rc);
	return PtInRect(&rc, pt) ? HT_CLIENT : HT_NONE;
}

inline BOOL KDxView::IsTabStop() 
{ 
	return HAS_FLAG(mViewStyle, VS_TABSTOP); 
}

inline void KDxView::SetTabStop(BOOL isTabStop)
{
	if (isTabStop)
		ADD_FLAG(mViewStyle, VS_TABSTOP);
	else
		DEL_FLAG(mViewStyle, VS_TABSTOP);
}

inline BOOL KDxView::IsFocusable()
{
	return HAS_FLAG(mViewStyle, VS_FOCUSABLE);
}

inline void KDxView::SetFocusable(BOOL isFocusable)
{
	if (isFocusable)
		ADD_FLAG(mViewStyle, VS_FOCUSABLE);
	else
		DEL_FLAG(mViewStyle, VS_FOCUSABLE);
}

inline BOOL KDxView::IsFocused(BOOL isInWnd)
{
	if ((NULL == mOwnerWindow) || (mOwnerWindow->FocusedView() != this))
		return FALSE;

	if (!isInWnd)
		return mOwnerWindow->IsActive();
	else
		return TRUE;
}

inline BOOL KDxView::CanFocus()
{
	return IsFocusable() && VisibleAndEnable();
}

inline BOOL KDxView::SetFocus()
{
	if (NULL == mOwnerWindow)
		return FALSE;

	return mOwnerWindow->SetFocusedView(this);
}

inline BOOL KDxView::IsCaptured()
{
	if (OBJECT_DERIVEDFROM(this, KDxScreen))
		return ((KDxScreen*)this)->CaptureView() == this;
	else if (mOwnerScreen)
		return mOwnerScreen->CaptureView() == this;
	else
		return FALSE;
}

inline BOOL KDxView::CanCapture()
{
	return VisibleAndEnable();
}

inline BOOL KDxView::SetCapture()
{
	if (OBJECT_DERIVEDFROM(this, KDxScreen))
		return ((KDxScreen*)this)->SetCaptureView(this);
	else if (mOwnerScreen)		
		return mOwnerScreen->SetCaptureView(this);
	else
		return FALSE;
}

inline void KDxView::ReleaseCapture()
{
	if (IsCaptured())
	{
		if (OBJECT_DERIVEDFROM(this, KDxScreen))
			((KDxScreen*)this)->SetCaptureView(NULL);
		else if (mOwnerScreen)
			mOwnerScreen->SetCaptureView(NULL);
	}
}

inline BOOL KDxView::IsVisible() 
{ 
	return mIsVisible; 
}

inline void KDxView::SetVisible(BOOL isVisible)
{
	if (mIsVisible != isVisible)
	{
		mIsVisible = isVisible;
		UpdateVisible(isVisible);
		DoNotify(NID_VISIBLECHANGED, DWORD(mIsVisible));
	}
}

inline BOOL KDxView::IsEnable() 
{ 
	return mIsEnable; 
}

inline void KDxView::SetEnable(BOOL isEnable)
{
	if (mIsEnable != isEnable)
	{
		mIsEnable = isEnable;
		UpdateEnable(isEnable);
		DoNotify(NID_ENABLECHANGED, DWORD(mIsEnable));
	}
}

inline BOOL KDxView::IsHoverView()
{
	KASSERT(mOwnerScreen);
	return (mOwnerScreen->HoverView() == this);
}

inline int KDxView::Group()
{
	return mGroup;
}

inline void KDxView::SetGroup(int group)
{
	mGroup = group;
}

inline IDxViewEvent* KDxView::ViewEvent() 
{ 
	return mViewEvent; 
}

inline IDxViewEvent* KDxView::SetViewEvent(IDxViewEvent* event)
{
	IDxViewEvent* preEvent = mViewEvent;
	mViewEvent = event;
	return preEvent;
}

inline void KDxView::HandleMouseWheel(KDxShiftState shift, short delta, const POINT& pt)
{
	if (!DoMouseWheel(shift, delta, pt))
	{
		if (mParentView)
			mParentView->HandleMouseWheel(shift, delta, pt);
	}
}

inline LRESULT KDxView::DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
{
	if (mViewEvent)
		return mViewEvent->OnMouse(this, action, shift, pt);
	else
		return 0;
}

inline LRESULT KDxView::DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
{
	if (mViewEvent)
		return mViewEvent->OnKeyboard(this, action, key, shift);
	else
		return 0;
}

inline void KDxView::DoNotify(KDxNotifyId id, DWORD param)
{
	if (mViewEvent)
		mViewEvent->OnNotify(this, id, param);
}

inline LRESULT KDxView::DoQuery(KDxQueryId id, DWORD param)
{
	if (mViewEvent)
		return mViewEvent->OnQuery(this, id, param);
	return 0;
}

inline void KDxView::DoUpdate(DWORD tick)
{
	if (mViewEvent)
		mViewEvent->OnUpdate(this, tick);
}

inline void KDxView::DoPaint(KDxRender* render)
{
	if (mViewEvent)
		mViewEvent->OnPaint(this, render);
}

inline void KDxView::HandlePostEvent(KDxPostId id, DWORD param1, DWORD param2)
{

}

inline BOOL KDxView::DoMouseWheel(KDxShiftState shift, short delta, const POINT& pt)
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

inline void KDxView::UpdateVisible(BOOL isVisible)
{
	if (!isVisible)
	{
		ReleaseCapture();
		ResetScreenHoverView();
		if (IsFocused(TRUE))
			mOwnerWindow->SetFocusedView(NULL);
	}

	KDxViewVector::iterator itr = mChildViews.begin();
	while (itr != mChildViews.end())
	{
		(*itr)->UpdateVisible(isVisible);
		++itr;
	}

}

inline void KDxView::UpdateEnable(BOOL isEnable)
{
	if (!isEnable)
	{
		ReleaseCapture();
		ResetScreenHoverView();
		if (IsFocused(TRUE))
			mOwnerWindow->SetFocusedView(NULL);
	}

	KDxViewVector::iterator itr = mChildViews.begin();
	while (itr != mChildViews.end())
	{
		(*itr)->UpdateEnable(isEnable);
		++itr;
	}
}

inline BOOL KDxView::VisibleAndEnable()
{
	// ��������ͼ���������ÿɼ���
	KDxView* view = this;
	do
	{
		if (!(view->mIsEnable && view->mIsVisible))
			return FALSE;
		view = view->mParentView;
	} while (view);

	return TRUE;
}

inline void KDxView::FreeChilds()
{
	// �����һ����ͼ��ǰ�ͷ�
	while (!mChildViews.empty())
	{
		KDxView* childView = mChildViews.back();
		mChildViews.pop_back();
		childView->mParentView = NULL;

		FREE_VIEW(childView);
	}
}

inline void KDxView::ResetScreenHoverView()
{
	if (OBJECT_DERIVEDFROM(this, KDxScreen))
	{
		KDxScreen* screen = (KDxScreen*)this;
		if (screen->HoverView() == this)
			screen->ResetHoverView();
	}
	else if (mOwnerScreen)
	{
		if (mOwnerScreen->HoverView() == this)
			mOwnerScreen->ResetHoverView();
	}
}

inline void KDxView::DelScreenPostEvent()
{
	if (OBJECT_DERIVEDFROM(this, KDxScreen))
	{
		KDxScreen* screen = (KDxScreen*)this;
		screen->DelPostEvents(this);
	}
	else if (mOwnerScreen)
	{
		mOwnerScreen->DelPostEvents(this);
	}
}

//------------------------------------------------------------------------------
// KDxWindow

/*
	�ƶ�ʱ�õ���״̬������ٶ�ͬʱֻ��һ�����������ƶ�������С
*/
_declspec(selectany) BOOL			gIsDraging = FALSE;	// ��־�Ƿ�ʼ�϶�
_declspec(selectany) KDxHitTest		gDragHitTest;		// �����϶�ʱ�ĵ������ֵ
_declspec(selectany) POINT			gDragPoint;			// �϶�ʱ��ԭʼλ��
_declspec(selectany) int			gOrgWidth;			// �϶�ʱ�Ĵ�Сλ��
_declspec(selectany) int			gOrgHeight;
_declspec(selectany) int			gOrgLeft;
_declspec(selectany) int			gOrgTop;

IMPLEMENT_RUNTIMEINFO(KDxWindow, KDxView)

inline BOOL KDxWindow::IsMovable() 
{ 
	return HAS_FLAG(mWndStyle, WS_MOVABLE); 
}

inline void KDxWindow::SetMovable(BOOL isMovable)
{ 
	if (isMovable) 
		ADD_FLAG(mWndStyle, WS_MOVABLE);
	else 
		DEL_FLAG(mWndStyle, WS_MOVABLE); 
}

inline BOOL KDxWindow::IsSizable() 
{ 
	return HAS_FLAG(mWndStyle, WS_SIZABLE); 
}

inline void KDxWindow::SetSizable(BOOL isSizable)
{
	if (isSizable)
		ADD_FLAG(mWndStyle, WS_SIZABLE);
	else
		DEL_FLAG(mWndStyle, WS_SIZABLE);
}

inline BOOL KDxWindow::IsEscClose() 
{ 
	return HAS_FLAG(mWndStyle, WS_ESCCLOSE); 
}

inline void KDxWindow::SetEscClose(BOOL isEscClose)
{
	if (isEscClose)
		ADD_FLAG(mWndStyle, WS_ESCCLOSE);
	else
		DEL_FLAG(mWndStyle, WS_ESCCLOSE);
}

inline BOOL KDxWindow::IsTopMost() 
{ 
	return HAS_FLAG(mWndStyle, WS_TOPMOST); 
}

inline void KDxWindow::SetTopMost(BOOL isTopMose)
{
	if (isTopMose)
		ADD_FLAG(mWndStyle, WS_TOPMOST);
	else
		DEL_FLAG(mWndStyle, WS_TOPMOST);
}

inline BOOL KDxWindow::IsActivatable()
{
	return HAS_FLAG(mWndStyle, WS_ACTIVATABLE);
}

inline void KDxWindow::SetActivatable(BOOL activatable)
{
	if (activatable)
		ADD_FLAG(mWndStyle, WS_ACTIVATABLE);
	else
		DEL_FLAG(mWndStyle, WS_ACTIVATABLE);
}

inline BOOL KDxWindow::Active()
{
	return mOwnerScreen->SetActiveWindow(this);
}

inline BOOL KDxWindow::IsActive()
{
	return mOwnerScreen->ActiveWindow() == this;
}

inline BOOL KDxWindow::CanActive()
{
	return VisibleAndEnable() && IsActivatable();
}

inline KDxView* KDxWindow::FocusedView() 
{ 
	return mFocusedView; 
}

inline BOOL KDxWindow::SetFocusedView(KDxView* view)
{
	if (mFocusedView == view)
		return TRUE;

	if (view && (!view->CanFocus()))
		return FALSE;

	KDxView* oldView = mFocusedView;
	mFocusedView = view;
	if (NULL != oldView)
		oldView->DoNotify(NID_FOCUSCHANGED, FALSE);
	if (NULL != mFocusedView)
		mFocusedView->DoNotify(NID_FOCUSCHANGED, TRUE);
	return TRUE;
}

inline KDxView* KDxWindow::FindNexTabtView(KDxView* startView, BOOL isForward)
{
	KDxViewVector viewVector;
	GenTabList(viewVector, this);
	if (viewVector.empty())
		return NULL;

	KDxViewVector::iterator itr = find(viewVector.begin(), viewVector.end(), startView);
	if (viewVector.end() == itr)
	{
		if (isForward)
			itr = viewVector.end() - 1;
		else
			itr = viewVector.begin();
	}

	KDxViewVector::iterator curItr = itr;
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

inline KDxView* KDxWindow::FindNextArrowView(KDxView* startView, BOOL isForward)
{
	KDxView* parentView;
	int pos;

	if (NULL == startView)
	{
		parentView = this;
		if (parentView->ChildCount() == 0)
			return NULL;

		if (isForward)
			pos = parentView->ChildCount() - 1;
		else
			pos = 0;
	}
	else
	{
		parentView = startView->ParentView();
		if ((NULL == parentView) || OBJECT_DERIVEDFROM(parentView, KDxScreen))
			return NULL;

		pos = parentView->IndexOf(startView);
	}

	int curPos = pos;
	KDxView* view = NULL;
	do 
	{
		if (isForward)
		{
			++curPos;
			if (curPos >= parentView->ChildCount())
				curPos = 0;
		}
		else
		{
			if (curPos == 0)
				curPos = parentView->ChildCount();
			--curPos;
		}
		view = parentView->ChildView(curPos);
		if (view->CanFocus() && (view->Group() == startView->Group()))
			return view;
	} while (curPos != pos);

	return NULL;
}

inline void KDxWindow::Show(BOOL isActive)
{
	SetVisible(TRUE);
	if (isActive)
		Active();
}

inline void KDxWindow::Close()
{
	if (HAS_FLAG(mWndState, WS_MODAL))
	{
		CloseModal(0);
	}
	else 
	{
		KDxCloseMode action = (KDxCloseMode)DoQuery(QID_CLOSE, NULL);
		if (cmHide == action)
			SetVisible(FALSE);
		else if (cmFree == action)
			mOwnerScreen->PostEvent(this, PID_RELEASE, 0, 0);
	}
}

inline void KDxWindow::Hide()
{
	SetVisible(FALSE);
}

inline int KDxWindow::ShowModal()
{
	if (IsVisible() || !IsEnable() || HAS_FLAG(mWndState, WS_MODAL))
		return 0;

	gIsDraging = FALSE;
	mOwnerScreen->SetCaptureView(NULL);
	DoNotify(NID_BEGINMODAL, 0);

	KDxWindowList disableList;
	mOwnerScreen->BeginModal(this, disableList);

	ADD_FLAG(mWndState, WS_MODAL);
	Show();
	KMsgLooper* msgLooper = mOwnerScreen->MsgLooper();
	while (IsModal() && !msgLooper->IsTerm())
		msgLooper->HandleMsg();

	mOwnerScreen->EndModal(this, disableList);
	Hide();
	DoNotify(NID_ENDMODAL, 0);
	return mModalResult;
}

inline BOOL KDxWindow::CloseModal(int ret)
{
	if (!IsModal())
		return FALSE;

	KDxCloseMode action = (KDxCloseMode)DoQuery(QID_CLOSE, NULL);
	if ((cmHide == action) || (cmFree == action))
	{
		DEL_FLAG(mWndState, WS_MODAL);
		mModalResult = ret;
		if (cmFree == action)
			mOwnerScreen->PostEvent(this, PID_RELEASE, 0, 0);
	}

	return TRUE;
}

inline BOOL KDxWindow::IsModal()
{ 
	return HAS_FLAG(mWndState, WS_MODAL); 
}

inline void KDxWindow::ScreenCenter()
{
	int x = (mOwnerScreen->Width() - mWidth) / 2;
	int y = (mOwnerScreen->Height() - mHeight) / 2;
	SetPos(x, y);
}

inline BOOL KDxWindow::IsDraging()
{
	return gIsDraging;
}

inline void KDxWindow::BeginDrag(const POINT& pt)
{
	gIsDraging = TRUE;
	gDragPoint.x = mOwnerScreen->MouseX();
	gDragPoint.y = mOwnerScreen->MouseY();
	gOrgLeft = mLeft;
	gOrgTop = mTop;
	gOrgWidth = mWidth;
	gOrgHeight = mHeight;	
}

inline void KDxWindow::DragTo(const POINT& pt)
{
	if (!gIsDraging)
		return;

	int dx = mOwnerScreen->MouseX() - gDragPoint.x;
	int dy = mOwnerScreen->MouseY() - gDragPoint.y;
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

inline void KDxWindow::EndDrag(const POINT& pt)
{
	gIsDraging = FALSE;
}

inline void KDxWindow::CancelDrag()
{
	if(gIsDraging)
	{
		gIsDraging = FALSE;
		SetPos(gOrgLeft, gOrgTop);
		SetSize(gOrgWidth, gOrgHeight);
	}
}

inline void KDxWindow::Finalize()
{
	if (IsActive())
		mOwnerScreen->ActiveNextWindow(this);

	KDxView::Finalize();
}

inline LRESULT KDxWindow::DoMouse(KDxMouseAction action, KDxShiftState state, const POINT& pt)
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

	return KDxView::DoMouse(action, state, pt);
}

inline void KDxWindow::DoNotify(KDxNotifyId id, DWORD param)
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

	KDxView::DoNotify(id, param);
}

inline LRESULT KDxWindow::DoQuery(KDxQueryId id, DWORD param)
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

	return KDxView::DoQuery(id, param);
}

inline KDxHitTest KDxWindow::HitTestView(const POINT& pt)
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

	return KDxView::HitTestView(pt);
}

inline BOOL KDxWindow::PreKeyHandle(WORD& key, KDxShiftState shift)
{
	// ���ڿ�ݼ�
	KDxShortcut shortcut = MAKE_SHORTCUT(key, shift);
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

			KDxView* pNextView = FindNexTabtView(mFocusedView, !HAS_FLAG(shift, ssShift));
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

			KDxView* pNextView = FindNextArrowView(mFocusedView, (VK_RIGHT == key) || (VK_DOWN == key));
			if (NULL != pNextView)
			{
				pNextView->SetFocus();
				return TRUE;
			}			
		}
	}

	return FALSE;
}

inline void KDxWindow::HandlePostAction(KDxPostId paId, DWORD param1, DWORD param2)
{
	if (PID_RELEASE == paId)
	{
		FREE_VIEW(this);
	}
}

inline void KDxWindow::GenTabList(KDxViewVector& viewVector, KDxView* parentView)
{
	if (NULL == parentView)
		return;

	KDxView* view;
	for (int i = 0; i < parentView->ChildCount(); ++i)
	{
		view = parentView->ChildView(i);
		viewVector.push_back(view);
		GenTabList(viewVector, view);
	}
}

//------------------------------------------------------------------------------
// KDxMsgLooper

inline void KDxMsgLooper::DoIdle(BOOL& isDone)
{
	if (mScreen)
	{
		// TODO
		mScreen->Update(0);
		if (KGetTickCount() - mTick >= mScreen->FrameTime())
		{
			mScreen->Paint();
			mTick = KGetTickCount();
		}
	}
	KMsgLooper::DoIdle(isDone);
}


//------------------------------------------------------------------------------
// KDxScreen

#define KAMA_DXSCREEN_ATOM  L"Kama.DxScreen.Atom"
#define KAMA_DXSCREEN_POST L"Kama.DxScreen.PostEvent"
_declspec(selectany) UINT WM_POSTEVENT = ::RegisterWindowMessageW(KAMA_DXSCREEN_POST);

IMPLEMENT_RUNTIMEINFO(KDxScreen, KDxView)

inline void KDxScreen::SetHostWnd(HWND hwnd)
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

inline HWND KDxScreen::HostWnd() 
{ 
	return mHostWnd; 
}

inline void KDxScreen::SetRender(KDxRender* render)
{
	mRender = render;
}

inline KDxRender* KDxScreen::Render()
{
	return mRender;
}

inline void KDxScreen::SetClip(BOOL canClip)
{
	mCanClip = canClip;
}

inline BOOL KDxScreen::CanClip()
{
	return mCanClip;
}

inline KDxWindow* KDxScreen::ChildWindow(int idx)
{
	KDxView* view = ChildView(idx);
	if (view && OBJECT_DERIVEDFROM(view, KDxWindow))
		return (KDxWindow*)view;

	return NULL;
}

inline KDxWindow* KDxScreen::ActiveWindow() 
{ 
	return mActiveWindow; 
}

inline KDxView* KDxScreen::FocusedView()
{
	if (!mActiveWindow)
		return NULL;
	return mActiveWindow->FocusedView();
}

inline BOOL KDxScreen::SetActiveWindow(KDxWindow* wnd)
{
	if (wnd == mActiveWindow)
		return TRUE;

	if ((NULL != wnd) && (!wnd->CanActive()))
		return FALSE;

	KDxWindow* oldWnd = mActiveWindow;
	mActiveWindow = wnd;
	if (NULL != oldWnd)
		oldWnd->DoNotify(NID_ACTIVECHANGED, FALSE);

	if (NULL != mActiveWindow)
	{
		mActiveWindow->BringToFront();
		mActiveWindow->DoNotify(NID_ACTIVECHANGED, TRUE);

		// Ѱ�ҵ�һ�����Ի�ý������ͼ���о��轹����������û�о�����Լ�
		if (!mActiveWindow->FocusedView() || (mActiveWindow->FocusedView() == mActiveWindow))
		{
			KDxView* view = mActiveWindow->FindNexTabtView(NULL, TRUE);
			if (NULL != view)
				view->SetFocus();
			else
				mActiveWindow->SetFocus();
		}
	}

	return TRUE;
}

inline void KDxScreen::ActiveNextWindow(KDxWindow* wnd)
{
	int idx = IndexOf(wnd);
	KDxWindow* nextWnd;
	while ((idx--) >= 0)
	{
		nextWnd = ChildWindow(idx);
		if (nextWnd->Active())
			break;
	}
}

inline KDxView* KDxScreen::CaptureView() 
{ 
	return mCaptureView; 
}

inline BOOL KDxScreen::SetCaptureView(KDxView* view)
{
	if (view == mCaptureView)
		return TRUE;

	if ((NULL != view) && (!view->CanCapture()))
		return FALSE;

	KDxView* oldView = mCaptureView;
	mCaptureView = view;
	if (NULL != oldView)
		oldView->DoNotify(NID_CAPTURECHANGED, FALSE);

	if (NULL != mCaptureView)
		mCaptureView->DoNotify(NID_CAPTURECHANGED, TRUE);
	return TRUE;
}

inline HCURSOR KDxScreen::GetCursor(KDxCursorId id)
{
	KDxCursorMap::iterator itr = mCursorMap.find(id);
	if (itr == mCursorMap.end())
		return NULL;

	return itr->second;

}

inline BOOL KDxScreen::AddCursor(KDxCursorId id, HCURSOR cursor)
{
	if (id < CID_USER)
		return FALSE;

	KDxCursorMap::iterator itr = mCursorMap.find(id);
	if (itr != mCursorMap.end())
		return FALSE;

	mCursorMap.insert(std::make_pair(id, cursor));
	return TRUE;

}

inline int KDxScreen::MouseX() 
{ 
	return mMouseX; 
}

inline int KDxScreen::MouseY() 
{ 
	return mMouseY; 
}

inline BOOL KDxScreen::PostEvent(KDxView* view, KDxPostId id, DWORD param1, DWORD param2)
{
	if (!IsWindow(mHostWnd))
		return FALSE;

	if (NULL == view)
		return FALSE;

	if (0 == WM_POSTEVENT)
		return FALSE;

	KDxPostInfo Info;
	Info.view = view;
	Info.id = id;
	Info.param1 = param1;
	Info.param2 = param2;
	mPostActionList.push_back(Info);

	PostMessageW(mHostWnd, WM_POSTEVENT, 0, 0);
	return TRUE;
}

inline void KDxScreen::DelPostEvents(KDxView* view)
{
	KDxPostInfoList::iterator itr = mPostActionList.begin();
	while (itr != mPostActionList.end())
	{
		if (itr->view == view)
			itr = mPostActionList.erase(itr);
		else
			++itr;
	}
}

inline void KDxScreen::BeginModal(KDxWindow* wnd, KDxWindowList& wndList)
{
	++mModalLevel;	

	KDxWindow* childWnd;
	for (int i = 0; i < ChildCount(); ++i)
	{
		childWnd = ChildWindow(i);
		if ((wnd != childWnd) && childWnd->IsVisible() && childWnd->IsEnable())
		{
			wndList.push_back(childWnd);
			childWnd->SetEnable(FALSE);
		}
	}
}

inline void KDxScreen::EndModal(KDxWindow* wnd, KDxWindowList& wndList)
{
	--mModalLevel;

	KDxWindowList::iterator itr = wndList.begin();
	while (itr != wndList.end())
	{
		(*itr)->SetEnable(TRUE);
		++itr;
	}
	wndList.clear();

	if (0 == mModalLevel)
	{
		if (MsgLooper()->IsTerm())
			::PostQuitMessage(0);
	}
}

inline KMsgLooper* KDxScreen::MsgLooper()
{
	if (NULL == mMsgLooper)
	{
		if (NULL == mDefMsgLooper)
			mDefMsgLooper = new KDxMsgLooper(this);
		return mDefMsgLooper;
	}
	return mMsgLooper;
}

inline void KDxScreen::SetMsgLooper(KMsgLooper* msgLooper)
{
	mMsgLooper = msgLooper;
}

inline DWORD KDxScreen::FrameTime() 
{ 
	return mFrameTime; 
}

inline void KDxScreen::SetFrameTime(DWORD time) 
{ 
	mFrameTime = time; 
}

inline KDxView* KDxScreen::HoverView() 
{ 
	return mHoverView; 
}

inline void KDxScreen::ResetHoverView() 
{ 
	mHoverView = NULL; 
}

inline void KDxScreen::Paint()
{
	if(IsVisible())
	{
		KASSERT(mRender != NULL);

		DoPaint(mRender);

		RECT rcPaint;
		GetScreenRect(rcPaint);
		PaintChilds(this, rcPaint, rcPaint);
	}
}

inline void KDxScreen::Update(DWORD tick)
{
	// ����ƽ��Ƴ�
	KDxView* view = GetViewAtPos(MakePoint(mMouseX, mMouseY));
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

inline KDxView* KDxScreen::Initialize(KDxView* parentView, KDxScreen* screen)
{
	KASSERT(NULL == parentView);
	KASSERT(NULL == screen);

	InitCursorTable();

	DoInitialize();

	return this;
}

inline void KDxScreen::Finalize()
{
	KDxView::Finalize();

	DestroyAllCursor();

	if (mDefMsgLooper)
		delete mDefMsgLooper;

	SetHostWnd(NULL);
	mRender = NULL;
}

inline BOOL KDxScreen::InsertChild(KDxView* childView, int pos , BOOL isCheck)
{
	// ��Ļ������ͼһ���Ǵ���
	KASSERT(NULL != childView);
	KASSERT(OBJECT_DERIVEDFROM(childView, KDxWindow));

	return KDxView::InsertChild(childView, pos, isCheck);
}

inline KDxView* KDxScreen::GetViewAtPos(const POINT& pt, BOOL allowDisabled)
{
	if (HitTestView(pt) && mIsVisible && (mIsEnable || allowDisabled))
	{
		POINT tmpPt;
		KDxWindow* wnd;
		KDxView* view;

		// �ȱ������㴰��
		KDxViewVector::reverse_iterator revItr = mChildViews.rbegin();
		while (revItr != mChildViews.rend())
		{	wnd = OBJECT_DERIVEDFROM((*revItr), KDxWindow) ? (KDxWindow*)(*revItr) : NULL;
			if (wnd && wnd->IsTopMost())
			{
				tmpPt = pt;
				tmpPt.x -= wnd->Left();
				tmpPt.y -= wnd->Top();
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
			wnd = OBJECT_DERIVEDFROM((*revItr), KDxWindow) ? (KDxWindow*)(*revItr) : NULL;
			if (wnd && (!wnd->IsTopMost()))
			{
				tmpPt = pt;
				tmpPt.x -= wnd->Left();
				tmpPt.y -= wnd->Top();
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

inline BOOL KDxScreen::PreKeyHandle(WORD& key, KDxShiftState shift)
{
	// ȫ�ֿ�ݼ�
	KDxShortcut shortcut = MAKE_SHORTCUT(key, shift);

	if (HandleShortcut(shortcut))
		return TRUE;

	// ���ڿ�ݼ�
	if (NULL != mActiveWindow)
		return mActiveWindow->PreKeyHandle(key, shift);

	return FALSE;
}

inline void KDxScreen::InitCursorTable()
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

inline void KDxScreen::DestroyAllCursor()
{
	KDxCursorMap::iterator itr;

	for(itr = mCursorMap.begin(); itr != mCursorMap.end(); ++itr)
	{
		if (itr->first >= CID_USER)
			::DestroyCursor(itr->second);
	}
	mCursorMap.clear();
}

inline void KDxScreen::PaintChilds(KDxView* parentView, const RECT& rcParentPaint, const RECT& rcParentScreen)
{
	RECT rcChildScreen, rcChildPaint;
	int orgOffsetX, orgOffsetY;
	orgOffsetX = mRender->PaintOffsetX();
	orgOffsetY = mRender->PaintOffsetY();

	if (OBJECT_DERIVEDFROM(parentView, KDxScreen))
	{
		KDxWindow* wnd;
		KDxView* view;
		// �Ȼ��ƷǶ��㴰��
		for (int i = 0; i < parentView->ChildCount(); ++i)
		{
			view = parentView->ChildView(i);
			wnd = OBJECT_DERIVEDFROM(view, KDxWindow) ? (KDxWindow*)view : NULL;
			if (wnd->IsVisible() && (!wnd->IsTopMost()))
			{
				rcChildScreen.left = rcParentScreen.left + wnd->Left();
				rcChildScreen.top = rcParentScreen.top + wnd->Top();
				rcChildScreen.right = rcChildScreen.left + wnd->Width();
				rcChildScreen.bottom = rcChildScreen.top + wnd->Height();
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
		for (int i = 0; i < parentView->ChildCount(); ++i)
		{
			view = parentView->ChildView(i);
			wnd = OBJECT_DERIVEDFROM(view, KDxWindow) ? (KDxWindow*)view : NULL;
			if (wnd->IsVisible() && (wnd->IsTopMost()))
			{
				rcChildScreen.left = rcParentScreen.left + wnd->Left();
				rcChildScreen.top = rcParentScreen.top + wnd->Top();
				rcChildScreen.right = rcChildScreen.left + wnd->Width();
				rcChildScreen.bottom = rcChildScreen.top + wnd->Height();
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
		KDxView* view;
		for (int i = 0; i < parentView->ChildCount(); ++i)
		{
			view = parentView->ChildView(i);
			if (view->IsVisible())
			{
				rcChildScreen.left = rcParentScreen.left + view->Left();
				rcChildScreen.top = rcParentScreen.top + view->Top();
				rcChildScreen.right = rcChildScreen.left + view->Width();
				rcChildScreen.bottom = rcChildScreen.top + view->Height();
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

inline void KDxScreen::UpdateChilds(KDxView* parentView, DWORD tick)
{
	KDxView* childView;
	for (int i = 0; i < parentView->ChildCount(); ++i)
	{
		childView = parentView->ChildView(i);
		if (childView->IsVisible())
		{
			childView->DoUpdate(tick);
			UpdateChilds(childView, tick);
		}
	}
}

inline void KDxScreen::SubclassWindow(HWND hwnd)
{
	// һ������ֻ����һ��Screen����
	HANDLE handle = GetPropW(hwnd, KAMA_DXSCREEN_ATOM);
	KASSERT(handle == NULL);

	mDefHostWndProc = (WNDPROC)::SetWindowLongW(hwnd, GWL_WNDPROC, (LONG)&StdWndProc);
	KASSERT(NULL != mDefHostWndProc);

	::SetPropW(hwnd, KAMA_DXSCREEN_ATOM, (HANDLE)this);
}

inline void KDxScreen::UnsubclassWindow(HWND hwnd)
{
	KASSERT(NULL != mDefHostWndProc);

	::RemovePropW(hwnd, KAMA_DXSCREEN_ATOM);
	::SetWindowLongW(hwnd, GWL_WNDPROC, (LONG)mDefHostWndProc);
}

inline LRESULT CALLBACK	KDxScreen::StdWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT lRet = 0;
	BOOL bDone = FALSE;
	KDxScreen* screen = NULL;

	screen = (KDxScreen*)::GetPropW(hwnd, KAMA_DXSCREEN_ATOM);
	if (NULL != screen)
		bDone = screen->WndProc(hwnd, msg, wparam, lparam, lRet);

	if (bDone)
		return lRet;
	else
		return CallWindowProcW(screen->mDefHostWndProc, hwnd, msg, wparam, lparam);
}

inline BOOL KDxScreen::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& lRet)
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

	return FALSE;
}

inline LRESULT KDxScreen::DefWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{ 
	return ::CallWindowProc(mDefHostWndProc, hwnd, msg, wparam, lparam); 
}

inline void KDxScreen::WMMouseMove(WPARAM wparam, LPARAM lparam)
{
	mMouseX = X_OF_LPARAM(lparam);
	mMouseY = Y_OF_LPARAM(lparam);

	KDxShiftState shift = VKeyToShiftState((WORD)wparam);
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
		KDxView* view = GetViewAtPos(pt, FALSE);
		if (NULL != view)
		{
			pt = view->ScreenToClient(pt);
			view->DoMouse(maMouseMove, shift, pt);
		}
	}								   
}

inline void KDxScreen::WMSize(WPARAM wparam, LPARAM lparam)
{
	SetPos(0, 0);
	SetSize(X_OF_LPARAM(lparam), Y_OF_LPARAM(lparam));
}

inline void KDxScreen::WMLButtonDblClk(WPARAM wparam, LPARAM lparam)
{
	KDxShiftState shift = VKeyToShiftState((WORD)wparam);
	POINT pt;

	pt = SmallPtToPoint(MAKEPOINTS(lparam));
	KDxView* view = GetViewAtPos(pt, FALSE);

	if (NULL != view)	
	{
		// �ȼ������
		SetActiveWindow(view->OwnerWindow());

		// �ٲ�������Լ���������¼�
		SetCaptureView(view);
		if (NULL != mCaptureView)
		{	
			pt = mCaptureView->ScreenToClient(pt);
			mCaptureView->DoMouse(maLButtonDblClk, shift, pt);
			mCaptureView->DoMouse(maLButtonDown, shift, pt);
			// ����
			if (!OBJECT_DERIVEDFROM(mCaptureView, KDxWindow))
				mCaptureView->SetFocus();
		}
	}
}

inline void KDxScreen::WMLButtonDown(WPARAM wparam, LPARAM lparam)
{
	KDxShiftState shift = VKeyToShiftState((WORD)wparam);
	POINT pt;

	pt = SmallPtToPoint(MAKEPOINTS(lparam));
	KDxView* view = GetViewAtPos(pt, FALSE);

	if (NULL != view)	
	{
		// �ȼ������
		SetActiveWindow(view->OwnerWindow());

		// �ٲ�������Լ���������¼�
		SetCaptureView(view);
		if (NULL != mCaptureView)
		{	
			pt = mCaptureView->ScreenToClient(pt);
			mCaptureView->DoMouse(maLButtonDown, shift, pt);
			// ����
			if (!OBJECT_DERIVEDFROM(mCaptureView, KDxWindow))
				mCaptureView->SetFocus();
		}
	}
}

inline void KDxScreen::WMLButtonUp(WPARAM wparam, LPARAM lparam)
{
	KDxShiftState shift = VKeyToShiftState((WORD)wparam);
	POINT pt;
	pt = SmallPtToPoint(MAKEPOINTS(lparam));

	KDxView* view = mCaptureView;
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

inline void KDxScreen::WMCancelMode(WPARAM wparam, LPARAM lparam)
{
	SetCaptureView(NULL);
	if (mActiveWindow)
		mActiveWindow->CancelDrag();
}

inline void KDxScreen::WMOtherMouse(KDxMouseAction action, WPARAM wparam, LPARAM lparam)
{
	KDxShiftState shift = VKeyToShiftState((WORD)wparam);
	POINT pt;

	pt = SmallPtToPoint(MAKEPOINTS(lparam));
	KDxView* view = GetViewAtPos(pt, FALSE);
	if (NULL != view)
	{	
		pt = view->ScreenToClient(pt);
		view->DoMouse(action, shift, pt);
	}
}

inline void KDxScreen::WMKeyDown(WPARAM wparam, LPARAM lparam)
{
	KDxShiftState shift = KeyDataToShiftState((WORD)lparam);
	WORD key = LOWORD(wparam);

	// �ȴ����ݼ���Tab���������
	if (!PreKeyHandle(key, shift))
	{
		if (NULL != mActiveWindow)
		{
			KDxView* view = mActiveWindow->FocusedView();
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

inline void KDxScreen::WMKeyMsg(KDxKeyAction action, WPARAM wparam, LPARAM lparam)
{
	KDxShiftState shift = KeyDataToShiftState((WORD)lparam);
	WORD key = LOWORD(wparam);

	if (NULL != mActiveWindow)
	{
		KDxView* view = mActiveWindow->FocusedView();
		if (NULL == view) 
			view = mActiveWindow;
		view->DoKeyboard(action, key, shift);
	}
	else
	{
		DoKeyboard(action, key, shift);
	}	
}

inline void KDxScreen::WMMouseWheel(WPARAM wparam, LPARAM lparam)
{
	KDxView* view = FocusedView();
	if (view)
	{
		short delta = (short)HIWORD(wparam);
		KDxShiftState shift = VKeyToShiftState(LOWORD(wparam));
		POINT pt = SmallPtToPoint(MAKEPOINTS(lparam));
		::ScreenToClient(mHostWnd, &pt);
		pt = view->ScreenToClient(pt);
		view->HandleMouseWheel(shift, delta, pt);
	}
}

inline void KDxScreen::WMContextMenu(WPARAM wparam, LPARAM lparam)
{
	POINTS pts = MAKEPOINTS(lparam);	
	if ((pts.x == -1) && (pts.y == -1))
	{
		// ���ɼ�������Ļ����˵��¼�
		if (mActiveWindow)
		{
			KDxView* view = mActiveWindow->FocusedView();
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
		KDxView* view = GetViewAtPos(pt);
		if (NULL != view)
			view->DoNotify(NID_CONTEXATMENU, (DWORD)&pt);
	}
}

inline void KDxScreen::WMPostAction(WPARAM wparam, LPARAM lparam)
{
	KDxPostInfoList::iterator itr = mPostActionList.begin();
	if (itr != mPostActionList.end())
	{
		KDxPostInfo Info = *itr;
		mPostActionList.pop_front();
		if (Info.view)
			Info.view->HandlePostEvent(Info.id, Info.param1, Info.param2);
	}
}

inline BOOL KDxScreen::WMSetCursor(WPARAM wparam, LPARAM lparam)
{
	if (LOWORD(lparam) == HT_CLIENT)
	{
		POINT pt = MakePoint(mMouseX, mMouseY);
		KDxView* view = GetViewAtPos(pt);
		if (view)
		{
			pt = view->ScreenToClient(pt);
			KDxHitTest htValue = view->HitTestView(pt);
			if (view->DoQuery(QID_SETCURSOR, htValue))
				return TRUE;			
		}
	}
	return FALSE;
}

}

#endif // __KAMA_KMDXVIEWS_H__