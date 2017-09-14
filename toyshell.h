
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
    bool alias();
    void getHistoryCommand(string line);
    void saveHistory();
    int execute();
    string errorMessage(int status);
    
    
    
   
    string name;
    int count;
    string terminator;
    command * workCommand;
    int aliasLimit;
    int aliasSizeX;
    
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
