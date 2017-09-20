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
   shell.aliasSizeX=0;
   int found =0;
   //Loop until command STOP or corresponding alias is entered 
  do{
    
   //Prompt
   cout<< shell.name<<"["<<shell.count<<"]"<<shell.terminator;
   getline(cin, command);
      
   //find if any comments are in string
    found = command.find('&');
    if(found >=0){ //omit any text after comment
        if(found==0 )
            command="";
        else
            command = command.substr(0,found);
    }  
      
   if(!command.empty()){
       
       
       //shell.original = command;
       shell.tokenize(command);

        alias = true;
        //check if command is alias
        //if no more aliases are found variable is set to false;
        do{ 
            alias = shell.alias(); 
            
        }while(alias);


      

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

       //Save command to History
      shell.saveHistory();
        //increase command count
      shell.increaseCount();
   }
  }while(status != 10);  

   return 0;
}
