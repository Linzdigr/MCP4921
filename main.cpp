#include <unistd.h>
#include <iostream>

using namespace std;

#include "MCP4921.h"

int main(int argc, char **argv) {

  if(getuid()) {
    printf("This program needs to be run as root.\n");
    exit (1);
  }

  cout << "Creating dac object..." << endl;

  MCP4921 *dac = nullptr;

  try {
    dac = new MCP4921();
  }
  catch(const string &s) {
    cerr << s << endl;
    return -1;
  }

  cout << "Setting DAC value to the max." << endl;

  dac->setRawValue(MCP4921::MAX_DAC_VALUE);

  cout << "DAC setted to " << MCP4921::MAX_DAC_VALUE << endl;

  return 0;
}
