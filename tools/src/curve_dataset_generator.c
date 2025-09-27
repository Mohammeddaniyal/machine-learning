/**
 * @file curve_dataset_generator.c
 * @brief Generates a CSV dataset representing a curve for ML testing.
 * @ingroup ml-tools-dataset
 * @{
 *
 * @author Mohammed Daniyal
 * @date 2025-09-26
 * @version 1.0
 *
 * This tool creates a CSV file with two columns: an integer X value range and a corresponding
 * calculated Y value based on the formula y = (sin(x/15))^2 * x + 10. It is useful for
 * generating synthetic regression data for testing ML algorithms.
 *
 * Usage:
 *   ./curve_dataset_generator output.csv start_x end_x
 *
 * Parameters:
 *   - output.csv: The path to the CSV file to be generated.
 *   - start_x: The start integer value for the X range.
 *   - end_x: The end integer value for the X range.
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
/**
 * @brief Main entry point for the curve dataset generator tool.
 *
 * Parses command-line arguments, generates curve data, and writes it to a CSV file.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success or with usage info on incorrect arguments.
 *
 * Example:
 * @code
 * ./curve_dataset_generator curve.csv 0 100
 * @endcode
 */
int main(int argc,char *argv[])
{
	double x,y;
	char *target;
	int start_x,end_x;
	FILE *file;
	int j;
	if(argc!=4)
	{
		printf("Specify [target.csv] [start_x] [end_x]\n");
		return 0;
	}
	target=argv[1];
	file=fopen(target,"w");
	fprintf(file,"0,1\n"); // header part
	start_x=atoi(argv[2]);
	end_x=atoi(argv[3]);
	for(j=start_x;j<=end_x;++j)
	{
		x=(double)j;
		y=pow(sin(1.0/15.0*x),2)*x+10;
		fprintf(file,"%lf,%lf\n",x,y);
	}
	fclose(file);
	return 0;
}
/** @} */
