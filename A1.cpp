#include "toyshell.h"
#include <iostream>
#include <string.h>
using namespace std;

int main()
{
   ToyShell shell;
   string command;
   bool alias = true;
   int status=0;    

   //Loop until command STOP or corresponding alias is entered 
   //MIGHT NEED TO CHANGE THIS
  do{
    
   //Prompt
   cout<< shell.name<<"["<<shell.count<<"]"<<shell.terminator;
   getline(cin, command); 
   shell.tokenize(command);


    //check if command is alias
    //if no more aliases are found variable is set to false;
  /*  do{ 
        shell.alias(command, alias); 
    }while(alias);
  */    


    //Check if command is to repeat previous command
    //Then fetch previous command to continue executing
  /*   if(command[0] == '!')
         command = shell.getHistoryCommand(command);
   */   


     //Save command to History
   //   shell.saveHistory(command);


     //Execute Command
      status = shell.execute();
      if(status==10){
        cout << "Stopping Shell" << endl;
          break;
      }


      //if anything but 0 is returned, call function to write out error message
      /*if(!status)
          cout<<shell.errorMessage(status)<<endl;
      */

      
    //increase command count
  shell.increaseCount();
      
  }while(status != 10);
  
    
  
    
    
    
    
    
    
    
    

   return 0;
}
