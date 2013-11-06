/*

   PhantomWindow.cpp
   
*/
/*
   Originally written by Ben Loftis.  Modified by Jeremy Friesner.
   This source code is available freely for any use, and is provided without any warranty or support.
   If you use this source extensively in your project, I humbly request that you credit me in your program.
*/

#include <stdio.h>
#include "PhantomWindow.h"
#include "PhantomView.h"

#include <app/Application.h>

PhantomWindow::PhantomWindow() : BWindow(BRect(100, 100, 300, 255), "Phantom Limb v1.0", B_TITLED_WINDOW, B_NOT_RESIZABLE)
{
   AddChild(_view = new PhantomView(Bounds()));
}

bool
PhantomWindow::QuitRequested(void)
{
   be_app->PostMessage(B_QUIT_REQUESTED);
   return(true);
}

void
PhantomWindow :: DispatchMessage(BMessage * msg, BHandler * handler)
{
   if (msg->what == B_KEY_DOWN)
   {
      int8 c;
      if (msg->FindInt8("byte", &c) == B_NO_ERROR)
      {
         switch(c)
         {
            case 'a': case 'A': PostMessage(PHANTOM_ANSWER_A,   _view); return;
            case 'b': case 'B': PostMessage(PHANTOM_ANSWER_B,   _view); return;
            case 't': case 'T': PostMessage(PHANTOM_BEGIN_TEST, _view); return;
         }
      }
   }
   BWindow::DispatchMessage(msg, handler);
}
