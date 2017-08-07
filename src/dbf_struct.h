#ifndef DBF_STRUCT
#define DBF_STRUCT
#pragma pack(push,1)
struct InfoOfDbfHead {
	unsigned char  tab;		//版本信息
	unsigned char  year;	//最近更新年
	unsigned char  month;
	unsigned char  day;
	unsigned int   recnum;	//文件中记录条数
	unsigned short offset;	//一条记录字段数
	unsigned short reclen;	//记录长度
	char  save[20];
};

struct InfoOfDbfIndex {
	unsigned char fname[11];	//字段名称
	unsigned char type;			//字段类型
	int  addr;					//字段地址
	unsigned char width;		//字段长度
	unsigned char doc;			//字段精度
	char save[14];
	InfoOfDbfIndex()
	{
	}
} ;

#pragma pack(pop)
#endif