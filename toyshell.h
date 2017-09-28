#include <string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> 
using namespace std;

extern char **environ;
struct command{
    int size;
    char **token;
};
struct job{
    int jobId;
    int processId;
    string line;
    struct tm * timeInfo;
    
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
    void unixExecution(string spath);
    
    int aliasLimit;
    int aliasSizeX;
   
    //string original;
    string name;
    int count;
    string terminator;
    command * workCommand;
    command * path;
    
    int jobLimit;
    int jobSize;
    int jobStored;
    job * jobs;
    
    
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
    void storeBackJob(int processId);
    void backJobs();
    void frontJob(string);
    void backCommand();
    bool condition();
    int conditionHelper(bool);
    void changeDirectories();
};
