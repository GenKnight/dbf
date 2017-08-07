#ifndef DBF_STRUCT
#define DBF_STRUCT
#pragma pack(push,1)
struct InfoOfDbfHead {
	unsigned char  tab;		//�汾��Ϣ
	unsigned char  year;	//���������
	unsigned char  month;
	unsigned char  day;
	unsigned int   recnum;	//�ļ��м�¼����
	unsigned short offset;	//һ����¼�ֶ���
	unsigned short reclen;	//��¼����
	char  save[20];
};

struct InfoOfDbfIndex {
	unsigned char fname[11];	//�ֶ�����
	unsigned char type;			//�ֶ�����
	int  addr;					//�ֶε�ַ
	unsigned char width;		//�ֶγ���
	unsigned char doc;			//�ֶξ���
	char save[14];
	InfoOfDbfIndex()
	{
	}
} ;

#pragma pack(pop)
#endif