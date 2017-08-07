#ifndef LOAD_SH_DBF_H
#define LOAD_SH_DBF_H

#include <string>
#include <map>
#include "load_dbf.h"

class load_sh_dbf : public load_dbf
{
public:
    load_sh_dbf(dbf_local_info& m_dbf_param);
	~load_sh_dbf();

public:
	virtual void init_field_info_sys();
	virtual void load_data_to_json(char* output, const char* str);
	
private:
	fixed_value m_fixed_value;
	std::vector <variable_field> m_variable_field;
    std::map<std::string, file_info> m_map_file_info;
};

#endif