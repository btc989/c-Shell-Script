#include "toyshell.h"
#include <string>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <iostream>


//Constructors. If no radius is specified, the default is 5.
ToyShell::ToyShell()
{
   count = 0;
   
  //get shellname and terminator from file 
   fstream read;
   read.open("shellName.txt");
   if(read.is_open()){
       while(!read.eof())
           read>>name;
   }
   fstream read2;    
  read2.open("shellTerminator.txt");
   if(read2.is_open()){
       while(!read2.eof())
           read2>>terminator;
   }    
}

ToyShell::~ToyShell(){
    
}

void ToyShell::increaseCount(){
   count++;
}

void ToyShell::tokenize(string commandLine){
    
   workCommand = new command; 
    
   char delim[]=" ,;";
   char *workCommandLine = new char [commandLine.length() + 1];  //c-string use$
   
   strcpy(workCommandLine, commandLine.c_str());
   int i=0; //initilize the counter
   
   workCommand->token[i] = strdup(strtok(workCommandLine, delim));

   char *test; 
   do
   {
    i++;
    test=strtok(NULL, delim);
    if(test==NULL)
        break;
    workCommand->token[i]=strdup(test);  
   } while(test!=NULL); 
     
   workCommand->size = i;
}

char * ToyShell::alias(){
    
}

int ToyShell::execute( ){
    int status = 0;
    //just set command to make life easier
    string command = workCommand->token[0];

    //make all lowercase
    for(int i=0; i<command.length(); i++)
        command[i] = tolower(command[i]);
    
    //if command is to stop program NOTE: compare returns 0 if equal
    if(!command.compare("stop"))
        return 10;


    /***** Set Shell Name *****/
    /*
    else if(!command.compare("setshellname"))
        setShellName(wordCommand->token[1]);  //need to check in function if there is more than 2 tokens
    */        
    

    //sets terminator
    /*
    else if( !command.compare("setterminator"))
        setShellTerminator(workCommand[1]);
    */

    //lists current history -> default array of 10
    /*
    else if( !command.compare("history")){
        outputHistory();
    }
    */  

    /***** Create new alias *****/
    
    else if(!command.compare("newname")){  //if first word off command is 'newname' run
        newAlias();  //call newAlias function
    }
    
      
    //output all aliases that have been defined
    /*
    else if( !command.compare("newnames")){
            outputAlias();
    } 
    */
    
    //savenewnames store all aliases in file
    /* 
    else if( !command.compare("savenewnames")){
        if(!saveAlias(workCommand[1]))
            return 3;
    } 
    */

    //readnewnames read all aliases from file
    /*
    else if( !command.compare("readnewnames")){
        if(!readAlias(workCommand[1]))
            return 4;
    } 
    */

    //if not a shell command try and execute as UNIX Command
    /*
    else{
        int returnCode = system(workCommand);
    }
    */   
        
    //if not UNIX Command Return error
    /*
    if(!returnCode)
            return 1;
    }
    */

    return status;
}

string ToyShell::errorMessage(int status){
    switch(status){
        case  1:
            return "This command is not reconginzed";
        case 2:
            return "New alias could not be made";
        case 3:
            return "File could not be written to";
        case 4: 
            return "Aliases could not be read from file";  
        default:
            return "";
    }
}

void ToyShell::setShellName(char * newName){
    
    //set class variable as new name
    name = newName;
    //open shell name file and rewrite shell with new name 
    ofstream ofs;
    ofs.open("shellName.txt", std::ofstream::out | std::ofstream::trunc);
    ofs<<name;
    ofs.close();
    
}

void ToyShell::setShellTerminator(char * newTerminator){
    
    //set class variable as new terminator
    terminator = newTerminator;
    //open shell name file and rewrite shell with new name 
    ofstream ofs;
    ofs.open("shellTerminator.txt", std::ofstream::out | std::ofstream::trunc);
    ofs<<name;
    ofs.close();
    
}

void ToyShell::saveHistory(){
    
    //dynamically add onto history array
    
    
    
    
}

char * ToyShell::getHistoryCommand(){
        
}

void ToyShell::outputHistory(){
  /*  if(historySize==0)
        cout<<"There is no command history";
    else{
        for(int i=0; i<historySize; i++)
            cout<<history[i]<<endl;
    }*/
}

void ToyShell::newAlias(){

        //using this to test the function
        storedA[0] = strdup("test");
        storedA[1] = strdup("cat");
        storedA[2] = strdup("dog");
        storedA[3] = strdup("red");
        storedA[4] = strdup("blue");
        storedA[5] = strdup("yellow");
        storedA[6] = strdup("green");
        storedA[7] = strdup("purple");
        storedA[8] = strdup("cyan");
        storedA[9] = strdup("teal");

        for (int i = 0; i < 10; i++){
            cout << storedA[i] << endl;
        }
        /********check to make sure this works !!!!!!!!*/

        if (workCommand->size == 2){  //delete alias command, only 2 tokens were stored
            bool changed = false; //used to see if anything changes int he for loop
            for(int i = 0; i < 10; i++){  //loop for the size of the alias array
                if (storedA[i] == workCommand->token[1]){  //if any of the aliases match
                    storedA[i] = NULL; //set the matching to NULL
                    changed = true; //set the changed to true
                }
            }
            if (!changed){  //we didn't change anyting in the for loop
                cout << "No aliases were found with that name" << endl; //prompt user there is nothing for that name
            }
        }

        /**************/
        
        else if(workCommand->size > 2){  //more than 2 tokens
            

            //search the array to see if the alias is already exists
            bool found = false; // used to see if alias exists
            int i = 0;
            while (!found && i < 10){  //search though storedA[i] for matching alias 
                if (storedA[i] == workCommand->token[1]){  //compare alias to stored
                    found = true;  //if found change to ture
                }
                i++;
            }
            
            /*
            workCommand->token[1] should be the name of the new alias, or alias to overwrite

            if overwriting{
                find the cString in storedA[i] with the same name as workCommand->token[1]
                store the new command in storedA[i]
            }
            */
            if (found) {  //does exist
                //storedA[i-1] = /* the rest of the workCommand appended together */
                cout << "*TEST NOT WORKING* Overwrote existing alias" << endl;
            }


            /*
            if new alias{
                check to see if the token[>1] is a valid statement...may not have to and just assume its valid
                store the alias in a spot where there is nothing...loop through storedA[i] until a NULL spot is found
            }
            */
            else { //doesn't exist
                i = 0;
                while (storedA[i] != NULL && i < 10){
                    i++;
                }
                if (i == 10)
                    cout << "no spots open to store alias" << endl;
                else {
                    //storedA[i] = /*  the rest of the workCommand appended together */
                    cout << "*TEST NOT WORKING* Created new alias" << endl;
                }
            }

            
        }

        else {  //only 1 token was specified and nothing else
            cout << "You didn't specify any alias" << endl;
        }
}

void ToyShell::outputAlias(){
  /*  if(aliasSizeX==0 && aliasSizeY)
        cout<<"There is no declared aliases";
    else{
        //go through 2d array print out new name and the actual name
        for(int i=0; i<aliasSizeX; i++)
            cout<<alias[i][0]<< " | "<<alias[i][1]<<endl;     
    }*/
}

int ToyShell::saveAlias(char * fileName){
    
}

int ToyShell::readAlias(char * fileName){
    
}
