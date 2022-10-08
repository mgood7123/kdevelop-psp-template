#include <Tools.h>

extern "C" {
	#include <pspctrl.h>
}

class UI {
public:
	bool force_pause = false;
	void* show(Tools & tools);
	void hide(Tools & tools, void* vram);
};
