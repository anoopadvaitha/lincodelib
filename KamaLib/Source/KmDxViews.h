/***********************************************************
  Filename:	KmDxViews.h
  Author:	Tramper
  Email:	linzhenqun@gmail.com
  Date:		2009/12/19

  Brief:    ����KamaLib������һ���֣���Tramper������ά����
			��Ȩû�У�������ʹ�ã�
***********************************************************/
#ifndef __KAMA_KMDXVIEWS_H__
#define __KAMA_KMDXVIEWS_H__
#include "KmCommons.h"

/*=======================================================================
  ˵��: Dx��ͼ�ؼ���
========================================================================*/
namespace kama
{

/*
	��궯������
*/
enum KDxMouseAction
{
	maMove,						// �ƶ�
	maLDown,					// �������
	maRDown,					// �Ҽ�����
	maMDown,					// �м�����
	maLUp,						// �������
	maRUp,						// �Ҽ�����
	maMUp,						// �м�����
	maLDblClk,					// ���˫��
	maRDblClk,					// �Ҽ�˫��
	maMDblClk,					// �м�˫��
	maClick						// ���
};					

/*
	���̶�������
*/
enum KDxKeyAction
{
	kaDown,						// ���̻���
	kaChar,						// �����ַ�
	kaUp						// ���̵���
};

/*
	Shift״̬
*/
enum KDxShiftState
{
	ssShift,					// Shift������
	ssAlt,						// Alt������
	ssCtrl						// Ctrl������
};

/*
	֪ͨ�¼�����, 0~NT_USER-1�ɿ�ܱ������û�����ʹ��������ֵ
*/
typedef DWORD KDxNotifyType;
#define NT_USER						0x10000
// �ߴ����ڸı䣬param: SIZE*=��Ҫ�ı�ĳߴ磬���Ըı����ֵ��Ӱ�����յĳߴ�
#define ntSizeChanging				1
// �ߴ�ı䣬param: NULL
#define ntSizeChanged				2
// λ�����ڸı䣬param: POINT*=��Ҫ�ı�ĳߴ磬���Ըı����ֵ��Ӱ�����յ�λ��
#define ntPosChanging				3
// λ�øı䣬param: NULL
#define ntPosChanged				4
// ����ı�	 param:  0=������, !0=����
#define ntActiveChanged				5
// ����ı�, param:��0=ʧȥ����, !0=��ý���
#define ntFocusChanged				6
// ��겶����ͼ�ı䣬param:��0=ʧȥ����, !0=��ò���
#define ntCaptureChnaged			7
// �ɼ��ı䣬param:��0=����, !0=�ɼ�
#define ntVisibleChanged			8
// ���øı䣬param:��0=����, !0=����
#define ntEnableChanged				9
// ����(�Ҽ�)�˵��¼���param��NULL=�ɼ��������, PPOINT=���������ģ�PPOINTָ�������VScreen��λ��
#define ntContextMenu				10
// ��ʼShowModal, param: NULL
#define ntBeginModal				11
// ����ShowModal, param: NULL
#define ntEndModal					12
// ��������ͼ, param: NULL
#define ntMouseEnter				13
// ����˳���ͼ, param: NULL
#define ntMouseLeave				14
// �ı��ı䣬 param: NULL
#define ntTextChanged				15

/*
	�����¼�����, 0~QT_USER-1�ɿ�ܱ������û�����ʹ��������ֵ
*/
typedef DWORD KDxQueryType;
#define QT_USER						0x10000
// �Ƿ�Ҫ����TAB������ֵ��0=������!0=����
#define qtWantTab					1
// �Ƿ�Ҫ�������������ֵ��0=������!0=���� 
#define qtWantArrows				2
// �Ƿ�Ҫ����ESC��������ֵ��0=������!0=���� 
#define qtWantEscape				3
// ���ڹر����󣬷���ֵ��CloseAction
#define qtClose						4
// ���ù�꣬������ param=�������ֵ������ֵ��!0=���óɹ�
#define qtSetCursor					5

/*
	��ͼ�������ֵ0~oxHT_USER-1�ɿ�ܱ������û�����ʹ������ֵ
*/
typedef DWORD KDxHitTest;
#define HT_USER					0x10000
#define htNone						0	  // ʲôҲû�У�͸��
#define htCient						1	  // ������ͻ���
#define htBorderLeft				2	  // ��Ե��
#define htBorderTop					3	  // ��Ե��
#define htBorderRight				4	  // ��Ե��
#define htBorderBottom				5	  // ��Ե��
#define htBorderLeftTop				6	  // ��Ե��
#define htBorderLeftBottom			7	  // ��Ե���
#define htBorderRightTop			8	  // ��Ե�Ҷ�
#define htBorderRightBottom			9	  // ��Ե�ҵ�
#define htMoveRegion				10	  // ���ƶ�����

/*
	�������, 0~CT_USER-1�ɿ�ܱ������û�����ʹ������ֵ
*/
typedef DWORD KDxCursorType;
#define ctUSER				0x10000
#define ctArrow				1		// Ĭ�Ϲ��
#define ctIbeam				2		// ����, �Ƶ��༭��ʱ���Ǹ���״
#define ctWait				3		// ɳ©, �ȴ����
#define ctCross				4		// ʮ�ֹ��
#define ctUpArrow			5		// ���ϼ�ͷ
#define ctSizeNWSE			6		// ����--���� �����ͷ  "\"
#define ctSizeNESW			7		// ����--���� �����ͷ	"/"
#define ctSIzeWE			8		// ���� �����ͷ "-"
#define ctSizeNS			9		// �ϱ� �����ͷ "|"
#define ctSizeAll			10		// �ƶ�, ʮ�ּ�ͷ
#define ctNo				11		// ԲȦ��б��, ��ʾ�ܾ�
#define ctAppStarting		12		// ����ɳ©, ��ʾ������������
#define ctHelp				13		// �����ʺ�, ��ʾ����
#define ctHand				14		// ��ָ���

/*
	��ͼ���
*/
typedef DWORD KDxViewStyle;
#define vsTabStop			0x01		// Tabֹͣ���뽹���ϵ����

/*
	���ڷ��
*/
typedef DWORD KDxWindowStyle;
#define wsMovable			0x01   // ���ƶ�
#define wsSizable			0x02   // ���϶�
#define wsTopMost			0x04   // ���㴰��
#define wsEscHide			0x08   // ESC�ر�

/*
	����״̬
*/
typedef DWORD KDxWndState;
#define wsModal				0x01	// ģ̬״̬

enum KDxButtonState
{
	bsNormal,						// ��ͨ״̬
	bsOver,							// ��꾭��ʱ��״̬
	bsDown							// ����ʱ��״̬
};

/*
	���ڹرն���
*/
enum KDxCloseAction
{
	caHide,							// ����
	caFree,							// �ͷ�
	caNone							// ʲô��Ҳ����
};

/*
	��ݼ�ֵ								  
*/
typedef DWORD Shortcut;

/*
	Ͷ���¼�ID
*/
typedef DWORD KDxPostId;
#define piRelease			1		// �ͷ��Լ����޲���

class KDxScreen;
class KDxWindow;
class KDxView;

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
	theclass Ҫ��������ͼ��
	parenview ����ͼ������ΪNULL
	ownerscreen ��������Ļ�ࣺKDxScreen
*/
#define NEW_VIEW(theclass, parenview, ownerscreen)\
	(theclass*)((new theclass)->Initialize(parenview, ownerscreen));

/*
	����ú��ͷ�һ����ͼ
*/
#define FREE_VIEW(theobject)\
	do{\
		theobject->Finalize();\
		delete theobject;\
	}while(0)


/*
	��ͼ��
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
	KDxScreen* OwnerScreen() 
	{ 
		return mOwnerScreen; 
	}

	/*
		ȡ����������
	*/
	KDxWindow* OwnerWindow() 
	{ 
		return mOwnerWindow; 
	}

	/*
		ȡ����ͼ
	*/
	KDxView* ParentView() 
	{ 
		return mParentView; 
	}

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
	int	ChildCount() 
	{ 
		return (int)mChildViews.size(); 
	}

	/*
		ȡ����ͼ��Ϊ�ӿ��ٶȣ����жϱ߽�
	*/
	KDxView* ChildView(int idx) 
	{ 
		return mChildViews[idx]; 
	}

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
	int	 UserData() 
	{ 
		return mUserData; 
	}

	/*
		���û�����
	*/
	void SetUserData(int data) 
	{ 
		mUserData = data; 
	}

	/*
		��
	*/
	int  Left() 
	{ 
		return mLeft; 
	}

	/*
		��
	*/
	int  Top() 
	{ 
		return mTop; 
	}

	/*
		��
	*/
	int  Width() 
	{ 
		return  mWidth; 
	}

	/*
		��
	*/
	int  Height() 
	{ 
		return mHeight; 
	}

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
	SIZE MinSize() 
	{ 
		return mMinSize; 
	}

	/*
		ȡ���ߴ�
	*/
	SIZE MaxSize() 
	{ 
		return mMaxSize; 
	}

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
	virtual KDxHitTest HitTestView(POINT pt);

	/*
		Tab����λ
	*/
	BOOL IsTabStop() 
	{ 
		return HAS_FLAG(mViewStyle, vsTabStop); 
	}

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
	BOOL IsVisible() 
	{ 
		return mIsVisible; 
	}

	/*
		��ɼ���
	*/
	void SetVisible(BOOL isVisible);

	/*
		�Ƿ����
	*/
	BOOL IsEnable() 
	{ 
		return mIsEnable; 
	}

	/*
		���ÿ�����
	*/
	void SetEnable(BOOL isEnable);

	/*
		ȡ��ͼ�¼��ӿ�
	*/
	IDxViewEvent* isEnable() { return mViewEvent; }

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
	void DelScreenPostAction();

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
IMPLEMENT_RUNTIMEINFO(KDxView, KObject)

}

#endif // __KAMA_KMDXVIEWS_H__