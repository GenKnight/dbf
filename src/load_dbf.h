#ifndef LOAD_DBF_H
#define LOAD_DBF_H

#include <string>
#include <vector>
#include <map>
#include "dbf_struct.h"
#include "base\thread.h"

#define filename_length 12
#define defaultvalue_length 16

struct file_info
{
	int filedstr;
	int filedsize;
	char type;
};

struct fixed_field
{
	char filename[filename_length];
	char default[defaultvalue_length];
};

struct fixed_value
{
	fixed_field mdate;
	fixed_field time;
	fixed_field jys;
	fixed_field ztj;
	fixed_field dtj;
	fixed_field jsp;
};

struct variable_field
{
    char filename[filename_length];
    int npos;
};

struct  dbf_local_info
{
	std::string file_path;
	std::string market;
	std::string inittime;
	std::string starttime;
	std::string endtime;
	std::string stopic;
	int nmsec;
	bool blocal;
	fixed_value m_fixed_value;
	std::vector <variable_field> m_variable_field;
};

struct dbf_info
{
	struct InfoOfDbfHead m_dbf_head;
	int fieldnum;
	struct InfoOfDbfIndex* my_dbf_index;
	dbf_local_info m_dbf_local_info;
};

class  load_dbf :public base::process_thread
{
public:
	load_dbf(){};
    load_dbf(dbf_local_info& m_dbf_param);
	virtual ~load_dbf();
	int init();
	int load_data();
	int print_data();
	void release();

public:
	virtual void init_field_info_sys() = 0;
	virtual void load_data_to_json(char* output, const char* str) = 0;

public:
	virtual void run();
	virtual void start();
	virtual void stop();

protected:
	dbf_info m_dbf_info;

private:
    FILE * fdbf;
    int fbpos;
	int buffsize;
	char* pnew;
	bool bchange;
};

#endif