#include <PRX.h>

#include <PLUGIN__%{APPNAMELC}.h>

PSP_MODULE_INFO("PLUGIN__%{APPNAMEID}", 0x3007, 1, 0);
PSP_MAIN_THREAD_ATTR(0);

PRX prx;

extern "C" {
	int module_start(int argc, char *argv[]) {
		prx.module_start(argc, argv, "THREAD__%{APPNAMEID}");
		return 0;
	}

	int module_stop(int argc, char *argv[]) {
		prx.module_stop(argc, argv);
		return 0;
	}
}
