#ifndef LRUEntry_h
#define LRUEntry_h

#include <app/Message.h>
#include <app/Messenger.h>
#include <interface/Menu.h>
#include <interface/MenuItem.h>
#include <interface/MenuField.h>
#include <interface/TextControl.h>
#include <support/List.h>
#include <support/String.h>

class LRUEntry : public BView
{
public:
   LRUEntry(BRect r, const char * label, float labelWidth, const char * units, const BMessage & message, int32 min, int32 max);
   ~LRUEntry();

   virtual void AllAttached();
   virtual void MessageReceived(BMessage * msg);

   void SetFromArchive(const BMessage & msg);
   void SaveToArchive(BMessage & msg) const;

   void AddEntry(int32 value, const char * optLabel = NULL);

   int32 GetValue() const;
   void SetValue(int32 value);

   void SetTarget(const BMessenger & messenger) {_target = messenger;}

private:
   void PostChangedMessage();

   BMessage _labels;
   BString _units;

   BMessage _message;
   BMessenger _target;

   BMenu * _menu;
   BMenuField * _menuField;
   BTextControl * _entry;

   int32 _minValue;
   int32 _maxValue;
};

#endif
