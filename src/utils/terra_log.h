#ifndef __P_TERRA_UTILS_LOG_H
#define __P_TERRA_UTILS_LOG_H

#include "../terra_defs.h"

#include <stdarg.h>
#include <stdio.h>

extern void terra_log_use_file();
extern void terra_log_info(char const * const, ...);
extern void terra_log_error(char const * const, ...);

#endif
