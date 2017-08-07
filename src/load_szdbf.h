#ifndef LOAD_SZ_DBF_H
#define LOAD_SZ_DBF_H

#include <map>
#include "load_dbf.h"

class load_sz_dbf : public load_dbf
{
public:
    load_sz_dbf(dbf_local_info& m_dbf_param);
	~load_sz_dbf();

public:
	virtual void init_field_info_sys();
	virtual void load_data_to_json(char* output, const char* str);

private:	
	fixed_value m_fixed_value;
	std::vector <variable_field> m_variable_field;
    std::map<std::string, file_info> m_map_file_info;
};

#endif