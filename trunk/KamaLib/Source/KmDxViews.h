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

namespace kama
{

//////////////////////////////////////////////////////////////////////////
// declare

/*
	��궯������
*/
enum KDxMouseAction
{
	maMove,							// �ƶ�
	maLDown,						// �������
	maRDown,						// �Ҽ�����
	maMDown,						// �м�����
	maLUp,							// �������
	maRUp,							// �Ҽ�����
	maMUp,							// �м�����
	maLDblClk,						// ���˫��
	maRDblClk,						// �Ҽ�˫��
	maMDblClk,						// �м�˫��
	maClick							// ���
};					

/*
	���̶�������
*/
enum KDxKeyAction
{
	kaDown,							// ���̻���
	kaChar,							// �����ַ�
	kaUp							// ���̵���
};

/*
	Shift״̬
*/
typedef DWORD KDxShiftState;
#define ssShift						0x01	// Shift������
#define ssAlt						0x02	// Alt������
#define ssCtrl						0x04	// Ctrl������

/*
	֪ͨ�¼�����, 0~NT_USER-1�ɿ�ܱ������û�����ʹ��������ֵ
*/
typedef DWORD KDxNotifyType;
#define NT_USER						0x10000
/*
	�ߴ����ڸı䣬param: SIZE*=��Ҫ�ı�ĳߴ磬���Ըı����ֵ��Ӱ�����յĳߴ�
*/
#define ntSizeChanging				1
/*
	�ߴ�ı䣬param: NULL
*/
#define ntSizeChanged				2
/*
	λ�����ڸı䣬param: POINT*=��Ҫ�ı�ĳߴ磬���Ըı����ֵ��Ӱ�����յ�λ��
*/
#define ntPosChanging				3
/*
	λ�øı䣬param: NULL
*/
#define ntPosChanged				4
/*
	����ı�	 param:  0=������, !0=����
*/
#define ntActiveChanged				5
/*
	����ı�, param:��0=ʧȥ����, !0=��ý���
*/
#define ntFocusChanged				6
/*
	��겶����ͼ�ı䣬param:��0=ʧȥ����, !0=��ò���
*/
#define ntCaptureChnaged			7
/*
	�ɼ��ı䣬param:��0=����, !0=�ɼ�
*/
#define ntVisibleChanged			8
/*
	���øı䣬param:��0=����, !0=����
*/
#define ntEnableChanged				9
/*
	����(�Ҽ�)�˵��¼���param��NULL=�ɼ��������, PPOINT=���������ģ�PPOINTָ�������screen��λ��
*/
#define ntContextMenu				10
/*
	��ʼShowModal, param: NULL
*/
#define ntBeginModal				11
/*
	����ShowModal, param: NULL
*/
#define ntEndModal					12
/*
	��������ͼ, param: NULL
*/
#define ntMouseEnter				13
/*
	����˳���ͼ, param: NULL
*/
#define ntMouseLeave				14
/*
	�ı��ı䣬 param: NULL
*/
#define ntTextChanged				15


/*
	�����¼�����, 0~QT_USER-1�ɿ�ܱ������û�����ʹ��������ֵ
*/
typedef DWORD KDxQueryType;
#define QT_USER						0x10000
/*
	�Ƿ�Ҫ����TAB������ֵ��0=������!0=����
*/
#define qtWantTab					1
/*
	�Ƿ�Ҫ�������������ֵ��0=������!0=���� 
*/
#define qtWantArrows				2
/*
	�Ƿ�Ҫ����ESC��������ֵ��0=������!0=���� 
*/
#define qtWantEscape				3
/*
	���ڹر����󣬷���ֵ��KDxCloseMode
*/
#define qtClose						4
/*
	���ù�꣬������ param=�������ֵ������ֵ��!0=���óɹ�
*/
#define qtSetCursor					5


/*
	��ͼ�������ֵ0~oxHT_USER-1�ɿ�ܱ������û�����ʹ������ֵ
*/
typedef DWORD KDxHitTest;
#define HT_USER						0x10000
#define htNone						0		// ʲôҲû�У�͸��
#define htClient					1		// ������ͻ���
#define htBorderLeft				2		// ��Ե��
#define htBorderTop					3		// ��Ե��
#define htBorderRight				4		// ��Ե��
#define htBorderBottom				5		// ��Ե��
#define htBorderLeftTop				6		// ��Ե��
#define htBorderLeftBottom			7		// ��Ե���
#define htBorderRightTop			8		// ��Ե�Ҷ�
#define htBorderRightBottom			9		// ��Ե�ҵ�
#define htMoveRegion				10		// ���ƶ�����

/*
	�������, 0~CT_USER-1�ɿ�ܱ������û�����ʹ������ֵ
*/
typedef DWORD KDxCursorType;
#define CT_USER						0x10000
#define ctArrow						1		// Ĭ�Ϲ��
#define ctIbeam						2		// ����, �Ƶ��༭��ʱ���Ǹ���״
#define ctWait						3		// ɳ©, �ȴ����
#define ctCross						4		// ʮ�ֹ��
#define ctUpArrow					5		// ���ϼ�ͷ
#define ctSizeNWSE					6		// ����--���� �����ͷ  "\"
#define ctSizeNESW					7		// ����--���� �����ͷ	"/"
#define ctSizeWE					8		// ���� �����ͷ "-"
#define ctSizeNS					9		// �ϱ� �����ͷ "|"
#define ctSizeAll					10		// �ƶ�, ʮ�ּ�ͷ
#define ctNo						11		// ԲȦ��б��, ��ʾ�ܾ�
#define ctAppStarting				12		// ����ɳ©, ��ʾ������������
#define ctHelp						13		// �����ʺ�, ��ʾ����
#define ctHand						14		// ��ָ���

/*
	��ͼ���
*/
typedef DWORD KDxViewStyle;
#define vsTabStop					0x01	// Tabֹͣ���뽹���ϵ����

/*
	���ڷ��
*/
typedef DWORD KDxWndStyle;
#define wsMovable					0x01	// ���ƶ�
#define wsSizable					0x02	// ���϶�
#define wsTopMost					0x04	// ���㴰��
#define wsEscClose					0x08	// ESC�ر�

/*
	����״̬
*/
typedef DWORD KDxWndState;
#define wsModal						0x01	// ģ̬״̬

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
/*
	���ɿ�ݼ�����
*/
#define MAKE_SHORTCUT(key, shift) (KDxShortcut)MAKELONG(key, shift)
/*
	�ӿ�ݼ�ȡ�����
*/
#define VKEY_OF_SHORTCUT(shortcut) LOWORD(shortcut)
/*
	�ӿ�ݼ�ȡ��Shift״̬
*/
#define SHIFT_OF_SHORTCUT(shortcut) (KDxShiftState)HIWORD(shortcut)


/*
	Ͷ���¼�ID
*/
typedef DWORD KDxPostId;
#define piRelease					1		// �ͷ��Լ����޲���

class KDxView;
class KDxWindow;
class KDxScreen;

/*
	��ͼ�¼��ӿ�
*/
interface IDxViewEvent
{
	/*
		����¼������£������ƶ�
	*/
	virtual void OnMouse(KDxView* view, KDxMouseAction action, KDxShiftState shift, const POINT& pt) 
	{

	}

	/*
		�����¼������£������ַ�
	*/
	virtual void OnKeyboard(KDxView* view, KDxKeyAction action, WORD& key, KDxShiftState shift) 
	{

	}

	/*
		ͨ�õ�֪ͨ�¼���KDxNotifyTypeָ��֪ͨ������, paramΪ�����Ĳ���
	*/
	virtual void OnNotify(KDxView* view, KDxNotifyType type, DWORD param) 
	{

	}

	/*
		ͨ�õ������¼���KDxQueryTypeָ�����������, paramΪ�����Ĳ���
	*/
	virtual LRESULT OnQuery(KDxView* view, KDxQueryType type, DWORD param) 
	{ 
		return 0; 
	}

	/*
		�����¼�
	*/
	virtual void OnUpdate(KDxView* view) 
	{

	}

	/*
		�����¼��� 
		rcPaint	Ҫ���Ƶ�����Ϊ��Ļ����
		rcScreen ��ͼ����Ļ�е�����
	*/
	virtual void OnPaint(KDxView* view, const RECT& rcPaint, const RECT& rcScreen) 
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
		mViewStyle(vsTabStop),
		mUserData(0), 
		mLeft(0), 
		mTop(0), 
		mWidth(100), 
		mHeight(60), 
		mIsVisible(TRUE), 
		mIsEnable(TRUE)
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
	void ClientRect(RECT& rc);

	/*
		ȡ����ͼ��ռ����Ļ����
	*/
	void ScreenRect(RECT& rc);

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
		ȡ��ͼ�¼��ӿ�
	*/
	IDxViewEvent* ViewEvent(); 

	/*
		����ͼ�¼��ӿڣ�������ǰһ���¼��ӿ�
	*/
	IDxViewEvent* SetViewEvent(IDxViewEvent* event);

public:
	/*
		����¼�
	*/
	virtual void DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt);

	/*
		�����¼�
	*/
	virtual void DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift);

	/*
		֪ͨ�¼�
	*/
	virtual void DoNotify(KDxNotifyType type, DWORD param);

	/*
		�����¼�
	*/
	virtual LRESULT DoQuery(KDxQueryType type, DWORD param);

	/*
		����
	*/
	virtual void DoUpdate();

	/*
		���ƣ�������ʲô����
	*/
	virtual void DoPaint(const RECT& rcPaint, const RECT& rcScreen);

	/*
		����Ͷ���¼�
	*/
	virtual void HandlePostEvent(KDxPostId id, DWORD param1, DWORD param2);

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
	SIZE				mMinSize;			// ��С�ߴ磬Ϊ0��ʾ������
	SIZE				mMaxSize;			// ���ߴ磬Ϊ0��ʾ������
	BOOL				mIsVisible;			// �ɼ�
	BOOL				mIsEnable;			// ����
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
		mWndStyle(wsMovable | wsSizable),
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

	virtual void DoMouse(KDxMouseAction action, KDxShiftState state, const POINT& pt);

	virtual void DoNotify(KDxNotifyType type, DWORD param);

	virtual LRESULT DoQuery(KDxQueryType type, DWORD param);

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
	*.  ��ʱ���ºͻ��ƣ�һ������Ϣѭ����Idleʱ��
		screen->Update();
		// �������Ҫ����֡��
		screen->Screen();
	*.  �ͷ�
		FREE_SCREEN(screen);
		
*/
class KDxScreen: public KDxView, public KDxShortcutMgr
{
	typedef std::map<KDxCursorType, HCURSOR> KDxCursorMap;
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
		mHoverView(NULL)
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
		ȡĳ���Ӵ���
	*/
	KDxWindow* ChildWindow(int idx);

	/*
		ȡ�����
	*/
	KDxWindow* ActiveWindow(); 

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
		ȡtype���͵Ĺ��
	*/
	HCURSOR GetCursor(KDxCursorType type);

	/*
		����Զ�����
	*/
	BOOL AddCursor(KDxCursorType type, HCURSOR cursor);

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
	virtual void Update();

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
	void UpdateChilds(KDxView* parentView);

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
	void WMLButtonDown(WPARAM wparam, LPARAM lparam);
	void WMLButtonUp(WPARAM wparam, LPARAM lparam);
	void WMCancelMode(WPARAM wparam, LPARAM lparam);
	void WMOtherMouse(KDxMouseAction action, WPARAM wparam, LPARAM lparam);
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

	return this;
}

inline void KDxView::Finalize()
{
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
	DoNotify(ntSizeChanging, (DWORD)(&sz));

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
	DoNotify(ntSizeChanged, NULL);
}

inline void KDxView::SetPos(int left, int top)
{
	if ((left == mLeft) && (top == mTop))
		return;

	POINT pt;
	pt.x = left;
	pt.y = top;
	DoNotify(ntPosChanging, (DWORD)(&pt));

	mLeft = pt.x;
	mTop = pt.y;
	DoNotify(ntPosChanged, NULL);
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

inline void KDxView::ClientRect(RECT& rc)
{
	SetRect(&rc, 0, 0, mWidth, mHeight);
}

inline void KDxView::ScreenRect(RECT& rc)
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
	ScreenRect(rc);
	POINT ptRet;
	ptRet.x = pt.x - rc.left;
	ptRet.y = pt.y - rc.top;
	return ptRet;
}

inline POINT KDxView::ClientToScreen(const POINT& pt)
{
	RECT rc;
	ScreenRect(rc);
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
	ClientRect(rc);
	return PtInRect(&rc, pt) ? htClient : htNone;
}

inline BOOL KDxView::IsTabStop() 
{ 
	return HAS_FLAG(mViewStyle, vsTabStop); 
}

inline void KDxView::SetTabStop(BOOL isTabStop)
{
	if (isTabStop)
		ADD_FLAG(mViewStyle, vsTabStop);
	else
		DEL_FLAG(mViewStyle, vsTabStop);
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
	return VisibleAndEnable();
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
		DoNotify(ntVisibleChanged, DWORD(mIsVisible));
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
		DoNotify(ntEnableChanged, DWORD(mIsEnable));
	}
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

inline void KDxView::DoMouse(KDxMouseAction action, KDxShiftState shift, const POINT& pt)
{
	if (mViewEvent)
		mViewEvent->OnMouse(this, action, shift, pt);
}

inline void KDxView::DoKeyboard(KDxKeyAction action, WORD& key, KDxShiftState shift)
{
	if (mViewEvent)
		mViewEvent->OnKeyboard(this, action, key, shift);
}

inline void KDxView::DoNotify(KDxNotifyType type, DWORD param)
{
	if (mViewEvent)
		mViewEvent->OnNotify(this, type, param);
}

inline LRESULT KDxView::DoQuery(KDxQueryType type, DWORD param)
{
	if (mViewEvent)
		return mViewEvent->OnQuery(this, type, param);
	return 0;
}

inline void KDxView::DoUpdate()
{
	if (mViewEvent)
		mViewEvent->OnUpdate(this);
}

inline void KDxView::DoPaint(const RECT& rcPaint, const RECT& rcScreen)
{
	if (mViewEvent)
		mViewEvent->OnPaint(this, rcPaint, rcScreen);
}

inline void KDxView::HandlePostEvent(KDxPostId id, DWORD param1, DWORD param2)
{

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
	return HAS_FLAG(mWndStyle, wsMovable); 
}

inline void KDxWindow::SetMovable(BOOL isMovable)
{ 
	if (isMovable) 
		ADD_FLAG(mWndStyle, wsMovable);
	else 
		DEL_FLAG(mWndStyle, wsMovable); 
}

inline BOOL KDxWindow::IsSizable() 
{ 
	return HAS_FLAG(mWndStyle, wsSizable); 
}

inline void KDxWindow::SetSizable(BOOL isSizable)
{
	if (isSizable)
		ADD_FLAG(mWndStyle, wsSizable);
	else
		DEL_FLAG(mWndStyle, wsSizable);
}

inline BOOL KDxWindow::IsEscClose() 
{ 
	return HAS_FLAG(mWndStyle, wsEscClose); 
}

inline void KDxWindow::SetEscClose(BOOL isEscClose)
{
	if (isEscClose)
		ADD_FLAG(mWndStyle, wsEscClose);
	else
		DEL_FLAG(mWndStyle, wsEscClose);
}

inline BOOL KDxWindow::IsTopMost() 
{ 
	return HAS_FLAG(mWndStyle, wsTopMost); 
}

inline void KDxWindow::SetTopMost(BOOL isTopMose)
{
	if (isTopMose)
		ADD_FLAG(mWndStyle, wsTopMost);
	else
		DEL_FLAG(mWndStyle, wsTopMost);
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
	return VisibleAndEnable();
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
		oldView->DoNotify(ntFocusChanged, FALSE);
	if (NULL != mFocusedView)
		mFocusedView->DoNotify(ntFocusChanged, TRUE);
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
		if (view->CanFocus())
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
	if (HAS_FLAG(mWndState, wsModal))
	{
		CloseModal(0);
	}
	else 
	{
		KDxCloseMode action = (KDxCloseMode)DoQuery(qtClose, NULL);
		if (cmHide == action)
			SetVisible(FALSE);
		else if (cmFree == action)
			mOwnerScreen->PostEvent(this, piRelease, 0, 0);
	}
}

inline void KDxWindow::Hide()
{
	SetVisible(FALSE);
}

inline int KDxWindow::ShowModal()
{
	if (IsVisible() || !IsEnable() || HAS_FLAG(mWndState, wsModal))
		return 0;

	gIsDraging = FALSE;
	mOwnerScreen->SetCaptureView(NULL);
	DoNotify(ntBeginModal, 0);

	KDxWindowList disableList;
	mOwnerScreen->BeginModal(this, disableList);

	ADD_FLAG(mWndState, wsModal);
	Show();
	KMsgLooper* msgLooper = mOwnerScreen->MsgLooper();
	while (IsModal() && !msgLooper->IsTerm())
		msgLooper->HandleMsg();

	mOwnerScreen->EndModal(this, disableList);
	Hide();
	DoNotify(ntEndModal, 0);
	return mModalResult;
}

inline BOOL KDxWindow::CloseModal(int ret)
{
	if (!IsModal())
		return FALSE;

	KDxCloseMode action = (KDxCloseMode)DoQuery(qtClose, NULL);
	if ((cmHide == action) || (cmFree == action))
	{
		DEL_FLAG(mWndState, wsModal);
		mModalResult = ret;
		if (cmFree == action)
			mOwnerScreen->PostEvent(this, piRelease, 0, 0);
	}

	return TRUE;
}

inline BOOL KDxWindow::IsModal()
{ 
	return HAS_FLAG(mWndState, wsModal); 
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
	if (gDragHitTest == htMoveRegion)
	{
		SetPos(dx + gOrgLeft, dy + gOrgTop);
	}
	else if (gDragHitTest == htBorderLeft)
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
	else if (gDragHitTest == htBorderTop)
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
	else if (gDragHitTest == htBorderLeftTop)
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
	else if (gDragHitTest == htBorderRight)
	{
		if ((mMaxSize.cx != 0) && (gOrgWidth + dx > mMaxSize.cx))
			SetSize(mMaxSize.cx, mHeight);
		else if ((mMinSize.cx != 0) && (gOrgWidth + dx < mMinSize.cx))
			SetSize(mMinSize.cx, mHeight);
		else
			SetSize(gOrgWidth + dx, mHeight);
	}
	else if (gDragHitTest == htBorderRightTop)
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
	else if (gDragHitTest == htBorderBottom)
	{
		if ((mMaxSize.cy != 0) && (gOrgHeight + dy > mMaxSize.cy))
			SetSize(mWidth, mMaxSize.cy);
		else if ((mMinSize.cy != 0) && (gOrgHeight + dy < mMinSize.cy))
			SetSize(mWidth, mMinSize.cy);
		else
			SetSize(mWidth, gOrgHeight + dy);
	}
	else if (gDragHitTest == htBorderLeftBottom)
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
	else if (gDragHitTest == htBorderRightBottom)
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

inline void KDxWindow::DoMouse(KDxMouseAction action, KDxShiftState state, const POINT& pt)
{
	// ������봦���϶�
	if (action == maMove)	
	{
		// ����ƶ��������϶�������
		if (IsDraging())
			DragTo(pt);
	}
	else if (action == maLDown)
	{
		// �����£��϶���ʼ
		gDragHitTest = HitTestView(pt);
		if ((gDragHitTest == htMoveRegion) || 
			((gDragHitTest >= htBorderLeft) && (gDragHitTest <= htBorderRightBottom)))
			BeginDrag(pt);
	}
	else if (action = maLUp)
	{
		// ��굯���϶�����
		EndDrag(pt);
	}

	KDxView::DoMouse(action, state, pt);
}

inline void KDxWindow::DoNotify(KDxNotifyType type, DWORD param)
{
	if (type == ntVisibleChanged)
	{
		// ���ؼ�����һ������
		if (!param)
		{
			if (IsActive())
				mOwnerScreen->ActiveNextWindow(this);
		}
	}
	else if (type == ntActiveChanged)
	{
		// ���轹�㣬�����ж���û�н�����ͼ���о��轹����������û�о�
		// Ѱ�ҵ�һ�����Ի�ý������ͼ���о��轹����������û�о�
		// ����Լ�
		if ((NULL != mFocusedView) && (mFocusedView != this))
			mFocusedView->SetFocus();
		else
		{
			KDxView* view = FindNexTabtView(NULL, TRUE);
			if (NULL != view)
				view->SetFocus();
			else
				SetFocus();
		}
	}

	KDxView::DoNotify(type, param);
}

inline LRESULT KDxWindow::DoQuery(KDxQueryType type, DWORD param)
{
	if (qtSetCursor == type)
	{
		HCURSOR hc = NULL;
		if (param == htBorderLeft)
			hc = mOwnerScreen->GetCursor(ctSizeWE);
		else if (param == htBorderRight)
			hc = mOwnerScreen->GetCursor(ctSizeWE);
		else if (param == htBorderTop)
			hc = mOwnerScreen->GetCursor(ctSizeNS);
		else if (param == htBorderBottom)
			hc = mOwnerScreen->GetCursor(ctSizeNS);
		else if (param == htBorderLeftTop)
			hc = mOwnerScreen->GetCursor(ctSizeNWSE);
		else if (param == htBorderRightBottom)
			hc = mOwnerScreen->GetCursor(ctSizeNWSE);
		else if (param == htBorderRightTop)
			hc = mOwnerScreen->GetCursor(ctSizeNESW);
		else if (param == htBorderLeftBottom)
			hc = mOwnerScreen->GetCursor(ctSizeNESW);
		else if (param == htMoveRegion)
			hc = mOwnerScreen->GetCursor(ctArrow);

		if (hc)
		{
			::SetCursor(hc);
			return TRUE;
		}
	}

	return KDxView::DoQuery(type, param);
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
			return htBorderRightBottom;

		::SetRect(&rc, 0, mHeight - mFrameSize - 4, mFrameSize + 4, mHeight);
		if (::PtInRect(&rc, pt))
			return htBorderLeftBottom;

		::SetRect(&rc, mWidth - mFrameSize - 4, 0, mWidth, mFrameSize + 4);
		if (::PtInRect(&rc, pt))
			return htBorderRightTop;

		::SetRect(&rc, 0, 0, mFrameSize + 4, mFrameSize + 4);
		if (::PtInRect(&rc, pt))
			return htBorderLeftTop;

		::SetRect(&rc, mWidth - mFrameSize, mFrameSize, mWidth, mHeight - mFrameSize);
		if (::PtInRect(&rc, pt))
			return htBorderRight;

		::SetRect(&rc, 0, mFrameSize, mFrameSize, mHeight - mFrameSize);
		if (::PtInRect(&rc, pt))
			return htBorderLeft;

		::SetRect(&rc, mFrameSize, mHeight - mFrameSize, mWidth - mFrameSize, mHeight);
		if (::PtInRect(&rc, pt))
			return htBorderBottom;

		::SetRect(&rc, mFrameSize, 0, mWidth - mFrameSize, mFrameSize);
		if (::PtInRect(&rc, pt))
			return htBorderTop;
	}

	if (IsMovable())
	{
		ClientRect(rc);
		return ::PtInRect(&rc, pt) ? htMoveRegion : htNone;
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
					if (mFocusedView->DoQuery(qtWantEscape, NULL))
						return FALSE;

				Close();
				return TRUE;
			}
		}
		if ((VK_TAB == key) && (!HAS_FLAG(shift, ssCtrl)))
		{
			if (mFocusedView && mFocusedView->DoQuery(qtWantTab, NULL))
				return FALSE;

			KDxView* pNextView = FindNexTabtView(mFocusedView, !HAS_FLAG(shift, ssShift));
			if (NULL != pNextView)
				pNextView->SetFocus();
			return TRUE;
		}
		else if ((VK_LEFT <= key) && (key <= VK_DOWN))
		{
			if (mFocusedView && mFocusedView->DoQuery(qtWantArrows, NULL))
				return FALSE;

			KDxView* pNextView = FindNextArrowView(mFocusedView, (VK_RIGHT == key) || (VK_DOWN == key));
			if (NULL != pNextView)
				pNextView->SetFocus();
			return TRUE;
		}
	}

	return FALSE;
}

inline void KDxWindow::HandlePostAction(KDxPostId paId, DWORD param1, DWORD param2)
{
	if (piRelease == paId)
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
		mScreen->Update();
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
		GetClientRect(mHostWnd, &rc);
		SetPos(0, 0);
		SetSize(rc.right, rc.bottom);
	}
}

inline HWND KDxScreen::HostWnd() 
{ 
	return mHostWnd; 
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

inline BOOL KDxScreen::SetActiveWindow(KDxWindow* wnd)
{
	if (wnd == mActiveWindow)
		return TRUE;

	if ((NULL != wnd) && (!wnd->CanActive()))
		return FALSE;

	KDxWindow* oldWnd = mActiveWindow;
	mActiveWindow = wnd;
	if (NULL != oldWnd)
		oldWnd->DoNotify(ntActiveChanged, FALSE);

	if (NULL != mActiveWindow)
	{
		mActiveWindow->BringToFront();
		mActiveWindow->DoNotify(ntActiveChanged, TRUE);
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
		oldView->DoNotify(ntCaptureChnaged, FALSE);

	if (NULL != mCaptureView)
		mCaptureView->DoNotify(ntCaptureChnaged, TRUE);
	return TRUE;
}

inline HCURSOR KDxScreen::GetCursor(KDxCursorType type)
{
	KDxCursorMap::iterator itr = mCursorMap.find(type);
	if (itr == mCursorMap.end())
		return NULL;

	return itr->second;

}

inline BOOL KDxScreen::AddCursor(KDxCursorType type, HCURSOR cursor)
{
	if (type < CT_USER)
		return FALSE;

	KDxCursorMap::iterator itr = mCursorMap.find(type);
	if (itr != mCursorMap.end())
		return FALSE;

	mCursorMap.insert(std::make_pair(type, cursor));
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
		RECT rcPaint;
		ScreenRect(rcPaint);
		DoPaint(rcPaint, rcPaint);
		PaintChilds(this, rcPaint, rcPaint);
	}
}

inline void KDxScreen::Update()
{
	// ����ƽ��Ƴ�
	KDxView* view = GetViewAtPos(MakePoint(mMouseX, mMouseY));
	if (view != mHoverView)
	{
		if(mHoverView)
			mHoverView->DoNotify(ntMouseLeave, NULL);
		mHoverView = view;
		if (mHoverView)
			mHoverView->DoNotify(ntMouseEnter, NULL);
	}

	if (IsVisible())
	{
		DoUpdate();
		UpdateChilds(this);
	}
}

inline KDxView* KDxScreen::Initialize(KDxView* parentView, KDxScreen* screen)
{
	KASSERT(NULL == parentView);
	KASSERT(NULL == screen);

	InitCursorTable();

	return this;
}

inline void KDxScreen::Finalize()
{
	KDxView::Finalize();

	DestroyAllCursor();

	if (mDefMsgLooper)
		delete mDefMsgLooper;

	SetHostWnd(NULL);
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
	mCursorMap.insert(std::make_pair(ctArrow,		::LoadCursor(NULL, IDC_ARROW)));
	mCursorMap.insert(std::make_pair(ctIbeam,		::LoadCursor(NULL, IDC_IBEAM)));
	mCursorMap.insert(std::make_pair(ctWait,		::LoadCursor(NULL, IDC_WAIT)));
	mCursorMap.insert(std::make_pair(ctCross,		::LoadCursor(NULL, IDC_CROSS)));
	mCursorMap.insert(std::make_pair(ctUpArrow,		::LoadCursor(NULL, IDC_UPARROW)));
	mCursorMap.insert(std::make_pair(ctSizeNWSE,	::LoadCursor(NULL, IDC_SIZENWSE)));
	mCursorMap.insert(std::make_pair(ctSizeNESW,	::LoadCursor(NULL, IDC_SIZENESW)));
	mCursorMap.insert(std::make_pair(ctSizeWE,		::LoadCursor(NULL, IDC_SIZEWE)));
	mCursorMap.insert(std::make_pair(ctSizeNS,		::LoadCursor(NULL, IDC_SIZENS)));
	mCursorMap.insert(std::make_pair(ctSizeAll,		::LoadCursor(NULL, IDC_SIZEALL)));
	mCursorMap.insert(std::make_pair(ctNo,			::LoadCursor(NULL, IDC_NO)));
	mCursorMap.insert(std::make_pair(ctAppStarting,	::LoadCursor(NULL, IDC_APPSTARTING)));
	mCursorMap.insert(std::make_pair(ctHelp,		::LoadCursor(NULL, IDC_HELP)));
	mCursorMap.insert(std::make_pair(ctHand,		::LoadCursor(NULL, IDC_HAND)));
}

inline void KDxScreen::DestroyAllCursor()
{
	KDxCursorMap::iterator itr;

	for(itr = mCursorMap.begin(); itr != mCursorMap.end(); ++itr)
	{
		if (itr->first >= CT_USER)
			::DestroyCursor(itr->second);
	}
	mCursorMap.clear();
}

inline void KDxScreen::PaintChilds(KDxView* parentView, const RECT& rcParentPaint, const RECT& rcParentScreen)
{
	RECT rcChildScreen, rcChildPaint;

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
					wnd->DoPaint(rcChildPaint, rcChildScreen);
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
					wnd->DoPaint(rcChildPaint, rcChildScreen);
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
					view->DoPaint(rcChildPaint, rcChildScreen);
					PaintChilds(view, rcChildPaint, rcChildScreen);
				}
			}
		}
	}
}

inline void KDxScreen::UpdateChilds(KDxView* parentView)
{
	KDxView* childView;
	for (int i = 0; i < parentView->ChildCount(); ++i)
	{
		childView = parentView->ChildView(i);
		if (childView->IsVisible())
		{
			childView->DoUpdate();
			UpdateChilds(childView);
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
		WMKeyMsg(kaUp, wparam, lparam);
	}
	else if (WM_CHAR == msg)
	{
		WMKeyMsg(kaChar, wparam, lparam);
	}
	else if (WM_MOUSEMOVE == msg)
	{
		WMMouseMove(wparam, lparam);
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
		WMOtherMouse(maLDblClk, wparam, lparam);
	}
	else if (WM_RBUTTONDBLCLK == msg)
	{
		WMOtherMouse(maRDblClk, wparam, lparam);
	}
	else if (WM_MBUTTONDBLCLK == msg)
	{
		WMOtherMouse(maMDblClk, wparam, lparam);
	}
	else if (WM_RBUTTONDOWN == msg)
	{
		WMOtherMouse(maRDown, wparam, lparam);
	}
	else if (WM_RBUTTONUP == msg)
	{
		WMOtherMouse(maRUp, wparam, lparam);
	}
	else if (WM_MBUTTONDOWN == msg)
	{
		WMOtherMouse(maMDown, wparam, lparam);
	}
	else if (WM_MBUTTONUP == msg)
	{
		WMOtherMouse(maMUp, wparam, lparam);
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
		mCaptureView->DoMouse(maMove, shift, pt);
	}
	else
	{
		pt = SmallPtToPoint(MAKEPOINTS(lparam));
		KDxView* view = GetViewAtPos(pt, FALSE);
		if (NULL != view)
		{
			pt = view->ScreenToClient(pt);
			view->DoMouse(maMove, shift, pt);
		}
	}								   
}

inline void KDxScreen::WMSize(WPARAM wparam, LPARAM lparam)
{
	SetPos(0, 0);
	SetSize(X_OF_LPARAM(lparam), Y_OF_LPARAM(lparam));
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
			mCaptureView->DoMouse(maLDown, shift, pt);
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
		view->ClientRect(rc);
		if (::PtInRect(&rc, pt))
			view->DoMouse(maClick, shift, pt);

		// ����
		view->DoMouse(maLUp, shift, pt);
	}
	else
	{
		view = GetViewAtPos(pt, FALSE);
		if (NULL != view)
		{
			pt = view->ScreenToClient(pt);
			view->DoMouse(maLUp, shift, pt);
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
			view->DoKeyboard(kaDown, key, shift);
		}
		else
		{
			DoKeyboard(kaDown, key, shift);
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
			view->DoNotify(ntContextMenu, NULL);
		}
	}
	else
	{
		// �����������Ļ����˵��¼�
		POINT pt = SmallPtToPoint(pts);
		::ScreenToClient(mHostWnd, &pt);
		KDxView* view = GetViewAtPos(pt);
		if (NULL != view)
			view->DoNotify(ntContextMenu, (DWORD)&pt);
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
	if (LOWORD(lparam) == HTCLIENT)
	{
		POINT pt = MakePoint(mMouseX, mMouseY);
		KDxView* view = GetViewAtPos(pt);
		if (view)
		{
			pt = view->ScreenToClient(pt);
			KDxHitTest htValue = view->HitTestView(pt);
			if (view->DoQuery(qtSetCursor, htValue))
				return TRUE;			
		}
	}
	return FALSE;
}

}

#endif // __KAMA_KMDXVIEWS_H__