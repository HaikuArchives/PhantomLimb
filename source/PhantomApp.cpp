/*
   
   PhantomApp.cpp
   
*/
/*
   Originally written by Ben Loftis.  Modified by Jeremy Friesner.
   This source code is available freely for any use, and is provided without any warranty or support.
   If you use this source extensively in your project, I humbly request that you credit me in your program.
*/

#include "PhantomApp.h"
#include "PhantomWindow.h"

int main(int, char**)
{   
   PhantomApp myApplication;
   myApplication.Run();
   return(0);
}

PhantomApp::PhantomApp() : BApplication("application/x-vnd.Sugoi.Phantom")
{
   (new PhantomWindow())->Show();
}

PhantomApp :: ~PhantomApp()
{
   // empty
}
