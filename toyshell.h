
#include <string>
#include <stdlib.h>
#include <string.h>
using namespace std;

struct command{
    int size;
    char **token;
};

class ToyShell
{
   
   public:
    ToyShell();
    ~ToyShell();
      
    void tokenize( string command);
    void tokenizePath( char* command);
    bool alias();
    int getHistoryCommand(string line);
    void saveHistory();
    int execute();
    string errorMessage(int status);
    int unixCommand();
    void unixExecution();
    
    int aliasLimit;
    int aliasSizeX;
   
    //string original;
    string name;
    int count;
    string terminator;
    command * workCommand;
    command * path;
     
    
    
    string storedA[10][2];
    string * history;
    int historySize;
    int historyArraySize;
    
    
    void increaseCount();
   private:
    
    
    void setShellName(string newName);
    void setShellTerminator(string newTerminator);
    void outputHistory();
    void newAlias();
    void outputAlias();
    int saveAlias(string fileName);
    int readAlias(string fileName);
     
};
