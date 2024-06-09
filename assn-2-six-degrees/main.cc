#include "imdb.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
  cout << "nothing" << endl;
  imdb db("/media/akhilkandi/DATA/Code/cs107/assn-2-six-degrees-data/little-endian/");
  vector<film> credits;
  db.getCredits("'Agent' Ava Hubbard", credits);
  int offset = db.getActorOffset("'Agent' Ava Hubbard");
  cout << "offset is " << offset << endl;
  cout << "film count is " << credits.size() << endl;
  for(film f : credits) {
    cout << f.title << endl;
  }
}
