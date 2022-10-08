#include <Tools.h>
#include <UI.h>

class PRX {
public:
	Tools tools;
	UI ui;

	char running = 0;
	char boot_path[255];
	char plug_path[255];
	char plug_drive[10];

	bool STARTED = false;
	void* vram = nullptr;

	static int main_thread(SceSize args, void *argp);
	void module_start(int argc, char *argv[], const char * thread_name);
	void module_stop(int argc, char * argv[]);
};
