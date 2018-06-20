#include <string.h>
#include "main.h"

int32_t __entry_menu(int32_t argc, char **argv) {
    //! *******************************************************************
    //! *                 Jump to our application                    *
    //! *******************************************************************
    return Menu_Main(argc,argv);
}
