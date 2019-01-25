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
 

//根据区间值来显示图片
#define WND_type_Image_Interval 11

//矩形控件
#define WND_type_Rectangle_Name "矩形控件"
#define WND_type_Rectangle   12

//圆形控件
#define WND_type_Circle_Name "圆形控件"
#define WND_type_Circle   13

//线形控件
#define WND_type_Line_Name "线形控件"
#define WND_type_Line	14


//控件V2.0

#define WND_type_image_button_V02  20
#define WND_type_image_button_V02_Name "输出离散量_V2"

#define WND_type_image_Led_V02  21
#define WND_type_image_Led_V02_Name "输入离散量_V2"

#define WND_type_image_Rotate_V02  22
#define WND_type_image_Rotate_V02_Name "模拟量输入(旋钮)_V2"

#define WMD_type_digital 23			// 以数码字段的形式显示数字
#define WND_type_digital_Name "数字文本框"



#define ACT_Show_dlg	"Show_Dlg"
#define ACT_Exit_Sys	"Exit_Sys"
 

#define  BASE_NUM  1000
//控件事件

//初始化
#define EVENT_TYPE_INIT  0
//关闭事件
#define EVENT_TYPE_CLOSE 1
//点击事件
#define EVENT_TYPE_CLICK 2
//刷新事件
#define EVENT_TYPE_REFRESH 3


/*
定义数据定义项的类型 
输入，输出，PWM输入
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
	 int nInterfaceId; ///-1,弹出窗口，-2,独立弹出窗口，供外部调用 
	 int nType;   //0：窗口，1：button，2：static 3,GROUP, 4: edit ,5:  Switch Static
	 int nX;
	 int nY;
	 int nW;
	 int nH;
	 string sTitle;
	 string sAttr;	 
	 int nSharedId;  //共享内存值

	 string sReserve1;
	 string sReserve2;

};

typedef struct tagTBLDATA
{
	int nId;						// 编号
	string sName;					// 测试项名称
	string sUnit;					// 单位（V 或 A)
	float fValue;					// 测量值
	int nPrecision;					// 小数点位数
	string sRef;					// 参考值（评价标准）
	string sStatus;					// 状态
	float fMinV;					// 评价标准最小值
	float fMaxV;					// 评价标准最大值
	int nSharedId;					// 共享内存ID
	int nDlgId;						// 归属的对话框ID
}TBLDATA,*pTBLDATA;

typedef struct tagBOARDCONFIG
{
	int nId;						// 编号
	string sSignalName;				// 信号名称
	//int nBusNumber;					// 总线号
	//int nDeviceNumber;				// 设备号
	//int nDriveId;					// 驱动编号（系统中的第几块驱动板卡）
	int nChannelIndex;				// 通道号
	float fCoefficient;				// 系数
	float fOffset;					// 偏置
	int nDlgId;						// 系统ID
	int nShareId;					// 共享内存
}BOARDCONFIG,*pBOARDCONFIG;

struct __SysInfo
{
	int nId;
	string sName;
	string sKey; 
};

//测试系统信息
struct __TestSysInfo
{
	int nId;
	string sName;
	string sMark;
};
//测试系统窗口信息
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
	string sName; //窗口名称，标注
	int nStyle ; //0:弹出、1，子窗口	
	string sTitle;
	int nW;		//宽
	int nH;		//高
	int nX;		//左
	int nY;		//上
	COLORREF backColor;//背景色

	//弹出窗口有效
	int nTitleH ;			//标题高度 
	COLORREF colorTitle;	//标题背景色 	
	COLORREF colorFont;		//标题字图背景色 
	int nFontSize;			//标题字体大小 
	
	int nDutIndex;//窗口对应的产品序号,
	int nTimer;//数据刷新时间;

	//状态栏
 	
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
	string sTitle;		//内容
	int nX;
	int nY;
	int nW;
	int nH;
	string sFontName;
	bool bBackColor;	//是否有背景色
	COLORREF backColor; //背景色
	COLORREF fontColor; //字体颜色
	int nFontSize;		//字体大小
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
	string sTitle;		//内容
	int nX;
	int nY;
	int nW;
	int nH;
	string sFontName;
	bool bBackColor;	//是否有背景色
	COLORREF backColor; //背景色
	COLORREF fontColor; //字体颜色
	int nFontSize;		//字体大小
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
		sFontName ="黑体";
		fontColor = RGB(255,255,255);
		nFontSize =16;
		nAlign = DT_LEFT;
	};
	string sName;
	int nCtrlIndex;
	string sTitle;		//内容
	int nX;
	int nY;
	int nW;
	int nH;
	
	string sFontName;
	COLORREF fontColor; //字体颜色
	int nFontSize;		//字体大小
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
	bool bEnable;		//是否可以操作
	int nDefualtValue;  //默认值

	int nValues[2]; //目前就2个值 	
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
	int nDataType;//数据类型，暂不用
	string sRange;//数据范围
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
 

//EPH5142、EPH5141、EPH5410、EPH5033A、MODBUS、EPH5121A板卡，单数据
#define DRIVE_TYPE_Channel   1

//协议类设备，如429，1553 
#define DRIVE_TYPE_Protocol  2
//支持几类设备
struct __DRIVE
{
	int nId;
	string sName;
	int nChannelNum;
	int nChannelBytes;
	string sMark;
};

//系统中有几个设备
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
	int nAutoConnect;	//1：自动连接，0：指定连接, 2=上位机连接，nBusNumber，nDeviceNumber 不为0时指定连接
	string sAttr;
	string sMark;		//板卡类型

	int nDriveType;
};

//共享内存，处理方式

//该值，为PWM
#define SHARED_DATA_Deal_Mode_PWM_Value_Name  "PWM电压值"

#define SHARED_DATA_Deal_Mode_PWM_Value  2
#define SHARED_DATA_Deal_Mode_PWM_Name  "PWM"
#define SHARED_DATA_Deal_Mode_PWM  1
#define SHARED_DATA_Deal_Mode_Default  0

//数据项定义
//共享内置的生成规则

/*
系统中
设备id：1~255
通道号：1~255
通道号：0~255

nSharedID = 设备id<<24 +系统设备ID<<16+通道号;

有设备1：离散量输入、输出、模拟量输出：nSharedID = 设备id<<24 +系统设备ID<<16+通道号
      2：模拟量输入：一般采集数据 :nSharedID= 设备id<<24 +系统设备ID<<16+通道号
	  3：模拟量输入：PWM电压值:nSharedID =设备id<<24 +系统设备ID<<16+(通道号+1)<<8+0 
	  4.协议：nSharedID =  (设备id<<24) +(系统设备ID<<16) +(nProtocolId<<8)+nIndex;  //nIndex从1开始
	 
	 5：集合:设备id（99）？
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
	float fCoefficient;	//倍数，对于 int,float ,double有效，默认值为1
	float fOffset;		//补偿值
	int nInOut;		//1 输入  2:输出 ，0：表示数据集合
	int nDealMode;  //数据处理方式，   1:PWM占空比，2：PWM电压值
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
	int nEventType;		 //0，打开初始化，1：关闭事件，2：点击事件，3,刷新
	int nGroup;			 // -1，无分组 ,0：分组的父, 子成员：nId
	int nSharedId;
	int nValue1;		//初始化/关闭事件:向处理程序，发送该值
	int nValue2;		//当在区间范围内，则处于nValue1~nValue2之间时，显示sPath  
	COLORREF  color;
	string sName;
	string sPath;
	string sRemarks;
};

//主要是用于文字颜色，根据共享内存值，设置颜色
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
	int nEventType;		 // 默认：3,刷新	
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

//测试提示
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


//共享内存测试值和颜色对应表
struct __Value_Color
{
	int nValue;
	COLORREF colorRef;
};
 
//矩形控件属性
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

//圆型控件属性
struct __CircleAttr
{
	int nId;
	int nX;
	int nY;
	int nDiameter;//直径
	string sName; //控件名称
	string sTitle;//显示内容
	int nFontSize;//字体大小
	COLORREF colorFont;  //字体颜色	

};
//标签属性
//背景色 、边线色、标签宽度，标签高度、字体颜色、字体、字号、选中标签颜色，未选中标签颜色
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

//线宽、实线长度、虚线长度、线色
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
		sFontName="黑体";
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

	//字体
	string sTitle;
	string sFontName;
	int nFontSize;
	COLORREF colorFont;
	
	//是否有背景，背景色，边线颜色
	bool bBackColor;
	COLORREF colorBack;
	COLORREF colorLine;

	//是否有背景，背景图的开始位置，默认情况下有背景，
	bool bBackBmp;
	int nBmpX;
	int nBmpY;

	// 事件类型（根据类型对点击事件做不同的处理）
	int nEventType;
};

//控件上的文字
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
	int nType;// 0:基础文字，nAlign >=0,
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
		sFontName ="黑体";
		bShowText =1;

		fValueMin =0;
		fValueMax =100;

		nId=0;		
	}
	__baseAttr baseAttr;
	
	int nId;
	
	int nShareId;//共享内存值

	//最小、大值
	float fValueMin;  
	float fValueMax;

	//共享内存值属性：是否显示，字体、字号、颜色、位置X\Y
	int bShowText;
	string sFontName;
	int nFontSize;
	COLORREF colorFont;	
	int nFontX;
	int nFontY;
	int nFontW;
	int nFontH;


	//图片，开始、结束角度，位置X,Y
	string sPicName;
	int nAngleStart;
	int nAngleEnd;
	
	int nPicX;
	int nPicY;

	//背景色，是否显示，背景色，边线颜色
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
	float fTimes ;//倍数
	string sMark;

};
 

struct __Goods_Number
{
	int nDlgId;
	int nModBusValue[5];   //nModBusValue      =  (nX3<<20)+(nX2<<10)+nX1
	int nStartIndex;//暂不用，表示nModBusValue有效位置
};

struct __Ctrl_Color_Font
{
	int nDlgId;
	int nId;
	int nColor;
};
 