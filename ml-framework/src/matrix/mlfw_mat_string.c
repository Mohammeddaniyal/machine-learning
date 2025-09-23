#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<mlfw_error.h>
#include<___mlfw_error.h>

extern __thread uint32_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

typedef struct __mlfw_mat_string{
	char ***data;
	dimension_t rows;
	dimension_t columns;
}mlfw_mat_string;


mlfw_mat_string * mlfw_mat_string_create_new(dimension_t rows, dimension_t columns)
{
    mlfw_mat_string *matrix;
    index_t r, k, c;
    mlfw_reset_error();

    if(rows <= 0 || columns <= 0)
    {
        _mlfw_set_error(MLFW_INVALID_MATRIX_DIMENSION_CODE, MLFW_INVALID_MATRIX_DIMENSION, rows, columns);
        return NULL;
    }

    matrix = (mlfw_mat_string *)malloc(sizeof(mlfw_mat_string));
    if(matrix == NULL)
    {
        _mlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, sizeof(mlfw_mat_string));
        return NULL;
    }

    matrix->data = (char ***)malloc(sizeof(char **) * rows);
    if(matrix->data == NULL)
    {
        _mlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, sizeof(char **) * rows);
        free(matrix);
        return NULL;
    }

    for(r = 0; r < rows; ++r)
    {
        matrix->data[r] = (char **)malloc(sizeof(char *) * columns);
        if(matrix->data[r] == NULL)
        {
            _mlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, sizeof(char *) * columns);
            for(k = 0; k < r; ++k) free(matrix->data[k]);
            free(matrix->data);
            free(matrix);
            return NULL;
        }
        else
        {
            for(c = 0; c < columns; ++c) matrix->data[r][c] = NULL; // very veryy IMP reason M1 lec18 12:15
        }
    }

    matrix->rows = rows;
    matrix->columns = columns;
    return matrix;
}

void mlfw_mat_string_destroy(mlfw_mat_string *matrix)
{
	index_t r,c;
    mlfw_reset_error();
	if(matrix==NULL) return;
	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;++c)
		{
			if(matrix->data[r][c]!=NULL) free(matrix->data[r][c]);
		}
		free(matrix->data[r]);
	}
	free(matrix->data);
	free(matrix);
}


mlfw_mat_string * mlfw_mat_string_from_csv(const char *csv_file_name, mlfw_mat_string *matrix, mlfw_row_vec_string **header)
{
    char header_string[1025];
    index_t header_index;    
    int index;
    char m;
    index_t r, c;
    char string[5001]; // 1 extra for \0 (string terminator)
    dimension_t rows, columns;
    FILE *file;

    mlfw_reset_error();

    if(csv_file_name == NULL || header == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, 
                        (csv_file_name == NULL ? "csv_file_name" : "header"));
        return NULL;
    }

    file = fopen(csv_file_name, "r");
    if(file == NULL)
    {
        _mlfw_set_error(MLFW_UNABLE_TO_OPEN_FILE_CODE, MLFW_UNABLE_TO_OPEN_FILE, csv_file_name, "csv_file_name");
        return NULL;
    }

    // read first line to get column count
    columns = 0;
    while(1)
    {
        m = fgetc(file);
        if(feof(file)) break;
        if(m == '\r') continue;
        if(m == ',') columns++;
        if(m == '\n') break;
    }
    columns++; // 0 commas -> 1 column

    *header = mlfw_row_vec_string_create_new(columns);
    if(mlfw_error())
    {
        fclose(file);
        return NULL;
    }

    // rewind and read header line
    rewind(file);
    index = 0;
    header_index = 0;
    while(1)
    {
        m = fgetc(file);
        if(feof(file)) break;
        if(m == '\r') continue;

        if(m == ',')
        {
            header_string[index] = '\0';
            mlfw_row_vec_string_set(*header, header_index, header_string);
            if(mlfw_error())
            {
                fclose(file);
                mlfw_row_vec_string_destroy(*header);
                *header = NULL;
                return NULL;
            }
            header_index++;
            index = 0;
            continue;
        }

        if(m == '\n')
        {
            header_string[index] = '\0';
            mlfw_row_vec_string_set(*header, header_index, header_string);
            if(mlfw_error())
            {
                fclose(file);
                mlfw_row_vec_string_destroy(*header);
                *header = NULL;
                return NULL;
            }
            break;
        }

        header_string[index++] = m;
    }

    // count rows and verify column consistency
    rows = 0;
    columns = 0;
    rewind(file);
    while(1)
    {
        m = fgetc(file);
        if(feof(file)) break;
        if(m == '\r') continue;
        if(rows == 0 && m == ',') columns++;
        if(m == '\n') rows++;
    }
    columns++; // 0 commas -> 1 column

    if(columns != mlfw_row_vec_string_get_size(*header))
    {
        _mlfw_set_error(MLFW_INVALID_MATRIX_HEADER_SIZE_CODE, MLFW_INVALID_MATRIX_HEADER_SIZE, 
                        "header", (uint32_t)mlfw_row_vec_string_get_size(*header), (uint32_t)columns);
        mlfw_row_vec_string_destroy(*header);
        *header = NULL;
        fclose(file);
        return NULL;
    }

    // create or validate matrix
    if(matrix == NULL)
    {
        matrix = mlfw_mat_string_create_new(rows, columns);
        if(matrix == NULL)
        {
            _mlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, sizeof(mlfw_mat_string));
            fclose(file);
            mlfw_row_vec_string_destroy(*header);
            *header = NULL;
            return NULL;
        }
    }
    else
    {
        if(matrix->rows != rows || matrix->columns != columns)
        {
            _mlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE, 
                            MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,
                            "matrix", matrix->rows, matrix->columns, rows, columns);
            fclose(file);
            mlfw_row_vec_string_destroy(*header);
            *header = NULL;
            return NULL;
        }
    }

    // skip header line
    rewind(file);
    while(1)
    {
        m = fgetc(file);
        if(feof(file)) break;
        if(m == '\r') continue;
        if(m == '\n') break;
    }

    // populate matrix
    r = 0;
    c = 0;
    index = 0;
    while(1)
    {
        m = fgetc(file);
        if(feof(file)) break;
        if(m == '\r') continue;

        if(m == ',' || m == '\n')
        {
            string[index] = '\0';
            matrix->data[r][c] = (char *)malloc(sizeof(char) * (strlen(string) + 1));
            if(matrix->data[r][c] == NULL)
            {
                _mlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, strlen(string) + 1);
                fclose(file);
                return NULL;
            }
            strcpy(matrix->data[r][c], string);
            index = 0;
            c++;
            if(c == matrix->columns)
            {
                c = 0;
                r++;
            }
            continue;
        }

        string[index++] = m;
    }

    fclose(file);
    return matrix;
}


void mlfw_mat_string_to_csv(mlfw_mat_string *matrix, const char *csv_file_name, mlfw_row_vec_string *header)
{
    index_t index, r, c;
    dimension_t header_size;
    char *ptr;
    FILE *file;
    char separator;

    mlfw_reset_error();

    if(matrix == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix");
        return;
    }

    if(csv_file_name == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "csv_file_name");
        return;
    }

    if(header == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "header");
        return;
    }

    header_size = mlfw_row_vec_string_get_size(header);
    if(header_size != matrix->columns)
    {
        _mlfw_set_error(MLFW_INVALID_MATRIX_HEADER_SIZE_CODE, MLFW_INVALID_MATRIX_HEADER_SIZE, "header", header_size, (uint32_t)matrix->columns);
        return;
    }

    file = fopen(csv_file_name, "w");
    if(file == NULL)
    {
        _mlfw_set_error(MLFW_UNABLE_TO_CREATE_FILE_CODE, MLFW_UNABLE_TO_CREATE_FILE, csv_file_name, "csv_file_name");
        return;
    }

    // Write header
    for(index = 0; index < header_size; ++index)
    {
        mlfw_row_vec_string_get(header, index, &ptr);
        if(ptr != NULL)
        {
            fputs(ptr, file);
            free(ptr);
        }

        separator = (index == header_size - 1) ? '\n' : ',';
        fputc(separator, file);
    }

    // Write data
    for(r = 0; r < matrix->rows; ++r)
    {
        for(c = 0; c < matrix->columns; ++c)
        {
            if(matrix->data[r][c] != NULL)
                fputs(matrix->data[r][c], file);

            separator = (c == matrix->columns - 1) ? '\n' : ',';
            fputc(separator, file);
        }
    }

    fclose(file);
}


void mlfw_mat_string_get(mlfw_mat_string *matrix, index_t row, index_t column, char **string)
{
    if(string == NULL) 
        return;

    *string = NULL;  // default value

    if(matrix == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix");
        return;
    }

    if(row < 0 || row >= matrix->rows)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, row, "row", 0, matrix->rows - 1);
        return;
    }

    if(column < 0 || column >= matrix->columns)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, column, "column", 0, matrix->columns - 1);
        return;
    }

	if(matrix->data[row][column] == NULL)
	{
    _mlfw_set_error(MLFW_MATRIX_STRING_NOT_SET_CODE, 
                    MLFW_MATRIX_STRING_NOT_SET, 
                    row, column);
    return;
	}

    *string = (char *)malloc(sizeof(char) * (strlen(matrix->data[row][column]) + 1));
    if(*string == NULL)
    {
        _mlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, strlen(matrix->data[row][column]) + 1);
        return;
    }

    strcpy(*string, matrix->data[row][column]);
}


void mlfw_mat_string_set(mlfw_mat_string *matrix, index_t row, index_t column, char *string)
{
    if(matrix == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix");
        return;
    }

    if(string == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "string");
        return;
    }

    if(row < 0 || row >= matrix->rows)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, row, "row", 0, matrix->rows-1);
        return;
    }

    if(column < 0 || column >= matrix->columns)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, column, "column", 0, matrix->columns-1);
        return;
    }

    if(matrix->data[row][column] != NULL)
        free(matrix->data[row][column]);

    matrix->data[row][column] = (char *)malloc(sizeof(char) * (strlen(string) + 1));
    if(matrix->data[row][column] == NULL)
    {
        _mlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, strlen(string)+1);
        return;
    }

    strcpy(matrix->data[row][column], string);
}


void mlfw_mat_string_get_dimensions(mlfw_mat_string *matrix,dimension_t *rows,dimension_t *columns)
{
	mlfw_reset_error();
	if(matrix==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix"); 
		return;
	}
	if(rows==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "rows");
		return;
	}
	if(columns==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "columns"); 
		return;
	}
	*rows=matrix->rows;
	*columns=matrix->columns;
}

mlfw_mat_string *mlfw_mat_string_transpose(mlfw_mat_string *matrix, mlfw_mat_string *transposed_matrix)
{
    index_t r, c;
    char *ptr;
	char we_created_container=0;
	mlfw_reset_error();
    if(matrix == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix");
        return NULL;
    }

    if(transposed_matrix == NULL)
    {
        transposed_matrix = mlfw_mat_string_create_new(matrix->columns, matrix->rows);
        if(mlfw_error()) return NULL;
		we_created_container=1;
    }
    else
    {
        if(transposed_matrix->rows != matrix->columns || transposed_matrix->columns != matrix->rows)
        {
            _mlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"transposed_matrix",transposed_matrix->rows,transposed_matrix->columns,matrix->columns,matrix->rows);
            return NULL;
        }
    }

    for(r = 0; r < matrix->rows; ++r)
    {
        for(c = 0; c < matrix->columns; ++c)
        {
            mlfw_mat_string_get(matrix, r, c, &ptr);
			if(mlfw_error()) goto cleanup;
            mlfw_mat_string_set(transposed_matrix, c, r, ptr);
			if(mlfw_error()) goto cleanup;
            free(ptr);
        }
    }

    return transposed_matrix;
	cleanup:
	if(we_created_container)
	{
		mlfw_mat_string_destroy(transposed_matrix);
		transposed_matrix=NULL;
	}
	return NULL;
}


void mlfw_mat_string_copy(mlfw_mat_string *target, mlfw_mat_string *source,
                          index_t target_row_index, index_t target_column_index,
                          index_t source_from_row_index, index_t source_from_column_index,
                          index_t source_to_row_index, index_t source_to_column_index)
{
    index_t target_r, target_c;
    index_t source_r, source_c;
    char *ptr;

    mlfw_reset_error();

    if(target == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "target");
        return;
    }

    if(source == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "source");
        return;
    }

    if(target_row_index < 0 || target_row_index >= target->rows)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, target_row_index, "target_row_index", 0, target->rows - 1);
        return;
    }

    if(target_column_index < 0 || target_column_index >= target->columns)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, target_column_index, "target_column_index", 0, target->columns - 1);
        return;
    }

    if(source_from_row_index < 0 || source_from_row_index >= source->rows)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, source_from_row_index, "source_from_row_index", 0, source->rows - 1);
        return;
    }

    if(source_from_column_index < 0 || source_from_column_index >= source->columns)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, source_from_column_index, "source_from_column_index", 0, source->columns - 1);
        return;
    }

    if(source_to_row_index < 0 || source_to_row_index >= source->rows)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, source_to_row_index, "source_to_row_index", 0, source->rows - 1);
        return;
    }

    if(source_to_column_index < 0 || source_to_column_index >= source->columns)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, source_to_column_index, "source_to_column_index", 0, source->columns - 1);
        return;
    }

    if(source_from_row_index > source_to_row_index)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,
                        source_from_row_index, source_to_row_index, 0, source->rows - 1);
        return;
    }

    if(source_from_column_index > source_to_column_index)
    {
        _mlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,
                        source_from_column_index, source_to_column_index, 0, source->columns - 1);
        return;
    }

    target_r = target_row_index;
    source_r = source_from_row_index;
    while(source_r <= source_to_row_index)
    {
        target_c = target_column_index;
        source_c = source_from_column_index;
        while(source_c <= source_to_column_index)
        {
            if(target_r >= 0 && target_r < target->rows && target_c >= 0 && target_c < target->columns)
            {
                if(target->data[target_r][target_c] != NULL)
                {
                    free(target->data[target_r][target_c]);
                    target->data[target_r][target_c] = NULL;
                }
                mlfw_mat_string_get(source, source_r, source_c, &ptr);
                if(ptr != NULL)
                {
                    mlfw_mat_string_set(target, target_r, target_c, ptr);
                    free(ptr);
                }
            }
            ++source_c;
            ++target_c;
        }
        ++source_r;
        ++target_r;
    }
}


mlfw_mat_string * mlfw_mat_string_shuffle(mlfw_mat_string *matrix, uint8_t how_many_times_to_shuffle, mlfw_mat_string *shuffled_matrix)
{
    int r;
    index_t a, b;
    index_t c, u, idx;
    index_t end_at_index;
    uint8_t j;
    char *tmp_var;
    char we_created_container = 0;

    mlfw_reset_error();

    if(matrix == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix");
        return NULL;
    }

    if(how_many_times_to_shuffle == 0)
    {
        _mlfw_set_error(MLFW_INVALID_SHUFFLE_COUNT_CODE, MLFW_INVALID_SHUFFLE_COUNT, how_many_times_to_shuffle);
        return NULL;
    }

    if(shuffled_matrix == NULL)
    {   
        shuffled_matrix = mlfw_mat_string_create_new(matrix->rows, matrix->columns);
        if(shuffled_matrix == NULL) return NULL;
        we_created_container = 1;
    }
    else
    {
        if(shuffled_matrix->rows != matrix->rows || shuffled_matrix->columns != matrix->columns)
        {
            _mlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,
                            MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,
                            "shuffled_matrix",
                            shuffled_matrix->rows, shuffled_matrix->columns,
                            matrix->rows, matrix->columns);
            return NULL;
        }
    }

    mlfw_mat_string_copy(shuffled_matrix, matrix, 0, 0, 0, 0, matrix->rows - 1, matrix->columns - 1);
    if(mlfw_error())
    {
        if(we_created_container)
        {
            mlfw_mat_string_destroy(shuffled_matrix);
            shuffled_matrix = NULL;
        }
        return NULL;
    }

    // b is the lower bound, last row index
    b = shuffled_matrix->rows - 1;
    srand(time(NULL));
    end_at_index = shuffled_matrix->rows - 3;

    for(j = 0; j < how_many_times_to_shuffle; ++j)
    {
        for(u = 0; u <= end_at_index; ++u)
        {
            a = u + 1; // upper bound
            r = rand();
            idx = (r % (b - a + 1)) + a;

            // swap the data at u and idx row
            for(c = 0; c < shuffled_matrix->columns; ++c)
            {
                tmp_var = shuffled_matrix->data[u][c];
                shuffled_matrix->data[u][c] = shuffled_matrix->data[idx][c];
                shuffled_matrix->data[idx][c] = tmp_var;
            }
        }
    }

    return shuffled_matrix;
}

mlfw_mat_string * mlfw_mat_string_delete_columns(
    mlfw_mat_string *matrix,
    index_t *indexes,
    dimension_t indexes_size,
    mlfw_mat_string *new_matrix)
{
    char *str;
    index_t i, y;
    index_t r, c;
    index_t m_column_index;
    dimension_t count_what;
    mlfw_mat_string *m;
    dimension_t m_rows, m_columns;
    dimension_t matrix_rows, matrix_columns;
    dimension_t new_matrix_rows, new_matrix_columns;

    mlfw_reset_error();

    if(matrix == NULL)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix");
        return NULL;
    }
    if(indexes == NULL || indexes_size == 0)
    {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "indexes");
        return NULL;
    }

    mlfw_mat_string_get_dimensions(matrix, &matrix_rows, &matrix_columns);

    // Check indexes bounds
    for(i = 0; i < indexes_size; ++i)
    {
        if(indexes[i] < 0 || indexes[i] >= matrix_columns)
        {
            _mlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, indexes[i], "indexes[i]", 0, matrix_columns - 1);
            return NULL;
        }
    }

    // Check duplicates
    y = 0;
    while(y < indexes_size)
    {
        count_what = indexes[y];
        for(i = y + 1; i < indexes_size; ++i)
        {
            if(count_what == indexes[i])
            {
                _mlfw_set_error(MLFW_DUPLICATE_INDEX_CODE, MLFW_DUPLICATE_INDEX, count_what);
                return NULL;
            }
        }
        y++;
    }

    m_rows = matrix_rows;
    m_columns = matrix_columns - indexes_size;

    if(new_matrix == NULL)
    {
        m = mlfw_mat_string_create_new(m_rows, m_columns);
        if(m == NULL) return NULL;
    }
    else
    {
        mlfw_mat_string_get_dimensions(new_matrix, &new_matrix_rows, &new_matrix_columns);
        if(m_rows != new_matrix_rows || m_columns != new_matrix_columns)
        {
            _mlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,
                            MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,
                            "new_matrix",
                            new_matrix_rows, new_matrix_columns,
                            m_rows, m_columns);
            return NULL;
        }
        m = new_matrix;
    }

    m_column_index = 0;
    for(c = 0; c < matrix_columns; ++c)
    {
        for(i = 0; i < indexes_size; ++i)
        {
            if(indexes[i] == c) break;
        }
        if(i < indexes_size) continue; // c found, hence to be ingnored as it is to be deleted

        for(r = 0; r < matrix_rows; ++r)
        {
            mlfw_mat_string_get(matrix, r, c, &str);
            if(str == NULL)
            {   
                if(m->data[r][c] != NULL)
                {
                    free(m->data[r][c]);
                    m->data[r][c] = NULL;
                }
            }
            else
            {
                mlfw_mat_string_set(m, r, m_column_index, str);
                free(str);
            }
        }
        m_column_index++;
    }

    return m;
}
