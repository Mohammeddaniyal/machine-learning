#include<inttypes.h>
#include<___mlfw_error.h>

uint8_t mlfw_error();
uint32_t mlfw_get_error_code();
void mlfw_get_error_string(char *error_string,uint8_t size);
void mlfw_get_debig_string(char *debig_string,uint_t size);

