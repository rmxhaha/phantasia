// read a file into memory
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  int length;
  char * buffer;

  ifstream is;
  is.open ("Text.txt", ios::binary );

  // get length of file:
  is.seekg (0, ios::end);
  length = is.tellg();
  is.seekg (0, ios::beg);

  // allocate memory:
  buffer = new char [length];

  // read data as a block:
  is.read (buffer,length);

  is.close();

  cout.write (buffer,length);

  delete[] buffer;
  return 0;
}