
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
    char * getHistoryCommand();
    void saveHistory();
    int execute();
    string errorMessage(int status);

    
    int count;
    
    //these may have to be char arrays
    string name;
    string terminator;
    command * workCommand;
    char * storedA[10];

    // history[10] ??
    
    void increaseCount();
   private:
    void setShellName(char * newName);
    void setShellTerminator(char * newTerminator);
    void outputHistory();
    void newAlias();
    void outputAlias();
    int saveAlias(char * fileName);
    int readAlias(char * fileName);
     
};
