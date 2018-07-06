#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define NUMBER_OF_STUDENTS 10

typedef struct{
  int id;
  char name[NUMBER_OF_STUDENTS];
  float grade;
}Student;

Student* create_students(int number_of_students, char initial_name[]);
Student create_student(int id, char initial_name[]);
float generate_random_grade();
void print_students(Student *students);
void print_student(Student student);

int main() {
  char student[10];
  Student *students;

  srand(time(NULL));
  strcpy(student, "Student ");

  students = create_students(NUMBER_OF_STUDENTS, student);

  if(students != NULL)
    print_students(students);

  free(students);

  return 0;
}

Student* create_students(int number_of_students, char initial_name[]){
  Student *student;
  student = malloc(sizeof(Student) * number_of_students);

  if(student == NULL){
    printf("malloc failed!");
    return student;
  }

  for(int i=0; i < number_of_students; i++){
    initial_name[7] = i+'0';
    initial_name[8] = '\0';
    student[i] = create_student(i, initial_name);
  }

  return student;
}

Student create_student(int id, char initial_name[]){
  Student student;
  student.id = id;
  strcpy(student.name, initial_name);
  student.grade = generate_random_grade();

  return student;
}

float generate_random_grade(){
  int random = rand() % 10;
  int grade;

  switch (random) {
    case 0: grade = 1.0; break;
    case 1: grade = 1.3; break;
    case 2: grade = 1.7; break;
    case 3: grade = 2.0; break;
    case 4: grade = 2.3; break;
    case 5: grade = 2.7; break;
    case 6: grade = 3.0; break;
    case 7: grade = 3.2; break;
    case 8: grade = 3.7; break;
    case 9: grade = 4.0; break;
    default: grade = 5.0; break;
  }

  return grade;
}

void print_students(Student* students){
  for(int i=0; i < NUMBER_OF_STUDENTS; i++)
    print_student(students[i]);
}

void print_student(Student student){
  printf("%d: %s ... %1.1f\n", student.id, student.name, student.grade);
}
