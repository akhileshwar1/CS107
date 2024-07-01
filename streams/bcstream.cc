/**
 * File: bcstream
 * --------------
 * Provides the implementation of the bcstream
 * class which is nothing byt a buffered character
 * stream.
 */

#include "bcstream.h"
#include "string"
#include <iostream>

using namespace std;

/**
 * Constructor: bcstream
 * ---------------------
 * Constructs an instance of the bcstream class
 * which takes a word from cin and stores it as
 * a buffer.
 */
Bcstream::Bcstream()
{

  std::cout << "Enter some text (end with Ctrl+D or an empty line):" << std::endl;
  string word;
  cin >> word;
  buffer = word;
  index = 0;
}

char Bcstream::getChar()
{
  if(index < buffer.size()) {
    char ch = buffer.at(index);
    ++index;
    return ch;
  }
  else {
    string word;
    cin >> word;
    buffer = word;
    index = 0;
    char ch = buffer.at(index);
    ++index;
    return ch;
  }
}
