/****************************************************************************
 * Resource files.
 * This file is generated automatically.
 * Includes 5 files.
 *
 * NOTE:
 * Any manual modification of this file will be overwriten by the generation.
 *****************************************************************************/
#include <resources/filelist.h>
#include <stdint.h>

extern const uint8_t font_ttf[];
extern const uint32_t font_ttf_size;

extern const uint8_t GithubIcon_png[];
extern const uint32_t GithubIcon_png_size;

extern const uint8_t HomeButtonIcon_png[];
extern const uint32_t HomeButtonIcon_png_size;

extern const uint8_t PlusButtonIcon_png[];
extern const uint32_t PlusButtonIcon_png_size;

extern const uint8_t TwitterIcon_png[];
extern const uint32_t TwitterIcon_png_size;

static ResourceFile ResourceList[] =
{
	{"font.ttf", font_ttf, font_ttf_size, NULL, 0},
	{"GithubIcon.png", GithubIcon_png, GithubIcon_png_size, NULL, 0},
	{"HomeButtonIcon.png", HomeButtonIcon_png, HomeButtonIcon_png_size, NULL, 0},
	{"PlusButtonIcon.png", PlusButtonIcon_png, PlusButtonIcon_png_size, NULL, 0},
	{"TwitterIcon.png", TwitterIcon_png, TwitterIcon_png_size, NULL, 0},
	{NULL, NULL, 0, NULL, 0}
};

ResourceFile * getResourceList(){ return ResourceList; }

