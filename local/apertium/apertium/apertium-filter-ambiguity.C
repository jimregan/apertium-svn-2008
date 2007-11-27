#include <cstdlib>
#include <cstdio>

#include <apertium/HMM.H>
#include <apertium/TSXReader.H>

#include <apertium/TaggerWord.H>

using namespace std;

int main(int argc, char *argv[]) {
  TSXReader reader;
  reader.read(argv[1]);

  TaggerWord::setArrayTags(reader.getTaggerData().getArrayTags());
  
  HMM hmm(&(reader.getTaggerData()));
  hmm.filter_ambiguity_classes(stdin, stdout);
  
  return EXIT_SUCCESS;  
}
