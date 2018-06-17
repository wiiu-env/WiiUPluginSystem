#ifndef DYNAMICLINKINGHELPER_H
#define DYNAMICLINKINGHELPER_H

#include "common/retain_vars.h"
#include "dynamic_linking_defines.h"
#include "utils/logger.h"
#include <string>
#include <vector>
#include "RelocationData.h"

class DynamicLinkingHelper {
public:
    static DynamicLinkingHelper *getInstance() {
        if(!instance) {
            instance = new DynamicLinkingHelper();
        }
        return instance;
    }

    static void destroyInstance() {
        if(instance) {
            delete instance;
            instance = NULL;
        }
    }


    /**
        Gets the function entry for a given function name. If the function name is not present in the list, it will be added.

        \param functionName Name of the function
        \return Returns a pointer to the entry which contains the functionName. Null on error or if the list full.
    **/
    dyn_linking_function_t * getOrAddFunctionEntryByName(const char * functionName);

    /**
        Gets the function import entry for a given function name. If the import is not present in the list, it will be added.
        This will return entries for _function_ imports.

        \param importName Name of the function
        \return Returns a pointer to the function import entry which contains the importName. Null on error or if the list full.
    **/
    dyn_linking_import_t * getOrAddFunctionImportByName(const char * importName);


    /**
        Gets the data import entry for a given data name. If the import is not present in the list, it will be added.
        This will return entries for _data_ imports.

        \param importName Name of the data
        \return Returns a pointer to the data import entry which contains the importName. Null on error or if the list full.
    **/
    dyn_linking_import_t * getOrAddDataImportByName(const char * importName);


    /**
        Gets the import entry for a given data name and type. If the import is not present in the list, it will be added.
        This will return entries for _data_ and _function_ imports, depending on the isData parameter.

        \param importName Name of the data
        \param isData Set this to true to return a data import

        \return Returns a pointer to the data import entry which contains the importName. Null on error or if the list full.
    **/
    dyn_linking_import_t * getOrAddImport(const char * importName, bool isData);


    bool addReloationEntry(RelocationData * relocationData);

    bool addReloationEntry(char type, size_t offset, int addend, void *destination, std::string name, ImportRPLInformation * rplInfo);

    bool addReloationEntry(char type, size_t offset, int addend, void *destination, dyn_linking_function_t * functionName, dyn_linking_import_t * importInfo);

    std::vector<dyn_linking_relocation_entry_t *> getAllValidDynamicLinkingRelocations();

    bool fillRelocations(std::vector<dyn_linking_relocation_entry_t *> entries );

    void clearAll();


protected:
private:

        DynamicLinkingHelper() {
        }

        ~DynamicLinkingHelper() {

        }

        static DynamicLinkingHelper *instance;
    };

#endif // DYNAMICLINKINGHELPER_H
