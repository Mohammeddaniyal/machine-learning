#ifndef __DMLFW__DATA__ENCODER__
#define __DMLFW__DATA__ENCODER__
#include<dmlfw_vector.h>
void dmlfw_encoder_encode_one_hot(char *source,char *target,dmlfw_row_vec_string *columns_to_encode);
void dmlfw_encoder_encode_binary(char *source,char *target,dmlfw_row_vec_string *columns_to_encode);

void dmlfw_encoder_encode(char *source,char *target,dmlfw_row_vec_string *columns_to_encode,char *algorithm);

#endif
