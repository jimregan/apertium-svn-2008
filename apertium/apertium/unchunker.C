#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

void help(const char *progname)
{
  cout << "USAGE: " << progname << " [input [output]]" << endl;
  cout << "Removes chunks from input, generating output in transfer format." << endl;
  exit(EXIT_SUCCESS);
}

void skipUntil(int const symbol, FILE *input)
{
  while(true)
  {
    int aux = fgetc_unlocked(input);
    if(feof(input))
    {
      return;
    }
    else if(aux == '\\')
    {
      aux = fgetc_unlocked(input);
      if(feof(input))
      {
        return;
      }
    }
    else if(aux == symbol)
    {
      return;
    }
  }
}

void outputUntil(int const symbol, FILE *input, FILE *output, 
                 bool outputlast = true)
{
  while(true)
  {
    int aux = fgetc_unlocked(input);
    if(feof(input))
    {
      return;
    }
    
    switch(aux)
    {
      case '\\':
        fputc_unlocked('\\', output);
        aux = fgetc_unlocked(input);
        if(feof(input))
        {
          return;
        }
        fputc_unlocked(aux, output);
        break;
        
      default:
        if(aux == symbol)
        {
          if(outputlast)
          {
            fputc_unlocked(aux, output);
          }
          return;
        }
        else
        {
          fputc_unlocked(aux, output);
        }
        break; 
    }
  }
}

void unchunker(FILE *input, FILE *output)
{
  while(true)
  {
    int symbol = fgetc_unlocked(input);
    if(feof(input))
    {
      return;      
    }
    
    switch(symbol)
    {
      case '\\':
        symbol = fgetc_unlocked(input);
        fputc_unlocked('\\', output);
        fputc_unlocked(symbol, output);
        break;
        
      case '[':
        fputc_unlocked('[', output);
        outputUntil(']', input, output);
        break;
  
      case '^':
        skipUntil('/', input);
        outputUntil('}', input, output, false);
        skipUntil('$', input);
        break;
      
      default:
        fputc_unlocked(symbol, output);
        break;
    }
  }
}

int main(int argc, char *argv[])
{
  FILE *input = stdin, *output = stdout;
  
  switch(argc)
  {
    case 3:
      output = fopen(argv[2], "w");
    case 2:
      input = fopen(argv[1], "r");
    case 1:
      break;   
    default:
      help(basename(argv[0]));
      break;
  } 
    
  unchunker(input, output);
  
  fclose(input);
  fclose(output);
  
  return EXIT_SUCCESS;
}
