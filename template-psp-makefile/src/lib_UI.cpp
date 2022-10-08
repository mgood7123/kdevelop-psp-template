#include <MiniC.h>
#include <UI.h>

extern "C" {
	#include <pspctrl_kernel.h>
	#include <pspdisplay.h>
	#include <pspdisplay_kernel.h>
	#include <pspimpose_driver.h>
	#include <pspsysclib.h>

	void sceImposeHomeButton(long);
}

void* UI::show(Tools & tools) {
    // hide and disable the home screen
    MiniC::log("show_menu: hiding home screen\n");
    sceImposeHomeButton(0);
    int home_enabled = sceImposeGetHomePopup();
    sceImposeSetHomePopup(0);

    // delay
    MiniC::log("show_menu: delaying...\n");
    sceKernelDelayThread(150000);

    // stop the game from receiving input (user mode input block)
    MiniC::log("show_menu: setting button masks\n");
    sceCtrlSetButtonMasks(0xFFFF, 1);  // mask lower 16 bits
    sceCtrlSetButtonMasks(0x10000, 2); // always return home key

    // setup controller
    MiniC::log("show_menu: setting up controller\n");
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    // setup vram
    MiniC::log("show_menu: setting up vram\n");
    void *vram = tools.get_vram(force_pause);
    MiniC::log("show_menu: vram (0x%08X)\n", vram);
    pspDebugScreenClearLineDisable();
    pspDebugScreenSetBackColor(0xFF9C0000);
    pspDebugScreenInitEx(vram, PSP_DISPLAY_PIXEL_FORMAT_565, 0);
    sceDisplaySetFrameBufferInternal(0, vram, 512, PSP_DISPLAY_PIXEL_FORMAT_565, PSP_DISPLAY_SETBUF_NEXTFRAME);

    // pause game
    if(force_pause) {
        MiniC::log("show_menu: pausing game\n");
        tools.pause_game(tools.thread_info, tools.thread_info.thid);
    }
    return vram;
}

void UI::hide(Tools & tools, void* vram) {
    MiniC::log("show_menu: resuming game\n");
    tools.resume_game(tools.thread_info, tools.thread_info.thid);

    // allow the game to receive input
    MiniC::log("show_menu: un-setting button masks\n");
    sceCtrlSetButtonMasks(0x10000, 0); // unset home key
    sceCtrlSetButtonMasks(0xFFFF, 0);  // unset mask

    // hide and enable the home screen
    MiniC::log("show_menu: hiding home screen\n");
    sceImposeSetHomePopup(1);
    sceImposeHomeButton(0);

    // return display to the main screen
    MiniC::log("show_menu: returning screen to normal frame buffer\n");
    sceDisplaySetFrameBufferInternal(0, 0, 512, PSP_DISPLAY_PIXEL_FORMAT_565, PSP_DISPLAY_SETBUF_NEXTFRAME);

    if(vram != (void*)0x44000000) {
        // delay
        MiniC::log("show_menu: delaying...\n");
        sceKernelDelayThread(150000);

        // clear home screen
        MiniC::log("show_menu: clearing screen\n");
        pspDebugScreenSetBackColor(0xFF9C9E39);
        pspDebugScreenClear();
    }
}
