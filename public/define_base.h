#pragma  once

//设备类型
//设备驱动名
#define DRIVE_TYPE_Modbus	"MODBUS"
#define DRIVE_TYPE_EPH5141	"EPH5141"
#define DRIVE_TYPE_EPH5410	"EPH5410"
#define DRIVE_TYPE_EPH5142	"EPH5142"

#define DRIVE_TYPE_EPH5272	"EPH5272"	//429
#define DRIVE_TYPE_EPH5273	"EPH5273"  //1553B
#define DRIVE_TYPE_EPH5033	"EPH5033A"  
#define DRIVE_TYPE_EPH5276	"EPH5276"  

//数据类型

#define DATA_TYPE_INT	0
#define DATA_TYPE_INT_LEN	4
#define DATA_TYPE_INT_NAME	"整型"

#define DATA_TYPE_FLOAT 1
#define DATA_TYPE_FLOAT_LEN 4
#define DATA_TYPE_FLOAT_NAME	"浮点型"

#define DATA_TYPE_STRING 2
#define DATA_TYPE_STRING_NAME	"字符串"

#define DATA_TYPE_DOUBLE 3
#define DATA_TYPE_DOUBLE_LEN 8
#define DATA_TYPE_DOUBLE_NAME	"双精度型"

#define DATA_TYPE_CHAR 4
#define DATA_TYPE_CHAR_LEN 1
#define DATA_TYPE_CHAR_NAME	"字符"

//在存放时，还是按多字节存放，仅仅是解析时，按unicode解析
#define DATA_TYPE_STRING_UNICODE  5
#define DATA_TYPE_STRING_UNICODE_NAME	"UNICODE字符"


//协议类型,共享内存对应一个协议
#define DATA_TYPE_PROTOCL  99
#define DATA_TYPE_PROTOCL_NAME	"协议"

//共享内存缓存区
#define DATA_TYPE_BUFFER		100
#define DATA_TYPE_BUFFER_NAME	"缓存区"
#define DATA_TYPE_BUFFER_LEN	1000*4		// 缓存区长度存入1000个浮点数据点
 
//xml  node name define 

///定义协议中的内容按字符处理，还是数字
#define  PROTOCOL_ITEM_INFO_Number_Name "数值"
#define  PROTOCOL_ITEM_INFO_Number  0
#define  PROTOCOL_ITEM_INFO_String_Name "字符"
#define  PROTOCOL_ITEM_INFO_String  1

#define PROTOCOL_ITEM_DATA_LEN_UNIT_bit		"BIT"
#define PROTOCOL_ITEM_DATA_LEN_UNIT_byte	"BYTE"



///log定义
#define LOG_NAME_HK  "hk_log"

#define LOG_HK_Param  "hk_Param"

#define LOG_HK_NAME_5276 "hK_log_5276"

#define LOG_HK_NAME_Proceess  "hK_log_Proceess"

#define LOG_HK_NAME_1553	"HK_LOG_1553"

#define LOG_HK_NAME_CAN		"HK_LOG_CAN"

#define LOG_HK_CALLSTACK	"HK_CALL_STACK"