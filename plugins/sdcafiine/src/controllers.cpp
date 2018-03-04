#include <utils/logger.h>
#include "controllers.h"

VPADData vpad;
s32 vpadError;

KPADData pads[4];
s32 padErrors[4];
u32 padTypes[4];

void initControllers() {
	KPADInit();
}

void releaseControllers() {
	for(int i = 0; i < 4; i++) {
		padErrors[i] = WPADProbe(i, &padTypes[i]);
		if(padErrors[i] == 0) {
			WPADDisconnect(i);
		}
	}

	//Clear buffers so future launches of SDCafiine don't read old data
	memset(&vpad, 0, sizeof(VPADData));
	vpadError = 0;

	memset(pads, 0, sizeof(KPADData) * 4);
	memset(padErrors, 0, sizeof(s32) * 4);
	memset(padTypes, 0, sizeof(u32) * 4);
}

void pollControllers() {
	VPADRead(0, &vpad, 1, &vpadError);

	for(int i = 0; i < 4; i++) {
		padErrors[i] = WPADProbe(i, &padTypes[i]);
		if(padErrors[i] == 0) {
			KPADRead(i, &pads[i], 1);
		}
	}
}

bool isButtonPressed(enum PadButton btn) {
	if(vpadError == 0) {
		switch (btn) {
		case PAD_BTN_A:
			if (vpad.btns_d & VPAD_BUTTON_A) return true;
			break;

		case PAD_BTN_UP:
			if (vpad.btns_d & VPAD_BUTTON_UP) return true;
			break;

		case PAD_BTN_DOWN:
			if (vpad.btns_d & VPAD_BUTTON_DOWN) return true;
			break;

		case PAD_BTN_L:
			if (vpad.btns_d & VPAD_BUTTON_L) return true;
			break;

		case PAD_BTN_R:
			if (vpad.btns_d & VPAD_BUTTON_R) return true;
			break;

		default:
			break;
		}
	}

	for(int i = 0; i < 4; i++) {
		if(padErrors[i] == 0) {
			if(pads[i].device_type < 2) {
				switch(btn) {
				case PAD_BTN_A:
					if (pads[i].btns_d & WPAD_BUTTON_A) return true;
					break;

				case PAD_BTN_UP:
					if (pads[i].btns_d & WPAD_BUTTON_UP) return true;
					break;

				case PAD_BTN_DOWN:
					if (pads[i].btns_d & WPAD_BUTTON_DOWN) return true;
					break;

				case PAD_BTN_L:
					if (pads[i].btns_d & WPAD_BUTTON_1) return true;
					break;

				case PAD_BTN_R:
					if (pads[i].btns_d & WPAD_BUTTON_2) return true;
					break;

				default:
					break;
				}
			}
			else {
				switch(btn) {
				case PAD_BTN_A:
					if (pads[i].classic.btns_d & WPAD_CLASSIC_BUTTON_A) return true;
					break;

				case PAD_BTN_UP:
					if (pads[i].classic.btns_d & WPAD_CLASSIC_BUTTON_UP) return true;
					break;

				case PAD_BTN_DOWN:
					if (pads[i].classic.btns_d & WPAD_CLASSIC_BUTTON_DOWN) return true;
					break;

				case PAD_BTN_L:
					if (pads[i].classic.btns_d & WPAD_CLASSIC_BUTTON_L) return true;
					break;

				case PAD_BTN_R:
					if (pads[i].classic.btns_d & WPAD_CLASSIC_BUTTON_R) return true;
					break;

				default:
					break;
				}
			}
		}
	}

	return false;
}
