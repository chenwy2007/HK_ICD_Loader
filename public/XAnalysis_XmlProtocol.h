#pragma once
/*
解析XML格式的协议，解析为数据结构
*/
//将sData，按照类型转换增加sToData
void Buf_AddTo_String(string sData,string sDataType,int nDataLen,string &sToData);

class XAnalysis_XmlProtocol
{
public:
	XAnalysis_XmlProtocol(void);
	~XAnalysis_XmlProtocol(void);

	//429协议
	static bool  Analysis_429(string &sInfo,__Protocol_429 &protocol);
	static void Analysis_429_Sub(string &sInfo,vector<__Comm_Protocol_Item>&vec,u8 &nLabel);
	 
	//1553 
	static bool Analysis_1553(string &sInfo,__Protocol_1553 &protocol);

	static bool Analysis_1553(__Comm_Protocol &ProItem,__Protocol_1553 &protocol);

	//统计1的个数
	static int Get_1_Count(u32 nValue);

	static bool Analysis_5276(string sInfo,__Protocol_5276 &protocol);

	//标准格式的基础项
	static bool Analysis_BaseInfo(CMarkup &xml,__Protocol_Base&base);

	//标准格式的协议项
	static bool Analysis_SubItem(string sInfo, string sReply_Condition,vector<__Comm_Protocol_Item>&items,int &nReplay_Condition_ShareId);

	static string GetDefaultValue(vector<__Comm_Protocol_Item>&items,int nDataLen,string sDataUnit);
};

