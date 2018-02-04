#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>

#include <utils/logger.h>

#include "utils.h"

// https://gist.github.com/ccbrown/9722406
void dumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		log_printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			log_printf(" ");
			if ((i+1) % 16 == 0) {
				log_printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					log_printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					log_printf("   ");
				}
				log_printf("|  %s \n", ascii);
			}
		}
	}
}

void *Module_ListAllocate(void *list, size_t entry_size, size_t num, size_t *capacity, size_t *count, size_t default_capacity) {
    void *result;

    while (*capacity < *count + num) {
        if (*count == 0) {
            *(void **)list = malloc(entry_size * default_capacity);

            if (*(void **)list == NULL)
                return NULL;

            *capacity = default_capacity;
        } else {
            void *temp;

            temp = realloc(*(void **)list, entry_size * *capacity * 2);
            if (temp == NULL)
                return NULL;

            *(void **)list = temp;
            *capacity = *capacity * 2;
        }
    }

    result = *(char **)list + entry_size * *count;
    (*count) += num;

    return result;
}
