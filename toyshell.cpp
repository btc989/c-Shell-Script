#include "toyshell.h"
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h> 
#include <sys/types.h>
#include <cstdio>
#include <dirent.h>
#include <fcntl.h>



/* Constructor called on inital creation of toyshell object
* Opens two files to get name and terminator of shell
* If files do not exist create them and fill in default values
*/
ToyShell::ToyShell()
{
   int aliasLimit = 10;
   int jobLimit = 10;
   int jobSize = 0;
   int jobStored = 1;
   int aliasSizeX=0;
   count = 0;
   history = new string[10];
   historySize=0;
   historyArraySize=10;
   oldpwd = getenv ("PWD");
   aInput=dup(0);
   aOutput=dup(1);
   input=dup(0);
   output=dup(1);    
     
    
   jobs = new job[10];
    
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
    close(input);
    close(output); 
    close(aInput);
    close(aOutput);
    
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
void ToyShell::tokenizeTemp(string commandLine){
    
   if(!commandLine.empty()){  
       
       tempCommand = new command; 

       char delim[]=" ,;";
       char *workCommandLine = new char [commandLine.length() + 1];  //c-string use$
       tempCommand->token= new char*[commandLine.length()];
       strcpy(workCommandLine, commandLine.c_str());
       int i=0; //initilize the counter

       tempCommand->token[i] = strdup(strtok(workCommandLine, delim));

       char *test; 
       do
       {
        i++;
        test=strtok(NULL, delim);
        if(test==NULL)
            break;
        tempCommand->token[i]=strdup(test);  
       } while(test!=NULL); 

       tempCommand->size = i;    
  }
}
/* TokenizePath is called when a unix command needs to be executed
* It converts the passed string into a cstring
* This cstring is then broken apart into seperate words using strtok
* The seperated command is stored as well as its size.
*/
void ToyShell::tokenizePath(char* commandLine){
    
 
       
       path = new command; 

       char delim[]=" :";
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

    //input=dup(0);
    //output=dup(1);
    //aInput=dup(0);
    //aOutput=dup(1);
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
        else{
             cout<<"Missing Parameter: new shell name"<<endl;
             status=1;
        }
    }
    //sets terminator
    else if( !command.compare("setterminator")){
        if(workCommand->size>1)
            setShellTerminator(workCommand->token[1]);
        else{
             cout<<"Missing Parameter: new shell terminator"<<endl;
             status =1;
        }
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
        else{
             cout<<"Missing Parameter: file name"<<endl;
            status = 1;
        }
    } 

      //readnewnames read all aliases from file
    else if( !command.compare("readnewnames")){
        if(workCommand->size>1)
            readAlias(workCommand->token[1]);
        else{
             cout<<"Missing Parameter: file name"<<endl;
            status =1;
        }
    } 

    //output all background process
    else if( !command.compare("backjobs")){
        backJobs();   
    } 
    // moves background job to foreground
     else if( !command.compare("frontjob")){
        if(workCommand->size>1)
            frontJob(workCommand->token[1]);
        else{
             cout<<"Missing Parameter: job id"<<endl;
             status=1;
        }
    } 
    
    // moves kill background job
     else if(!command.compare("cull")){
        if(workCommand->size>1)
            status=cull(workCommand->token[1]);
        else{
             cout<<"Missing Parameter: job id"<<endl;
             status=1;
        }
    } 

    else if (!command.compare("back")){
        if(workCommand->size>1){
            cout << "Too may parameters" << endl;
            status=1;
        }
        else
            backCommand();
    }
     else if (!command.compare("cd")){
        
        changeDirectories();
    }
    //conditional excecution command
    else if ( !command.compare("cond")){
        if (workCommand->size >= 6) {//must be at least this big
            int status = 0;
            bool temp = false;
            temp = condition();
            status = conditionHelper(temp);
        }
        else{
            cout << "Missing Parameters" << endl;
            status=1;
        }
    }

    //conditional excecution command
    else if ( !command.compare("notcond")){
        if (workCommand->size >= 5) {//must be at least this big
            int status = 0;
            bool temp = false;
            temp = condition();
            status = conditionHelper(!temp);
        }
        else{
            cout << "Missing Parameters" << endl;
            status=1;
        }
    }
    else if ( !command.compare("output")){
        //print any text after display
       for(int i=1; i<workCommand->size; i++)
       {
           cout<<workCommand->token[i]<<" ";
       }
        cout<<endl;
    }
    else if ( !command.compare("usescript")){
         if(workCommand->size<2){
            cout << "Missing Parameters" << endl;
            status=1;
         }
        else
           status= executeScript();
    }
    //if not a shell command try and execute as UNIX Command
    else{
        
        status = unixCommand();
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
            cout<<"Line "<<i+1<<" "<<history[i]<<endl;
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
    //aInput=dup(0);
    //aOutput=dup(1);
    
    //check for any input file
    for(int i=0; i<workCommand->size; i++){
        string temp = workCommand->token[i];
        if(!temp.compare("[")){
            
            if(i+1 < workCommand->size){
                
                string filename = workCommand->token[i+1];
                status = inputFile(filename);  
                dup2( input, 0);
                
                for(int j = i; j< workCommand->size; j++)
                {
                    
                    if(j<workCommand->size-1)
                        workCommand->token[j] = workCommand->token[j+1];
                    else
                        workCommand->token[workCommand->size-1]= '\0';
                    
                }
                workCommand->size--;
            }
            else{
                cout<< "Error: missing file name"<<endl;
                return 1;
            }
            break; 
        } 
    }
    //check for any output file
    for(int i=0; i<workCommand->size; i++){
        string temp = workCommand->token[i];
        if(!temp.compare("]")){
            if(i+1 < workCommand->size){
                
                string filename = workCommand->token[i+1];
                status = outputFile(filename);  
                dup2( output, 1);
                
                for(int j = i; j< workCommand->size; j++)
                {
                    
                    if(j<workCommand->size-2)
                        workCommand->token[j] = workCommand->token[j+2];
                    else
                        workCommand->token[workCommand->size-1]= '\0';
                        workCommand->token[workCommand->size-2]= '\0';
                    
                }
                workCommand->size = workCommand->size-2;
            }
            else{
                cout<< "Error: missing file name"<<endl;
                return 1;
            }
            
            break; 
        }
    }
    
    //Check for any pipe commands
    for(int i=0; i<workCommand->size; i++){
        string temp = workCommand->token[i];
         
        if(!temp.compare("@")){
            int status = piping();
            dup2( aInput, 0);
            dup2( aOutput, 1);
            return status;
        } 
    }
    
    
    //first get full path 
    char* pPath;
    pPath = getenv ("PATH");
    bool found = false;
    bool isWait = true;
    string spath="";
   
    string command = workCommand->token[workCommand->size-1];

    //if process is  not supposed to wait
    if(!command.compare("-")){
        
        isWait = false;
        //remove - sign 
        workCommand->token[workCommand->size-1]= '\0';
        workCommand->size=workCommand->size-1 ;  
    } 
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
          
        childPid = fork ();
        if (childPid == -1)
        {
            fprintf (stderr, "Process %d failed to fork!\n", getpid ());
            return 1 ;
        }
        //in child process
        if (childPid == 0)
        {
            unixExecution(spath);
            //return 10 to stop shell if error has occurred with execve
            return 10;
        }
        //in parent
        else
        {
            //if parent should wait for child to return
            if(isWait){
                do
                {
                    waitPid = wait (&status);  
                } while (waitPid != childPid);      
            }
            else{
                //store not waited for job
                storeBackJob(childPid);   
            }
            //otherwise continue
            dup2( aInput, 0);
            dup2( aOutput, 1);
            return 0;
        }
     }
     else
        cout<<"Error: Command entered not recongized"<<endl;
      
    //frees up each space in memory->clears out tokenize
    for (int i=0; i<path->size; i++)
            free(path->token[i]); 

    //Clean up the array of words
    delete [] path->token;
  }
   dup2( aInput, 0);
   dup2( aOutput, 1); 
           
}

void ToyShell::unixExecution(string spath){
  
    //make sure last character is null
    //there is extra space here since in 
    //tokenize workCommand allocates 
    //enough space for all the characters
    workCommand->token[workCommand->size]= '\0';
          
    //then use excev
    execve(spath.c_str(),workCommand->token, environ);
     
}
    
void ToyShell::storeBackJob(int processId){
    
    dup2(aOutput, fileno(stdout));
    if(jobLimit-1 < jobSize){
        cout<<"Error: Maximum number of jobs being executed, please wait for process to finish then try again"<<endl;
        return;
    }
    
    time_t rawtime;
    time (&rawtime);
    string command="";
    for(int i=0; i<workCommand->size; i++){
        command+= workCommand->token[i];
        command +=" ";
    }
    
    jobs[jobSize].jobId = jobStored;
    jobs[jobSize].processId = processId;
    jobs[jobSize].line = command;
    jobs[jobSize].timeInfo = localtime (&rawtime); 
    
    
    cout<<"Job has been added to background process"<<endl;
    
    cout<<"Job Id  "<<"Process Id  "<<"Command  "<<"Time Created   "<<endl;
    
     
     
 cout<<jobs[jobSize].jobId<<"   "<<jobs[jobSize].processId<<"   "<< jobs[jobSize].line<<"  "<<asctime(jobs[jobSize].timeInfo)<<"   "<<endl;
    
    jobSize++;
    jobStored++;
    
    dup2(output, fileno(stdout));
    
}

void ToyShell::backJobs(){
    
    pid_t waitPid;
    pid_t pid;
    int status;
    if(jobSize==0){
        cout<<"There are no background jobs executing"<<endl;
        return;
    }
    
    cout<<"Status   "<<"Job Id  "<<"Process Id  "<<"Command  "<<"Time Created   "<<endl;
    
    for(int i=0; i<jobSize; i++){
        
       pid= jobs[i].processId;
       waitPid = waitpid(pid, NULL, WNOHANG);          
       if(waitPid==0)
        cout<<"Running ";
       else
        cout<<"Done ";
     
        cout<<jobs[i].jobId<<"   "<<jobs[i].processId<<"   "<< jobs[i].line<<"  "<<asctime(jobs[i].timeInfo)<<"   "<<endl; 
        
        //mark job for deletion from list
        if(waitPid!=0)
            jobs[i].jobId=0;    
    }
    
    //removes executing job from the array.
    for(int i=0; i<jobSize; i++){
        
        if(jobs[i].jobId==0)
        {
            if(i!=jobSize-1){
                jobs[i].jobId=jobs[jobSize-1].jobId;
                jobs[i].processId=jobs[jobSize-1].processId;
                jobs[i].line=jobs[jobSize-1].line; 
                jobs[i].timeInfo=jobs[jobSize-1].timeInfo;
            }
            jobSize--;  
        }
    } 
}

void ToyShell::frontJob(string temp){
    int status;
    pid_t waitPid;
    int j=0;
    int processId=0;
    int found = false;
    // object from the class stringstream
    stringstream convert(temp);    
    int jobId = 0;
    //convert string into integer
    convert >> jobId;
    
    if(jobId <0 || jobId>jobStored){
        cout<<"Error: invalid job id entered"<<endl;
        return;
    }
    
    //find process id for corresponding job id
    for(int i=0; i<jobSize; i++){
        if(jobs[i].jobId==jobId)
        {
            processId = jobs[i].processId;
            j = i;
            found = true;
            break;
        }    
    }
    if(found){
        do
        { 
            waitPid = wait (&status);
        } while (waitPid != processId);

        //removes completed job from the table
        if(j=jobSize-1){
            jobs[j].jobId=jobs[jobSize-1].jobId;
            jobs[j].processId=jobs[jobSize-1].processId;
            jobs[j].line=jobs[jobSize-1].line; 
            jobs[j].timeInfo=jobs[jobSize-1].timeInfo;
        }
        jobSize--;   
    }
    else{
        cout<<"Error: process not found"<<endl;
        return;
    }
}

int ToyShell::cull(string temp){
    int status;
    pid_t waitPid;
    int j=0;
    int processId=0;
    int found = false;
    // object from the class stringstream
    stringstream convert(temp);    
    int jobId = 0;
    //convert string into integer
    convert >> jobId;
    
    if(jobId <0 || jobId>jobStored){
        cout<<"Error: invalid job id entered"<<endl;
        return 1;
    }
    
    //find process id for corresponding job id
    for(int i=0; i<jobSize; i++){
        if(jobs[i].jobId==jobId)
        {
            processId = jobs[i].processId;
            j = i;
            found = true;
            break;
        }    
    }
    if(found){
        //convert int to string
        stringstream out;
        out << processId;
        string pid = out.str();

        //kill the remaining job
        string command = "kill "+pid;
       
        //clear out work command
        if(workCommand->size !=0){
            for (int i=0; i<workCommand->size; i++)
                free(workCommand->token[i]); //frees up each space in memory

            //Clean up the array of words
            delete [] workCommand->token;     // cleans up words allocated space
        }
        tokenize(command);
        status = unixCommand();
        
        //removes completed job from the table
        if(j=jobSize-1){
            jobs[j].jobId=jobs[jobSize-1].jobId;
            jobs[j].processId=jobs[jobSize-1].processId;
            jobs[j].line=jobs[jobSize-1].line; 
            jobs[j].timeInfo=jobs[jobSize-1].timeInfo;
        }
        jobSize--;   
    }
    else{
        cout<<"Error: process not found"<<endl;
        return 1;
    }
    return status;
}

void ToyShell::changeDirectories(){
    string command="";
   
    if(workCommand->size ==1 ){
        //go to home 
        char* pPath;
        pPath = getenv ("HOME");
        oldpwd = getenv ("PWD");
        chdir(pPath); 
        setenv("PWD",pPath,1);
    }
    else
    {
        command = workCommand->token[1];
        if(!command.compare("..")){
            char* pPath;
            pPath = getenv ("PWD");  //get the previous directory

            char * ptr;
            //find last occurence of /
            ptr = strrchr( pPath, '/' );

            string sPath="";
            //create a new string up to the last /
            for(int i=0; i<ptr-pPath ; i++)
            {
                sPath+=pPath[i];
            }
             //change directory
            oldpwd = getenv ("PWD");
            chdir(sPath.c_str()); 
            setenv("PWD",sPath.c_str(),1);
        }
        else{
            //change into specific directory
            char* pPath;
            pPath = getenv ("PWD");  //get the previous directory

            char * ptr;
            //find last occurence of /
            ptr = strrchr( pPath, '/' );

            string sPath="";
            //create a new string up to the last /
            for(int i=0; i<strlen(pPath) ; i++)
            {
                sPath+=pPath[i];
            }
            sPath+="/"+command;
             //change directory
            if(chdir(sPath.c_str()) < 0)
                cout<<"Path could not be found"<<endl;
            else{
                oldpwd = getenv ("PWD");
                setenv("PWD",sPath.c_str(),1);
            }
        }
    }   
}

void ToyShell::backCommand(){

    string stuff = "cd ";
    stuff += oldpwd;
    cout << stuff << endl;

    string temp = getenv ("PWD");
    chdir(oldpwd.c_str());
    setenv("PWD",oldpwd.c_str(),1);
    oldpwd = temp;
}

bool ToyShell::condition(){
    //cond (expression) command
    //CHECKE <file_name>, CHECKD <file_name>, CHECKR <file_name>, CHECKW <file_name>, and CHECKX <file_name> 

    /* Assumptions */
    //cond is the token[0]
    //open bracket is token[1]
    //numeric expression is token[2] and token[3]
    //close bracket is token[4]
    //command is token[5]+


    //first get full path 
    char* pPath;
    pPath = getenv ("PATH");
    bool found = false;
    string spath="";
    
    //then seperate and tokenize the path by :
    tokenizePath(pPath);

    string expressF = ""; //used for front of expression
    string expressB = ""; //used for back of expression
    string command = ""; //used to store the command to execute

    expressF = workCommand->token[2];  //setting the command to variables for easy working
    expressB = workCommand->token[3];
    for (int i = 5; i < (workCommand->size - 1); i++){ //while in loop add to string
        command += string(workCommand->token[i]) + " ";
    }
    
    if(workCommand->size >= 6)
        command += string(workCommand->token[(workCommand->size - 1)]);
    
 
    //make file check lowercase
    //make all lowercase
    for(int i=0; i<expressF.length(); i++)
        expressF[i] = tolower(expressF[i]);
    
    if(!expressF.compare("checkr")){  
          //check if file is there 
          //and if it is readable
        if((access(expressB.c_str(), R_OK))==0){
            found=true;
            cout << "File is readable" << endl; 
        }
        else
            cout << "File is not readable" << endl;   
    }

    else if(!expressF.compare("checkd")){
        struct stat filestatus_buffer; //used for the stat function 
        int temp = 0;
        for (int i = 0; i < path->size; i++){
            spath = path->token[i];
            spath += "/";
            spath += expressB;
            
            //temp = stat(spath.c_str(), filestatus_buffer); 
            temp = stat(spath.c_str(), &filestatus_buffer); 
            
            if(temp>=0 && S_ISDIR( filestatus_buffer.st_mode)){
                 found = true;
                 cout<<"Name is a directory"<<endl;
            }
            //cout << errno << endl; //this will break the stat command when its uncommented, so will any message
               
        } 
    }

    else if(!expressF.compare("checke")){
        //check if file is there 
        //and if it is writeable
        if((access(expressB.c_str(), F_OK))==0){
            found=true;
            cout << "File does exist" << endl;
        }   
        else
            cout << "File does not exist" << endl;
    }

    else if(!expressF.compare("checkw")){
          //check if file is there 
          //and if it is writeable
        if((access(expressB.c_str(), W_OK))==0){
            found=true;
            cout << "File is writeable" << endl;
        }   
        else
            cout << "File is not writeable" << endl;
    }   

    else if(!expressF.compare("checkx")){
          //check if file is there 
          //and if it is executable
        if((access(expressB.c_str(), X_OK))==0){
            found=true;
            cout << "File is executable" << endl;
        }  
        else   
            cout << "File is not executable" << endl; 
    }    
    
    return found; 
}

int ToyShell::conditionHelper(bool found) {
    if(found && workCommand->size >= 6){
        string command = "";
        for (int i = 5; i < (workCommand->size - 1); i++){ //while in loop add to string
            command += string(workCommand->token[i]) + " ";
        }
        command += string(workCommand->token[(workCommand->size - 1)]);

        //clear out work command
        if(workCommand->size !=0){
            for (int i=0; i<workCommand->size; i++)
                free(workCommand->token[i]); //frees up each space in memory

            //Clean up the array of words
            delete [] workCommand->token;     // cleans up words allocated space
        }
        tokenize(command);
            //call execute again
        int status = execute();
        return status;
    } 
    return 0;
}


int ToyShell::piping(){
    
    int status;
    pid_t waitPid;
    pid_t childPid;
    bool isWait = true;
      //if process is  not supposed to wait
    string bcommand = workCommand->token[workCommand->size-1];
    if(!bcommand.compare("-")){
        
        isWait = false;
        //remove - sign 
        workCommand->token[workCommand->size-1]= '\0';
        workCommand->size=workCommand->size-1 ;  
    } 
     
        childPid = fork ();
        if (childPid == -1)
        {
            fprintf (stderr, "Process %d failed to fork!\n", getpid ());
            return 1 ;
        }
        //in child process
        if (childPid == 0)
        {
            int fd = dup(input); //will have to change to proper input 
            for(int i=0; i<workCommand->size; i++)
            {
                int tempi =i;
                string temp= workCommand->token[i];
                if(!temp.compare("@") || tempi== workCommand->size-1)//hit pipe or end of command
                {
                    if(i-1<0 || i>= workCommand->size)
                    {
                        cout<<"Error: formatting of pipe command is off"<<endl;
                        return 1;
                    }

                    //get all parts of first UNIX command
                    string command1="";
                    int j=0;
                    if(i== workCommand->size-1)
                    {
                        for( j =i; j>-1; j--)
                        {
                            string temp= workCommand->token[j];
                            if(!temp.compare("@")){
                                j++;;
                                i++;
                                break;
                            }
                        }
                    }
                    else{
                        for( j =i-1; j>=0; j--)
                        {
                            string temp= workCommand->token[j];
                            if(!temp.compare("@")){
                                j++;
                                break;
                            }
                            else if (j==0){
                                break;
                            }
                        }
                    }

                    command1 = workCommand->token[j];

                    string commandRest="";
                    for(int k=j; k<i; k++)
                        commandRest += string(workCommand->token[k])+" ";

                   tokenizeTemp(commandRest);
                   tempCommand->token[tempCommand->size]= '\0';
                    //Check if both commands are unix commands

                    string spath1 = checkPath(command1);

                    if(!spath1.compare(command1))
                    {
                        cout<<"Error: UNIX command "<<command1<<" entered was not recongized"<<endl;
                        return 1;
                    }

                    fd = subProcess(spath1,fd, isWait);
                    if (fd < 0)
                    {
                        return 1;
                    }   
                }   
            }

            //Will have to change to proper output location
            char foo[4096];
            int nbytes = read(fd, foo, sizeof(foo));

            dup2(output, fileno(stdout));

            printf("%.*s\n", nbytes, foo); 

            close(0);
            dup(fd);
            close(fd);
            close(input);
            close(output); 
            close(aInput);
            close(aOutput);

            exit(0);
        }
        //in parent
        else
        {
            //if parent should wait for child to return
            if(isWait){
                do
                {
                    waitPid = wait (&status);  
                } while (waitPid != childPid);      
            }
            else{
                //store not waited for job
                storeBackJob(childPid);   
            }    
            return 0;
        } 
}

int ToyShell::subProcess( string path, int inputStream, bool isWait){
    
     int f_des[2];
     pid_t childPid = 0;
     if (pipe(f_des)==-1)
     {
        perror("Pipe");
        close(inputStream);
        return 1;
     }
         
     childPid = fork ();
    if (childPid == -1)
    {
        fprintf (stderr, "Process %d failed to fork!\n", getpid ());
        close(f_des[0]);
        close(f_des[1]);
        close(inputStream);
        return 1 ;
    }
    //in child process
    if (childPid == 0)
    {
        close(f_des[0]);

        close(0);
        dup(inputStream);
        close(inputStream);

        close(1);
        dup(f_des[1]);
        close(f_des[1]);
           
        execve(path.c_str(),tempCommand->token, environ);
        perror("execve"); 
        close(input);
        close(output); 
        close(aInput);
        close(aOutput);
        exit(3);          
     }
     if(!isWait) 
         storeBackJob(childPid);
    
     close(inputStream);
     close(f_des[1]);

    return f_des[0];
}
    
string ToyShell::checkPath(string command){
    //first get full path 
    char* pPath;
    pPath = getenv ("PATH");
    string spath="";
    bool found = false;
    if (pPath!=NULL){
        //then seperate and tokenize the path by :
        tokenizePath(pPath);

        //loop through each name in path/
        for(int i=0; i<path->size; i++){

        //append command to the end
        spath= path->token[i];
        spath +="/";
        spath +=command;

        //check if file is there 
        //and if it is executable
        if((access(spath.c_str(), X_OK))==0){
            found=true;
            break;
        }
                
        }
    }
    //frees up each space in memory->clears out tokenize
    for (int i=0; i<path->size; i++)
            free(path->token[i]); 

    //Clean up the array of words
    delete [] path->token;
    if(found)
        return spath;
    else
        return command;
}



int ToyShell::executeScript(){
    
   string fileName = workCommand->token[1];
    //get shellname and terminator from file 
   fstream read;
   string command;
   bool aliasTest = false;
   int found =0;
   int status =0;
   if(scriptName != fileName && pScriptName != fileName){
       read.open(fileName.c_str());
       if(read.is_open()){
           
           pScriptName = scriptName;
           scriptName = fileName;
           while(!read.eof()){
               getline(read, command);

               cout<<"command "<<command<<endl;
               //frees up each space in memory->clears out tokenize
                //for (int i=0; i<workCommand->size; i++)
                  //      free(workCommand->token[i]); 

                //Clean up the array of words
                //delete [] workCommand->token;


               //find if any comments are in string
               found = command.find('$');
                if(found >=0){ //omit any text after comment
                    if(found==0 )
                        command="";
                    else
                        command = command.substr(0,found);
                }  


                if(!command.empty()){     

                       tokenize(command);

                        aliasTest = true;
                        //check if command is alias
                        //if no more aliases are found variable is set to false;
                        do{ 
                            aliasTest = alias(); 
                        }while(aliasTest);   


                      //Execute Command
                      status = execute();
                    cout<<"after execute "<<status<<endl;
                      if(status != 0){
                        cout << "Stopping Script" << endl;
                          read.close(); 
                          return 1;
                        }
                   }
           }
            read.close(); 
           return 0;
       }
        else{
           cout<<"Error: could not open script file"<<endl;   
            return 1;
        }
   }
    else{
        cout<<"Error: Can not execute script of the same name as current script"<<endl;
        return 1;
    }
    
}


int ToyShell::inputFile(string fileName){

   input=open(fileName.c_str(),O_RDONLY);
   if(input == -1){
       cout<<"Error could not open file"<<endl;
       return 1;
   }
}
int ToyShell::outputFile(string fileName){
  
   output=open(fileName.c_str(),O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
   if(output == -1){
       cout<<"Error could not open file"<<endl;
       return 1;
   }  
}
