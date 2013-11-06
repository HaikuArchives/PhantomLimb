#ifndef TONER_WINDOW_H
#define TONER_WINDOW_H

/*
   
   PhantomWindow.h
   
*/
/*
   Originally written by Ben Loftis.  Modified by Jeremy Friesner.
   This source code is available freely for any use, and is provided without any warranty or support.
   If you use this source extensively in your project, I humbly request that you credit me in your program.
*/

#include <interface/Window.h>

class PhantomWindow : public BWindow  
{
public:
   PhantomWindow();

   virtual bool QuitRequested(void);
   virtual void DispatchMessage(BMessage * msg, BHandler * handler);

private:
   BView * _view;
};

#endif
