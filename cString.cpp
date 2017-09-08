#include <iostream>
#include <string>
using namespace std;

char **tokenize(string);

int main ()
{
   string commandLine;
   char **token;
   cout << "Enter the c-string you would like to tokenize" << endl;
   getline(cin, commandLine);
   token=tokenize(commandLine);
   cout << token[0] << endl;

   return 0;
}


char **tokenize(string commandLine){
   char delim[]=" ,;";
   static char *token[100];  //variable used to store the end result command
   char *workCommandLine = new char [commandLine.length() + 1];  //c-string use$

   strcpy(workCommandLine, commandLine.c_str());
   int i=0; //initilize the counter
   token[i] = strtok(workCommandLine, delim);
   do
   {
   i++;
   } while((token[i]=strtok(NULL, delim))!=NULL);

   return token;
}
