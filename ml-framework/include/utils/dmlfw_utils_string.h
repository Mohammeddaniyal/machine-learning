#ifndef __DMLFW__UTILS__STRING__
#define __DMLFW__UTILS__STRING__
#include<inttypes.h>
int dmlfw_strcmp_case_insensitive(const char *left,const char *right);
void dmlfw_uint32_to_binary(uint32_t number,char *string);
#endif
