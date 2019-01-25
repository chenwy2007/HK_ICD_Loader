#pragma  once

//�豸����
//�豸������
#define DRIVE_TYPE_Modbus	"MODBUS"
#define DRIVE_TYPE_EPH5141	"EPH5141"
#define DRIVE_TYPE_EPH5410	"EPH5410"
#define DRIVE_TYPE_EPH5142	"EPH5142"

#define DRIVE_TYPE_EPH5272	"EPH5272"	//429
#define DRIVE_TYPE_EPH5273	"EPH5273"  //1553B
#define DRIVE_TYPE_EPH5033	"EPH5033A"  
#define DRIVE_TYPE_EPH5276	"EPH5276"  

//��������

#define DATA_TYPE_INT	0
#define DATA_TYPE_INT_LEN	4
#define DATA_TYPE_INT_NAME	"����"

#define DATA_TYPE_FLOAT 1
#define DATA_TYPE_FLOAT_LEN 4
#define DATA_TYPE_FLOAT_NAME	"������"

#define DATA_TYPE_STRING 2
#define DATA_TYPE_STRING_NAME	"�ַ���"

#define DATA_TYPE_DOUBLE 3
#define DATA_TYPE_DOUBLE_LEN 8
#define DATA_TYPE_DOUBLE_NAME	"˫������"

#define DATA_TYPE_CHAR 4
#define DATA_TYPE_CHAR_LEN 1
#define DATA_TYPE_CHAR_NAME	"�ַ�"

//�ڴ��ʱ�����ǰ����ֽڴ�ţ������ǽ���ʱ����unicode����
#define DATA_TYPE_STRING_UNICODE  5
#define DATA_TYPE_STRING_UNICODE_NAME	"UNICODE�ַ�"


//Э������,�����ڴ��Ӧһ��Э��
#define DATA_TYPE_PROTOCL  99
#define DATA_TYPE_PROTOCL_NAME	"Э��"

//�����ڴ滺����
#define DATA_TYPE_BUFFER		100
#define DATA_TYPE_BUFFER_NAME	"������"
#define DATA_TYPE_BUFFER_LEN	1000*4		// ���������ȴ���1000���������ݵ�
 
//xml  node name define 

///����Э���е����ݰ��ַ�������������
#define  PROTOCOL_ITEM_INFO_Number_Name "��ֵ"
#define  PROTOCOL_ITEM_INFO_Number  0
#define  PROTOCOL_ITEM_INFO_String_Name "�ַ�"
#define  PROTOCOL_ITEM_INFO_String  1

#define PROTOCOL_ITEM_DATA_LEN_UNIT_bit		"BIT"
#define PROTOCOL_ITEM_DATA_LEN_UNIT_byte	"BYTE"



///log����
#define LOG_NAME_HK  "hk_log"

#define LOG_HK_Param  "hk_Param"

#define LOG_HK_NAME_5276 "hK_log_5276"

#define LOG_HK_NAME_Proceess  "hK_log_Proceess"

#define LOG_HK_NAME_1553	"HK_LOG_1553"

#define LOG_HK_NAME_CAN		"HK_LOG_CAN"

#define LOG_HK_CALLSTACK	"HK_CALL_STACK"