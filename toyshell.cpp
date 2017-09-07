#include "toyshell.h"

//Constructors. If no radius is specified, the default is 5.
ToyShell::ToyShell()
{
   count = 0;
   name = "toyshell";
   terminator = "->";
}

ToyShell::increaseCount()
{
   count++;
}
