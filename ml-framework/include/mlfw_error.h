#ifndef __MLFW__ERROR__
#define __MLFW__ERROR__
#include<inttypes.h>

uint8_t mlfw_error();
uint32_t mlfw_get_error_code();
void mlfw_get_error_string(char *error_string,uint8_t size);
void mlfw_get_debug_string(char *debig_string,uint_t size);
uint32_t mlfw_get_error_string_length();
uint32_t mlfw_get_debug_string_length();
#endif
