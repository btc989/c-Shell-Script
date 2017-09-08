#include <string>
using namespace std;

class ToyShell
{
   public:
    ToyShell();
    ~ToyShell();
      
    char * tokenize( char * command);
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
