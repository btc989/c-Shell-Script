#include "toyshell.h"
#include <iostream>
using namespace std;

int main()
{
   ToyShell shell;
    
  //maybe make this into vector??
   char command[128];
   //Prompt
   cout<< shell.name<<"["<<shell.count<<"]"<<shell.terminator;
   cin.getline(command, 128); 
   
//loop through aliases to see if any matches command entered then substitute in the proper command
    
    
    
    
    
   //Loop until command STOP or corresponding alias is entered 
   
  while(!strcmp(command, "STOP")){
      
      
      
      
      
      
      //sets shell name
      
      //sets terminator
      
      //lists current history -> default array of 10
      
      //! re-executes certain command in history list
      
      //new alias for command
      
      //output all aliases that have been defined
      
      // savenewnames store all aliases in file
      
      //readnewnames read all aliases from file
      
      
      
      //if not a shell command try and execute as UNIX Command
      
      
      
      
      
      
      
      
      
      
      
    //Prompt at end of loop
   cout<< shell.name<<"["<<shell.count<<"]"<<shell.terminator;
   cin.getline(command, 128);
    //loop through aliases to see if any matches command entered then substitute in the proper command  
      
      
  }
  
    
  
    
    
    
    
    
    
    
    

   return 0;
}