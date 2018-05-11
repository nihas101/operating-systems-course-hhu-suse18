typedef struct matrix {
  long long* matrix;
  int rows;
  int cols;
}matrix;

matrix* create_matrix(int rows, int cols);
void initialize_matrix(matrix *matrix);
matrix* multiply_matrices(matrix *mat_a, matrix *mat_b);
void print_matrix(matrix *mat);
void free_matrix(matrix *mat);
