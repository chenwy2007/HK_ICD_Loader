#pragma  once

#include "define_base.h"
 
#define LOGNAME_HK_DataProcessSys  "DataProcessSys"
//
#define WM_MSG_WND_ATTR  WM_USER +1009

#define WM_MSG_OPEN_DLG  WM_USER +1010

#define WM_MSG_TEST_TIP  WM_USER +1011

#define WM_MSG_NEW_DUT_TIP	  WM_USER +1012

#define WM_MSG_CUR_POINT	 WM_USER +1013

//
#define IDD_DIALOG_BASE                 129
#define IDD_DIALOG_BASE_CHILD           130

#define WND_type_dlg	0
#define WND_type_button 1
#define WND_type_static 2
 
#define WND_type_group	3
#define WND_type_edit	4
#define WND_type_switch_static 5
#define WND_type_swtich_button 6
#define WND_type_tab	7

//
#define WND_type_switch_static_image	8
#define WND_type_switch_button_image	9

#define WND_type_switch_static_rectangle 10
 

//��������ֵ����ʾͼƬ
#define WND_type_Image_Interval 11

//���οؼ�
#define WND_type_Rectangle_Name "���οؼ�"
#define WND_type_Rectangle   12

//Բ�οؼ�
#define WND_type_Circle_Name "Բ�οؼ�"
#define WND_type_Circle   13

//���οؼ�
#define WND_type_Line_Name "���οؼ�"
#define WND_type_Line	14


//�ؼ�V2.0

#define WND_type_image_button_V02  20
#define WND_type_image_button_V02_Name "�����ɢ��_V2"

#define WND_type_image_Led_V02  21
#define WND_type_image_Led_V02_Name "������ɢ��_V2"

#define WND_type_image_Rotate_V02  22
#define WND_type_image_Rotate_V02_Name "ģ��������(��ť)_V2"

#define WMD_type_digital 23			// �������ֶε���ʽ��ʾ����
#define WND_type_digital_Name "�����ı���"



#define ACT_Show_dlg	"Show_Dlg"
#define ACT_Exit_Sys	"Exit_Sys"
 

#define  BASE_NUM  1000
//�ؼ��¼�

//��ʼ��
#define EVENT_TYPE_INIT  0
//�ر��¼�
#define EVENT_TYPE_CLOSE 1
//����¼�
#define EVENT_TYPE_CLICK 2
//ˢ���¼�
#define EVENT_TYPE_REFRESH 3


/*
�������ݶ���������� 
���룬�����PWM����
*/
#define DATA_INOUT_TYPE_IN  1
#define DATA_INOUT_TYPE_OUT  2


struct __baseAttr
{
	__baseAttr()
	{
		sName="";
		nCtrlIndex=0;
		nX =0;
		nY =0;
		nW =0;
		nH =0;

	}
	string sName;
	int nCtrlIndex;
	int nX;
	int nY;
	int nW;
	int nH;
};

struct __CtrlInfo
{	
	 string sName;
	 int nId;
	 int nIndex;		
	 int nInterfaceId; ///-1,�������ڣ�-2,�����������ڣ����ⲿ���� 
	 int nType;   //0�����ڣ�1��button��2��static 3,GROUP, 4: edit ,5:  Switch Static
	 int nX;
	 int nY;
	 int nW;
	 int nH;
	 string sTitle;
	 string sAttr;	 
	 int nSharedId;  //�����ڴ�ֵ

	 string sReserve1;
	 string sReserve2;

};

typedef struct tagTBLDATA
{
	int nId;						// ���
	string sName;					// ����������
	string sUnit;					// ��λ��V �� A)
	float fValue;					// ����ֵ
	int nPrecision;					// С����λ��
	string sRef;					// �ο�ֵ�����۱�׼��
	string sStatus;					// ״̬
	float fMinV;					// ���۱�׼��Сֵ
	float fMaxV;					// ���۱�׼���ֵ
	int nSharedId;					// �����ڴ�ID
	int nDlgId;						// �����ĶԻ���ID
}TBLDATA,*pTBLDATA;

typedef struct tagBOARDCONFIG
{
	int nId;						// ���
	string sSignalName;				// �ź�����
	//int nBusNumber;					// ���ߺ�
	//int nDeviceNumber;				// �豸��
	//int nDriveId;					// ������ţ�ϵͳ�еĵڼ��������忨��
	int nChannelIndex;				// ͨ����
	float fCoefficient;				// ϵ��
	float fOffset;					// ƫ��
	int nDlgId;						// ϵͳID
	int nShareId;					// �����ڴ�
}BOARDCONFIG,*pBOARDCONFIG;

struct __SysInfo
{
	int nId;
	string sName;
	string sKey; 
};

//����ϵͳ��Ϣ
struct __TestSysInfo
{
	int nId;
	string sName;
	string sMark;
};
//����ϵͳ������Ϣ
struct __TestSysInfo_UI
{	
	int nSysId;
	int nDlgId;
	string sName;
	int nType;
	string sMark;
};
struct __DLGInfo
{
	int nId;
	int nSysId;
	string sInfo;
}; 
#define  DLG_STYLE_POPOP  0
#define  DLG_STYLE_CHILD  1
struct __DLGAttr
{
	__DLGAttr()
	{
		nDutIndex =-1;
		nTimer =-1;
	}
	int nCtrlIndex;
	string sName; //�������ƣ���ע
	int nStyle ; //0:������1���Ӵ���	
	string sTitle;
	int nW;		//��
	int nH;		//��
	int nX;		//��
	int nY;		//��
	COLORREF backColor;//����ɫ

	//����������Ч
	int nTitleH ;			//����߶� 
	COLORREF colorTitle;	//���ⱳ��ɫ 	
	COLORREF colorFont;		//������ͼ����ɫ 
	int nFontSize;			//���������С 
	
	int nDutIndex;//���ڶ�Ӧ�Ĳ�Ʒ���,
	int nTimer;//����ˢ��ʱ��;

	//״̬��
 	
};
struct __BT_ACT
{
	string sName;
	string sParam;
};
struct __ButtonAttr
{
	__ButtonAttr()
	{
		nCtrlIndex =0;
	}

	int nCtrlIndex;
	string sName;
	string sTitle;
	int nX;
	int nY;
	int nW;
	int nH;
	int nFontSize;
	COLORREF fontColor;
	COLORREF backColor;
	COLORREF lineColor;

	vector<__BT_ACT>actVec;
};


struct __StaticAttr
{
	__StaticAttr()
	{
		nCtrlIndex=0;
		sName="";
	}
	int nCtrlIndex;
	string sName;
	string sTitle;		//����
	int nX;
	int nY;
	int nW;
	int nH;
	string sFontName;
	bool bBackColor;	//�Ƿ��б���ɫ
	COLORREF backColor; //����ɫ
	COLORREF fontColor; //������ɫ
	int nFontSize;		//�����С
	int nAlign;			//DT_LEFT DT_CENTER, DT_RIGHT	
};

struct __DigitalStaticAttr
{
	__DigitalStaticAttr()
	{
		nCtrlIndex=0;
		nSharedId =0;
		sName="";
		nCtrlType = 0;
		sRange = "";
		sDispContend = "";
	}
	int nCtrlIndex;
	string sName;
	string sTitle;		//����
	int nX;
	int nY;
	int nW;
	int nH;
	string sFontName;
	bool bBackColor;	//�Ƿ��б���ɫ
	COLORREF backColor; //����ɫ
	COLORREF fontColor; //������ɫ
	int nFontSize;		//�����С
	int nAlign;			//DT_LEFT DT_CENTER, DT_RIGHT	
	int nSharedId;
	string sDispContend;
	string sRange;
	int nCtrlType;
};

struct __SwitchStaticAttr
{
	int nCtrlIndex;
	string sName;
	string sTitle;
	int nX;
	int nY;
	int nW;
	int nH;
	
	bool bBackColor;
	COLORREF backColor;
	COLORREF lineColor;
	map<int,COLORREF>valuesMap;
	int nDefaultValue;
	int nSharedId;
};

struct __GroupAttr
{
	__GroupAttr()
	{
		sName="";
		nCtrlIndex =0;
		sTitle="";
		nX =0;
		nY =0;
		nW =0;
		nH =0;
		sFontName ="����";
		fontColor = RGB(255,255,255);
		nFontSize =16;
		nAlign = DT_LEFT;
	};
	string sName;
	int nCtrlIndex;
	string sTitle;		//����
	int nX;
	int nY;
	int nW;
	int nH;
	
	string sFontName;
	COLORREF fontColor; //������ɫ
	int nFontSize;		//�����С
	int nAlign;			//DT_LEFT DT_CENTER, DT_RIGHT	
};
struct __SwitchButtonAttr
{
	int nCtrlIndex;
	string sName;
	int nX;
	int nY;
	int nW;
	int nH;
	int nSharedId;
	bool bEnable;		//�Ƿ���Բ���
	int nDefualtValue;  //Ĭ��ֵ

	int nValues[2]; //Ŀǰ��2��ֵ 	
};


struct __EditAttr
{
	__EditAttr()
	{
		nCtrlIndex=0;
		nDataType=0;
		nSharedId =0;
		sName="";
		sRange="";
	};
	int nCtrlIndex;	
	string sName;
	int nX;
	int nY;
	int nW;
	int nH;
	int nSharedId;
	int nDataType;//�������ͣ��ݲ���
	string sRange;//���ݷ�Χ
};

struct __ImageCtrlAttr
{
	int nCtrlIndex;
	string sName;

	string sTitle;
	int nX;
	int nY;
	int nW;
	int nH;
	int nFontSize;

	COLORREF fontColor; 
	int nCtrlType;  //#define WND_type_switch_static_image	8  #define WND_type_switch_button_image	9
};
 

//EPH5142��EPH5141��EPH5410��EPH5033A��MODBUS��EPH5121A�忨��������
#define DRIVE_TYPE_Channel   1

//Э�����豸����429��1553 
#define DRIVE_TYPE_Protocol  2
//֧�ּ����豸
struct __DRIVE
{
	int nId;
	string sName;
	int nChannelNum;
	int nChannelBytes;
	string sMark;
};

//ϵͳ���м����豸
struct __SYS_Drive
{
	__SYS_Drive()
	{
		nAutoConnect=0;
	};
	int nId;
	string sName;
	string sDriveName;
	int nDriveId;
	int nBusNumber;
	int nDeviceNumber;	
	int nAutoConnect;	//1���Զ����ӣ�0��ָ������, 2=��λ�����ӣ�nBusNumber��nDeviceNumber ��Ϊ0ʱָ������
	string sAttr;
	string sMark;		//�忨����

	int nDriveType;
};

//�����ڴ棬����ʽ

//��ֵ��ΪPWM
#define SHARED_DATA_Deal_Mode_PWM_Value_Name  "PWM��ѹֵ"

#define SHARED_DATA_Deal_Mode_PWM_Value  2
#define SHARED_DATA_Deal_Mode_PWM_Name  "PWM"
#define SHARED_DATA_Deal_Mode_PWM  1
#define SHARED_DATA_Deal_Mode_Default  0

//�������
//�������õ����ɹ���

/*
ϵͳ��
�豸id��1~255
ͨ���ţ�1~255
ͨ���ţ�0~255

nSharedID = �豸id<<24 +ϵͳ�豸ID<<16+ͨ����;

���豸1����ɢ�����롢�����ģ���������nSharedID = �豸id<<24 +ϵͳ�豸ID<<16+ͨ����
      2��ģ�������룺һ��ɼ����� :nSharedID= �豸id<<24 +ϵͳ�豸ID<<16+ͨ����
	  3��ģ�������룺PWM��ѹֵ:nSharedID =�豸id<<24 +ϵͳ�豸ID<<16+(ͨ����+1)<<8+0 
	  4.Э�飺nSharedID =  (�豸id<<24) +(ϵͳ�豸ID<<16) +(nProtocolId<<8)+nIndex;  //nIndex��1��ʼ
	 
	 5������:�豸id��99����
*/

struct __DATA_CONFIG
{
	__DATA_CONFIG()
	{
		fCoefficient =1;
		nDealMode =SHARED_DATA_Deal_Mode_Default;
	};

	int nId;
	string sName;
	int nDutIndex;
	int nSysDriveId;
	int nDriveId;
	int nChannelIndex;
	int nSharedID;

	int nDataType; 
	int nDataSize;
	float fCoefficient;	//���������� int,float ,double��Ч��Ĭ��ֵΪ1
	float fOffset;		//����ֵ
	int nInOut;		//1 ����  2:��� ��0����ʾ���ݼ���
	int nDealMode;  //���ݴ���ʽ��   1:PWMռ�ձȣ�2��PWM��ѹֵ
	string sMark;
	string sDefaultValue;
	string sExtArr;
	string sSysDriveName;
};
//
struct __Ctrl_Event
{
	__Ctrl_Event()
	{
		nId=0;
		nGroup=-1;
		nValue1 =0;
		nValue2=0;
		nSharedId =0;
		color =0;
		sName="";
		sRemarks="";
	}
	int nId;
	int nCtrlId;
	int nEventType;		 //0���򿪳�ʼ����1���ر��¼���2������¼���3,ˢ��
	int nGroup;			 // -1���޷��� ,0������ĸ�, �ӳ�Ա��nId
	int nSharedId;
	int nValue1;		//��ʼ��/�ر��¼�:������򣬷��͸�ֵ
	int nValue2;		//�������䷶Χ�ڣ�����nValue1~nValue2֮��ʱ����ʾsPath  
	COLORREF  color;
	string sName;
	string sPath;
	string sRemarks;
};

//��Ҫ������������ɫ�����ݹ����ڴ�ֵ��������ɫ
struct __Ctrl_Event_Text
{	
	__Ctrl_Event_Text()
	{
		nId =0;
		nTextId=0;
	};
	int nId;
	int nTextId;
	int nCtrlId;
	int nEventType;		 // Ĭ�ϣ�3,ˢ��	
	int nSharedId;
	int nValue1;	// 	
	int nValue2;	//
	COLORREF  color;
	string sName;	
	string sRemarks;
};


//
#define  COPY_DATA_CMD_Close			0x00
#define  COPY_DATA_CMD_Init				0x01
#define  COPY_DATA_CMD_Event			0x02
#define  COPY_DATA_CMD_Exit				0x03
#define  COPY_DATA_CMD_Send_Protocol	0x04

#define  COPY_DATA_CMD_Start_Read_Data  0x05
#define COPY_DATA_CMD_1553_BUSA_B		0X06

#define COPY_DATA_CMD_Rectify			0x07
#define COPY_DATA_CMD_Start_Write_data	0x08
 
struct __Copy_Data_Info
{
	int nShareId;
	int nValue;
};

//������ʾ
struct __Ctrl_Test_Tip
{
	int nId;
	int nParentId;
	int nIndex;
	int nCtrlId;
	int nCtrlType;	
	string sName;
	string sTip;
};

struct __Ctrl_TestTip
{
	int nCtrlId;
	int nId;	 
	int nIndex;
	string sName;
	string sTip;
};


//�����ڴ����ֵ����ɫ��Ӧ��
struct __Value_Color
{
	int nValue;
	COLORREF colorRef;
};
 
//���οؼ�����
struct __RectangleAttr
{
	int nId;
	int nCtrlIndex;

	int nCtrlType;
	string sName;
	int nX;
	int nY;
	int nW;
	int nH;
	string sTitle;
	int nFontSize;
	COLORREF colorFont;
	COLORREF colorLine;
	int nSharedId;
	__Value_Color  vs[2];
};

//Բ�Ϳؼ�����
struct __CircleAttr
{
	int nId;
	int nX;
	int nY;
	int nDiameter;//ֱ��
	string sName; //�ؼ�����
	string sTitle;//��ʾ����
	int nFontSize;//�����С
	COLORREF colorFont;  //������ɫ	

};
//��ǩ����
//����ɫ ������ɫ����ǩ��ȣ���ǩ�߶ȡ�������ɫ�����塢�ֺš�ѡ�б�ǩ��ɫ��δѡ�б�ǩ��ɫ
struct __TabAttr
{
	int nId;
	int nCtrlIndex;
	string sName;
	string sTitle;
	int nX;
	int nY;
	int nW;
	int nH;
	COLORREF colorBack;
	COLORREF colorLine;
	int nTabWidth;
	int nTabHeight;
	string sFontName;
	int nFontSize;
	COLORREF colorFont;
	COLORREF colorSelTab;
	COLORREF colorUnSelTab;	
};

//�߿�ʵ�߳��ȡ����߳��ȡ���ɫ
struct __LineAttr
{
	__baseAttr baseAttr;

	int nLineWidth;

	int nRealLineLength;

	int nDottedLineLength;

	COLORREF colorLine;
};

struct __ButtonAttr_V02
{
	__ButtonAttr_V02()
	{
		sTitle="";
		sFontName="����";
		nFontSize =16;
		colorFont =RGB(255,0,0);
		
		bBackColor =FALSE;
		colorBack =RGB(28,28,28);
		colorLine =RGB(255,255,255);
		
		bBackBmp =true;
		nBmpX =0;
		nBmpY =0;

		nEventType = 0;
	};
	__baseAttr baseAttr;

	//����
	string sTitle;
	string sFontName;
	int nFontSize;
	COLORREF colorFont;
	
	//�Ƿ��б���������ɫ��������ɫ
	bool bBackColor;
	COLORREF colorBack;
	COLORREF colorLine;

	//�Ƿ��б���������ͼ�Ŀ�ʼλ�ã�Ĭ��������б�����
	bool bBackBmp;
	int nBmpX;
	int nBmpY;

	// �¼����ͣ��������ͶԵ���¼�����ͬ�Ĵ���
	int nEventType;
};

//�ؼ��ϵ�����
struct __TextAttr_V02
{
	__TextAttr_V02()
	{
		nId=0;
		nX=0;
		nY=0;
		nAlign=-1;
	}
	int nId;
	int nCtrlId;
	int nType;// 0:�������֣�nAlign >=0,
	string  sTitle;
	string sFontName;
	int nFontSize;
	COLORREF colorFont;
	int nAlign;
	int nX;
	int nY;
};

struct __RotateAttr_V02
{	
	__RotateAttr_V02()
	{
		nPicY=0;
		nPicX=0;
		colorBack =RGB(28,28,28);
		colorLine =RGB(28,28,28);
		bShowBackColor =0;

		nAngleStart =0;
		nAngleEnd =180;
				
		nFontY =0;
		nFontX=0;
		nFontH =30;
		nFontW =50;
		colorFont=RGB(255,255,255);

		nFontSize =16;
		sFontName ="����";
		bShowText =1;

		fValueMin =0;
		fValueMax =100;

		nId=0;		
	}
	__baseAttr baseAttr;
	
	int nId;
	
	int nShareId;//�����ڴ�ֵ

	//��С����ֵ
	float fValueMin;  
	float fValueMax;

	//�����ڴ�ֵ���ԣ��Ƿ���ʾ�����塢�ֺš���ɫ��λ��X\Y
	int bShowText;
	string sFontName;
	int nFontSize;
	COLORREF colorFont;	
	int nFontX;
	int nFontY;
	int nFontW;
	int nFontH;


	//ͼƬ����ʼ�������Ƕȣ�λ��X,Y
	string sPicName;
	int nAngleStart;
	int nAngleEnd;
	
	int nPicX;
	int nPicY;

	//����ɫ���Ƿ���ʾ������ɫ��������ɫ
	int bShowBackColor;
	COLORREF colorBack;	
	COLORREF colorLine;		
};
struct __UnitData
{
	
	int nID;
	int nSysId ;
	string sName;
	int nDataType;
	int nDataLen;
	int nDecimalNum;
	string sDefault;
	float fTimes ;//����
	string sMark;

};
 

struct __Goods_Number
{
	int nDlgId;
	int nModBusValue[5];   //nModBusValue      =  (nX3<<20)+(nX2<<10)+nX1
	int nStartIndex;//�ݲ��ã���ʾnModBusValue��Чλ��
};

struct __Ctrl_Color_Font
{
	int nDlgId;
	int nId;
	int nColor;
};
 