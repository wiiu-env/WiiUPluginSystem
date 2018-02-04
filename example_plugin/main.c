#include <wups.h>
#include <string.h>

WUPS_MODULE_NAME("test module");
WUPS_MODULE_VERSION("v1.0");
WUPS_MODULE_AUTHOR("Maschell");
WUPS_MODULE_LICENSE("BSD");

int func(void);

static int value = 15;

static int my_func(void)
{
   int res = func();
   return 4 * value * res;
}

WUPS_MUST_REPLACE(func,WUPS_LOADER_LIBRARY_COREINIT, my_func);
