#include <string>
using namespace std;

class ToyShell
{
   public:
      ToyShell();
      ~ToyShell();
      
     const char * tokenize(const char * command);
     const char * alias(const char * workCommand);
     const char * getHistoryCommand(const char * workCommand);
     void saveHistory(const char * workCommand);
    
     int execute(const char * workCommand);
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
