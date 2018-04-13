typedef struct {
  int number_of_cents;
  int rest_in_cents;
}Change;

typedef struct {
  int fifty;
  int twenty;
  int fife;
  int two;
  int one;
}Coins;

Coins calc_coins(int in_cent);
