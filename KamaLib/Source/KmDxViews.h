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
enum KMouseAction
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
// #define MACTION_MOVE		1	// �ƶ�
// #define MACTION_LDOWN		2	// �������
// #define MACTION_RDOWN		3	// �Ҽ�����
// #define MACTION_MDOWN		4	// �м�����
// #define MACTION_LUP			5	// �������
// #define MACTION_RUP			6	// �Ҽ�����
// #define MACTION_MUP			7	// �м�����
// #define MACTION_LDBLCLK		8	// ���˫��
// #define MACTION_RDBLCLK		9	// �Ҽ�˫��
// #define MACTION_MDBLCLK		10	// �м�˫��
// #define MACTION_CLICK		11	// ���

/*
	���̶�������
*/
enum KKeyAction
{
	kaDown,						// ���̻���
	kaChar,						// �����ַ�
	kaUp						// ���̵���
};
// typedef DWORD KeyAction;
// #define KACTION_DOWN		1	// ���̻���
// #define KACTION_CHAR		2	// �����ַ�
// #define KACTION_UP		3	// ���̵���

/*
	Shift״̬
*/
enum KShiftState
{
	ssShift,					// Shift������
	ssAlt,						// Alt������
	ssCtrl						// Ctrl������
};
// typedef WORD ShiftState;
// #define SSTATE_SHIFT	0x01	// Shift������
// #define SSTATE_ALT		0x02	// Alt������
// #define SSTATE_CTRL		0x04	// Ctrl������

/*
	֪ͨ�¼�����, 0~NT_USER-1�ɿ�ܱ������û�����ʹ��������ֵ
*/
typedef DWORD KNotifyType;
#define NT_USER						0x10000
// �ߴ����ڸı䣬dwParam: SIZE*=��Ҫ�ı�ĳߴ磬���Ըı����ֵ��Ӱ�����յĳߴ�
#define ntSizeChanging				1
// �ߴ�ı䣬dwParam: NULL
#define ntSizeChanged				2
// λ�����ڸı䣬dwParam: POINT*=��Ҫ�ı�ĳߴ磬���Ըı����ֵ��Ӱ�����յ�λ��
#define ntPosChanging				3
// λ�øı䣬dwParam: NULL
#define ntPosChanged				4
// ����ı�	 dwParam:  0=������, !0=����
#define ntActiveChanged				5
// ����ı�, dwParam:��0=ʧȥ����, !0=��ý���
#define ntFocusChanged				6
// ��겶����ͼ�ı䣬dwParam:��0=ʧȥ����, !0=��ò���
#define ntCaptureChnaged			7
// �ɼ��ı䣬dwParam:��0=����, !0=�ɼ�
#define ntVisibleChanged			8
// ���øı䣬dwParam:��0=����, !0=����
#define ntEnableChanged				9
// ����(�Ҽ�)�˵��¼���dwParam��NULL=�ɼ��������, PPOINT=���������ģ�PPOINTָ�������VScreen��λ��
#define ntContextMenu				10
// ��ʼShowModal, dwParam: NULL
#define ntBeginModal				11
// ����ShowModal, dwParam: NULL
#define ntEndModal					12
// ��������ͼ, dwParam: NULL
#define ntMouseEnter				13
// ����˳���ͼ, dwParam: NULL
#define ntMouseLeave				14
// �ı��ı䣬 dwParam: NULL
#define ntTextChanged				15
	
// typedef DWORD NotifyType;
// // �ߴ�ı䣬dwParam: NULL
// #define NTYPE_SIZECHANGED			1	
// 	// λ�øı䣬dwParam: NULL
// #define NTYPE_POSCHANGED			2	
// 	// ����ı�	 dwParam:  0=������, !0=����
// #define NTYPE_ACTIVECHANGED			3	
// 	// ����ı�, dwParam:��0=ʧȥ����, !0=��ý���
// #define NTYPE_FOCUSCHANGED			4	
// 	// ��겶����ͼ�ı䣬dwParam:��0=ʧȥ����, !0=��ò���
// #define NTYPE_CAPTRURECHANGED		5   
// 	// �ɼ��ı䣬dwParam:��0=����, !0=�ɼ�
// #define NTYPE_VISIBLECHANGED		6	
// 	// ���øı䣬dwParam:��0=����, !0=����
// #define NTYPE_ENBALECHANGED			7	
// 	// ����(�Ҽ�)�˵��¼���dwParam��NULL=�ɼ��������, PPOINT=���������ģ�PPOINTָ�������VScreen��λ��
// #define NTYPE_CONTEXTMENU			8
// 	// λ�����ڸı䣬dwParam: POINT*=��Ҫ�ı�ĳߴ磬���Ըı����ֵ��Ӱ�����յ�λ��
// #define NTYPE_POSCHINGING			9
// 	// �ߴ����ڸı䣬dwParam: SIZE*=��Ҫ�ı�ĳߴ磬���Ըı����ֵ��Ӱ�����յĳߴ�
// #define NTYPE_SIZECHANGING			10
// 	// ��ʼShowModal, dwParam: NULL
// #define NTYPE_BEGINMODAL			11
// 	// ����ShowModal, dwParam: NULL
// #define NTYPE_ENDMODAL				12
// 	// ��������ͼ, dwParam: NULL
// #define NTYPE_MOUSEENTER			13
// 	// ����˳���ͼ, dwParam: NULL
// #define NTYPE_MOUSELEAVE			14
// 	// �ı��ı䣬 dwParam: NULL
// #define NTYPE_TEXTCHANGED			15

/*
	�����¼�����, 0~QT_USER-1�ɿ�ܱ������û�����ʹ��������ֵ
*/
typedef DWORD KQueryType;
#define QT_USER						0x10000
// �Ƿ�Ҫ����TAB������ֵ��0=������!0=����
#define qtWantTab					1
// �Ƿ�Ҫ�������������ֵ��0=������!0=���� 
#define qtWantArrows				2
// �Ƿ�Ҫ����ESC��������ֵ��0=������!0=���� 
#define qtWantEscape				3
// ���ڹر����󣬷���ֵ��CloseAction
#define qtClose						4
// ���ù�꣬������ dwParam=�������ֵ������ֵ��!0=���óɹ�
#define qtSetCursor					5
// �����¼�����
// typedef DWORD QueryType;
// �Ƿ�Ҫ����TAB������ֵ��0=������!0=����
// #define QTYPE_WANTTAB				1	
// 	// �Ƿ�Ҫ�������������ֵ��0=������!0=���� 
// #define QTYPE_WANTARROWS			2	
// 	// �Ƿ�Ҫ����ESC��������ֵ��0=������!0=���� 
// #define QTYPE_WANTESCAPE			3
// 	// ���ڹر����󣬷���ֵ��CloseAction
// #define QTYPE_CLOSE					4
// 	// ���ù�꣬������ dwParam=�������ֵ������ֵ��!0=���óɹ�
// #define QTYPE_SETCURSOR				5

/*
	��ͼ�������ֵ0~oxHTV_USER-1�ɿ�ܱ������û�����ʹ������ֵ
*/
typedef DWORD KHitTestValue;
#define HTV_USER					0x10000
#define htvNone						0	  // ʲôҲû�У�͸��
#define htvCient					1	  // ������ͻ���
#define htvBorderLeft				2	  // ��Ե��
#define htvBorderTop				3	  // ��Ե��
#define htvBorderRight				4	  // ��Ե��
#define htvBorderBottom				5	  // ��Ե��
#define htvBorderLeftTop			6	  // ��Ե��
#define htvBorderLeftBottom			7	  // ��Ե���
#define htvBorderRightTop			8	  // ��Ե�Ҷ�
#define htvBorderRightBottom		9	  // ��Ե�ҵ�
#define htvMoveRegion				10	  // ���ƶ�����

// 	// ��ͼ�������ֵ
// 	// ע�⣺0x0~HTVALUE_USER-1 �ι����ʹ�ã��ⲿ������ HTVALUE_USER~0xFFFFFFFF ������������ֵ
// 	typedef DWORD HitTestValue;
// #define HTVALUE_USER				0x10000
// #define HTVALUE_NONE				0x0000	// ʲôҲû�У�͸��
// #define HTVALUE_CLIENT				0x0001	// ������ͻ���
// #define HTVALUE_BORDER_LEFT			0x0002	// ��Ե��
// #define HTVALUE_BORDER_TOP			0x0003	// ��Ե��
// #define HTVALUE_BORDER_RIGHT		0x0004	// ��Ե��
// #define HTVALUE_BORDER_BOTTOM		0x0005	// ��Ե��
// #define HTVALUE_BORDER_LEFTTOP		0x0006	// ��Ե��
// #define HTVALUE_BORDER_LEFTBOTTOM	0x0007	// ��Ե���
// #define HTVALUE_BORDER_RIGHTTOP		0x0008	// ��Ե�Ҷ�
// #define HTVALUE_BORDER_RIGHTBOTTOM	0x0009	// ��Ե�ҵ�
// #define HTVALUE_MOVE_REGION			0x000A  // ���ƶ�����

/*
	�������, 0~CT_USER-1�ɿ�ܱ������û�����ʹ������ֵ
*/
typedef DWORD KCursorType;
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
// 	// �������
// 	// ע�⣺0x0~VIDC_USER-1 �ι����ʹ�ã��ⲿ������ VIDC_USER~0xFFFFFFFF �������Լ��Ĺ������
// 	typedef DWORD CursorType;
// #define VIDC_USER			0x10000
// #define VIDC_ARROW			0x0001	// Ĭ�Ϲ��
// #define VIDC_IBEAM			0x0002	// ����, �Ƶ��༭��ʱ���Ǹ���״
// #define VIDC_WAIT           0x0003	// ɳ©, �ȴ����
// #define VIDC_CROSS          0x0004	// ʮ�ֹ��
// #define VIDC_UPARROW        0x0005	// ���ϼ�ͷ
// #define VIDC_SIZENWSE       0x0006	// ����--���� �����ͷ  "\"
// #define VIDC_SIZENESW       0x0007	// ����--���� �����ͷ	"/"
// #define VIDC_SIZEWE         0x0008	// ���� �����ͷ "-"
// #define VIDC_SIZENS         0x0009	// �ϱ� �����ͷ "|"
// #define VIDC_SIZEALL        0x000A	// �ƶ�, ʮ�ּ�ͷ
// #define VIDC_NO             0x000B	// ԲȦ��б��, ��ʾ�ܾ�
// #define VIDC_APPSTARTING    0x000C	// ����ɳ©, ��ʾ������������
// #define VIDC_HELP			0x000D	// �����ʺ�, ��ʾ����
// #define VIDC_HAND			0x000E	// ��ָ���

/*
	��ͼ���
*/
typedef DWORD KViewStyle;
#define vtTopStop			0x01		// Tabֹͣ���뽹���ϵ����
// 	// ��ͼ���
// typedef DWORD ViewStyle;
// #define VSTYLE_TABSTOP		0x1		// Tabֹͣ���뽹���ϵ����

/*
	���ڷ��
*/
typedef DWORD KWindowStyle;
#define wsMovable			0x01   // ���ƶ�
#define wsSizable			0x02   // ���϶�
#define wsTopMost			0x04   // ���㴰��
#define wsEscHide			0x08   // ESC�ر�
// 	// ���ڷ��
// 	typedef DWORD WindowStyle;
// #define WSTYLE_MOVABLE		0x1		// ���ƶ�
// #define WSTYLE_SIZABLE		0x2		// ���϶�
// #define WSTYLE_TOPMOST		0x4		// ���㴰��
// #define WSTYLE_ESCHIDE		0x8		// ESC�ر�

/*
	����״̬
*/
typedef DWORD KDxWndState;
#define wsModal				0x01	// ģ̬״̬
// 	// ����״̬
// 	typedef DWORD WindowState;
// #define WSTATE_MODAL		0x1		// ģ̬״̬

	typedef DWORD ButtonState;
#define BS_NORMAL			0x1		// ��ͨ״̬
#define BS_OVER				0x2		// ��꾭��ʱ��״̬
#define BS_DOWN				0x3		// ����ʱ��״̬


	// ���ڹرն���
	typedef DWORD CloseAction;
#define CACTION_HIDE		0x00	// ����
#define CACTION_FREE		0x01	// �ͷ�
#define CACTION_NONE		0x02	// ʲô��Ҳ����

	// ��ݼ�ֵ								  
	typedef DWORD Shortcut;

	// ����ID
	typedef DWORD PostActionID;
	// �ͷ��Լ����޲���
#define PAID_RELEASE		1

}
#endif // __KAMA_KMDXVIEWS_H__