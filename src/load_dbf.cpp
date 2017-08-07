#include <iostream>
#include <windows.h>
#include <string>
#include "load_dbf.h"
#include "base/util.h"
using namespace std;

load_dbf::load_dbf(dbf_local_info& m_dbf_param)
    : fdbf(NULL),
    pnew(NULL)
{
	m_dbf_info.m_dbf_local_info = m_dbf_param;
}

load_dbf::~load_dbf()
{
	release();
}


int load_dbf::init()
{
	if (fdbf == NULL) {
		if ((fdbf = fopen(m_dbf_info.m_dbf_local_info.file_path.c_str(), "r+b")) == NULL) {
			return -1;
		}
	}
	
	fseek(fdbf, 0L, SEEK_SET);
	fread(&(m_dbf_info.m_dbf_head), sizeof(InfoOfDbfHead), 1, fdbf);
	m_dbf_info.fieldnum = (m_dbf_info.m_dbf_head.offset - 1) / 32 - 1;	
	fseek(fdbf, sizeof(InfoOfDbfHead), SEEK_SET);
	m_dbf_info.my_dbf_index = new InfoOfDbfIndex[m_dbf_info.fieldnum];

	for (int i = 0; i < m_dbf_info.fieldnum; i++) {
		fread(&(m_dbf_info.my_dbf_index[i]), sizeof(InfoOfDbfIndex), 1, fdbf);
	}
	//
    init_field_info_sys();
	//
	fbpos = ftell(fdbf) + 1;
	//
	buffsize = m_dbf_info.m_dbf_head.reclen * m_dbf_info.m_dbf_head.recnum + 1;
    pnew = new char[buffsize];
    if (pnew == NULL) {
		return -1;
	}
    memset(pnew, 0, buffsize);
	return 0;
}

int load_dbf::load_data()
{
	if (fdbf == NULL) {
		return -1;
	}
	fseek(fdbf, fbpos, SEEK_SET);
	fread(pnew, buffsize-1, 1, fdbf);
	return 0;
}

int load_dbf::print_data()
{
	//string output;
    char data[1024];

	char* pstr1;
	pstr1 = pnew;
	pstr1 = pstr1 + m_dbf_info.m_dbf_head.reclen ;
    for (size_t i = 0; i < m_dbf_info.m_dbf_head.recnum-1; i++)
    {
        memset(data, 0, 1024);
        load_data_to_json(data, pstr1);

        printf("this is a quote messagae: %s\n", data);
        pstr1 = pstr1 + m_dbf_info.m_dbf_head.reclen;
    }
	return 0;
}

void load_dbf::start()
{
	base::process_thread::start();
}

void load_dbf::stop()
{
	base::process_thread::stop();
}

void load_dbf::run()
{
	while (is_running()) {
		//read data
		load_data();
        print_data();

        base::util::sleep(10000);
	}
}

void load_dbf::release()
{
    if (pnew) {
        delete[] pnew;
        pnew = NULL;
	}
	if (fdbf) {
		fclose(fdbf);
		fdbf = NULL;
	}
	set_running(false);
}