#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>
#include <ThreadInfo.h>

extern "C" {
	#include <pspsysmem_kernel.h>
}

class Tools {
public:
    ThreadInfo thread_info;
	static uidControlBlock *find_uid_control_block_by_name(const char *name, const char *parent);
	void *get_vram(bool & force_pause);
	void pause_game(ThreadInfo & thread_info, SceUID thread_id);
	void resume_game(ThreadInfo & thread_info, SceUID thread_id);
};

#endif
