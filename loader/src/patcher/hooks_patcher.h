#ifndef _HOOKS_FUNCTION_PATCHER_H
#define _HOOKS_FUNCTION_PATCHER_H

#include <utils/function_patcher.h>

#ifdef __cplusplus
extern "C" {
#endif

extern hooks_magic_t method_hooks_hooks[];
extern uint32_t method_hooks_size_hooks;
extern volatile uint32_t method_calls_hooks[];

#ifdef __cplusplus
}
#endif

#endif /* _HOOKS_FUNCTION_PATCHER_H */
