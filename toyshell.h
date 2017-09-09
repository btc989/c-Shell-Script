
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
    char * alias( char * workCommand);
    char * getHistoryCommand( char * workCommand);
    void saveHistory( char * workCommand);
    
    int execute( char * workCommand);
      
    string errorMessage(int status);

    
    int count;
    
    //these may have to be char arrays
    string name;
    string terminator;
    
    // history[10] ??
    //aliases[10][2] ??
    void increaseCount();
   private:
    void setShellName(char * newName);
    void setShellTerminator(char * newTerminator);
    void outputHistory();
    int newAlias(char * workCommand);
    void outputAlias();
    int saveAlias(char * fileName);
    int readAlias(char * fileName);
     
};
