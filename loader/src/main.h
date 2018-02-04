#ifndef _MAIN_H_
#define _MAIN_H_

#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/os_types.h>

/* Main */
#ifdef __cplusplus
extern "C" {
#endif

//! C wrapper for our C++ functions
int Menu_Main(int argc, char **argv);
void loadAndProcessElf(const char * elfPath);

#ifdef __cplusplus
}
#endif

#endif
