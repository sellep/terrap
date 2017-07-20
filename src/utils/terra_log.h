#ifndef __P_TERRA_UTILS_LOG_H
#define __P_TERRA_UTILS_LOG_H

#include "../terra_defs.h"

#include <stdarg.h>
#include <stdio.h>

extern BOOL terra_log_use_file;

#define TERRA_LOG_USE_FILE() terra_log_use_file = TRUE;

extern void terra_log_info(char const * const, ...);
extern void terra_log_error(char const * const, ...);

#endif
