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
     void initShellName();
     void initTerminator();
      
     string errorMessage(int status);

    
     int count;
    
     //these may have to be char arrays
     string name;
     string terminator;
    
    // history[10] ??
    //aliases[10][2] ??
    void increaseCount();
      //void setShellName();
   private:
     
};
