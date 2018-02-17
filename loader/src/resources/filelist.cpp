/****************************************************************************
 * Resource files.
 * This file is generated automatically.
 * Includes 5 files.
 *
 * NOTE:
 * Any manual modification of this file will be overwriten by the generation.
 *****************************************************************************/
#include <resources/filelist.h>

extern const u8 font_ttf[];
extern const u32 font_ttf_size;

extern const u8 GithubIcon_png[];
extern const u32 GithubIcon_png_size;

extern const u8 HomeButtonIcon_png[];
extern const u32 HomeButtonIcon_png_size;

extern const u8 PlusButtonIcon_png[];
extern const u32 PlusButtonIcon_png_size;

extern const u8 TwitterIcon_png[];
extern const u32 TwitterIcon_png_size;

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

