#include <MiniC.h>
#include <Tools.h>

extern "C" {
    #include <pspthreadman.h>
}


uidControlBlock *Tools::find_uid_control_block_by_name(const char *name, const char *parent) {
    uidControlBlock *entry;
    uidControlBlock *end;

    entry = SysMemForKernel_536AD5E1();

    entry = entry->parent;
    end = entry;
    entry = entry->nextEntry;

    do {
        if(entry->nextChild != entry) {
            do {
                uidControlBlock  *ret = NULL;
                entry = entry->nextChild;

                if(name) {
                    if (strcmp(entry->name, name) == 0) {
                        ret = entry;
                    }
                }

                if(ret) {
                    if(parent && ret->type) {
                        if(strcmp(parent, ret->type->name) == 0) {
                            return ret;
                        }
                    } else {
                        return ret;
                    }
                }
            } while (entry->nextChild != entry->type);
            entry = entry->nextChild;
        }

        entry = entry->nextEntry;
    } while (entry->nextEntry != end);

    return 0;
}

void * Tools::get_vram(bool & force_pause) {
    uidControlBlock *SceImpose_FrameBuffer = find_uid_control_block_by_name("SceImpose_FrameBuffer", NULL);

    MiniC::log("get_vram: SceImpose_FrameBuffer (0x%08X)\n", SceImpose_FrameBuffer);

    if(SceImpose_FrameBuffer) {
        force_pause = 0;
        return (void *)(*(u32*)((u32)SceImpose_FrameBuffer + 0x34) | 0xA0000000);
    } else {
        force_pause = 1;
        return (void*)0x44000000;
    }
}

void Tools::pause_game(ThreadInfo & thread_info, SceUID thread_id) {
    if(thread_info.pause_thid <= 0) {
        thread_info.pause_thid = thread_id;
        sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, thread_info.thread_buf_now, MAX_THREAD, &thread_info.thread_count_now);
        int x, y, match;
        for(x = 0; x < thread_info.thread_count_now; x++) {
            match = 0;
            SceUID tmp_thid = thread_info.thread_buf_now[x];
            for(y = 0; y < thread_info.thread_count_start; y++) {
                if((tmp_thid == thread_info.thread_buf_start[y]) || (tmp_thid == thread_id)) {
                    match = 1;
                    break;
                }
            }
            if(match == 0) {
                sceKernelSuspendThread(tmp_thid);
            }
        }
    }
}

void Tools::resume_game(ThreadInfo & thread_info, SceUID thread_id) {
    if(thread_info.pause_thid == thread_id) {
        thread_info.pause_thid = -1;
        int x, y, match;
        for(x = 0; x < thread_info.thread_count_now; x++) {
            match = 0;
            SceUID tmp_thid = thread_info.thread_buf_now[x];
            for(y = 0; y < thread_info.thread_count_start; y++) {
                if((tmp_thid == thread_info.thread_buf_start[y]) || (tmp_thid == thread_id)) {
                    match = 1;
                    break;
                }
            }
            if(match == 0) {
                sceKernelResumeThread(tmp_thid);
            }
        }
    }
}
