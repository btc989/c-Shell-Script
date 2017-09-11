
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
   
   public:
    ToyShell();
    ~ToyShell();
      
    void tokenize( string command);
    char * alias();
    void getHistoryCommand(string line);
    void saveHistory();
    int execute();
    string errorMessage(int status);
    
    
    
    //these may have to be char arrays
    string name;
    int count;
    string terminator;
    command * workCommand;
    char * storedA[10];
    string * history;
    int historySize;
    int historyArraySize;

    // history[10] ??
    
    void increaseCount();
   private:
    void setShellName(string newName);
    void setShellTerminator(string newTerminator);
    void outputHistory();
    void newAlias();
    void outputAlias();
    int saveAlias(char * fileName);
    int readAlias(char * fileName);
     
};
