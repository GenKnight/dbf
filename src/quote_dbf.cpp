#include <iostream>
#include <map>
#include "base/pugixml.hpp"
#include "base/util.h"
#include "load_dbf.h"
#include "load_shdbf.h"
#include "load_szdbf.h"
#include "quote_dbf.h"
using namespace std;

quote_dbf* quote_dbf::m_sInstance = NULL;

quote_dbf::quote_dbf()
{
}

quote_dbf::~quote_dbf()
{
	for (size_t i = 0; i < m_vec_load.size(); i++) {
		if (m_vec_load.at(i).m_hInstance) {
			m_vec_load.at(i).m_hInstance->stop();
			delete m_vec_load.at(i).m_hInstance;
			m_vec_load.at(i).m_hInstance = NULL;
		}
	}
}

quote_dbf& quote_dbf::get_instance()
{
	if (m_sInstance == NULL) {
        m_sInstance = new quote_dbf();
	}

	return *m_sInstance;
}

int quote_dbf::load_config(const std::string xml_path, std::string& smsg)
{
	int nRet = 0;
	if (xml_path.empty()) {
		return -1;
	}

	std::string tmp;
	pugi::xml_document doc;

	if (!doc.load_file(xml_path.c_str())) {
		return -1;
	}

	pugi::xml_node xroot = doc.child("root");
	if (xroot.empty()) {
		return -1;
	}

	pugi::xml_node xq_source = xroot.child("q_source");
	if (xq_source.empty()) {
		return -1;
	}
	pugi::xml_node xdbf = xq_source.child("dbf");
	if (xdbf.empty()) {
		return -1;
	}
	pugi::xml_node xfielpath = xdbf.child("file_path");
	bool bflag;
	dbf_local_info m_dbf;
	while (!(xfielpath.empty())) {
		bflag = xfielpath.attribute("enable").as_bool();
		if (bflag == true) {
			m_dbf.stopic = xfielpath.child("topic").text().as_string();
			m_dbf.market = xfielpath.child("market").text().as_string();
            m_dbf.nmsec = xfielpath.child("delay").text().as_int();
			m_dbf.inittime = xfielpath.child("init").text().as_string();
			m_dbf.starttime = xfielpath.child("start").text().as_string();
			m_dbf.endtime = xfielpath.child("end").text().as_string();
            m_dbf.file_path = xfielpath.child("path").text().as_string();
			
			pugi::xml_node xhq_field = xfielpath.child("hq_field");
			if (!xhq_field.empty() && xhq_field.attribute("enable").as_bool()) {
				m_dbf.blocal = false;

				pugi::xml_node xdate = xhq_field.child("date");
				tmp = xdate.text().as_string();
				sprintf(m_dbf.m_fixed_value.mdate.filename, "%s", tmp.c_str());
				tmp = xdate.attribute("default").as_string();
				sprintf(m_dbf.m_fixed_value.mdate.default, "%s", tmp.c_str());
				//
				pugi::xml_node xtime = xhq_field.child("time");
				tmp = xtime.text().as_string();
				sprintf(m_dbf.m_fixed_value.time.filename, "%s", tmp.c_str());
				tmp = xtime.attribute("default").as_string();
				sprintf(m_dbf.m_fixed_value.time.default, "%s", tmp.c_str());
				
                //
                pugi::xml_node xjys = xhq_field.child("jys");
                tmp = xjys.text().as_string();
                sprintf(m_dbf.m_fixed_value.jys.filename, "%s", tmp.c_str());
                tmp = xjys.attribute("default").as_string();
                sprintf(m_dbf.m_fixed_value.jys.default, "%s", tmp.c_str());
                //
				pugi::xml_node xztj = xhq_field.child("ztj");
				tmp = xztj.text().as_string();
				sprintf(m_dbf.m_fixed_value.ztj.filename, "%s", tmp.c_str());
				tmp = xztj.attribute("default").as_string();
				sprintf(m_dbf.m_fixed_value.ztj.default, "%s", tmp.c_str());
				//
				pugi::xml_node xdtj = xhq_field.child("dtj");
				tmp = xdtj.text().as_string();
				sprintf(m_dbf.m_fixed_value.dtj.filename, "%s", tmp.c_str());
				tmp = xdtj.attribute("default").as_string();
				sprintf(m_dbf.m_fixed_value.dtj.default, "%s", tmp.c_str());
				//
				pugi::xml_node xjsp = xhq_field.child("jsp");
				tmp = xjsp.text().as_string();
				sprintf(m_dbf.m_fixed_value.jsp.filename, "%s", tmp.c_str());
				tmp = xjsp.attribute("default").as_string();
				sprintf(m_dbf.m_fixed_value.jsp.default, "%s", tmp.c_str());

				pugi::xml_node xfields = xhq_field.child("fields");
				if (xfields.empty()) {
					return -1;
				}

				variable_field m_val_value;
				pugi::xml_node xfield = xfields.child("field");
				while (!(xfield.empty())) {
					tmp = xfield.text().as_string();
					sprintf(m_val_value.filename, "%s", tmp.c_str());
					m_val_value.npos = xfield.attribute("colume").as_int();
					m_dbf.m_variable_field.push_back(m_val_value);
					xfield = xfield.next_sibling("field");
				}
			} else {
				m_dbf.blocal = true;
			}

			m_vec_dbf_info.push_back(m_dbf);
		}
		xfielpath = xfielpath.next_sibling("file_path");
	}

	return 0;
}

int quote_dbf::init(std::string& smsg, const std::string config_path)
{
	int nRet = 0;

	nRet = load_config(config_path, smsg);
	if (nRet != 0) {
		return nRet;
	}

	for (size_t i = 0; i < m_vec_dbf_info.size(); i++) {
		dbf_run_info m_run_info;	
		load_dbf* qs = NULL;
		if (m_vec_dbf_info[i].market == "sh") {
            qs = new load_sh_dbf(m_vec_dbf_info[i]);
			m_run_info.m_hInstance = qs;
			m_run_info.is_init = false;
		} else {
            qs = new load_sz_dbf(m_vec_dbf_info[i]);
			m_run_info.m_hInstance = qs;
			m_run_info.is_init = false;
		}
		m_vec_load.push_back(m_run_info);
	}
	return 0;
}


void quote_dbf::run()
{
	while (is_runing) {
		for (size_t i = 0; i < m_vec_load.size(); i++) {
            if (m_vec_load[i].is_init == false) {
                int nRet = m_vec_load[i].m_hInstance->init();
                if (nRet != 0) {
                    return;
                }
                m_vec_load[i].is_init = true;
                m_vec_load[i].m_hInstance->start();
            } else if (m_vec_load[i].is_init == true) {
				if (m_vec_load[i].m_hInstance->is_running() == false) {
					m_vec_load[i].m_hInstance->stop();
					m_vec_load[i].is_init = false;
				}
			}
		}
		base::util::sleep(1000);
	}
}

void quote_dbf::start()
{
	is_runing = true;
	run();
}

void quote_dbf::stop()
{
	for (size_t i = 0; i < m_vec_load.size(); i++) {
		if (m_vec_load[i].m_hInstance) {
			m_vec_load[i].m_hInstance->stop();
            delete m_vec_load[i].m_hInstance;
            m_vec_load[i].m_hInstance = NULL;
		}
	}
	is_runing = false;
	base::util::sleep(1000);
}

bool quote_dbf::is_running()
{
	return is_runing;
}