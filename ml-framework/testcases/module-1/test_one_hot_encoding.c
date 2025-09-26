#include<dmlfw_encoder.h>
#include<stdio.h>
int main()
{
	int encoded_columns[3]={1,4,5};
	dmlfw_encoder_encode_one_hot("insurance.csv","one_hot_encoded_insurance_.csv",encoded_columns,3,1);
}
