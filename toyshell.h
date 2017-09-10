
#include <string>
#include <stdlib.h>
#include <string.h>
using namespace std;

struct command{
    int size;
    char *token[100];
};

class ToyShell
{
  command * workCommand;
   public:
    ToyShell();
    ~ToyShell();
      
    void tokenize( string command);
    bool alias();
    void getHistoryCommand(string line);
    void saveHistory();
    
    int execute();
      
    string errorMessage(int status);

    
    int count;
    
    //these may have to be char arrays
    string name;
    string terminator;
    
    string * history;
    int historySize;
    int historyArraySize;
    char * aliases[10][2];
    int aliasSizeX;
    int aliasSizeY;
    void increaseCount();
   private:
    void setShellName(string newName);
    void setShellTerminator(string newTerminator);
    void outputHistory();
    bool newAlias();
    void outputAlias();
    int saveAlias(char * fileName);
    int readAlias(char * fileName);
     
};
