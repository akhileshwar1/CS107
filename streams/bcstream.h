#ifndef __bcstream__
#define __bcstream__

/**
 * File: bcstream
 * --------------
 * Encapsulates a buffered character stream
 * that outputs the characters from an internal
 * buffer so that calls to the disk are minimized.
 */

#include <vector>
#include <string>

class Bcstream {

  public:

    /**
     * Constructs a bcsteam by taking a string from cin and 
     * storing it in buffer, from where the successive reads are done.
     */

    Bcstream(); 

    char getChar();

  private:
    std::string buffer;
    int index;

};

#endif
