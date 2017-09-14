#include "toyshell.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>

//Constructors. If no radius is specified, the default is 5.
ToyShell::ToyShell()
{
   int aliasLimit = 10;
    
   count = 0;
    
   history = new string[10];
   historySize=0;
   historyArraySize=1;
    
   
    
   int aliasSizeX=0;
    
    cout<<"in intit "<<aliasSizeX<<endl;
  //get shellname and terminator from file 
   fstream read;
   read.open("shellName.txt");
   if(read.is_open()){
       while(!read.eof())
           read>>name;
   }
   read.close();
   fstream read2;    
  read2.open("shellTerminator.txt");
   if(read2.is_open()){
       while(!read2.eof())
           read2>>terminator;
   }   
   read2.close();
}

ToyShell::~ToyShell(){
    if(workCommand->size !=0){
        for (int i=0; i<workCommand->size; i++)
           free(workCommand->token[i]); //frees up each space in memory

      //Clean up the array of words
      delete [] workCommand->token;     // cleans up words allocated space
    }
}

void ToyShell::increaseCount(){
   count++;
}

void ToyShell::tokenize(string commandLine){
    
   if(!commandLine.empty()){  
       
       workCommand = new command; 

       char delim[]=" ,;";
       char *workCommandLine = new char [commandLine.length() + 1];  //c-string use$
       workCommand->token= new char*[commandLine.length()];
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
}

bool ToyShell::alias(){
    
   
   string fullCommand = "";
   if(aliasSizeX<=0)
        return false;
     //loop through entire command->each word
    for(int i=0; i<workCommand->size; i++) 
    {
        //loop through entire alias list
         for(int j=0; j<aliasSizeX; j++)
         {
             //compare current word with the alias->stored in the first column of 2d array
             if(!storedA[j][0].compare(workCommand->token[i])){
                for(int k=0; k<i; k++)
                    fullCommand += string(workCommand->token[k])+" ";
                cout<<fullCommand<<endl;
                fullCommand += storedA[j][1]+" ";
                 cout<<fullCommand<<endl;
                for(int l=i+1; l<workCommand->size; l++)
                    fullCommand += string(workCommand->token[l])+" ";
                cout<<fullCommand<<endl;
                 
                 if(workCommand->size !=0){
        for (int i=0; i<workCommand->size; i++)
           free(workCommand->token[i]); //frees up each space in memory

      //Clean up the array of words
      delete [] workCommand->token;     // cleans up words allocated space
    }
                 
                 
                 
                tokenize(fullCommand); 
                                //Problem here what if they say "dd | ff", it will only store the dd part and delete the "| ff"
                return true;  //this may have to be outside the first for loop
             }
         }
    } 
    return false;
}

int ToyShell::execute( ){

    
    int status = 0;
    //just set command to make life easier
    string command = workCommand->token[0];
    
    //Check if command is to repeat previous command
    //Then fetch previous command to continue executing
     if(!command.compare("!")){
         if(workCommand->size>1){
            getHistoryCommand(workCommand->token[1]);
            //reset command since it has changed
            command = workCommand->token[0];
         }
         else
             cout<<"Missing Parameter: history line number"<<endl;
     }
    
    //make all lowercase
    for(int i=0; i<command.length(); i++)
        command[i] = tolower(command[i]);
    
     //if command is to stop program NOTE: compare returns 0 if equal
    if(!command.compare("stop"))
        return 10;

    //sets shell name
    else if(!command.compare("setshellname")){
        if(workCommand->size>1)
            setShellName(workCommand->token[1]);
        else
             cout<<"Missing Parameter: history line number"<<endl;
    }
    //sets terminator
    else if( !command.compare("setterminator")){
        if(workCommand->size>1)
            setShellTerminator(workCommand->token[1]);
        else
             cout<<"Missing Parameter: history line number"<<endl;
    }

    //lists current history -> default array of 10
    else if( !command.compare("history")){
        outputHistory();
    }

    //new alias for command
    else if( !command.compare("newname")){
        newAlias();
            
    }
    
    
      //output all aliases that have been defined
    else if( !command.compare("newnames")){
            outputAlias();
    } 
/*
      // savenewnames store all aliases in file
     else if( !command.compare("savenewnames")){
        if(!saveAlias(workCommand[1]))
            return 3;
    } 

      //readnewnames read all aliases from file
    else if( !command.compare("readnewnames")){
        if(!readAlias(workCommand[1]))
            return 4;
    } */

      //if not a shell command try and execute as UNIX Command
    else{
        string fullCommand;
         for(int i=0; i<workCommand->size; i++)
             fullCommand += string(workCommand->token[i])+" ";
         int returnCode = system(fullCommand.c_str());
        
         //if not UNIX Command Return error
         if(!returnCode)
            return 1;
    }
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

void ToyShell::getHistoryCommand(string line){
    
 
    // object from the class stringstream
    stringstream convert(line);    
    int lineNum = 0;
    //convert string into integer
    convert >> lineNum;
    
    if(lineNum>= historySize){
        cout<<"Line number entered was greater then amount of history"<<endl;
        return;
    }
    //get requested command
    string command = history[lineNum];
    //clear out current command for replacement
    if(workCommand->size !=0){
        for (int i=0; i<workCommand->size; i++)
           free(workCommand->token[i]); //frees up each space in memory

      //Clean up the array of words
      delete [] workCommand->token;     // cleans up words allocated space
    }
    //call tokenize to repeat process
    tokenize(command);   
}

void ToyShell::outputHistory(){
    if(historySize==0)
        cout<<"There is no command history";
    else{
        for(int i=0; i<historySize; i++)
            cout<<history[i]<<endl;
    }
}

void ToyShell::newAlias(){

        /********check to make sure this works !!!!!!!!*/
        
        if (workCommand->size == 2){  //delete alias command, only 2 tokens were stored
            bool changed = false; //used to see if anything changes int he for loop
            for(int i = 0; i < 10; i++){  //loop for the size of the alias array
                if (!storedA[i][0].compare( workCommand->token[1]) == 0){  //if any of the aliases match
                    storedA[i][0] = ""; //set the matching to NULL
                    changed = true; //set the changed to true
                }
            }
            if (!changed){  //we didn't change anyting in the for loop
                cout << "No aliases were found with that name" << endl; //prompt user there is nothing for that name
            }
            else {
                cout << "Alias deleted" << endl;
            }
        }

        /**************/
        
        else if(workCommand->size > 2){  //more than 2 tokens
            
            
            
            //search the array to see if the alias is already exists
            bool found = false; // used to see if alias exists
            string command;
            for(int i=2; i<workCommand->size; i++)
                    command+= string(workCommand->token[i])+" ";
                
            
            for(int i=0; i<aliasSizeX; i++){  //search though storedA[i] for matching alias 
                if (storedA[i][1] == command){  //compare alias to stored
                    found = true;  //if found change to ture
                    break;
                }
                
            }
            
            /*******
            workCommand->token[1] should be the name of the new alias, or alias to overwrite

            if overwriting{
                find the cString in storedA[i] with the same name as workCommand->token[1]
                store the new command in storedA[i]
            }
            *******/
            if (found) {  //does exist
                //storedA[i-1] = /* the rest of the workCommand appended together */
              
                
                cout << "*TEST NOT WORKING* Overwrote existing alias" << endl;
            }


            /********
            if new alias{
                check to see if the token[>1] is a valid statement...may not have to and just assume its valid
                store the alias in a spot where there is nothing...loop through storedA[i] until a NULL spot is found
            }
            ********/
            else { //doesn't exist
                
                if (aliasSizeX == 10){
                    cout << "no spots open to store alias" << endl;
                    return;
                }
                aliasSizeX++;
                storedA[aliasSizeX-1][0] = workCommand->token[1];
                storedA[aliasSizeX-1][1] = command;
            }
        }
        else {  //only 1 token was specified and nothing else
            cout << "You didn't specify any alias" << endl;
        }
}

void ToyShell::outputAlias(){
    if(aliasSizeX==0)
        cout<<"There is no declared aliases";
    else{
        //go through 2d array print out new name and the actual name
        for(int i=0; i<aliasSizeX; i++)
            cout<<storedA[i][0]<< " | "<<storedA[i][1]<<endl;     
    }
}

int ToyShell::saveAlias(string fileName){
     //open shell name file and rewrite shell with new name 
    ofstream ofs;
    ofs.open(fileName.c_str(), std::ofstream::out | std::ofstream::trunc);
    if(ofs.is_open()){
        for(int i=0; i<aliasSizeX; i++){
                ofs<<storedA[i][0]<<endl;
                ofs<<storedA[i][1]<<endl; 

        }
        ofs.close();
    }
    else{
        cout<<"Error: Could not open file"<<endl;
        return 1;
    }
    
    return 0;
    
    
}

int ToyShell::readAlias(string fileName){
    string newAlias="";
    string command="";
    fstream read;    
    read.open(fileName.c_str());
    if(read.is_open()){
       while(!read.eof()){
           //read entire line in from file
           //ASSUMPTIONS IS THAT ALIAS IS PLACED ON FIRST LINE AND THE COMMAND IS PLACED ON THE SECOND
           getline(read, newAlias);
           //incase file not formated correctly
           if(read.eof()){
               cout<<"Error:given file is not formated properly. Odd number of lines"<<endl;
               return 1;
           }
            //get command for alias    
           getline(read, command);
           if (aliasSizeX == 10){
                cout << "no spots open to store alias" << endl;
               return 1;
           }
           aliasSizeX++;
           //storedA[aliasSizeX][0] = newAlias;
           //storedA[aliasSizeX][1] = command;       
              
       }
       read.close();
    }
    else{
        cout<<"Error: Could not open file"<<endl;
        return 1;
    }
    
    return 0;
}
