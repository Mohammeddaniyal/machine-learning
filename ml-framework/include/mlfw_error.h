#ifndef __MLFW__ERROR__
#define __MLFW__ERROR__
#include<inttypes.h>

uint8_t mlfw_error(void);
uint32_t mlfw_get_error_code(void);
void mlfw_get_error_string(char *error_string,uint32_t size);
void mlfw_get_debug_string(char *debug_string,uint32_t size);
uint32_t mlfw_get_error_string_length(void);
uint32_t mlfw_get_debug_string_length(void);
#endif
