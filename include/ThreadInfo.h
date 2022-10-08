#ifndef THREAD_INFO_H
#define THREAD_INFO_H

#define MAX_THREAD 64

extern "C" {
    #include <pspsysclib.h>
	#include <pspthreadman.h>
}

class ThreadInfo {
public:
	SceUID thid;
	int thread_count_start, thread_count_now, pause_thid = -1;
	SceUID thread_buf_start[MAX_THREAD], thread_buf_now[MAX_THREAD];
};

#endif
