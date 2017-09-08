#include "toyshell.h"
#include <string>
#include <stdlib.h>

//Constructors. If no radius is specified, the default is 5.
ToyShell::ToyShell()
{
   count = 0;
   name = "toyshell";
   terminator = "->";
}

void ToyShell::increaseCount()
{
   count++;
}

const char * ToyShell::tokenize(const char * command){
    
}
const char * ToyShell::alias(const char * workCommand){
    
}
const char * ToyShell::getHistoryCommand(const char * workCommand){
    
}
void ToyShell::saveHistory(const char * workCommand){
    
}
int ToyShell::execute(const char * workCommand){
     //sets shell name
      
      //sets terminator
      
      //lists current history -> default array of 10
      
      //! re-executes certain command in history list
      
      //new alias for command
      
      //output all aliases that have been defined
      
      // savenewnames store all aliases in file
      
      //readnewnames read all aliases from file
      
      //if not a shell command try and execute as UNIX Command
    
      //if not UNIX Command Return error
    
      return status;
}
void ToyShell::initShellName(){
    
}
void ToyShell::initTerminator(){
    
    
}

string ToyShell::errorMessage(int status){
    
}
