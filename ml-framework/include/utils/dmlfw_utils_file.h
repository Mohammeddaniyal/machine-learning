#ifndef __DMLFW__UTILS__FILE__
#define __DMLFW__UTILS__FILE__
#include<inttypes.h>
uint64_t dmlfw_get_csv_rows_count(const char *csv_file_name);
uint64_t dmlfw_get_csv_columns_count(const char *csv_file_name);
void dmlfw_get_csv_dimensions(const char *csv_file_name,uint64_t *rows,uint64_t *columns);
#endif
