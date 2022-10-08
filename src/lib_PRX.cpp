#include <MiniC.h>
#include <PRX.h>

extern "C" {
    #include <pspinit.h>
    #include <psploadcore.h>
}

int PRX::main_thread(SceSize args, void *argp) {
    PRX & info = *((PRX*)(argp));
    // wait for sceKernelLibrary to load
    while(!sceKernelFindModuleByName("sceKernelLibrary")) {
        sceKernelDelayThread(1000000);
    }
    sceKernelDelayThread(1000000);
    MiniC::log("main_thread: sceKernelLibrary loaded\n");

    info.running = 1;

    while(info.running) {
        SceCtrlData pad;
        sceCtrlPeekBufferPositive(&pad, 1);
        if (pad.Buttons & PSP_CTRL_HOME && pad.Buttons & PSP_CTRL_START) {
            if (info.STARTED == false) {
                info.STARTED = true;
                info.vram = info.ui.show(info.tools);
            }
        }
        if (pad.Buttons & PSP_CTRL_HOME && pad.Buttons & PSP_CTRL_SELECT) {
            if (info.STARTED == true) {
                info.STARTED = false;
                info.ui.hide(info.tools, info.vram);
            }
        }
        sceKernelDelayThread(15000);
    }

    return sceKernelExitDeleteThread(0);
}

void PRX::module_start(int argc, char *argv[], const char * thread_name) {
    MiniC::log("module_start: module started\n");

    char *path = (char*)argv;
    int last_trail = -1, first_trail = -1;
    int i;

    if(path) {
        for(i = 0; path[i]; i++){
            if(path[i] == '/') {
                if(first_trail < 0) {
                    first_trail = i;
                }
                last_trail = i;
            }
        }

        // get the plugin path
        if(last_trail >= 0 && last_trail < sizeof(plug_path)) {
            memcpy(plug_path, path, last_trail);
            plug_path[last_trail] = 0;
        }

        // get the drive
        if(first_trail >= 0 && first_trail < sizeof(plug_drive)) {
            memcpy(plug_drive, path, first_trail);
            plug_drive[first_trail] = 0;
        }
    }

    strcpy(boot_path, sceKernelInitFileName());

    MiniC::log("module_start: sceKernelInitFileName (%s)\n", boot_path);

    // get thread list
    sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, tools.thread_info.thread_buf_start, MAX_THREAD, &tools.thread_info.thread_count_start);
    // create and start main thread
    tools.thread_info.thid = sceKernelCreateThread(thread_name, &main_thread, 0x18, 0x1000, 0, nullptr);
    MiniC::log("module_start: sceKernelCreateThread (0x%08X)\n", tools.thread_info.thid);
    if(tools.thread_info.thid >= 0) {
        sceKernelStartThread(tools.thread_info.thid, sizeof(PRX), this);
        MiniC::log("module_start: sceKernelStartThread\n");
    }
}

void PRX::module_stop(int argc, char * argv[]) {
    MiniC::log("module_start: module stopped\n");
    running = 0;

    // wait for the thread to finish
    SceUInt time = 100 * 1000;
    int ret = sceKernelWaitThreadEnd(tools.thread_info.thid, &time);

    // force terminate the thread
    if(ret < 0) {
        sceKernelTerminateDeleteThread(tools.thread_info.thid);
    }
}
