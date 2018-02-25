#include <wups.h>

WUPS_PLUGIN_NAME("NNU Patcher");
WUPS_PLUGIN_DESCRIPTION("Enables eshop access on lower firmware versions.");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("BSD");

DECL_FUNCTION(int, NeedsNetworkUpdate__Q2_2nn3nimFPb, bool * result){   
    *result = 0;
    return 0;
}

WUPS_MUST_REPLACE(NeedsNetworkUpdate__Q2_2nn3nimFPb,WUPS_LOADER_LIBRARY_NN_NIM, NeedsNetworkUpdate__Q2_2nn3nimFPb);
