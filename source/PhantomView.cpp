/*
   
   PhantomView.cpp
   
*/
/*
   Originally written by Ben Loftis.  Modified by Jeremy Friesner.
   This source code is available freely for any use, and is provided without any warranty or support.
   If you use this source extensively in your project, I humbly request that you credit me in your program.
*/

#include "PhantomView.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <interface/Button.h>
#include <interface/CheckBox.h>
#include <interface/StringView.h>
#include <interface/Window.h>
#include <storage/FindDirectory.h>
#include <storage/File.h>
#include <storage/Path.h>
#include "PhantomPlayer.h"
#include "LRUEntry.h"

const char * PHANTOM_LIMB_SETTINGS_FILE_NAME = "phantom_limb_settings";

PhantomView::PhantomView(BRect rect) : BView(rect, "PhantomView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW) , _testThreadID(-1), _awaitingAnswer(false)
{ 
   SetViewColor(216,216,216);
   srand(time(NULL));

   // Load Settings...
   BMessage settingsMsg;
   {
      BPath sPath;
      if (find_directory(B_USER_SETTINGS_DIRECTORY, &sPath) == B_NO_ERROR) 
      {
         sPath.Append(PHANTOM_LIMB_SETTINGS_FILE_NAME);
         _settingsFileEntry.SetTo(sPath.Path());
         BFile settingsFile(&_settingsFileEntry, B_READ_ONLY);
         if (settingsFile.InitCheck() == B_NO_ERROR) (void) settingsMsg.Unflatten(&settingsFile);
      }
   }

   // Set up GUI
   BRect r(Bounds());
   r.bottom = r.top + 25.0f;
   const float menuWidth = 110.0f;

   {
      _volume = new LRUEntry(r, "Sound Volume", menuWidth, "dB", BMessage(PHANTOM_VOLUME_CHANGED), -120, 0);
      _volume->AddEntry(0, "Unity");
      _volume->AddEntry(-3);
      _volume->AddEntry(-6);
      _volume->AddEntry(-10);
      _volume->AddEntry(-20);
      _volume->AddEntry(-40);

      _volume->SetValue(-3);
      BMessage temp;
      if (settingsMsg.FindMessage("volume", &temp) == B_NO_ERROR) _volume->SetFromArchive(temp);

      r.OffsetBy(0, r.Height());
      AddChild(_volume);
   }

   {
      _base = new LRUEntry(r, "Base Frequency",  menuWidth, "Hz", BMessage(PHANTOM_BASE_FREQUENCY_CHANGED), 20, 20000);
      _base->AddEntry(20);
      _base->AddEntry(30);
      _base->AddEntry(40);
      _base->AddEntry(60);
      _base->AddEntry(80);
      _base->AddEntry(120);
      _base->AddEntry(240);
      _base->AddEntry(480);
      _base->AddEntry(1000);
      _base->AddEntry(2000);
      _base->AddEntry(4000);
      _base->AddEntry(8000);
      _base->AddEntry(16000);

      _base->SetValue(480);
      BMessage temp;
      if (settingsMsg.FindMessage("base", &temp) == B_NO_ERROR) _base->SetFromArchive(temp);

      r.OffsetBy(0, r.Height());
      AddChild(_base);
   }

   {
      _delta = new LRUEntry(r, "Delta Frequency", menuWidth, "Hz", BMessage(PHANTOM_DELTA_FREQUENCY_CHANGED), 0, 20000);
      _delta->AddEntry(0, "off");
      _delta->AddEntry(1);
      _delta->AddEntry(2);
      _delta->AddEntry(5);
      _delta->AddEntry(10);
      _delta->AddEntry(20);
      _delta->AddEntry(50);
      _delta->AddEntry(100);
      _delta->AddEntry(500);
      _delta->AddEntry(1000);
      _delta->AddEntry(2000);
      _delta->AddEntry(5000);

      _delta->SetValue(50);
      BMessage temp;
      if (settingsMsg.FindMessage("delta", &temp) == B_NO_ERROR) _delta->SetFromArchive(temp);

      r.OffsetBy(0, r.Height());
      AddChild(_delta);
   }
  
   float mid = (r.left+r.right)/2.0f;
   {
      const char * auditionStr = "Audition";
      BRect a(r);
      float len = StringWidth(auditionStr)+20.0f;
      a.left = mid-len/2.0f;
      a.right = mid+len/2.0f;
      _audition = new BCheckBox(a, NULL, auditionStr, new BMessage(PHANTOM_AUDITION_TOGGLED));

      r.OffsetBy(0, r.Height());
      AddChild(_audition);
   }

   {
      BRect s(r.InsetByCopy(2,0));
      const char * testButtonStr = "Test";
      float tw = StringWidth(testButtonStr)+10.0f;
      _testButton = new BButton(BRect(mid-tw,s.top,mid+tw,s.bottom), NULL, "Test", new BMessage(PHANTOM_BEGIN_TEST));
      s.OffsetBy(0, s.Height());
      AddChild(_testButton);

      BRect aRect(s);
      aRect.right = aRect.left + 30.0f;
      _aButton = new BButton(aRect, NULL, "A", new BMessage(PHANTOM_ANSWER_A));
      AddChild(_aButton);

      BRect bRect(s);
      bRect.left = bRect.right - 30.0f;
      _bButton = new BButton(bRect, NULL, "B", new BMessage(PHANTOM_ANSWER_B));
      AddChild(_bButton);

      _testStatus = new BStringView(BRect(aRect.right+5.0f, s.top-3.0f, bRect.left-5.0f, s.bottom-3.0f), NULL, "Phantom Limb Ready");
      _testStatus->SetAlignment(B_ALIGN_CENTER);
      AddChild(_testStatus);
   }

   _player = new PhantomPlayer();
   UpdatePlayer();
   UpdateButtons();
}

PhantomView::~PhantomView()
{
   if (_testThreadID >= 0) 
   {
      int32 junk;
      wait_for_thread(_testThreadID, &junk);
   }

   delete _player;

   BMessage settingsMsg;
   {
      BMessage volume;
      _volume->SaveToArchive(volume);
      settingsMsg.AddMessage("volume", &volume);
   }
   {
      BMessage base;
      _base->SaveToArchive(base);
      settingsMsg.AddMessage("base", &base);
   }
   {
      BMessage delta;
      _delta->SaveToArchive(delta);
      settingsMsg.AddMessage("delta", &delta);
   }

   // Attempt to save settings...
   BFile settingsFile(&_settingsFileEntry, B_WRITE_ONLY|B_CREATE_FILE|B_ERASE_FILE);
   if (settingsFile.InitCheck() == B_NO_ERROR) (void) settingsMsg.Flatten(&settingsFile);
}

void
PhantomView :: AllAttached()
{
   BView::AllAttached();
   _audition->SetTarget(this);
   _volume->SetTarget(this);
   _base->SetTarget(this);
   _delta->SetTarget(this);
   _testButton->SetTarget(this);
   _aButton->SetTarget(this);
   _bButton->SetTarget(this);
}

void
PhantomView :: UpdatePlayer()
{
   _player->SetParams(_base->GetValue(), (_audition->Value() == B_CONTROL_ON)?_volume->GetValue():LEVEL_MUTE);
}

void 
PhantomView :: UpdateButtons()
{
   _testButton->SetEnabled((_testThreadID < 0)&&(_awaitingAnswer == false));
   _aButton->SetEnabled(_awaitingAnswer);
   _bButton->SetEnabled(_awaitingAnswer);
}

void
PhantomView::MessageReceived(BMessage * msg)
{
   switch(msg->what)
   {
      case PHANTOM_VOLUME_CHANGED:
      case PHANTOM_BASE_FREQUENCY_CHANGED:
      case PHANTOM_AUDITION_TOGGLED:
         UpdatePlayer();
      break;

      case PHANTOM_BEGIN_TEST:
         if (_testThreadID < 0)
         {
            _testThreadID = spawn_thread(TestThreadFunc, "test thread", B_NORMAL_PRIORITY, this); 
            if (_testThreadID >= 0)
            {
               _audition->SetValue(B_CONTROL_OFF);
               UpdatePlayer();
               UpdateButtons();

               _testSign[0] = _testSign[1] = 0;
               while((_testSign[0] == _testSign[1])||((_testSign[0] != 0)&&(_testSign[1] != 0))) for (int i=0; i<2; i++) _testSign[i] = (rand()%3)-1;
               (void) resume_thread(_testThreadID);
            }
         }
      break;

      case PHANTOM_TEST_STEP:
      {
         bool play;
         int32 sign;
         if ((msg->FindBool("play", &play) == B_NO_ERROR)&&(msg->FindInt32("sign", &sign) == B_NO_ERROR)) 
         {
            _player->SetParams(_base->GetValue()+(sign*_delta->GetValue()), play?_volume->GetValue():LEVEL_MUTE);

            const char * comment;
            if (msg->FindString("comment", &comment) == B_NO_ERROR) _testStatus->SetText(comment);
         }
      }
      break;

      case PHANTOM_TEST_DONE:
         if (_testThreadID >= 0)
         {
            int32 junk;
            (void) wait_for_thread(_testThreadID, &junk);
            _testThreadID = -1;

            char buf[128];
            sprintf(buf, "Which tone was %liHz?", _base->GetValue());
            _testStatus->SetText(buf);
            _awaitingAnswer = true;
            UpdateButtons();
         }
      break;

      case PHANTOM_ANSWER_A: case PHANTOM_ANSWER_B:
      {
         if (_awaitingAnswer)
         {
            bool wasB    = (msg->what == PHANTOM_ANSWER_B);
            bool correct = (_testSign[wasB?1:0] == 0);
            int32 base   =_base->GetValue();
            int32 delta  =_delta->GetValue();
            int32 toneA  = base+(_testSign[0]*delta);
            int32 toneB  = base+(_testSign[1]*delta); 

            const char * comment = correct ? "Correct!" : "Wrong!";
            _testStatus->SetText(comment);

            printf("BaseTone=%liHz, ToneA=%liHz, ToneB=%liHz, YouSaid=%c/%liHz, %s\n", base, toneA, toneB, wasB?'B':'A', wasB?toneB:toneA, comment);
            _awaitingAnswer = false;
            UpdateButtons();
         }
      }
      break;

      default:
         BView::MessageReceived(msg);
      break;
   }
}

long PhantomView :: TestThread()
{
   const bigtime_t breakTime = 500000;  // 0.5 second
   const bigtime_t toneTime  = 1500000; // 1.5 second

   SendTestPlayMessage(_testSign[0], true, "Playing Tone A");
   snooze(toneTime);
   SendTestPlayMessage(_testSign[0], false, "");  // a short pause
   snooze(breakTime);
   SendTestPlayMessage(_testSign[1], true, "Playing Tone B");
   snooze(toneTime);
   SendTestPlayMessage(_testSign[1], false, "");
   Window()->PostMessage(PHANTOM_TEST_DONE, this);

   return 0;
}

// convenience method for the test thread... sends a message to the window thread to update playing sound
void PhantomView :: SendTestPlayMessage(int32 deltaSign, bool play, const char * comment)
{
   BMessage msg(PHANTOM_TEST_STEP);
   msg.AddInt32("sign", deltaSign);
   msg.AddBool("play", play);
   msg.AddString("comment", comment);
   Window()->PostMessage(&msg, this);
}
