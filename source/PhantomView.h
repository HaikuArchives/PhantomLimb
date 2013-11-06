#ifndef TONER_VIEW_H
#define TONER_VIEW_H

/*
   
   PhantomView.h
   
*/
/*
   Originally written by Ben Loftis.  Modified by Jeremy Friesner.
   This source code is available freely for any use, and is provided without any warranty or support.
   If you use this source extensively in your project, I humbly request that you credit me in your program.
*/


#include <interface/View.h>
#include <storage/Entry.h>
#include <kernel/OS.h>

class PhantomPlayer;
class LRUEntry;
class BCheckBox;
class BButton;
class BStringView;

enum 
{
   PHANTOM_VOLUME_CHANGED = 'phnt',
   PHANTOM_BASE_FREQUENCY_CHANGED,
   PHANTOM_DELTA_FREQUENCY_CHANGED,
   PHANTOM_AUDITION_TOGGLED,
   PHANTOM_BEGIN_TEST,
   PHANTOM_TEST_STEP,
   PHANTOM_TEST_DONE,
   PHANTOM_ANSWER_A,
   PHANTOM_ANSWER_B,
};
  
class PhantomView : public BView
{
public:
   PhantomView(BRect frame); 
   ~PhantomView(); 

   virtual void MessageReceived(BMessage * msg);
   virtual void AllAttached();

private:   
   void SendTestPlayMessage(int32 deltaSign, bool play, const char * comment);
   void UpdatePlayer();
   void UpdateButtons();

   BEntry _settingsFileEntry;

   LRUEntry * _volume;
   LRUEntry * _base;
   LRUEntry * _delta;

   BCheckBox * _audition;

   BButton * _testButton;
   BStringView * _testStatus;
   BButton * _aButton;
   BButton * _bButton;

   PhantomPlayer * _player;

   thread_id _testThreadID;
   int32 _testSign[2];
   bool _awaitingAnswer;

   static long TestThreadFunc(void * a) {return ((PhantomView*)a)->TestThread();}
   long TestThread();
};

#endif
