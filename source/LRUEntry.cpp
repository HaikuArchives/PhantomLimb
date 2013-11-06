#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <interface/StringView.h>
#include "LRUEntry.h"

enum {
   LRU_COMMAND_NEW_TEXT = 'lru$',
   LRU_COMMAND_MENU_ITEM
};

LRUEntry :: LRUEntry(BRect r, const char * label, float labelWidth, const char * units, const BMessage & message, int32 minValue, int32 maxValue) : BView(r, NULL, B_FOLLOW_TOP|B_FOLLOW_LEFT, 0), _units(units), _message(message), _minValue(minValue), _maxValue(maxValue)
{
   const float margin = 2.0f;

   SetViewColor(216,216,216);

   _menu = new BMenu(label);
   _menuField = new BMenuField(BRect(margin, margin, margin+labelWidth, r.Height()-margin), NULL, NULL, _menu);    
   AddChild(_menuField);

   _entry = new BTextControl(BRect(_menuField->Frame().right+margin, margin, r.Width()-(margin), r.Height()-margin), NULL, NULL, "", new BMessage(LRU_COMMAND_NEW_TEXT));
   AddChild(_entry);
}

LRUEntry :: ~LRUEntry()
{
   // empty
}

void
LRUEntry :: AllAttached()
{
   BView::AllAttached();
   _menu->SetTargetForItems(this);
   _entry->SetTarget(this);
}

void
LRUEntry :: SetValue(int32 value)
{
   // see if it maps to a label...
   bool found = false;  // default
   for (int i=_labels.CountNames(B_ANY_TYPE)-1; i>=0; i--)
   {
      char * name;
      type_code type;
      int32 count;
      if (_labels.GetInfo(B_ANY_TYPE, 0, &name, &type, &count) == B_NO_ERROR)
      {
         int32 nextValue;
         if (_labels.FindInt32(name, &nextValue) == B_NO_ERROR)
         {
            if (nextValue == value)
            {
               _entry->SetText(name);
               found = true;
            }
         }
      }
   }
   if (found == false)
   {
           if (value < _minValue) SetValue(_minValue);
      else if (value > _maxValue) SetValue(_maxValue);
      else
      {
         char buf[128];
         sprintf(buf, "%li %s", value, _units.String());
         _entry->SetText(buf);
      }
   }
}


int32
LRUEntry :: GetValue() const
{
   const char * text = _entry->Text();
   for (int i=_labels.CountNames(B_ANY_TYPE)-1; i>=0; i--)
   {
      char * name;
      type_code type;
      int32 count;
      if (_labels.GetInfo(B_ANY_TYPE, 0, &name, &type, &count) == B_NO_ERROR)
      {
         int32 value;
         if ((_labels.FindInt32(name, &value) == B_NO_ERROR)&&(strcasecmp(name, text) == 0)) return value;
      }
   }
   return atoi(text);
}

void
LRUEntry :: PostChangedMessage()
{
   _target.SendMessage(&_message);
}

void 
LRUEntry :: MessageReceived(BMessage * msg)
{
   switch(msg->what)
   {
      case LRU_COMMAND_NEW_TEXT:
         SetValue(GetValue());
         PostChangedMessage();
      break;

      case LRU_COMMAND_MENU_ITEM:
      {
         int32 value;
         if (msg->FindInt32("value", &value) == B_NO_ERROR) 
         {
            SetValue(value);
            PostChangedMessage();
         }
      }
      break;

      default:
         BView::MessageReceived(msg);
      break;
   }
}

void
LRUEntry :: AddEntry(int32 value, const char * optLabel)
{
   if (optLabel) _labels.AddInt32(optLabel, value);
   BMessage * msg = new BMessage(LRU_COMMAND_MENU_ITEM);
   msg->AddInt32("value", value);

   char buf[128];
   sprintf(buf, "%li %s", value, _units.String());

   BMenuItem * mi = new BMenuItem(optLabel?optLabel:buf, msg);
   mi->SetTarget(this);
   _menu->AddItem(mi);
}


void
LRUEntry :: SetFromArchive(const BMessage & archive)
{
   int32 value;
   if (archive.FindInt32("value", &value) == B_NO_ERROR) SetValue(value);
}

void
LRUEntry :: SaveToArchive(BMessage & archive) const
{
   archive.AddInt32("value", GetValue());
}
