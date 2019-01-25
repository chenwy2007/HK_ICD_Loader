#include "StdAfx.h"
#include "XTestData.h"


XTestData::XTestData(void)
{
	// int nId = InitInterface382(1280,1000);	
	//m_nSysId =1;
	////int 	nId = InitInterface434(1280,1000);
	//int nId = InitInterface383A(1280,1000);
	//InitInterface433(1280,1024);
	return;

	int nRe = AfxMessageBox("是否重新生成数据",MB_YESNO);
	if(nRe ==IDYES)
	{

		CManageDb_Sqllite & db = CManageDb_Sqllite::GetInstance();

		db.ExecDML("delete from t_sys");
		db.ExecDML("delete from t_ctrl");
		db.ExecDML("delete from t_sys_ctrl");
		InitMainInterface_CPA(); 
	}
}


XTestData::~XTestData(void)
{
}
	void XTestData::Delete_dlg(int nDlgId)
	{
		 

		char lpSql[1024];
		sprintf(lpSql,"delete from t_sys_ctrl where  nDlgId = %d ",nDlgId);

		sprintf(lpSql,"delete   from t_data_config where nDlgId =  %d ",nDlgId);

		sprintf(lpSql,"delete  from t_dut where nDlgId =  %d ",nDlgId);

		sprintf(lpSql,"delete  from t_test_tip where nCtrlId in (select nId  from t_ctrl where nInterfaceId = %d)",nDlgId);
		sprintf(lpSql,"delete  from t_test_tip where nCtrlId in  ( select nId  from t_ctrl where nInterfaceId  in( select nId  from t_ctrl where nInterfaceId = %d))",nDlgId);
			 

			
		sprintf(lpSql,"delete  from t_test_tip where nCtrlId in  ( select nId  from t_ctrl where nInterfaceId  in( select nId  from t_ctrl where nInterfaceId  in(select nId  from t_ctrl where nInterfaceId = %d)))",nDlgId);
			
			
		sprintf(lpSql,"delete  from t_ctrl_event where nCtrlId  = %d",nDlgId);

		sprintf(lpSql,"delete  from t_ctrl_event where nCtrlId in (select nId  from t_ctrl where nInterfaceId = %d)",nDlgId);
			
		sprintf(lpSql,"delete  from t_ctrl_event where nCtrlId in  ( select nId  from t_ctrl where nInterfaceId  in( select nId  from t_ctrl where nInterfaceId =%d))",nDlgId);
			
		sprintf(lpSql,"delete  from t_ctrl_event where nCtrlId in  ( select nId  from t_ctrl where nInterfaceId  in( select nId  from t_ctrl where nInterfaceId  in(select nId  from t_ctrl where nInterfaceId = %d)))",nDlgId);
			
		sprintf(lpSql,"",nDlgId);
			
		sprintf(lpSql,"delete  from t_ctrl_image  where nCtrlId  = %d",nDlgId);

		sprintf(lpSql,"delete  from t_ctrl_image where nCtrlId in (select nId  from t_ctrl where nInterfaceId = %d)",nDlgId);
			
		sprintf(lpSql,"delete  from t_ctrl_image where nCtrlId in ( select nId  from t_ctrl where nInterfaceId  in( select nId  from t_ctrl where nInterfaceId = %d))",nDlgId);	
			
		sprintf(lpSql,"delete  from t_ctrl_image where nCtrlId in ( select nId  from t_ctrl where nInterfaceId  in( select nId  from t_ctrl where nInterfaceId  in(select nId  from t_ctrl where nInterfaceId = %d)))",nDlgId);
			
		sprintf(lpSql,"delete  from t_ctrl  where nID = %d",nDlgId);
		sprintf(lpSql,"delete  from  t_ctrl where nInterfaceId = %d",nDlgId);
		sprintf(lpSql,"delete  from t_ctrl where nInterfaceId in (select nId  from t_ctrl where nInterfaceId = %d)",nDlgId);
		sprintf(lpSql,"delete  from t_ctrl where nInterfaceId in ( select nId  from t_ctrl where nInterfaceId  in( select nId  from t_ctrl where nInterfaceId = %d))",nDlgId);

			
			
			
			


	}
//
int XTestData::InitMainInterface_CPA()
{
	int nId;
	char lpSql[1024];
	CManageDb_Sqllite & db = CManageDb_Sqllite::GetInstance();
	int nSysId = db.GetMaxId("select max(nId) from  t_sys")+1;
	m_nSysId = nSysId;

	int nDlgId = db.GetMaxId("select max(nId) from t_ctrl")+1;
	sprintf(lpSql,"insert into t_sys values(%d,'CPA测试系统','CPA')",nSysId);
	db.ExecDML(lpSql);
	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'启动窗口')",nSysId,nDlgId);
	db.ExecDML(lpSql);

	int nW = 800;
	int nH =600;
		
	sprintf(lpSql,"insert into  t_ctrl values(%d,-1,0,0,0,%d,%d,'CPA测试系统','',0,'启动窗口','','')",nDlgId,nW,nH);
	db.ExecDML(lpSql);

	  nW =1900;
	  nH = 1000;
	nId = InitInterface383A(nW,nH);
	 
	nId = InitInterface415(nW,nH);
		
	nId = InitInterface604(nW,nH);
	  
	nId = InitInterface434(nW,nH);
	 
	nId = InitInterface382(1900,1000);	
	 
	nId = InitInterface391(1900,1000);

	nW =150;
	nH = 50;
	int x = 100; 
	int y  =100;
	int nDis = 50;
	
	CString  strTitle ="KZH-427";
	nId = InitInterface427(-1,1900,1000);
	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'KZH-427')",nSysId,nId);
	db.ExecDML(lpSql);


	CMarkup bt427;
	bt427.AddElem("Attr");
	bt427.AddAttrib("Act","ShowDlg");
	bt427.AddAttrib("Param1",nId);
	bt427.AddAttrib("Exit",0);

	CString strInfo = bt427.GetDoc();  

	sprintf(lpSql,"insert into t_ctrl values((select max(nId)+1 from t_ctrl),%d,1,%d,%d,%d,%d,'%s','%s',0,'KZH-427','','')",nDlgId,x,y,nW,nH,strTitle,strInfo);	
	db.ExecDML(lpSql);


	strTitle ="KZH-430";

	nId = InitInterface430(-1,1900,1000);
	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'KZH-430')",nSysId,nId);
	db.ExecDML(lpSql);

	CMarkup bt430;
	bt430.AddElem("Attr");
	bt430.AddAttrib("Act","ShowDlg");
	bt430.AddAttrib("Param1",nId);
	bt430.AddAttrib("Exit",0);


	 strInfo = bt430.GetDoc();  

	sprintf(lpSql,"insert into t_ctrl values((select max(nId)+1 from t_ctrl),%d,1,%d,%d,%d,%d,'%s','%s',0,'KZH-430','','')",nDlgId,x,y,nW,nH,strTitle,strInfo);	
	db.ExecDML(lpSql);
	
	 

	nId = InitInterface528(-1,1900,1000);
	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'KZH-528')",nSysId,nId);
	db.ExecDML(lpSql);

	 strTitle ="KZH-528";

	 CMarkup bt528;
	 bt528.AddElem("Attr");
	 bt528.AddAttrib("Act","ShowDlg");
	 bt528.AddAttrib("Param1",nId);
	 bt528.AddAttrib("Exit",0);

	 strInfo = bt528.GetDoc();  
	 x+=(nDis + nW);	 

	 sprintf(lpSql,"insert into t_ctrl values((select max(nId)+1 from t_ctrl),%d,1,%d,%d,%d,%d,'%s','%s',0,'KZH-528','','')",nDlgId,x,y,nW,nH,strTitle,strInfo);	
	 db.ExecDML(lpSql);

	return 0;

}
int  XTestData::InitInterface528(int nParentId,int nW ,int nH)
{
	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	 
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

	int nReId = nMaxId;
	 
	CString strTitle ="KZH-528";
	
	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,0,10,10,%d,%d,'CPA测试系统','%s',0,'%s','','')",nMaxId,nParentId,nW,nH,"",strTitle);
	db.ExecDML(lpSql);
	 
	int nLeft = 20;
	int nTop = 80;
	int nDis = 30;

	int nChildW = (nW -2*nLeft- nDis)/2;
	int nChildH = (nH -2*nTop- 2*nDis)/3;
	int x ,y;
	for(int  i =0;i<5;i++)
	{ 
		x = nLeft +(nChildW+nDis) * (i%2);
		y = nTop +(nChildH +nDis) * (i/2);	 
		InitInterface528_Sub(nMaxId,x,y,nChildW,nChildH,i+1); 
 
	}
	return nReId;
}

int  XTestData::InitInterface528_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH,int nIndex )
{
	int nReWndId =0;

	CMarkup xmlDlg;
	xmlDlg.AddElem("Wnd");
	xmlDlg.AddAttrib("style","Child");
	CString strInfo = xmlDlg.GetDoc();  
	CString strTitle[]={"仪表板左亮度","中央控制台亮度","仪表板右亮度","上部控制台亮度","驾驶舱顶灯亮度"};

	int x = nLeft;
	int y = nTop;

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	nReWndId = nMaxId;

	CString str;
	str.Format("%d#KZH-528",nIndex);

	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,0,%d,%d,%d,%d,'','%s',0,'%s','','')",nMaxId++,nParentId,nLeft,nTop,nW,nH,strInfo,str);
	db.ExecDML(lpSql);

	x=0;
	y=10;
	 
	nH -=20;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,3,%d,%d,%d,%d,'%d#KZH-528','',0,'%d#KZH-528','','')",nMaxId++,nReWndId,x,y,nW,nH,nIndex,nIndex);
	db.ExecDML(lpSql);
	
	InitInterface528_2_Sub(nReWndId,0,nH *3/5,nW,nH *2 /5,0);
	
	nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

	x +=5;
	int nDis = 3;

	int nClildW = (nW-4*nDis)/5-3;
	int nClildH = nH*2/5;

	y+=30;
 
	int nEditW = 80;
	int nEditH = 20;
	int nStaticW  =35;
	 
	int nPosY;
	int nPosX;
	nDis = 5;
	for(int i=0;i<5;i++)
	{	 
		nPosX =0;
		nPosY =0;
		//group
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,3,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nClildW,nClildH,strTitle[i],strTitle[i]);
		db.ExecDML(lpSql);
		 
		nPosY +=30;
		nPosX +=25;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'正常','',0,'正常','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nStaticW,nEditH);
		db.ExecDML(lpSql); 
		
		nPosY+=nEditH*2;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'应急','',0,'应急','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nStaticW,nEditH);
		db.ExecDML(lpSql);

 
		nPosX +=(nStaticW+nDis);
		nPosY =30;

		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,4,%d,%d,%d,%d,'','',0,'正常','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nEditW,nEditH);
		db.ExecDML(lpSql);
		
		nPosY+=nEditH*2;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,4,%d,%d,%d,%d,'','',0,'应急','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nEditW,nEditH);
		db.ExecDML(lpSql);


		nPosX +=(nEditW+nDis);
		nPosY =30;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'%%','',0,'正常','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,10,nEditH);
		db.ExecDML(lpSql);

	
		nPosY+=nEditH*2;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'%%','',0,'应急','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,10,nEditH);
		db.ExecDML(lpSql);


		x+=(nClildW+nDis);
	}
	 
	return  nReWndId;
}

int XTestData:: InitInterface528_2_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex)
{
	 
	CString strTitle[]={"客舱顶灯1","客舱顶灯绿灯亮度","客舱顶灯2","28V正常","24正常"};

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();

	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	
	int nDis = 5;
	int nClildW = (nW-4*nDis)/4-3;
	int nClildH = nH;

	int x = nLeft+5;
	int y =nTop;
	int nPosX =0;
	int nPosY =0;
	char lpSql[1024];
	for(int i=0;i<3;i++)
	{	

		nPosX =0;
		nPosY =0;
		//group
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,3,%d,%d,%d,%d,'%s','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,nClildW,nClildH,strTitle[i]);
		db.ExecDML(lpSql);

		if(i ==0 || i ==2)
		{
			nPosX =nClildW/4-10;
			nPosY =30;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'白','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,20,20);
			db.ExecDML(lpSql);
		 
			nPosX +=nClildW/2-10;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'绿','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,20,20);
			db.ExecDML(lpSql);

			nPosY +=20;
			
			nPosX  = nClildW/4/2+12 ;

			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,nClildW/6,nClildW/6);
			db.ExecDML(lpSql);

			nPosX  = nClildW*5/8+5  ;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,nClildW/6,nClildW/6);
			db.ExecDML(lpSql);
		}
		else if(i ==1)
		{
			nPosX =nClildW/4;
			nPosY =nClildH/2;
			
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,4,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,nClildW/2,20);
			db.ExecDML(lpSql);

			nPosX =nClildW *3/4;
			 
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'V','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,20,20);
			db.ExecDML(lpSql);


		}
 
		x+=(nClildW+nDis);
	}
	//"28V正常","24正常"
	nClildW /=2; 
	nPosX =0;
	nPosY =0;
	for(int i=0;i<2;i++)
	{
		nPosX =0;
		nPosY =0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,3,%d,%d,%d,%d,'%s','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,nClildW,nClildH,strTitle[i+3]);
		db.ExecDML(lpSql);

		nPosX = nClildW/2 -15;
		nPosY =20;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'开','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,20,20);
		db.ExecDML(lpSql);

		nPosX = nClildW/3;
		nPosY +=20;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,6,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,nClildW/3,nClildH*2/5);
		db.ExecDML(lpSql);

		nPosX = nClildW/2-15;
		nPosY +=nClildH*2/5;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'关','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,20,20);
		db.ExecDML(lpSql);

		x+=(nClildW+nDis);
	}
	
	return  0;
} 


int  XTestData::InitInterface430(int nParentId,int nW ,int nH )
{
	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();

	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

 
	CMarkup xmlDlg;
	xmlDlg.AddElem("Attr");
	xmlDlg.AddAttrib("Mark","KZH-430");
	CString strInfo = xmlDlg.GetDoc();  

	CString strTitle = "KZH-430";
	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,0,10,10,%d,%d,'CPA测试系统','%s',0,'%s','','')",nMaxId,nParentId,nW,nH,strInfo,strTitle);
	db.ExecDML(lpSql); 
	int nDlgId =nMaxId;
	nMaxId++;
	//table;
	int nTabW = nW -40;
	int nTabH = nH -80;

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,7,20,60,%d,%d,'','',0,'','','')",nMaxId,nDlgId,nTabW,nTabH);
	db.ExecDML(lpSql);
	int nTabId = nMaxId;
	for(int i =0;i<5;i++)
	{
		InitInterface430_Sub(nTabId,1,25,nTabW-3,nTabH-27,i);
	}
	
	return nDlgId;

}
int  XTestData::InitInterface430_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex)
{
	 
	int nReWndId =0;

	CMarkup xmlDlg;
	xmlDlg.AddElem("Wnd");
	xmlDlg.AddAttrib("style","Child");
	CString strInfo = xmlDlg.GetDoc();  
	
	int x = nLeft;
	int y = nTop;

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

	nReWndId = nMaxId;

	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,0,%d,%d,%d,%d,'%d#','',0,'','','')",nMaxId++,nParentId,nLeft,nTop,nW,nH,nIndex);
	db.ExecDML(lpSql);

	CMarkup xmlTitle;
	xmlTitle.AddElem("Attr");
	xmlTitle.AddAttrib("Back",1);
	xmlTitle.AddAttrib("BackColor",RGB(255,255,255));
	xmlTitle.AddAttrib("Align",1);
	xmlTitle.AddAttrib("FontSize",150);
	strInfo = xmlTitle.GetDoc();  


	int nChildW = nW/2;
	int nChildH = 60;

	x=(nW-nChildW)/2;
	y = 80;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'KZH-430','%s',0,'','','')",nMaxId++,nReWndId,x,y,nChildW,nChildH,strInfo);
	db.ExecDML(lpSql);
	 
	CString strTitle[]={"左登机口","右登机口","货仓门","舱门电源","EMP选择","DAP选择","导光板"};


	nChildW =400;
	nChildH = 200;

	int nDis = 50;
	x = (nW- 3*nChildW - 2*nDis)/2;

	y +=100;

	int nPosX ,nPosY;

	int nItemW = nChildW/6;

	int nItemH = nChildH/3;
	CString str2[4][12]={ 
						{"X1:4","X1:3","X1:5","X1:7","X1:6","X1:8","X1:10","X1:9","X1:11","X1:13","X1:12","X1:14"},
						{"X1:16","X1:15","X1:17","X1:19","X1:18","X1:20","X1:22","X1:21","X1:23","X1:25","X1:24","X1:26"},
						{"X1:28","X1:27","X1:29","X1:31","X1:30","X1:32","X1:34","X1:33","X1:35","X1:37","X1:36","X1:38"},
						{"X1:40","X1:39","X1:42","X1:43","X1:42","X1:44","X1:46","X1:45","X1:47","X1:49","X1:48","X1:50"},
					};

	
	CMarkup xml1;
	xml1.AddElem("Attr");
	xml1.AddAttrib("Align",1);
	xml1.AddAttrib("FontSize",80);
	strInfo = xml1.GetDoc();  

	CMarkup xmlSwitchStatic;
	xmlSwitchStatic.AddElem("Attr"); 
	xmlSwitchStatic.AddAttrib("BackColor",RGB(128,128,128));
	xmlSwitchStatic.AddAttrib("LineColor",RGB(255,255,255));
	xmlSwitchStatic.AddAttrib("Default",0);

	COLORREF color[]={RGB(0,64,0),RGB(255,0,0)};
	for(int i =0;i<2;i++)
	{
		CMarkup xml;
		xml.AddElem("Color");
		xml.AddAttrib("Index",i);
		xml.AddAttrib("Value",color[i]);
		xmlSwitchStatic.AddChildSubDoc(xml.GetSubDoc());
	}
	CString strSwitchStatic = xmlSwitchStatic.GetSubDoc();
	  
	int nX1 =x;
	y+=20;
	for(int i=0;i<4;i++)
	{	 
		if(i/3)
			x =nX1;

		nPosX =0;
		nPosY = i/3*(nChildH+80);
		//group
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,3,%d,%d,%d,%d,'%s','',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nChildW,nChildH,strTitle[i]);
		db.ExecDML(lpSql);

		nPosY +=20;
		//第一行 打开、关闭
		for(int n =0;n<2;n++)
		{
			nPosX +=nItemW;			
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'打开','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nItemW,20,strInfo);
			db.ExecDML(lpSql);
			nPosX +=nItemW;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'关闭','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nItemW,20,strInfo);
			db.ExecDML(lpSql);
			nPosX +=nItemW;
		}
		//
		nPosX =0;
		nPosY +=20;
		//x1:4 .....  

		for(int n =0;n<2;n++)
		{
			nPosX =0;
			for(int k =0;k<6;k++)
			{
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'%s','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nItemW,20,str2[i][n*6+k],strInfo);
				db.ExecDML(lpSql);				 
				nPosX+=nItemW;
			}
			nPosY +=20;
			nPosX =0;
			for(int k =0;k<2;k++)
			{
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'开','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,30,20,strInfo);
				db.ExecDML(lpSql);
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'关','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY+nItemH -40,30,20,strInfo);
				db.ExecDML(lpSql);

				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,6,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nReWndId,x+nPosX+30,y+nPosY,20,nItemH-20);
				db.ExecDML(lpSql);

				nPosX+=(nItemW+10);
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY+5,40,40,strSwitchStatic);
				db.ExecDML(lpSql);

				nPosX+=nItemW;
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY+5,40,40,strSwitchStatic);
				db.ExecDML(lpSql);
				nPosX+=nItemW;
			}

			nPosY +=(nItemH-10);


		}
		x+=(nChildW+nDis); 
	}


	nChildW = 300;
	 
	y = y+nChildH+80;

	CString str3[2][2]={{"EMP1","EMP2"},{"DAP1","DAP2"}};

	CString str4[2][6]={
						{"X1:52","X1：51","X1：53","X1:52","X1：55","X4：56"},
						{"X1:58","X1：57","X1：59","X1:61","X1：60","X4：62"}
						};

	x = (nW- 3*nChildW - 2*nDis)/2 + nChildW;

	nItemW = nChildW/3;
	//"EMP选择","DAP选择"
	for(int i =0;i<2;i++)
	{
		nPosX =0;
		nPosY =0;
		//group
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,3,%d,%d,%d,%d,'%s','',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nChildW,nChildH,strTitle[4+i]);
		db.ExecDML(lpSql);
	
		//EMP1，EMP2，DAP1，DAP2;
		nPosY =20;
		nPosX +=nItemW;
		for(int n =0;n<2;n++)
		{		
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'%s','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nItemW,20,str3[i][n],strInfo);
			db.ExecDML(lpSql);
			nPosX +=nItemW;
		}
		nPosY +=20;
		nPosX =0;

		for(int n =0;n<2;n++)
		{
			nPosX =0;
			for(int k=0;k<3;k++)
			{
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'%s','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nItemW,20,str4[i][n*3+k],strInfo);
				db.ExecDML(lpSql);
				nPosX +=nItemW;
			}
			nPosY+=20;
			nPosX =10;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'开','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,30,20,strInfo);
			db.ExecDML(lpSql);
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'关','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY+nItemH-40,30,20,strInfo);
			db.ExecDML(lpSql);

			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,6,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nReWndId,x+nPosX+30,y+nPosY,20,nItemH-20);
			db.ExecDML(lpSql);

			nPosX +=(nItemW+18);
			
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY+5,40,40,strSwitchStatic);
			db.ExecDML(lpSql);

			nPosX +=(nItemW+5);
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY+5,40,40,strSwitchStatic);
			db.ExecDML(lpSql);
			nPosY +=(nItemH-10);
			 
		}
	  
		x+=(nChildW+nDis);

	}
	//x = (nW- 3*nChildW - 2*nDis)/2 + nChildW;
	
	nChildW = 100;
	nPosX =0;
	nPosY =0;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,3,%d,%d,%d,%d,'导光板','',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nChildW,nChildH);
	db.ExecDML(lpSql);
	 
	nPosY +=40;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'开','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nChildW,20,strInfo);
	db.ExecDML(lpSql);

	nPosY +=20;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,6,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nReWndId,x+nPosX+40,y+nPosY,20,nItemH);
	db.ExecDML(lpSql);
	nPosY +=(nItemH+10);

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'关','%s',0,'','','')",nMaxId++,nReWndId,x+nPosX,y+nPosY,nChildW,20,strInfo);
	db.ExecDML(lpSql);

	return 0;
}

int XTestData::InitInterface427(int nParentId,int nW ,int nH )
{

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();

	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

	int nDlgId = nMaxId;

	CMarkup xmlDlg;
	xmlDlg.AddElem("Attr");
	xmlDlg.AddAttrib("Mark","KZH-427");
	CString strInfo = xmlDlg.GetDoc();  

	CString strTitle ="KZH-427";
	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,0,10,10,%d,%d,'CPA测试系统','%s',0,'%s','','')",nMaxId,nParentId,nW,nH,strInfo,strTitle);
	db.ExecDML(lpSql);

	int nLeft = 20;
	int nTop = 80;
	int nDis = 30;

	int nChildW = (nW -2*nLeft- nDis)/2;
	int nChildH = (nH -2*nTop- 2*nDis)/3;
	int x ,y;
	for(int  i =0;i<5;i++)
	{ 
		x = nLeft +(nChildW+nDis) * (i%2);
		y = nTop +(nChildH +nDis) * (i/2);	 
		InitInterface427_Sub(nDlgId,x,y,nChildW,nChildH,i+1); 

	}

	return nDlgId;
}
int XTestData::InitInterface427_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex)
{
	int nReWndId =0;

	CMarkup xmlDlg;
	xmlDlg.AddElem("Wnd");
	xmlDlg.AddAttrib("style","Child");
	CString strInfo = xmlDlg.GetDoc();  
 
	int x = nLeft;
	int y = nTop;

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	nReWndId = nMaxId;

	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,0,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nParentId,nLeft,nTop,nW,nH,strInfo);
	db.ExecDML(lpSql);

	x=0;
	y=10;

	nH -=20;
//	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,3,%d,%d,%d,%d,'%d#KZH-427','')",nMaxId++,nReWndId,x,y,nW,nH,nIndex);
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,3,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nReWndId,x,y,nW,nH);
	db.ExecDML(lpSql);
	 
	InitInterface427_2_Sub(nReWndId,10,0,nW,nH,nIndex);

	return  nReWndId;
}
int  XTestData::InitInterface427_2_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex)
{
	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	char lpSql[1024];
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	int x,y;
	
	CMarkup xmlTitle;
	xmlTitle.AddElem("Attr");
	xmlTitle.AddAttrib("Back",1);
	xmlTitle.AddAttrib("BackColor",RGB(255,255,255));
	xmlTitle.AddAttrib("Align",1);
	xmlTitle.AddAttrib("FontSize",120);
	CString strInfo = xmlTitle.GetDoc();  

	int nChildW = nW/2;
	int nChildH = 40;

	x=(nW-nChildW)/2;
	y = 40;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'%d#KZH-427','%s',0,'','','')",nMaxId++,nParentId,x,y,nChildW,nChildH,nIndex,strInfo);
	db.ExecDML(lpSql);
	 

	CString strTitle[]={"大气传感器加温","关闭空调","座舱加温","风挡防冰","进气道防护网","前轮操纵","进气道防护网","导光板"};
	int nDis =5;
	 nChildW =(nW-2*nLeft-9*nDis)/10;
	int nItemW[]={nChildW,nChildW,nChildW*1.5,nChildW,nChildW*1.5,nChildW,nChildW*2,nChildW};
	

	x =nLeft;
	y = nTop +nH*2/5;
	for(int i =0;i<8;i++)
	{
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,3,%d,%d,%d,%d,'%s','',0,'','','')",nMaxId++,nParentId,x,y,nItemW[i],nH*3/5,strTitle[i]);
		db.ExecDML(lpSql);
		x+=(nItemW[i]+nDis);
	}

	nTop = y;
	int nGroupH = nH*3/5;
	x = nLeft;
	y = nTop +nGroupH/2+20;

	 
	CMarkup xmlSwitchStatic;
	xmlSwitchStatic.AddElem("Attr"); 
	xmlSwitchStatic.AddAttrib("BackColor",RGB(205,220,239));
	xmlSwitchStatic.AddAttrib("LineColor",RGB(255,255,255));
	xmlSwitchStatic.AddAttrib("Default",0);

	COLORREF color[]={RGB(0,64,0),RGB(255,0,0)};
	for(int i =0;i<2;i++)
	{
		CMarkup xml;
		xml.AddElem("Color");
		xml.AddAttrib("Index",i);
		xml.AddAttrib("Value",color[i]);
		xmlSwitchStatic.AddChildSubDoc(xml.GetSubDoc());
	}
	CString strSwitchStatic = xmlSwitchStatic.GetSubDoc();

	int nPosX =(nChildW-40)/2;
	for(int i =0;i<2;i++)
	{
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nParentId,x+nPosX,y,40,40,strSwitchStatic);
		db.ExecDML(lpSql);	
		nPosX +=(nChildW+nDis);
	}
	x+=2*(nChildW+nDis);
	y =nTop+20;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'自动','',0,'','','')",nMaxId++,nParentId,x,y,nItemW[2],20);
	db.ExecDML(lpSql);
	y+=20;
	nPosX=(nItemW[2]-40)/2;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nParentId,x+nPosX,y,40,40,strSwitchStatic);
	db.ExecDML(lpSql);	

	y = nTop +nGroupH/2;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'热','',0,'','','')",nMaxId++,nParentId,x,y,nItemW[2]/2,20);
	db.ExecDML(lpSql);
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'冷','',0,'','','')",nMaxId++,nParentId,x+nItemW[2]/2,y,nItemW[2]/2,20);
	db.ExecDML(lpSql);
	y+=20;

	nPosX=(nItemW[2]/2-40)/2;
	for(int i =0;i<2;i++)
	{
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nParentId,x+nPosX,y,40,40,strSwitchStatic);
		db.ExecDML(lpSql);	
		nPosX+=nItemW[2]/2;
	}
	x+=(nItemW[2]+nDis);
	nPosX =(nChildW-40)/2;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nParentId,x+nPosX,y,40,40,strSwitchStatic);
	db.ExecDML(lpSql);	

	
	x+=(nItemW[3]+nDis);
	//4 进气道防护网
	CString str4[]={"抬起","放下"};
	nPosX=(nItemW[4]/2-40)/2;
	for(int i =0;i<2;i++)
	{
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'%s','',0,'','','')",nMaxId++,nParentId,x+i*nItemW[4]/2,y-20,nItemW[4]/2,20,str4[i]);
		db.ExecDML(lpSql);	
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nParentId,x+nPosX,y,40,40,strSwitchStatic);
		db.ExecDML(lpSql);	
		nPosX+=nItemW[4]/2;
	}
	//5前轮操纵
	x+=(nItemW[4]+nDis);
	nPosX=(nItemW[5]-40)/2;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nParentId,x+nPosX,y,40,40,strSwitchStatic);
	db.ExecDML(lpSql);	
	
	//6"进气道防护网",
	x+=(nItemW[5]+nDis);
	CString str6[2][3]={{"X2:A","滑行","着陆"},{"X2:B","抬起","放下"}};
	
	int nW1 = nItemW[6]/3;
	y=nTop+20;
	for(int i =0;i<2;i++)
	{
		
		nPosX =0;
		for(int k=0;k<3;k++)
		{
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'%s','',0,'','','')",nMaxId++,nParentId,x+nPosX,y,nW1,20,str6[i][k]);			
			db.ExecDML(lpSql);	
			nPosX+=nW1;
		}
		y+=20;
		nPosX =0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'开','',0,'','','')",nMaxId++,nParentId,x+nPosX,y,30,20);
		db.ExecDML(lpSql);
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'关','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+20,30,20);
		db.ExecDML(lpSql);
		
		nPosX +=30;

		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,6,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nParentId,x+nPosX,y,20,40);
		db.ExecDML(lpSql);
		

		nPosX=nW1;
		nPosX +=(nW1-40)/2;
		for(int k =0;k<2;k++)
		{
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,5,%d,%d,%d,%d,'','%s',0,'','','')",nMaxId++,nParentId,x+nPosX,y,40,40,strSwitchStatic);
			db.ExecDML(lpSql);	
			nPosX+=nW1;
		}


		y=nTop+nGroupH/2+10;
	}


	x+=(nItemW[6]+nDis);
	 //7"导光板"
	y=nTop;
	int nPosY=20;
	nPosX =0;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'开','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,nItemW[7],20);
	db.ExecDML(lpSql);

	nPosY+=20;
	nPosX=(nItemW[7]-20)/2;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,6,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nParentId,x+nPosX,y+nPosY,20,nGroupH/2);
	db.ExecDML(lpSql);
	nPosY +=(nGroupH/2+10);
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'关','',0,'','','')",nMaxId++,nParentId,x,y+nPosY,nItemW[7],20);
	db.ExecDML(lpSql);

	return 0;

}

int  XTestData::InitInterface391(int nW ,int nH )
{
	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

	//增加窗口
	char lpSql[1024];
	CString strTitle ="KZH-391";
	sprintf(lpSql,"insert into  t_ctrl values(%d,-1,0,10,10,%d,%d,'CPA测试系统','',0,'%s','','')",nMaxId,nW,nH,strTitle);
	db.ExecDML(lpSql); 
	int nDlgId = nMaxId;

	//增加到系统、窗口对应表
	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'KZH-391')",m_nSysId,nDlgId);
	db.ExecDML(lpSql); 


	int nLeft = 20;
	int nTop = 80;
	int nDis = 30;

	int nChildW = (nW -2*nLeft- nDis)/2;
	int nChildH = (nH -2*nTop- 2*nDis)/3;
	int x ,y;
	for(int  i =0;i<5;i++)
	{ 
		x = nLeft +(nChildW+nDis) * (i%2);
		y = nTop +(nChildH +nDis) * (i/2);	 
		InitInterface391_Sub(nMaxId,x,y,nChildW,nChildH,i+1); 

	}

	return nDlgId;

}

int XTestData:: InitInterface391_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,int nIndex)
{
	int nDlgId =0;
	 
	int x = nLeft;
	int y = nTop;

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	nDlgId = nMaxId;

	CString str;
	str.Format("%d#KZH-391",nIndex);

	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'','',0,'%d#KZH-391','','')",nMaxId++,nParentId,WND_type_dlg,nLeft,nTop,nW,nH,nIndex);
	db.ExecDML(lpSql);
		x=0;
	y=10;

	nH -=20;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%d#KZH-391','',0,'%d#KZH-391','','')",nMaxId++,nDlgId,WND_type_group,x,y,nW,nH,nIndex,nIndex);
	db.ExecDML(lpSql);
	 

	int nChildW = nW/2;
	int nChildH = 40;

	x=(nW-nChildW)/2;
	y = 20;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'%d#KZH-391','',0,'','','')",nMaxId++,nDlgId,x,y,nChildW,nChildH,nIndex);
	db.ExecDML(lpSql);


	CString strTitle1[]={"仪表板","操作台","冷光灯","客舱顶灯绿光亮度","仿照度光敏传感器"};

	int nDis =5;
	
	y+=nChildH;
	//GROUP 
	nChildW = (nW - 7*nDis)/8;
	nChildH = (nH  - 60 - 2*nDis)/2;
	int nItemW,nItemH;
 
	int nPosX=0,nPosY=0;

	int GroupW[]={200,200,200,130,130};
	nDis = (nW - 860)/6;
	x = nDis;
 	for(int i =0; i<5;i++)
	{ 
		nPosX =0;
		nPosY =0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x,y, GroupW[i],nChildH,strTitle1[i],strTitle1[i]);
		db.ExecDML(lpSql);

		if(i<3)
		{ 
			nPosY =20;
			nPosX =20;
			nItemH =20;
			nItemW =  GroupW[i]/2;
			//亮度调节
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,"亮度调节","亮度调节");
			db.ExecDML(lpSql);
			nPosY +=30;

			//edit
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nDlgId,WND_type_edit,x+nPosX,y+nPosY,nItemW,nItemH);
			db.ExecDML(lpSql);
			nPosX+=(nItemW );
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,20,nItemH,"v","v");
			db.ExecDML(lpSql);

			nPosY = 20;
			nPosX +=30;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,40,nItemH,"开关","开关");
			db.ExecDML(lpSql);
			nPosY +=30;
		
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,30,30,"开关","开关");
			db.ExecDML(lpSql);
		}
		else 
		{
			nPosY +=40;
			nPosX +=40;
			nItemW =  GroupW[i]/2;
			nItemH =20;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'','',0,'','','')",nMaxId++,nDlgId,WND_type_edit,x+nPosX,y+nPosY,nItemW,nItemH);
			db.ExecDML(lpSql);
			nPosX +=nItemW;
			if(i ==3)
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,30,30,"KΩ","KΩ");
			else
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,30,30,"V","V");
			db.ExecDML(lpSql);
		}
		x+=(nDis+GroupW[i]);
	}

	CString strTitle2[]={"检灯","昼/夜","自动/手动","信号灯开关","电源","导光灯"};
	

	y+=(nChildH+10);
	int GroupW2[]={200,200,113,113,113,113};
	CString sTitle3[2][2]={	{"X1:Y","X1:a"},{"X1:X","X1:b"}	};
	x=nDis;
	for(int i =0;i<6;i++)
	{
		nPosX =0;
		nPosY =0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x,y, GroupW2[i],nChildH,strTitle2[i],strTitle2[i]);
		db.ExecDML(lpSql);
		if(i<2)
		{
			nPosX =GroupW2[i]/4;
			for(int k =0;k<2;k++)
			{
				nPosY=10;				
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,30,30,sTitle3[i][k],sTitle3[i][k]);
				db.ExecDML(lpSql);
				nPosY+=30;
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,30,30,"开关","开关");
				db.ExecDML(lpSql);

				nPosX =GroupW2[i]/2;
			}			 
		}
		else if(i<5)
		{
			nPosX=(GroupW2[i]/2 -20);
			nPosY=40;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,30,30,"开关","开关");
			db.ExecDML(lpSql);
		}
		else 
		{
			nPosX=(GroupW2[i]/2 -20);
			nPosY=10;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,30,30,"开","开");
			db.ExecDML(lpSql);
			nPosY+=20;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,40,nChildH/2,"开关","开关");
			db.ExecDML(lpSql);
			nPosY+=nChildH/2;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,30,30,"关","关");
			db.ExecDML(lpSql);
		}
		
		
		
		x+=(nDis+GroupW2[i]);
	}
	 
	return  nDlgId;

}
int XTestData::InitInterface382(int nW,int nH)
{
	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

	//增加窗口
	char lpSql[1024];
	CString strTitle ="KZH-382";
	sprintf(lpSql,"insert into  t_ctrl values(%d,-1,0,10,10,%d,%d,'CPA测试系统','',0,'%s','','')",nMaxId,nW,nH,strTitle);
	db.ExecDML(lpSql); 
	int nDlgId = nMaxId;

	//增加到系统、窗口对应表
	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'KZH-382')",m_nSysId,nDlgId);
	db.ExecDML(lpSql); 


	int nLeft = 20;
	int nTop = 80;
	int nButtom = 40;
	int nDis = 30;
	int nHDis=20;

	int nChildW = (nW -2*nLeft- nDis)/2;
	int nChildH = (nH -nTop- nButtom - 2*nHDis)/3;
	int x ,y;
	for(int  i =0;i<5;i++)
	{ 
		x = nLeft +(nChildW+nDis) * (i%2);
		y = nTop +(nChildH +nHDis) * (i/2);	 
		InitInterface382_Sub(nMaxId,x,y,nChildW,nChildH,i+1); 

	}

	return nDlgId;
}


int XTestData:: InitInterface382_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,int nIndex)
{
	
	CString str;
	int nChildW ,nChildH ,x,y,nPosX,nPosY,nDis,nSideH,nItemW,nItemH  ;
	int nDlgId =0;

	x = nLeft;
	y = nTop;

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	nDlgId = nMaxId;
	 
	str.Format("%d#KZH-382",nIndex);

	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'','',0,'%d#KZH-382','','')",nMaxId++,nParentId,WND_type_dlg,nLeft,nTop,nW,nH,nIndex);
	db.ExecDML(lpSql);
	x=0;
	y=0;
	 
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%d#KZH-382','',0,'%d#KZH-382','','')",nMaxId++,nDlgId,WND_type_group,x,y,nW-1,nH-1,nIndex,nIndex);
	db.ExecDML(lpSql);

	return 0;
	//x=(nW-nChildW)/2;
	//y = 20;
	//sprintf(lpSql,"insert into  t_ctrl values(%d,%d,2,%d,%d,%d,%d,'%d#KZH-382','',0,'','','')",nMaxId++,nDlgId,x,y,nChildW,nChildH,nIndex);
	//db.ExecDML(lpSql);

	nTop =40;
	nSideH =50;

	nDis =10;
	
	nChildW =180;
	nChildH = (nH  -nSideH-  nDis)/2;

	CString strTitle1[2][2]={{"主警告","主警告备份"},{"信号灯按","导光板"}};
	CString strTitle1_sub[2][2]={{"X2:A","X2:B"},{"X2:C","X2:D"}};
	///主警告","主警告备份
	y =nTop;
	x=nDis;
	nPosX =0;
	nPosY =0;
	for(int k_cnt =0;k_cnt<2;k_cnt++)
	{			
		nPosX=0;
		nPosY =0;
		str = strTitle1[0][k_cnt];
		nItemW = nChildW;
		nItemH =nChildH;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);
		 nPosX=0;
		for(int n=0;n<2;n++)
		{
			nPosY =20;
			str=strTitle1_sub[k_cnt][n];
			nItemW = nChildW/2;
			nItemH =20;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);

			nPosY+=20;
			str="开";			
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);
			
			nPosY+=20;			
			str="开关";			
			nItemW =30;
			nItemH =30;
			nPosX+=30;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);

			nPosY+=nItemH;			
			str="关";			
			nItemW =nChildW/2;
			nItemH =20;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX-30,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);

			nPosX =nChildW/2;
		}
		y+=(nChildH+nDis);
	}
	
	x+=(nChildW+nDis);
	///信号灯按","导光板
	nChildW =130;

	nPosX =0;
	nPosY =0;
	y=nTop;
	for(int i =0;i<2;i++)
	{
		nPosX =0;
		nPosY =0;
		str = strTitle1[1][i];
		nItemW = nChildW;
		nItemH =nChildH;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
		db.ExecDML(lpSql);
		if(i ==0)
		{
			str="信号灯按";
			nItemW = 40;
			nItemH =40;
			nPosX = (nChildW-nItemW)/2;
			nPosY = (nChildH-nIndex)/2;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);

		}
		else
		{	
			nPosX =0;
			nPosY=20;
			nItemW =40;

			nPosX=(nChildW/2-nItemW/2);

			nItemH =20;
			str="开";			
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);

			nPosY+=20;			
			str="导光板";
			
			nItemH =40;

			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);

			nPosY+=nItemH;			
			str="关";						
			nItemH =20;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);

			nPosX =nChildW/2;
		}

		y+=(nChildH+nDis);
	}

	////干扰应急
	x +=(nChildW +nDis);
	y = nTop;
	str ="干扰应急";

	nPosX =0;
	nPosY =0;
	nChildH = nH - nSideH;
	
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
	db.ExecDML(lpSql);

	str="干扰应急";
	nItemH =40;
	nItemW = 40;
	nPosX =(nChildW -nItemW)/2;
	nPosY = (nChildH -nItemH)/2;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql);



	nPosY+=(nChildH+nDis);
	x +=nChildW +nDis;

	CString strTitle2[]={"炸弹引信","导弹供气","武器供电"};

	nChildH = (nH -nSideH -nDis)/3;
	 
	int nX1 =x;	
	y =nTop;
	for(int i =0;i<3;i++)
	{
		nPosX =0;
		nPosY =0;
		str = strTitle2[i];
		nItemH =nChildH;
		nItemW = nChildW;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
		db.ExecDML(lpSql);


		nItemH =40;
		nItemW = 40;
		nPosX =(nChildW -nItemW)/2;
		nPosY = (nChildH -nItemH)/2;
		
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);

		x +=(nChildW+nDis);
	}
	str = "武器应急投弃";
	x= nX1;
	nPosX =0;
	nPosY =0;
	nChildW=nChildW*3+20;
	y +=(nChildH+nDis);
	nChildH = (nH -nSideH -nDis)*2/3;
	
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
	db.ExecDML(lpSql);


	
	nItemW =40;
	nItemH =40;
	
	//nDis = nChildW/ - 4*nItemW)/5;
	
	//x +=nDis;
	CString strTitle3[]={"全投","诱饵切缆","机翼","机身","备份炮","按压"};
	nPosY =0;
	int nIndex=0;
	nPosY=20;
	for(int i =0;i<2;i++)
	{
		
		nItemH =20;
		nItemW =nChildW/4;
		str = strTitle3[nIndex++];		
		nPosX = 0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);

		nItemW =40;
		nItemH =40;
		
		nPosY+=20;	
		nPosX=(nChildW/4 - nItemW)/2;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);
		nPosY+=nItemH;
	}

	x+=nChildW/4;

	str = strTitle3[nIndex++];
	nPosX = (nChildW/4 -nItemW)/2;
	nPosY = (nChildH- nItemH-40)/2;

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql);

	nPosY+=30;		 
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql);
	  
  	x+=nChildW/4;
	nPosY=20;
	for(int i =0;i<2;i++)
	{

		nItemH =20;
		nItemW =nChildW/4;
		str = strTitle3[nIndex++];		
		nPosX = 0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);

		nItemW =40;
		nItemH =40;

		nPosY+=20;	
		nPosX=(nChildW/4 - nItemW)/2;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);
		nPosY+=nItemH;
	}

	x+=nChildW/4;

	str = strTitle3[nIndex++];
	nPosX = (nChildW/4 -nItemW)/2;
	nPosY = (nChildH- nItemH-40)/2;

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql);

	nPosY+=30;		 
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql);
	 
	return  nDlgId;

}

int XTestData::InitInterface434(int nW,int nH)
{
	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

	//增加窗口
	char lpSql[1024];
	CString strTitle ="KZH-434";
	sprintf(lpSql,"insert into  t_ctrl values(%d,-1,0,10,10,%d,%d,'CPA测试系统','',0,'%s','','')",nMaxId,nW,nH,strTitle);
	db.ExecDML(lpSql); 
	int nDlgId = nMaxId;

	//增加到系统、窗口对应表

	CString str = "KZH-434";
	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'%s')",m_nSysId,nDlgId,str);
	db.ExecDML(lpSql); 


	int nLeft = 20;
	int nTop = 80;
	int nButtom = 40;
	int nDis = 30;
	int nHDis=20;

	int nChildW = (nW -2*nLeft- nDis)/2;
	int nChildH = (nH -nTop- nButtom - 2*nHDis)/3;
	int x ,y;
	for(int  i =0;i<5;i++)
	{ 
		x = nLeft +(nChildW+nDis) * (i%2);
		y = nTop +(nChildH +nHDis) * (i/2);	 
		str.Format("%d#KZH-434",i+1);
		InitInterface434_Sub(nMaxId,x,y,nChildW,nChildH,str); 
	}

	return nDlgId;
}
int  XTestData::InitInterface434_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,CString strTitle)
{

	CString str;
	int nChildW ,nChildH ,x,y,nPosX,nPosY,nDis,nSideH,nItemW,nItemH  ;
	int nDlgId =0;
	int nIndex;

	x = nLeft;
	y = nTop;

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	nDlgId = nMaxId;
	 
	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'','',0,'%s','','')",nMaxId++,nParentId,WND_type_dlg,nLeft,nTop,nW,nH,strTitle);
	db.ExecDML(lpSql);
	x=0;
	y=0;

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x,y,nW-1,nH-1,strTitle,strTitle);
	db.ExecDML(lpSql);

	return 0;
	nTop =40;
	nSideH =50;
	
	nDis=20;

	nChildW = (nW-9*nDis)/8;
	nChildH =(nH - nSideH - nDis)/2;

	CString strTitle1[]={"灯检","外部灯检","1接通","2接通","3接通","4接通","5接通","6接通","28应急电源","28应急电源(备份)","请吸氧","氧源低压","伤员呼叫","导光板"};

	x = nDis;
	y = nTop;
	nPosX =0;
	nPosY =0;
	nIndex =0;
	
	for(int i =0;i<8;i++)
	{
		nPosY =0;
		str = strTitle1[nIndex++];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
		db.ExecDML(lpSql);

		str ="开";
		nItemH = 20;
		nItemW = nChildW;
		nPosY = 20;
		nPosX = 0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);
	 
		nItemW =30;
		nItemH = nChildH/2-10;
		nPosY+=20;
		nPosX = (nChildW-nItemW)/2;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);
		
		nPosY += nItemH;
		str ="关";
		nItemH = 20;
		nItemW = nChildW;
		
		nPosX = 0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);
		x +=nChildW+nDis;
	}
	
	nChildW =(nW - 7*nDis)/6-1;

	x =nDis;
	y+=nChildH+nDis;
	nPosX =0;
	nPosY =0;
	for(int i =0;i<6;i++)
	{
		nPosY =0;
		str = strTitle1[nIndex++];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
		db.ExecDML(lpSql);

		str ="开";
		nItemH = 20;
		nItemW = nChildW;
		nPosY = 20;
		nPosX = 0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);

		nItemW =30;
		nItemH = nChildH/2-10;
		nPosY+=20;
		nPosX = (nChildW-nItemW)/2;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);

		nPosY += nItemH;
		str ="关";
		nItemH = 20;
		nItemW = nChildW;

		nPosX = 0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);
		x +=nChildW+nDis;
	}

	return 0;
}
int XTestData::InitInterface604(int nW,int nH)
{
	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

	//增加窗口
	char lpSql[1024];
	CString str = "KZH-604";
	sprintf(lpSql,"insert into  t_ctrl values(%d,-1,0,10,10,%d,%d,'CPA测试系统','',0,'%s','','')",nMaxId,nW,nH,str);
	db.ExecDML(lpSql); 
	int nDlgId = nMaxId;

	//增加到系统、窗口对应表

	
	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'%s')",m_nSysId,nDlgId,str);
	db.ExecDML(lpSql); 


	int nLeft = 20;
	int nTop = 80;
	int nButtom = 40;
	int nDis = 30;
	int nHDis=20;

	int nChildW = (nW -2*nLeft- nDis)/2;
	int nChildH = (nH -nTop- nButtom - 2*nHDis)/3;
	int x ,y;
	for(int  i =0;i<5;i++)
	{ 
		x = nLeft +(nChildW+nDis) * (i%2);
		y = nTop +(nChildH +nHDis) * (i/2);	 
		str.Format("%d#KZH-604",i+1);
		InitInterface604_Sub(nMaxId,x,y,nChildW,nChildH,str); 
	}

	return nDlgId;
}
int XTestData:: InitInterface604_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,CString strTitle)
{
	CString str;
	int nChildW ,nChildH ,x,y,nPosX,nPosY,nDis,nSideH,nItemW,nItemH  ;
	int nDlgId =0;
	int nIndex;

	x = nLeft;
	y = nTop;

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	nDlgId = nMaxId;

	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'','',0,'%s','','')",nMaxId++,nParentId,WND_type_dlg,nLeft,nTop,nW,nH,strTitle);
	db.ExecDML(lpSql);
	x=0;
	y=0;

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x,y,nW-1,nH-1,strTitle,strTitle);
	db.ExecDML(lpSql);
	
	nTop =30;
	nSideH =30;
	nDis = 5;
	nChildW = (nW - 4*nDis)/3;	
	nChildH =(nH - nSideH - nDis)/2;

	CString strTitle1[]={"BFLL/VML","进气道防护网","应急放下起落架","悬挂物投弃按钮","着陆灯","昼夜转换","导光板"};
	CString strTitle2[2][4]={{"X1:21","X1:1","X2:6","X2:1"},{"X3:21","X3:1","X4:6","X4:1"}};


	nIndex =0;
	nPosX =0;
	x =nDis;
	y =nTop;
	for(int i =0;i<3;i++)
	{
		nPosY =0;
		str = strTitle1[nIndex++];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
		db.ExecDML(lpSql); 
		x +=nChildW+nDis;
	}

	y =nTop;
	nPosY+=15;
	for(int k =0;k<2;k++)
	{
		nPosX=0;
		x=nDis;
		nItemW =nChildW/4;
		nItemH =20;
		for(int i =0;i<4;i++)
		{
			str = strTitle2[k][i];
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 
			nPosX+=nItemW;
		}
		y+=20;

		x=nDis;
		for(int i =0;i<4;i++)
		{			
			nPosX=5;
			nItemH=15;
			nItemW=20;
			if(i%2==0)
			{
				str = "开";
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
				db.ExecDML(lpSql); 
				str = "关";
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY+15,nItemW,nItemH,str,str);
				db.ExecDML(lpSql);  
			}
			nPosX +=25;
			nItemH = 30;
			nItemW = 30;
			if(i%2==0)
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			else
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,30,30,str,str);

			db.ExecDML(lpSql); 
			x+=nChildW/4;
		}
		y+=30;		
	}	

	y = nTop;
	x=nChildW+nDis+nDis;
	 
	nPosX =0;
	CString strTitle3[2][5]={{"X1:X16","X1:9","X1:10","作战","训练"},{"X3:16","X3:9","X3:10","",""}};
	 
	x=nChildW+nDis+nDis;
	nPosX =0;
	
	for(int i =0;i<3;i++)
	{
 
		nItemW =nChildW/3;
		nItemH =15;

		str = strTitle3[0][i];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql); 
		if(i>0)
		{
			str = strTitle3[0][i+3];
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY+nItemH,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 
		}
		nPosX+=nItemW;
	}
	y+=nItemH;

	y+=5;
	x=nChildW+nDis+nDis;
	for(int i =0;i<3;i++)
	{
		nPosX=10;

		if(i ==0)
		{
 			str = "开";
			nItemH=15;
			nItemW=20;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 

			str = "关";
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY+15,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);  
		}

		nPosX +=25;
		nItemH = 30;
		nItemW = 30;
		if(i==0)
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		else
		{

			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY+10,30,30,str,str);
		}

		db.ExecDML(lpSql); 
		x+=nChildW/3;
	}
	
	y+=	40;
	x=nChildW+nDis+nDis;
	nPosX =0;

	for(int i =0;i<3;i++)
	{

		nItemW =nChildW/3;
		nItemH =15;

		str = strTitle3[1][i];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql); 		
		nPosX+=nItemW;
	}
	y+=15;
	x=nChildW+nDis+nDis;
	for(int i =0;i<3;i++)
	{
		nPosX=10;

		if(i ==0)
		{
			str = "开";
			nItemH=15;
			nItemW=20;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 

			str = "关";
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY+15,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);  
		}

		nPosX +=25;
		nItemH = 30;
		nItemW = 30;
		if(i==0)
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		else
		{

			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,30,30,str,str);
		}

		db.ExecDML(lpSql); 
		x+=nChildW/3;
	}

	 
	CString strTitle4[2][4]={{"X1:2","X1:20","X2:2","X2:5"},{"X3:2","X3:20","",""}};
	int nItem[2]={4,2};
	y =nTop+15;;
	nPosY =0;
	for(int k =0;k<2;k++)
	{
		nPosX=0;
		x=nDis+2*(nChildW+nDis);
		nItemW =nChildW/4;
		nItemH =20;
		for(int i =0;i<nItem[k];i++)
		{
			str = strTitle4[k][i];
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 
			nPosX+=nItemW;
		}
		y+=20;

		x=nDis+2*(nChildW+nDis);
		for(int i =0;i<nItem[k];i++)
		{			
			nPosX=5;
			nItemH=15;
			nItemW=20;
			if(i%2==0)
			{
				str = "开";
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
				db.ExecDML(lpSql); 
				str = "关";
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY+15,nItemW,nItemH,str,str);
				db.ExecDML(lpSql);  
			}
			nPosX +=25;
			nItemH = 30;
			nItemW = 30;
			if(i%2==0)
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			else
				sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,30,30,str,str);

			db.ExecDML(lpSql); 
			x+=nChildW/4;
		}
		y+=30;		
	}	



	nPosY+=20;

	nChildW = (nW -5*nDis)/9;

	int nGroupW[]={nChildW*4,nChildW*2,nChildW,nChildW*2};

	x =nDis;
	y=nTop+nChildH;
	//y+=nChildH+nDis;
	nPosX =0;
	nPosY =0;

	for(int i =0;i<4;i++)
	{
		nPosY =0;
		str = strTitle1[nIndex++];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nGroupW[i],nChildH,str,str);
		db.ExecDML(lpSql); 
		x +=nGroupW[i]+nDis;
	}



	CString strTitle5[2][5]={{"X1:17","应急1","副油箱1","选投1","干扰弹1"},{"X3:18","应急2","副油箱2","选投2","干扰弹2"}};
	x =nDis;
	y=nTop+nChildH;

	for(int k =0;k<2;k++)
	{
		nPosX =0;
		nPosY =20;
		nItemW=nGroupW[0]/5;
		nItemH = 15;

		str = strTitle5[k][0];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql); 
		nPosX =5;
		nPosY +=15;
		nItemW =15;
		str = "开";
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql); 
		str = "关";
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY+15,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);  
		nPosX+=25;
		nItemW =30;
		nItemH =30;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);  
		y +=nChildH/2-10;

	}
	y=nTop +nChildH;
	nPosX =0;
	nPosY=0;

	for(int k =0;k<2;k++)
	{
		x =nDis+nGroupW[0]/5;
		nPosY =13;		
		nItemW = nGroupW[0]/5;
		nPosX =0;
		for(int i =0;i<4;i++)
		{
			str = strTitle5[k][1+i];			
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 

			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX+25,y+nPosY+22,30,30,str,str);
			db.ExecDML(lpSql); 


			nPosX+=nItemW;
		}
		y +=nChildH/2-8;
		

	}
	x=nDis+nGroupW[0];

	
	CString strTitle6[]={"着/滑行","自动"};
	 
	nPosX =0;
	nPosY =30;
	y=nTop +nChildH;
	for(int i=0;i<2;i++)
	{
		nItemW = nGroupW[1]/2;
		nItemH =20;
		str = strTitle6[i];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql); 

		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX+25,y+nPosY+22,40,40,str,str);
		db.ExecDML(lpSql); 
		nPosX +=nGroupW[1]/2;
	}
	///着陆灯
	x=nDis+nGroupW[0]+nGroupW[1];
	y=nTop +nChildH;

	nItemW = nGroupW[2];
	nItemH =20;
	str = "开";
	nPosX =0;
	nPosY =20;

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql); 

	nPosY+=20;
	nItemW = 40;
	nItemH = nChildH/2;
	nPosX = (nGroupW[2]-nItemW)/2;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql); 
	
	nPosY+=nItemH;
	nItemW = nGroupW[2];
	nItemH = 20;
	nPosX=0;
	
	str = "关";
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql); 

	///导光板


	x=nDis+nGroupW[0]+nGroupW[1]+nGroupW[2];
	y=nTop +nChildH;

	CString strTitle7[]={"蓝白光","夜视灯"};

	for(int i =0;i<2;i++)
	{
		nItemW = nGroupW[3]/2;

		nItemH =20;
		str = strTitle7[i];
		nPosX =0;
		nPosY =15;

		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql); 
		nPosY+=20;

		str = "开";
		nPosX =0;
	 
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql); 

		nPosY+=15;
		nItemW = 40;
		nItemH = nChildH/2-10;
		nPosX = (nGroupW[2]-nItemW)/2;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql); 

		nPosY+=nItemH;
		nItemW = nGroupW[2];
		nItemH = 20;
		nPosX=0;

		str = "关";
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql); 
		x +=nGroupW[3]/2;
	}


	return 0;
}

int XTestData::InitInterface415(int nW,int nH)
{
	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

	//增加窗口
	char lpSql[1024];
	CString strTitle = "KZH-415";
	CString str =strTitle;
	int nDlgId = nMaxId;
	sprintf(lpSql,"insert into  t_ctrl values(%d,-1,0,10,10,%d,%d,'CPA测试系统','',0,'%s','','')",nMaxId++,nW,nH,str);
	db.ExecDML(lpSql); 
	
	
	//增加到系统、窗口对应表


	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'%s')",m_nSysId,nDlgId,str);
	db.ExecDML(lpSql); 


	int nLeft = 20;
	int nTop = 80;
	int nButtom = 40;
	int nDis = 30;
	int nHDis=20;

	int nChildW = (nW -2*nLeft- nDis)/2;
	int nChildH = (nH -nTop- nButtom - 2*nHDis)/3;
	int x ,y;

	for(int  i =0;i<5;i++)
	{ 
		x = nLeft +(nChildW+nDis) * (i%2);
		y = nTop +(nChildH +nHDis) * (i/2);	 
		str.Format("%d#%s",i+1,strTitle);
		 InitInterface415_Sub(nDlgId,x,y,nChildW,nChildH,str); 
	}
	return nDlgId;
}
int XTestData:: InitInterface415_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,CString strTitle)
{
	CString str;
	int nChildW ,nChildH ,x,y,nPosX,nPosY,nDis,nSideH,nItemW,nItemH  ;
	char lpSql[1024];
	int nIndex;
	
	
	
	x = nLeft;
	y = nTop;

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();

	nChildW= nW;
	nChildH = nH;
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	int nDlgId = nMaxId;
	str = strTitle;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'','',0,'%s','','')",nMaxId++,nParentId,WND_type_dlg,x,y,nChildW,nChildH,str);
	db.ExecDML(lpSql);
	x=0;
	y=0;

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x,y,nChildW-1,nChildH-1,str,str);
	db.ExecDML(lpSql);

	nDis = 5;
	nTop = 30;
	nSideH =40;
	


	CString strTitle1[]={"左警灯","左发动机火警","右警灯","右发动机火警","自动/手动","信号灯开关","检灯","昼/夜"," "," "," "," ","导光板","防照度光敏"};
	nIndex =0;
	int nGroupW[]={150,190,150,190,100,100};

	nChildH = (nH -nSideH-nDis)/2;
	nPosX =0;
	nPosY=0;
	y =nTop;
	x=nDis;

	for(int i =0;i<6;i++)
	{
		str = strTitle1[nIndex++];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nGroupW[i],nChildH,str,str);
		db.ExecDML(lpSql); 
		x +=nGroupW[i]+nDis;
	}

	x =nDis;
	y = nTop +nChildH+nDis;
	nChildW =110;
	int nGroupW2[]={110,110,110,110,110,110,100,110};
	for(int i=0;i<8;i++)
	{
		str = strTitle1[nIndex++];
		
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nGroupW2[i],nChildH,str,str);
		db.ExecDML(lpSql); 
		x +=nGroupW2[i]+nDis;
	}

	CString strTitle2[]={"X2:H","X2:J","X1:EE","X1:X","X1:Y",  "X2:F","X2:G","X1:DD","X1:c","X1:b"};
	nIndex =0;
	
	x = nDis;
	y =nTop;
	for (int i_cnt =0;i_cnt<2;i_cnt++)
	{
		nPosX = 0;
		nPosY =40;
		for(int i =0;i<2;i++)
		{	
			nItemW = nGroupW[i_cnt*2]/2;
			nItemH=20;
			str = strTitle2[nIndex++];
			
			nPosX= i*nItemW;

			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 
			
			nItemH =40;
			nPosX =  nItemW*i+nItemW/2 - nItemH/2;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY+20,nItemH,nItemH,str,str);
			db.ExecDML(lpSql); 

			
		}
		x+=nGroupW[i_cnt*2]+nDis;
		
		for(int i =0;i<3;i++)
		{	
			nItemW = nGroupW[i_cnt*2+1]/3;
			nItemH=20;
			nPosX =i*nItemW;

			str = strTitle2[nIndex++];
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 

			nItemH =40;
			nPosX = nItemW *i+nItemW/2 - nItemH/2;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY+20,nItemH,nItemH,str,str);
			db.ExecDML(lpSql); 

		}
		x+=nGroupW[i_cnt*2+1]+nDis;
	}

	for(int i =0;i<2;i++)
	{
		nItemH =50;
		nItemW =50;
		nPosX = (nGroupW[4+i]-nItemW)/2;
		nPosY = (nChildH - nItemH)/2;
		str = strTitle1[4+i];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY+20,nItemH,nItemH,str,str);
		db.ExecDML(lpSql); 
		x+=nGroupW[4+i]+nDis;
	}
 
	CString strTitle3[]={"X1:Y","X1:a", "X1:X","X1:b", "X1:X","X1:b", "X1:X","X1:b", "X1:X","X1:b", "X1:X","X1:b", "X1:X","X1:b"};
	x =nDis;
	nIndex =0;
	y = nTop +nChildH+nDis;
	for(int i_cnt =0;i_cnt<6;i_cnt++)
	{
		nItemW = nGroupW2[i_cnt]/2;	 
		nPosY=40;
		for(int i =0;i<2;i++ )
		{ 
			nPosX =i*nItemW;
			nItemH=20;
			str = strTitle3[nIndex++];
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 

			nItemH =40;
			nPosX = nItemW *i+nItemW/2 - nItemH/2;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY+20,nItemH,nItemH,str,str);
			db.ExecDML(lpSql); 
		}
		x +=nGroupW2[i_cnt]+nDis;
	}
	
	y = nTop +nChildH+nDis;
	nPosY =10;
	nPosX =0;
	nItemW = nGroupW2[6];
	str = "开";
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql); 
	
	nItemH = 40;
	nPosX = (nItemW- nItemH)/2;
	nPosY  +=30;
	str ="导光板";
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,nItemH,nItemH,str,str);
	db.ExecDML(lpSql); 

	nPosY +=nItemH;
	nPosX =0; 
	str = "关";
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql); 

	x +=nGroupW2[6]+nDis;

	nItemW = nGroupW2[7]-50;
	nItemH =20;
	nPosX =20;
	nPosY =(nChildH-nItemH)/2;
	
	str="防照度光敏";
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_edit,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
	db.ExecDML(lpSql); 
	
	nPosX+=nItemW;
	nItemW=20;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'V','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str);
	db.ExecDML(lpSql); 

	return 0;
}

int XTestData::InitInterface383A(int nW,int nH)
{
	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

	//增加窗口
	char lpSql[1024];
	CString strTitle = "KZH-383A";
	CString str =strTitle;
	int nDlgId = nMaxId;
	sprintf(lpSql,"insert into  t_ctrl values(%d,-1,0,10,10,%d,%d,'CPA测试系统','',0,'%s','','')",nMaxId++,nW,nH,str);
	db.ExecDML(lpSql); 


	//增加到系统、窗口对应表
	 
	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'%s')",m_nSysId,nDlgId,str);
	db.ExecDML(lpSql); 


	int nLeft = 20;
	int nTop = 80;
	int nButtom = 40;
	int nDis = 30;
	int nHDis=20;

	int nChildW = (nW -2*nLeft- nDis)/2;
	int nChildH = (nH -nTop- nButtom - 2*nHDis)/3;
	int x ,y;

	for(int  i =0;i<5;i++)
	{ 
		x = nLeft +(nChildW+nDis) * (i%2);
		y = nTop +(nChildH +nHDis) * (i/2);	 
		str.Format("%d#%s",i+1,strTitle);
		InitInterface383A_Sub(nDlgId,x,y,nChildW,nChildH,str); 
	}
	return nDlgId;
}
int XTestData:: InitInterface383A_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,CString strTitle)
{
	CString str;
	int nChildW ,nChildH ,x,y,nPosX,nPosY,nDis,nSideH,nItemW,nItemH  ;
	char lpSql[1024];
	int nIndex;
	 

	x = nLeft;
	y = nTop;

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();

	nChildW= nW;
	nChildH = nH;
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	int nDlgId = nMaxId;
	str =strTitle;
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'','',0,'%s','','')",nMaxId++,nParentId,WND_type_dlg,x,y,nChildW,nChildH,str);
	db.ExecDML(lpSql);
	x=0;
	y=0;

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x,y,nChildW-1,nChildH-1,str,str);
	db.ExecDML(lpSql);

	return 0;
	nDis = 5;
	nTop = 30;
	nSideH =40;

	nChildH = (nH-nSideH -nDis)/2;
	nChildW = 200;

	CString strTitle1[2]={"主警告","主警告备份"};
	CString strTitle1_Sub[] ={"X2:A","X2:B","X2:C","X2:D"};

	nIndex =0;
	
	x =nDis;
	y = nTop;
	for(int i =0;i<2;i++)
	{ 
		str = strTitle1[i];
		nPosX =0;
		nPosY =0;
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
		db.ExecDML(lpSql); 


		for(int i_cnt =0;i_cnt<2;i_cnt++)
		{
			nItemW =nChildW/2;
			nItemH =20;
			nPosX =i_cnt*nItemW;
			nPosY=20;
			str = strTitle1_Sub[nIndex++];
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 

			str ="开";
			nPosY+=nItemH;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql); 

			str ="主警告X2:A";
			nPosY+=nItemH;
			nPosX =i_cnt*nItemW +(nItemW-30)/2;
			nItemH = nChildH-80;
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_swtich_button,x+nPosX,y+nPosY,40,nItemH,str,str);
			db.ExecDML(lpSql); 

			nPosY+=nItemH-10;
			nPosX =i_cnt*nItemW;
		
			str ="关";		
			sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
			db.ExecDML(lpSql);
		}

		y +=nChildH+nDis;
	}
	CString strTitle2[]={"干扰应急","导光板","信号灯按压"};
	
	x +=nChildW+nDis;
	nChildW =150;
	nChildH =(nH-nSideH -2*nDis)/3;

	y = nTop;
	for(int i =0;i<3;i++)
	{
		nPosX =0;
		nPosY =0;
		str = strTitle2[i];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
		db.ExecDML(lpSql); 

		nItemH =40;

		nPosX = (nChildW - nItemH)/2;
		nPosY =(nChildH - nItemH)/2;

		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,nItemH,nItemH,str,str);
		db.ExecDML(lpSql); 

		 y+=nChildH+nDis;
	}

	x +=nChildW+nDis;
	y = nTop;
	nChildH = (nH-nSideH -nDis)*3/5;
	nChildW = nW -4*nDis - 350;

	nPosX =0;
	nPosY =0;
	str = "武器应急投弃";
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
	db.ExecDML(lpSql); 

	CString strTitle3[]={"全投","机身","机翼","按压"};
	
	int nOldX = x;
	nItemW = nChildW/4;
	for(int i =0;i<4;i++)
	{
		nPosX = 0;
		if(i%2 ==0)
			nPosY =nChildH/3; 
		else 
			nPosY =30;
		
		nItemH =20;
		str = strTitle3[i];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);

		nItemH =40;
		nPosX = (nItemW-nItemH)/2;
		nPosY +=30;

		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,nItemH,nItemH,str,str);
		db.ExecDML(lpSql); 
		x += nItemW;

	}

	x = nOldX;

	 
	y+=nChildH+nDis;

	nChildH = (nH-nSideH -2*nDis)*2/5;
	nPosX =0;
	nPosY =0;
	str = "方向舵调校";
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x+nPosX,y+nPosY,nChildW,nChildH,str,str);
	db.ExecDML(lpSql);

	CString strTitle4[]={"左","右"};
	nItemW = nChildW/2;
	for(int i =0;i<2;i++)
	{
		nPosX = 0;
		nPosY =20;
		nItemH =20;
		str = strTitle4[i];
		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_static,x+nPosX,y+nPosY,nItemW,nItemH,str,str);
		db.ExecDML(lpSql);

		nItemH =40;
		nPosX = (nItemW-nItemH)/2;
		nPosY +=20;

		sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_switch_static,x+nPosX,y+nPosY,nItemH,nItemH,str,str);
		db.ExecDML(lpSql); 
		x += nItemW;

	}


	return 0;
}


int  XTestData::InitInterface433(int nW ,int nH )
{
	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();

	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;

 
	CMarkup xmlDlg;
	xmlDlg.AddElem("Attr");
	xmlDlg.AddAttrib("Mark","KZH-433");
	CString strInfo = xmlDlg.GetDoc();  

	CString strTitle = "KZH-433";
	char lpSql[1024];
	sprintf(lpSql,"insert into  t_ctrl values(%d,0,%d,0,10,10,%d,%d,'%s','%s',0,'%s','','')",nMaxId,-1,nW,nH,strTitle,strInfo,strTitle);
	db.ExecDML(lpSql); 
	int nDlgId =nMaxId;
		 
	sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'%s')",m_nSysId,nDlgId,strTitle);
	db.ExecDML(lpSql); 


	nMaxId++;
	//table;
	int nTabW = nW -40;
	int nTabH = nH -80;

	sprintf(lpSql,"insert into  t_ctrl values(%d,0,%d,7,20,60,%d,%d,'','',0,'','','')",nMaxId,nDlgId,nTabW,nTabH);
	db.ExecDML(lpSql);
	int nTabId = nMaxId;
	for(int i =0;i<5;i++)
	{
		InitInterface433_Sub(nTabId,1,25,nTabW-3,nTabH-27,i);
	}
	
	return nDlgId;

}
int  XTestData::InitInterface433_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex)
{
	
	CString str;
	int nChildW ,nChildH ,x,y,nPosX,nPosY,nDis,nSideH,nItemW,nItemH  ;
	char lpSql[1024];
 	 

	x = nLeft;
	y = nTop;

	CManageDb_Sqllite &db = CManageDb_Sqllite::GetInstance();

	nChildW= nW;
	nChildH = nH;
	int nMaxId= db.GetMaxId("select max(nId) from t_ctrl")+1;
	int nDlgId = nMaxId;
	str ="KZH-433";
	sprintf(lpSql,"insert into  t_ctrl values(%d,0,%d,%d,%d,%d,%d,%d,'','',0,'%s','','')",nMaxId++,nParentId,WND_type_dlg,x,y,nChildW,nChildH,str);
	db.ExecDML(lpSql);
	x=0;
	y=0;

	sprintf(lpSql,"insert into  t_ctrl values(%d,0,%d,%d,%d,%d,%d,%d,'%s','',0,'%s','','')",nMaxId++,nDlgId,WND_type_group,x,y,nChildW-1,nChildH-1,str,str);
	db.ExecDML(lpSql);

	return nDlgId;
}
