#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include "quote_dbf.h"

using namespace std;

void ctrl_c_signal(int signo) {
	if (signo == SIGINT) {
		printf("get sigint sigal----");
        if (quote_dbf::get_instance().is_running()) {
            quote_dbf::get_instance().stop();
		}
		printf("delete dbf_to_mq sucess---\n");
	}
}

int proc_main(std::string config_file, std::string log_config_file)
{
	int nRet = 0;
	std::string msg_error;

    nRet = quote_dbf::get_instance().init(msg_error, config_file);
	if (nRet != 0) {
		return nRet;
	}
    quote_dbf::get_instance().start();

	while (true) {
		Sleep(100000);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	std::string config_file = "./config/dbf-to-mq.xml";
	std::string log_file = "./config/log-config.xml";

	signal(SIGINT, ctrl_c_signal);
	signal(SIGILL, ctrl_c_signal);
	signal(SIGFPE, ctrl_c_signal);
	signal(SIGSEGV, ctrl_c_signal);
	signal(SIGTERM, ctrl_c_signal);
	signal(SIGBREAK, ctrl_c_signal);
	signal(SIGABRT, ctrl_c_signal);
	proc_main(config_file, log_file);

	return 0;
}

