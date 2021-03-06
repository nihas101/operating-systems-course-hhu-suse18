#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX 100

#define A_ROWS 3
#define A_COLS 2

#define B_ROWS 2
#define B_COLS 3

#define getAddress(matrix_pointer, current_row, current_column, columns) (matrix_pointer + current_row * columns + current_column)

typedef struct {
  long long* matrix;
  int rows;
  int cols;
}matrix;

matrix* create_matrix(int rows, int cols);
void initialize_matrix(matrix *matrix);
int random_int();
void print_matrix(matrix *mat);
matrix* multiply_matrices(matrix *mat_a, matrix *mat_b);
void multiply_row_col(matrix *mat_a, matrix *mat_b, int row, int col, matrix *mat_c);
void free_matrix(matrix *mat);

int main() {
  srand(time(NULL));
  matrix *matrix_a = create_matrix(A_ROWS, A_COLS);
  matrix *matrix_b = create_matrix(B_ROWS, B_COLS);
  matrix *matrix_c;

  if(matrix_a != NULL && matrix_b != NULL){
    printf("A:\n");
    initialize_matrix(matrix_a);
    print_matrix(matrix_a);
    printf("\nB:\n");
    initialize_matrix(matrix_b);
    print_matrix(matrix_b);
    matrix_c = multiply_matrices(matrix_a, matrix_b);
    printf("\nC:\n");
    print_matrix(matrix_c);
  }

  free_matrix(matrix_a);
  free_matrix(matrix_b);
  free_matrix(matrix_c);

  return 0;
}

matrix* create_matrix(int rows, int cols){
  matrix *mat = malloc(sizeof(matrix));
  if(mat == NULL) return NULL;

  mat->matrix = malloc(sizeof(long long)*rows*cols);
  if(mat == NULL){
    free(mat);
    return NULL;
  }

  mat->rows = rows;
  mat->cols = cols;

  return mat;
}

void initialize_matrix(matrix *mat){
    for(int i=0; i < mat->rows * mat->cols; i++){
      *(mat->matrix + i) = random_int();
    }
}

int random_int(){
  return rand() % MAX;
}

matrix* multiply_matrices(matrix *mat_a, matrix *mat_b){
  matrix *mat_c = create_matrix(mat_a->rows, mat_b->cols);

  for(int x=0; x < mat_a->rows; x++){
    for(int y=0; y < mat_b->cols; y++){
      multiply_row_col(mat_a, mat_b, x, y, mat_c);
    }
  }

  return mat_c;
}

void multiply_row_col(matrix *mat_a, matrix *mat_b, int row, int col, matrix *mat_c){
  *(mat_c->matrix + row*mat_c->cols + col) = 0;
  for(int x=0; x < mat_a->cols; x++){
    *(mat_c->matrix + row*mat_c->cols + col) +=
          (*(mat_a->matrix + row*mat_a->cols + x)) *
          (*(mat_b->matrix + col + x*mat_b->cols));
  }
}

void print_matrix(matrix *mat){
  for(int i=0; i < mat->rows * mat->cols; i++){
    if(i != 0 && i % mat->cols == 0) printf("\n");
    printf("%lld ", *(mat->matrix + i));
  }
  printf("\n");
}

void free_matrix(matrix *mat){
  free(mat->matrix);
  free(mat);
}
