#include "imdb.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
  cout << "nothing" << endl;
  imdb db("/media/akhilkandi/DATA/Code/cs107/assn-2-six-degrees-data/little-endian/");
  vector<film> credits;
  vector<string> players;
  db.getCredits("'Agent' Ava Hubbard", credits);
  struct film f;
  f.title = "Tapeheads";
  f.year = 88;
  db.getCast(f, players);
  cout << "film count is " << credits.size() << endl;
  for(film f : credits) {
    cout << f.title << endl;
  }
}
