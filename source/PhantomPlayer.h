#ifndef TONE_PLAYER_H
#define TONE_PLAYER_H

/*
   
   PhantomPlayer.h
   
*/
/*
   Originally written by Ben Loftis.  Modified by Jeremy Friesner.
   This source code is available freely for any use, and is provided without any warranty or support.
   If you use this source extensively in your project, I humbly request that you credit me in your program.
*/

#include <media/SoundPlayer.h> 

#define LEVEL_MUTE -120

class PhantomPlayer : public BSoundPlayer
{
public:
   PhantomPlayer(); 
   ~PhantomPlayer(); 

   // sound playback starts the first time this is called!
   void SetParams(int16 frequencyHz, int16 levelDb);
   
private:
   static void BufferProcStub(void * a, void *buffer, size_t size, const media_raw_audio_format &format) {((PhantomPlayer*)a)->BufferProc(buffer, size, format);}
   void BufferProc(void *buffer, size_t size, const media_raw_audio_format &format);

   bool _started;
   volatile bool _needsRecalc;
   uint16 _freq;
   float  _level;
   float  _targetLevel; 

   double _p, _q0, _q1, _q2;                  //vars for the phasor
   double _white, _b0, _b1, _b2, _b3, _b4, _b5, _b6;   //vars for the pink noise generator
};

#endif
