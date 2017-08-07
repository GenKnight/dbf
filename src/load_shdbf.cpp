#include <iostream>
#include "load_shdbf.h"

using namespace std;

load_sh_dbf::load_sh_dbf(dbf_local_info& m_dbf_param)
	:load_dbf(m_dbf_param)
{
	if (m_dbf_param.blocal == false) {
		m_fixed_value = m_dbf_param.m_fixed_value;
		m_variable_field = m_dbf_param.m_variable_field;
	}
}

load_sh_dbf::~load_sh_dbf()
{
	
}

void load_sh_dbf::init_field_info_sys()
{
	file_info m_sh_file;
	int npos;
	m_map_file_info.clear();
	for (size_t i = 0; i < m_variable_field.size(); i++) {
		npos = m_variable_field[i].npos;
		m_sh_file.filedstr = m_dbf_info.my_dbf_index[npos].addr;
		m_sh_file.type = m_dbf_info.my_dbf_index[npos].type;
		if (npos == m_dbf_info.fieldnum -1) {
			m_sh_file.filedsize = m_dbf_info.m_dbf_head.reclen - m_dbf_info.my_dbf_index[npos].addr;
		} else {
			m_sh_file.filedsize = m_dbf_info.my_dbf_index[npos + 1].addr - m_dbf_info.my_dbf_index[npos].addr;
		}
		m_map_file_info[m_variable_field[i].filename] = m_sh_file;
	}
}

void load_sh_dbf::load_data_to_json(char* output, const char* str)
{
	char* pstr, *epstr;
	//comm value
	{
		strcat(output, "{\"");
		//
		strcat(output, m_fixed_value.mdate.filename);
		strcat(output, "\":");	
		strcat(output, m_fixed_value.mdate.default);
		strcat(output, ",\"");
		//
		strcat(output, m_fixed_value.time.filename);
		//strcat(output, "mtime");
		strcat(output, "\":");
		strcat(output, m_fixed_value.time.default);
		strcat(output, ",\"");
		//
		strcat(output, m_fixed_value.ztj.filename);
		strcat(output, "\":");
		strcat(output, m_fixed_value.ztj.default);
		strcat(output, ",\"");
		//
		strcat(output, m_fixed_value.dtj.filename);
		strcat(output, "\":");
		strcat(output, m_fixed_value.dtj.default);
		strcat(output, ",\"");
		//
		strcat(output, m_fixed_value.jsp.filename);
		strcat(output, "\":");
		strcat(output, m_fixed_value.jsp.default);
		strcat(output, ",");
	}

	for (std::map<std::string, file_info>::iterator itmap = m_map_file_info.begin(); itmap != m_map_file_info.end(); itmap++)
	{
		strcat(output, "\"");
		strcat(output, itmap->first.c_str());
		strcat(output, "\":");
		pstr = (char*)(str + itmap->second.filedstr);
		epstr = pstr + itmap->second.filedsize - 1;
		while (*pstr == ' ' && pstr <= epstr) {
			pstr++;
		}
		while (*epstr == ' ' && pstr <= epstr) {
			epstr--;
		}
		char* ptr_output = output + strlen(output);
		if (itmap->second.type == 'c' || itmap->second.type == 'C')
		{
			*ptr_output++ = '\"';
			while (pstr <= epstr) {
				*ptr_output++ = *pstr++;
			}
			*ptr_output++ = '\"';
		}
		else
		{
			while (pstr <= epstr) {
				*ptr_output++ = *pstr++;
			}
		}
		*ptr_output++ = ',';
		*ptr_output = '\0';
	}

	strcat(output, "\"excode\":\"");
	strcat(output, m_dbf_info.m_dbf_local_info.market.c_str());
	strcat(output, "\"}");
}

