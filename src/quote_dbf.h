#ifndef QUOTE_DBF_H_
#define QUOTE_DBF_H_

#include <string>
#include <vector>
#include "load_dbf.h"

struct dbf_run_info
{
	bool is_init;
	load_dbf* m_hInstance;
};

class quote_dbf
{
public:
    quote_dbf();
    ~quote_dbf();
    static quote_dbf &get_instance();

public:
	int init(std::string& smsg, const std::string config_path);
	int load_config(const std::string config_path, std::string& smsg);
	void run();
	void start();
	void stop();
	bool is_running();

private:
    static quote_dbf* m_sInstance;

	std::vector<dbf_local_info> m_vec_dbf_info;
	std::vector<dbf_run_info> m_vec_load;

	bool is_runing;
};

#endif