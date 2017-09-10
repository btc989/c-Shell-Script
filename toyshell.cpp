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
   aliasSizeX=0;
   aliasSizeY=0;
    
   history = new string[10];
   historySize=0;
   historyArraySize=1;
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
void ToyShell::increaseCount()
{
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
 bool ToyShell::alias(){
    
     if(aliasSizeX==0)
        return false;
  /*   //loop through entire command->each word
    for(int i=0; i<workCommand->size; i++) 
    {
        //loop through entire alias list
         for(int j=0; j<aliasSizeX; j++)
         {
             //compare current word with the alias->stored in the first column of 2d array
             if(!workCommand->token[i].compare(aliases[j][0])){
                 
                 workCommand->token[i]=strdup(aliases[j][1]);
                 return true;
             }

         }
    }*/
     
     return false;    
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
    //sets shell name
    else if(!command.compare("setshellname"))
        setShellName(workCommand->token[1]);
      //sets terminator
    else if( !command.compare("setterminator"))
        setShellTerminator(workCommand->token[1]);
      //lists current history -> default array of 10
    else if( !command.compare("history")){
        outputHistory();
    }
   /*   //new alias for command
    else if( !command.compare("newname")){
        if(!newAlias(workCommand))
            return 2;
    }
      //output all aliases that have been defined
    else if( !command.compare("newnames")){
            outputAlias();
    } 
      // savenewnames store all aliases in file
     else if( !command.compare("savenewnames")){
        if(!saveAlias(workCommand[1]))
            return 3;
    } 
      //readnewnames read all aliases from file
    else if( !command.compare("readnewnames")){
        if(!readAlias(workCommand[1]))
            return 4;
    } 
      //if not a shell command try and execute as UNIX Command
    else{
        int returnCode = system(workCommand);
       
        //if not UNIX Command Return error
        if(!returnCode)
            return 1;
    }*/
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

void ToyShell::setShellName(string newName){
    
    //set class variable as new name
    name = newName;
    //open shell name file and rewrite shell with new name 
    ofstream ofs;
    ofs.open("shellName.txt", std::ofstream::out | std::ofstream::trunc);
    ofs<<name;
    ofs.close();
    return;
}

void ToyShell::setShellTerminator(string newTerminator){
    
    //set class variable as new terminator
    terminator = newTerminator;
    //open shell name file and rewrite shell with new name 
    ofstream ofs;
    ofs.open("shellTerminator.txt", std::ofstream::out | std::ofstream::trunc);
    ofs<<terminator;
    ofs.close();
    
}
void ToyShell::saveHistory(){
    
    //dynamically add onto history array
    if(historySize+1>=historyArraySize){
        string* grownArray = new string[historyArraySize+10];
        for (int i=0; i < historyArraySize; ++i)
           grownArray[i] = history[i];
        // enlarge newly allocated array:
        historyArraySize+= 10;
        // release old memory
        delete[] history;
        // reassign history pointer to point to expanded array
        history = grownArray;
    }
    
    string command;
    for(int i=0; i<workCommand->size; i++)
        command += string(workCommand->token[i])+" ";
    history[historySize+1]=command;
    historySize++; 
}
char * ToyShell::getHistoryCommand(){
    
    
    
}
void ToyShell::outputHistory(){
    if(historySize==0)
        cout<<"There is no command history";
    else{
        for(int i=0; i<historySize; i++)
            cout<<history[i]<<endl;
    }
}
int ToyShell::newAlias(){
        
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
