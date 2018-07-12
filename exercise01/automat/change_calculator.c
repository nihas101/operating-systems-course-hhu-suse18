#include "change_calculator.h"

Change calc_change(int cent, Change change);

Coins calc_coins(int in_cents){
  Coins coins;
  Change change;
  change.rest_in_cents = in_cents;

  change = calc_change(50, change);
  coins.fifty = change.number_of_cents;

  change = calc_change(20, change);
  coins.twenty = change.number_of_cents;

  change = calc_change(5, change);
  coins.fife = change.number_of_cents;

  change = calc_change(2, change);
  coins.two = change.number_of_cents;
  coins.one = change.rest_in_cents;

  return coins;
}

Change calc_change(int cent, Change change){
  int number_of_cents = 1;
  Change resChange;

  while(change.rest_in_cents >= cent*number_of_cents)
    number_of_cents++;

  resChange.number_of_cents = (number_of_cents-1);
  resChange.rest_in_cents = change.rest_in_cents - (resChange.number_of_cents*cent);

  return resChange;
}
