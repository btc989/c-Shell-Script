#include "toyshell.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

/* Constructor called on inital creation of toyshell object
* Opens two files to get name and terminator of shell
* If files do not exist create them and fill in default values
*/
ToyShell::ToyShell()
{
   int aliasLimit = 10;
   int aliasSizeX=0;
   count = 0;
   history = new string[10];
   historySize=0;
   historyArraySize=10;
   
    
  //get shellname and terminator from file 
   fstream read;
   read.open("shellName.txt");
   if(read.is_open()){
       while(!read.eof())
           read>>name;
       read.close();
   }
   else{
       //create shellName.txt and set default name
        read.open("shellName.txt", fstream::out);
        read << "Toyshell";
        name = "Toyshell";
        read.close();
    }
   
   fstream read2;    
   read2.open("shellTerminator.txt");
   if(read2.is_open()){
       while(!read2.eof())
           read2>>terminator;
       read2.close();
   }   
   else{
       //create shellTerminator.txt and set default terminator
       read2.open("shellTerminator.txt", fstream::out);
       read2 << "->";
       terminator = "->";
       read2.close();
   }
   
}

/* Deconstructor called at end of program
* Clears all memory allocated by the new and strup commands
* 
*/
ToyShell::~ToyShell(){
    if(workCommand->size !=0){
        for (int i=0; i<workCommand->size; i++)
           free(workCommand->token[i]); //frees up each space in memory

      //Clean up the array of words
      delete [] workCommand->token;     // cleans up words allocated space
    }
}
/* IncreaseCount is called at the end on loop in main
* Keeps track of how many commands have been entered
* 
*/
void ToyShell::increaseCount(){
   count++;
}
/* Tokenize is called when a command is entered
* It converts the passed string into a cstring
* This cstring is then broken apart into seperate words using strtok
* The seperated command is stored as well as its size.
*/
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
/* TokenizePath is called when a unix command needs to be executed
* It converts the passed string into a cstring
* This cstring is then broken apart into seperate words using strtok
* The seperated command is stored as well as its size.
*/
void ToyShell::tokenizePath(char* commandLine){
    
 
       
       path = new command; 

       char delim[]=":";
       char *workCommandLine = new char [strlen(commandLine) + 1];  //c-string use$
       path->token= new char*[strlen(commandLine)];
       strcpy(workCommandLine, commandLine);
       int i=0; //initilize the counter

       path->token[i] = strdup(strtok(workCommandLine, delim));

       char *test; 
       do
       {
        i++;
        test=strtok(NULL, delim);
        if(test==NULL)
            break;
        path->token[i]=strdup(test);  
       } while(test!=NULL); 

       path->size = i;    
  
}
/* Alias called before the execution of a command
* Each part of the command is looped through to check if any word matchs
* the list of given aliases
* if a match is made the actual command is substituted in for the alias
* the allocated memory is then cleared and the command is sent to tokenize 
* to be seperated again
*Returns true if an alias was found to continue alias loop in main
*Returns false if there is no aliases
*/
bool ToyShell::alias(){
   string fullCommand = "";
   string command = workCommand->token[0];
   if(aliasSizeX<=0)
        return false;
    if(!command.compare("newname")|| !command.compare("NEWNAME"))
        return false;
    
     //loop through entire command->each word
    for(int i=0; i<workCommand->size; i++) 
    {
        //loop through entire alias list
         for(int j=0; j<aliasSizeX; j++)
         {
             //compare current word with the alias->stored in the first column of 2d array
             if(!storedA[j][0].compare(workCommand->token[i])){
                 //grabs any part of the command before the alias
                for(int k=0; k<i; k++)
                    fullCommand += string(workCommand->token[k])+" ";
                //adds the alias
                fullCommand += storedA[j][1]+" ";
                 //adds any part of the command after the alias
                for(int l=i+1; l<workCommand->size; l++)
                    fullCommand += string(workCommand->token[l])+" ";
                 
                //frees up each space in memory->clears out tokenize
                for (int i=0; i<workCommand->size; i++)
                    free(workCommand->token[i]); 

                //Clean up the array of words
                delete [] workCommand->token;     // cleans up words allocated space
                //sends the string to be seperated and stored
                tokenize(fullCommand); 
                                
                return true;  //this may have to be outside the first for loop
             }
         }
    } 
    return false;
}
/* Execute checks the given command and calls the corresonding function
*Returns 0 if all executed
*Returns any other int as an error code
*/
int ToyShell::execute( ){

    int status = 0;
    //just set command to make life easier
    string command = workCommand->token[0];
    
    //Check if command is to repeat previous command
    //Then fetch previous command to continue executing
     if(!command.compare("!")){
         if(workCommand->size == 2){
            int test = getHistoryCommand(workCommand->token[1]);
            //reset command since it has changed
            if(test == 1)
                return status;
             
            command = workCommand->token[0]; 
         }
         else{
             cout<<"Missing Parameter: history line number"<<endl;
             return status;
         }
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
             cout<<"Missing Parameter: new shell name"<<endl;
    }
    //sets terminator
    else if( !command.compare("setterminator")){
        if(workCommand->size>1)
            setShellTerminator(workCommand->token[1]);
        else
             cout<<"Missing Parameter: new shell terminator"<<endl;
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

      // savenewnames store all aliases in file
     else if( !command.compare("savenewnames")){
        if(workCommand->size>1)
            saveAlias(workCommand->token[1]);
        else
             cout<<"Missing Parameter: file name"<<endl;
    } 

      //readnewnames read all aliases from file
    else if( !command.compare("readnewnames")){
        if(workCommand->size>1)
            readAlias(workCommand->token[1]);
        else
             cout<<"Missing Parameter: file name"<<endl;
    } 

      //if not a shell command try and execute as UNIX Command
    else{
        
        status = unixCommand();
        
            
        
       /* string fullCommand;
         for(int i=0; i<workCommand->size; i++)
             fullCommand += string(workCommand->token[i])+" ";
         int returnCode = system(fullCommand.c_str());
        
         //if not UNIX Command Return error
         if(!returnCode)
            return 1;*/
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
/* Setshellname opens default file and stores the new shell name
*  it also sets the variable name to the new shell name
*/
void ToyShell::setShellName(string newName){
    
    //set class variable as new name
    name = newName;
    //open shell name file and rewrite shell with new name 
    ofstream ofs;
    ofs.open("shellName.txt", std::ofstream::out | std::ofstream::trunc);
    ofs<<name;
    ofs.close();
    
}
/* Setshellterminator opens default file and stores the new shell terminator
*  it also sets the variable terminator to the new shell terminator
*/
void ToyShell::setShellTerminator(string newTerminator){
    
    //set class variable as new terminator
    terminator = newTerminator;
    //open shell name file and rewrite shell with new name 
    ofstream ofs;
    ofs.open("shellTerminator.txt", std::ofstream::out | std::ofstream::trunc);
    ofs<<terminator;
    ofs.close();
    
}
/* saveHistory saves each command entered
*  The default array size is 10, if more then 10 commands are entered
*  Then array is shifted to remove first element
*/
void ToyShell::saveHistory(){
    
    //dynamically add onto history array
    string command;
    for(int i=0; i<workCommand->size; i++)
        command += string(workCommand->token[i])+" ";
    if(historySize>=historyArraySize){
        
        /*If we want to add the dynamic allocation back
        string* grownArray = new string[historyArraySize+10];
        for (int i=0; i < historyArraySize; ++i)
           grownArray[i] = history[i];
        // enlarge newly allocated array:
        historyArraySize+= 10;
        // release old memory
        delete[] history;
        // reassign history pointer to point to expanded array
        history = grownArray;*/
        
        for(int i=0; i<historySize; i++){
            if(i<historySize-1)
                history[i] = history[i+1];       
        }
        history[historySize-1]=command;
    }
    else{
    
        history[historySize]=command;
        historySize++; 
    }
}
/* Gethistorycommand takes the line number given 
*  converts it into an int and then finds the corresponding 
*  location in the history array
*/
int ToyShell::getHistoryCommand(string line){
    
    // object from the class stringstream
    stringstream convert(line);    
    int lineNum = 0;
    //convert string into integer
    convert >> lineNum;
    lineNum--;
    if(lineNum>= historySize){
        cout<<"Line number entered was greater then amount of history"<<endl;
        return 1;
    }
    if(lineNum< 0){
        cout<<"Line number entered was less then 0"<<endl;
        return 1;
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
    //original=command;
    //call tokenize to repeat process
    tokenize(command);
    return 0;
}
/* outputHistory outputs all history saved
*  
*/
void ToyShell::outputHistory(){
    if(historySize==0)
        cout<<"There is no command history";
    else{
        for(int i=0; i<historySize; i++)
            cout<<history[i]<<endl;
    }
}
/* newAlias has three different processes
*  Deleting an alias
*  Replacing an alias
*  Adding a brand new alias
*/
void ToyShell::newAlias(){          

    if(workCommand->size >= 2){  //more than 2 tokens
            
        //frees up each space in memory->clears out tokenize
     /*   for (int i=0; i<workCommand->size; i++)
            free(workCommand->token[i]); 

        //Clean up the array of words
        delete [] workCommand->token;     // cleans up words allocated space
        //sends the string to be seperated and stored
        tokenize(original);*/
            
        bool changed = false; //used to see if anything changes in the for loop
        bool found = false;
        string temp = "";
        if(workCommand->size>2){
            for (int i = 2; i < workCommand->size; i++){  //create a string out of workCommand

                if(workCommand->size != i+1)
                    temp+= string(workCommand->token[i])+" ";
                else
                    temp += string(workCommand->token[i]);
            }
        }
            
        //make all lowercase
        for(int i=0; i<temp.length(); i++)
            temp[i] = tolower(temp[i]);
            
        string alias = workCommand->token[1];
        //make all lowercase
        for(int i=0; i<alias.length(); i++)
            alias[i] = tolower(alias[i]);
             
            
        if(temp == alias) //if alias is the same as the command
                cout<<"Alias is same as a command, alias not created"<<endl;
        else{
            
            for(int i = 0; i < aliasSizeX; i++){  //loop for the size of the alias array

                if (alias == storedA[i][0] && workCommand->size == 2){  //if any of the aliases match
                    
                    if(aliasSizeX > 1){
                        storedA[i][0] = storedA[aliasSizeX-1][0]; //set the matching to NULL
                        storedA[i][1] = storedA[aliasSizeX-1][1]; //set the matching to NULL
                    }
                    else{
                        storedA[i][0] = ""; //set the matching to NULL
                        storedA[i][1] = ""; //set the matching to NULL
                    }
                        
                    aliasSizeX--;
                    changed = true; //set the changed to true
                    return;
                }
                
                if (storedA[i][1] == temp && workCommand->size > 2){  //compare alias to stored
                    storedA[i][0] = alias;
                    return;
                }
                //if replacing existing alias with new command
                if (storedA[i][0] == alias && workCommand->size > 2){  //compare alias to stored
                    storedA[i][1] = temp;
                    return;
                }
            }
            
            if(!found){ //doesn't exist

                if (aliasSizeX == 10){
                    cout << "no spots open to store alias" << endl;
                    return;
                }
                aliasSizeX++;
                storedA[aliasSizeX-1][0] = workCommand->token[1];
                storedA[aliasSizeX-1][1] = temp;
            }
        }     
    }
    else {  //only 1 token was specified and nothing else
            cout << "You didn't specify any alias" << endl;
    }
}

void ToyShell::outputAlias(){
    if(aliasSizeX<=0)
        cout<<"There is no declared aliases" << endl;
    else{
        //go through 2d array print out new name and the actual name
        for(int i=0; i<aliasSizeX; i++)
            cout<<storedA[i][0]<< " | "<<storedA[i][1]<<endl;     
    }
}
/* saveAlias opens given file and stores all aliases
*  if file does not exist it is created
*/
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
/* readAlias opens a given file and reads in all aliases from file
*  it saves existing aliases and appends new aliases to the array
*/
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
               return 0;
           }
            //get command for alias    
           getline(read, command);
           if (aliasSizeX == 10){
                cout << "no spots open to store alias" << endl;
               return 1;
           }
           bool found =false;
           for(int i = 0; i < aliasSizeX; i++){  //loop for the size of the alias array

                if (newAlias == storedA[i][0]){
                    found = true;
                    break;
                }  //if any of the aliases match
           }
           
           if(!found){
                storedA[aliasSizeX][0] = newAlias;
                storedA[aliasSizeX][1] = command;       
                aliasSizeX++; 
           }
       }
       read.close();
    }
    else{
        cout<<"Error: Could not open file"<<endl;
        return 1;
    }
    return 0;
}
int ToyShell::unixCommand(){
    
    pid_t childPid = 0;
    pid_t waitPid;
    int status;

    childPid = fork ();
    if (childPid == -1)
    {
        fprintf (stderr, "Process %d failed to fork!\n", getpid ());
        return 1 ;
    }
    //in child process
    if (childPid == 0)
    {
        unixExecution();
        //return 10 to stop shell if error has occurred with execve
        return 10;
    }
    //in parent
    else
    {
        //if process is  supposed to wait
        string command = workCommand->token[workCommand->size-1];
        
        if(command.compare("-")){
            do
            {
                waitPid = wait (&status);  
            } while (waitPid != childPid);      
        }
        else{
            //store not waited for job
        }
        //otherwise continue
        return 0;
    }   
}

void ToyShell::unixExecution(){
  //first get full path 
  char* pPath;
  pPath = getenv ("PATH");
  bool found = false;
  string spath="";
  string command="";
  if (pPath!=NULL){
  
      //then seperate and tokenize the path by :
      tokenizePath(pPath);
      
      //loop through each name in path/
      for(int i=0; i<path->size; i++){
          
        //append command to the end
        spath= path->token[i];
        spath +="/";
        spath +=workCommand->token[0];
      
      //check if file is there 
      //and if it is executable
       if((access(spath.c_str(), X_OK))==0){
           found=true;
           break;
       }   
      }
      
      if(found){
          //not sure what this should be set to
           char *envp[] = { NULL };
           
          //make sure last character is null
          //there is extra space here since in 
          //tokenize workCommand allocates 
          //enough space for all the characters
          workCommand->token[workCommand->size]= '\0';
          
          //then use excev
           execve(spath.c_str(),workCommand->token, envp);
      }
      
      else{
          cout<<"Error: Command entered not recongized"<<endl;
      }
        //frees up each space in memory->clears out tokenize
        for (int i=0; i<path->size; i++)
                    free(path->token[i]); 

        //Clean up the array of words
        delete [] path->token;
  }
}
    
    
    
    
    
    
    
    
    
    
    

