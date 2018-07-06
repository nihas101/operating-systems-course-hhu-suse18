#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "change_calculator.h"

int run_tests(void);
int run_automat(void);
void print_help(void);
int read_user_input(void);
void print_coins(Coins coins);
void print_cents(char value[], int amount);

int main() {
  return run_automat();
}

int run_automat(void){
  int cents;
  Coins result;

  print_help();
  cents = read_user_input();
  result = calc_coins(cents);
  print_coins(result);
  return 0;
}

void print_help(void){
  printf("Type an amount of money in cents to convert to change.\n");
  printf("Amount in cents: ");
}

int read_user_input(void){
  int in_cent;
  scanf("%d", &in_cent);
  return in_cent;
}

void print_coins(Coins coins){
  printf("Coins:\n");
  print_cents("50", coins.fifty);
  print_cents("20", coins.twenty);
  print_cents("5", coins.fife);
  print_cents("2", coins.two);
  print_cents("1", coins.one);
}

void print_cents(char value[], int amount){
  printf("%s Cent(s): %d\n", value, amount);
}
