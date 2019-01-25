#include "StdAfx.h"
#include "XAnalysis_XmlProtocol.h"

//sDataType,���Σ��ַ������ַ�
void  Buf_AddTo_String(string sData,string sDataType,int nDataLen,string &sToData)
{
	if(sData.empty())
		return;
	if(sDataType==DATA_TYPE_INT_NAME)
	{
		u32  nV32 =0;
		u8  nV8=0;
		sscanf(sData.c_str(),"%x",&nV32);
		for(int i=nDataLen-1;i>=0;i--)
		{
			nV8 = nV32>>(8*i);
			sToData+=(char)nV8;
		}
	}
	else if(sDataType == DATA_TYPE_STRING_NAME || sDataType ==DATA_TYPE_CHAR_NAME)
	{
		sToData+=sData;
	}
}
XAnalysis_XmlProtocol::XAnalysis_XmlProtocol(void)
{
}


XAnalysis_XmlProtocol::~XAnalysis_XmlProtocol(void)
{
}
bool XAnalysis_XmlProtocol::Analysis_429(string &sInfo,__Protocol_429 &protocol)
{
	// LOG_Out_3(LOG_NAME_HK,__FUNCTION__,__LINE__," sProtocol=%s ",sInfo.c_str());

	//CMarkup xml(sInfo.c_str());
	//if(!xml.FindElem("Attr"))
	//	return false;

	//string sTxt;
	////Name ��Ver ��channel
	//protocol.sName = xml.GetAttrib("Name");
	//protocol.sVer = xml.GetAttrib("Ver");
	//protocol.nChannel = atoi(xml.GetAttrib("channel"));

	////���ͻ��� 
	//sTxt =xml.GetAttrib("SendMode");
	//if(sTxt ==PROTOCOL_SendMode_1)
	//	protocol.nSendMode =PROTOCOL_SendMode_1_Value;
	//else if(sTxt ==PROTOCOL_SendMode_2)
	//	protocol.nSendMode =PROTOCOL_SendMode_2_Value;
	//else if(sTxt ==PROTOCOL_SendMode_3)
	//	protocol.nSendMode =PROTOCOL_SendMode_3_Value;
	//else
	//	protocol.nSendMode =PROTOCOL_SendMode_0_Value;

	////��������(Kbps)
	//protocol.SendSpeed = atoi(xml.GetAttrib("SendSpeed"));
	//protocol.SendSpeed =protocol.SendSpeed>0?protocol.SendSpeed:100;

	////ʱ����(ms)
	//protocol.nTimes = atoi(xml.GetAttrib("times"));
	//protocol.nTimes =protocol.nTimes>0?protocol.nTimes:100;

	////У��λ
	//sTxt =xml.GetAttrib("parity");
	//if(sTxt==PROTOCOL_Parity_Even)
	//	protocol.nParity = PROTOCOL_Parity_Even_Value;
	//else if(sTxt==PROTOCOL_Parity_Odd)
	//	protocol.nParity = PROTOCOL_Parity_Odd_Value;
	//else
	//	protocol.nParity = PROTOCOL_Parity_No_Value;

	//if(xml.FindChildElem("SubItems"))
	//{
	//	string s =xml.GetChildSubDoc();		
	//	XAnalysis_XmlProtocol::Analysis_429_Sub(s,protocol.items,protocol.nLabel);
	//}
	 
	return true;
}
void XAnalysis_XmlProtocol::Analysis_429_Sub(string &sInfo,vector<__Comm_Protocol_Item>&vec,u8 &nLabel)
{
	//CMarkup xml(sInfo.c_str());
	//if(!xml.FindElem("SubItems"))
	//	return;
	//string  sTxt;
	//while(xml.FindChildElem("item"))
	//{	
	//	 __Comm_Protocol_Item item;
	//	 
	//	 item.nAddr = atoi(xml.GetChildAttrib("addr"));
	//	 item.nDataLen =atoi(xml.GetChildAttrib("length"));

	//	 //Э��������
	//	 sTxt = xml.GetChildAttrib("type");

	//	 if(sTxt ==PROTOCOL_Item_Label)		
	//	 {
	//		 item.nItemType = PROTOCOL_Item_Label_Value;
	//	 }
	//	 else if(sTxt ==PROTOCOL_Item_Data)
	//		 item.nItemType = PROTOCOL_Item_Data_Value;
	//	 else if(sTxt ==PROTOCOL_Item_Parity)
	//		 item.nItemType = PROTOCOL_Item_Parity_Value;
	//	 else
	//	 {
	//		 ///�Ƿ����ݣ��ݲ�������Դͷ��֤������ȷ
	//	 }

	//	 sTxt = xml.GetChildAttrib("data_type");
	//	 if(sTxt==DATA_TYPE_INT_NAME)
	//		 item.nDataType = DATA_TYPE_INT;		 
	//	 else  if(sTxt==DATA_TYPE_FLOAT_NAME)
	//		  item.nDataType = DATA_TYPE_FLOAT;	
	//	 else  if(sTxt==DATA_TYPE_STRING_NAME)
	//		 item.nDataType = DATA_TYPE_STRING;	
	//	 else  if(sTxt==DATA_TYPE_DOUBLE_NAME)
	//		 item.nDataType = DATA_TYPE_DOUBLE;	
	//	 else  if(sTxt==DATA_TYPE_CHAR_NAME)
	//		 item.nDataType = DATA_TYPE_CHAR;	
	//	
	//	 item.sName = xml.GetChildAttrib("name");
	//	 if(item.nItemType == PROTOCOL_Item_Label_Value)		 
	//	 {
	//		/* char lpbuf[16]={"0x90"};
	//		 sscanf(lpbuf,"0x%x",&nLabel);*/
	//		  sscanf(item.sName.c_str(),"0x%x",&nLabel);			 
	//	 }

	//	 item.nShareId = atoi( xml.GetChildAttrib("Shared"));
	//	 item.fCoefficient =atof(xml.GetChildAttrib("coefficient"));
	//	 item.fOffset =atof(xml.GetChildAttrib("offset"));
	//	 
	//	 sTxt =xml.GetChildAttrib("default");
	//	 sscanf(sTxt.c_str(),"0x%x",&item.nDefaultValue);
	//	 
	//	 item.sMark =xml.GetChildAttrib("mark");

	//	 vec.push_back(item);
	//	 
	//}
} 

//ͳ��1�ĸ���
int XAnalysis_XmlProtocol::Get_1_Count(u32 nValue)
{
	int nCount=0;
	u32 nV =0x01;
	for(int i=1;i<31;i++)
	{
		if(nValue *(nV>>i))
		{
			nCount++;
		}
	}
	return nCount;
}

bool XAnalysis_XmlProtocol::Analysis_1553(string &sInfo,__Protocol_1553 &protocol)
{ 	
	protocol.base.items.clear();
	LOG_Out_3(LOG_NAME_HK,__FUNCTION__,__LINE__," sProtocol=%s \n\r ",sInfo.c_str());

	CMarkup xml(sInfo.c_str());
	if(!xml.FindElem("Attr"))
		return false;
	Analysis_BaseInfo(xml,protocol.base);

	protocol.nRtAddr = atoi(xml.GetAttrib(XML_RtAddr));
	protocol.nRtSubAddr = atoi(xml.GetAttrib(XML_RtSubAddr));
	protocol.sInterrupt = xml.GetAttrib(XML_Interrupt);
	protocol.sSrcEquipment = xml.GetAttrib(XML_SourceEquipment);
	protocol.sDestEquipment = xml.GetAttrib(XML_DestEquipment);
	 
	if(xml.FindChildElem("SubItems"))
	{
		string s =xml.GetChildSubDoc();				 
		XAnalysis_XmlProtocol::Analysis_SubItem(s,protocol.base.sReply_Condition,protocol.base.items,protocol.base.nReply_protocol_ShareId);			 
	}  
	return true;
}

bool XAnalysis_XmlProtocol::Analysis_1553(__Comm_Protocol &ProItem,__Protocol_1553 &protocol)
{
	protocol.base.items.clear();
	string& sInfo = ProItem.sProtocol;

	LOG_Out_3(LOG_NAME_HK,__FUNCTION__,__LINE__," sProtocol=%s \n\r ",sInfo.c_str());

	CMarkup xml(sInfo.c_str());
	if(!xml.FindElem("Attr"))
		return false;

	//��ȡbase���ֶ���Ϣ
	__Protocol_Base& base = protocol.base;
	//���ơ�����
	base.sName = ProItem.sName;
	base.sDescribe =ProItem.sProtocolNo;

	//����/���ͣ�����ģʽ
	base.sRecSend = ProItem.nRecOrSend == 1 ? "����":"����";
	base.sSendMode =xml.GetAttrib(XML_SendMode);

	//ˢ��ʱ��
	CString sRefreshTime = xml.GetAttrib(XML_RefreshTime);
	int nPos = sRefreshTime.Find("ms");
	if(nPos > 0)
		base.nRefreshTime = _tstoi(sRefreshTime.Left(nPos));
	else
		base.nRefreshTime = _tstoi(sRefreshTime);

	//���ݵ�λ�����ݳ���
	base.sSubDataUnit =xml.GetAttrib(XML_DataUnit);
	base.nSubDataLen =atoi(xml.GetAttrib(XML_DataLen));

	//�����ڴ�
	base.nSharedId = atoi(xml.GetAttrib(XML_Shared));

	//��ȡBlock����Э��Ļ����ֶ���Ϣ
	protocol.nRtAddr = ProItem.nATAddr;
	protocol.nRtSubAddr = ProItem.nATsubAddr;
	protocol.sInterrupt = xml.GetAttrib(XML_Interrupt);
	protocol.sSrcEquipment = xml.GetAttrib(XML_SourceEquipment);
	protocol.sDestEquipment = xml.GetAttrib(XML_DestEquipment);

	return true;
}

bool XAnalysis_XmlProtocol::Analysis_5276(string sInfo,__Protocol_5276 &protocol)
{
	protocol.base.items.clear();
	LOG_Out_3(LOG_NAME_HK,__FUNCTION__,__LINE__," sProtocol=%s \n\r ",sInfo.c_str());

	CMarkup xml(sInfo.c_str());
	if(!xml.FindElem("Attr"))
		return false;
	protocol.nChannelNo = atoi(xml.GetAttrib(XML_ChannelNo))-1;
	Analysis_BaseInfo(xml,protocol.base);

	if(xml.FindChildElem("SubItems"))
	{
		string s =xml.GetChildSubDoc();				 
		XAnalysis_XmlProtocol::Analysis_SubItem(s,protocol.base.sReply_Condition,protocol.base.items,protocol.base.nReply_protocol_ShareId);			 
	}
	//��ñ��ĵı�־����־������ͷ�����ȣ�������
	protocol.sHeaderFlag="";
	CString sTxt;

	for(int i =0,end = protocol.base.items.size();i<end;i++)
	{
		__Comm_Protocol_Item& item = protocol.base.items[i];
		sTxt = item.sItemType.c_str();
		sTxt.MakeUpper();
		if(sTxt ==PROTOCOL_Item_Header || sTxt == PROTOCOL_Item_Label || sTxt ==PROTOCOL_Item_Data_len ||sTxt ==PROTOCOL_Item_Cmd)
		{
			 Buf_AddTo_String(item.sDefaultValue,item.sDataType,item.nDataLen,protocol.sHeaderFlag);
		}
		else
			break;

	}
	LOG_Out_3(LOG_HK_Param,__FUNCTION__,__LINE__,"protocol.sHeaderFlag = ");
	LOG_OutHex(LOG_HK_Param,__FUNCTION__,__LINE__,protocol.sHeaderFlag.c_str(),protocol.sHeaderFlag.size(),false,false);	

	return true;
}

bool XAnalysis_XmlProtocol::Analysis_BaseInfo(CMarkup &xml,__Protocol_Base&base)
{
	//���ơ�����
	base.sName = xml.GetAttrib(XML_Name);
	base.sDescribe =xml.GetAttrib(XML_Describe);

	//����/���ͣ�����ģʽ
	base.sRecSend = xml.GetAttrib(XML_RecSend);
	base.sSendMode =xml.GetAttrib(XML_SendMode);

	//У��λ��ˢ��ʱ��
	base.sCheeckMode =xml.GetAttrib(XML_CheckMode);
	base.nRefreshTime =atoi(xml.GetAttrib(XML_RefreshTime));

	//���ݵ�λ�����ݳ���
	base.sSubDataUnit =xml.GetAttrib(XML_DataUnit);
	base.nSubDataLen =atoi(xml.GetAttrib(XML_DataLen));

	//Ӧ������������ֵ ��Ӧ��Э��
	base.sReply_Condition  =xml.GetAttrib(XML_Reply_Condition);
	base.sReplay_Value  =xml.GetAttrib(XML_Reply_Value);
	base.sReply_protocol_Name  =xml.GetAttrib(XML_Reply_protocol);
	
	//�����ڴ�
	base.nSharedId = atoi(xml.GetAttrib(XML_Shared));
	return true;
}

bool XAnalysis_XmlProtocol::Analysis_SubItem(string sInfo, string sReply_Condition,vector<__Comm_Protocol_Item>&items,int &nReplay_Condition_ShareId)
{
	CMarkup xml2(sInfo.c_str());
	if(!xml2.FindElem("SubItems"))
		return false;
	CString str = xml2.GetElemContent();
	CMarkup xml(str);

	CString  sTxt;
	CString sItemType;
	CString sDataType;
	while(xml.FindElem("Item"))
	{	
		__Comm_Protocol_Item item;

		//��ʼλ ������
		item.nAddr = atoi(xml.GetAttrib(XML_Item_Addr));
		item.nDataLen =atoi(xml.GetAttrib(XML_Item_Length));

		//Э��������
		sItemType = xml.GetAttrib(XML_Item_Item_type);		
		sItemType.MakeUpper();
		item.sItemType = sItemType;
		
		//��������
		sDataType = xml.GetAttrib(XML_Item_Data_type);
		sDataType.MakeUpper();
		item.sDataType = sDataType;
		
		//ϵ��������ֵ
		item.fCoefficient =atof(xml.GetAttrib(XML_Item_Coefficient));
		item.fOffset =atof(xml.GetChildAttrib(XML_Item_Offset));		

		//����,�����ڴ�
		item.sName = xml.GetAttrib(XML_Name);
		item.nShareId = atoi( xml.GetAttrib(XML_Shared));
		
		//Ĭ��ֵ����ע
		item.sDefaultValue =xml.GetAttrib(XML_Item_Default);
		item.sMark =xml.GetAttrib(XML_Item_Mark);

		//Э��������λ��item
		if(sItemType ==PROTOCOL_Item_Item)
		{
			item.sSubDataUnit =xml.GetAttrib(XML_DataUnit);
			item.nSubDataLen =atoi(xml.GetAttrib(XML_DataLen));

			if(xml.FindChildElem("SubItems"))
			{
				string s =xml.GetChildSubDoc();				 
				XAnalysis_XmlProtocol::Analysis_SubItem(s,sReply_Condition,item.items,nReplay_Condition_ShareId);			 
			}
		}
		else if(item.sName == sReply_Condition)
		{
			nReplay_Condition_ShareId = item.nShareId;
		}
		items.push_back(item);

	}
	return true; 
}

string XAnalysis_XmlProtocol::GetDefaultValue(vector<__Comm_Protocol_Item>&items,int nDataLen,string sDataUnit)
{
	string strRe="";
	if(sDataUnit==PROTOCOL_Data_Len_Unit_Bit)
		strRe.resize(nDataLen/8);
	else
		strRe.resize(nDataLen);

	for(int i =0,end =items.size();i<end;i++)
	{
		__Comm_Protocol_Item &item = items[i];
		if(item.sItemType ==PROTOCOL_Item_Item)
		{
			strRe+=GetDefaultValue(item.items,item.nSubDataLen,item.sSubDataUnit);
		}
		else
		{
			if(item.sDataType ==DATA_TYPE_INT_NAME)
			{
				int nValue = atoi(item.sDefaultValue.c_str());
				nValue =nValue<<item.nAddr;
			}
		}
		

	}

	return strRe;
}