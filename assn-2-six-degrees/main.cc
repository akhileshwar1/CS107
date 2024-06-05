#include "imdb.h"
#include "imdb-utils.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
  cout << "nothing" << endl;
  imdb db("../assn-2-six-degrees-data/little-endian");
  vector<film> credits;
  db.getCredits("akhil", credits); 
}
