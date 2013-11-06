/*
   PhantomPlayer.cpp
   Originally written by Ben Loftis.  Modified by Jeremy Friesner.
   This source code is available freely for any use, and is provided without any warranty or support.
   If you use this source extensively in your project, I humbly request that you credit me in your program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PhantomPlayer.h" 

PhantomPlayer::PhantomPlayer(void) : BSoundPlayer("Phantom Limb", &BufferProcStub, NULL, this)
{
   _started = false;
   _needsRecalc = false;
}

PhantomPlayer :: ~PhantomPlayer()
{
   if (_started) Stop();
}

void
PhantomPlayer::SetParams(int16 inFreq, int16 levelDb)
{
   if ((_started == false)||(inFreq != _freq))
   {
      _freq = inFreq;
      _needsRecalc = true;
   }

   _targetLevel = pow(10.0f, levelDb/20.0);

   if (_started == false)
   {
      _level = 0;
      _started = true;
      SetHasData(true);
      Start();
   }
}

void 
PhantomPlayer :: BufferProc(void *buffer, size_t size, const media_raw_audio_format &format) 
{
   //we're not smart enough to handle different sample rates / formats
   if (format.format != media_raw_audio_format::B_AUDIO_FLOAT) return; 
 
   if (_needsRecalc)
   {
      _needsRecalc = false;

      //calculate phase change per sample, in radians
      double w = 2 * PI * _freq / format.frame_rate;

      //change phase constant
      _p = 2.0 * cos(w);

      //recalculate q0 as if it were at this new frequency
      _q0 = sin(-2 * w + ( asin(_q1) + w) );
   }

   //convert the buffer data into handy values
   float *buf = (float *) buffer; 
  
   //iterate through buffer & fill with audio data
   size_t numSamples = size / sizeof(float); 
   for (size_t i = 0; i < numSamples; i += format.channel_count) 
   {
      _q2 = _p * _q1 - _q0;
      _q0 = _q1;
      _q1 = _q2;

      buf[i] = _level * _q2;

      //Fill other channel(s)
      for (uint j = 1; j < format.channel_count; j++) buf[i + j] = buf[i];

      // Interpolate level controls
      _level = (_targetLevel*0.0001f)+(_level*0.9999f);
   }
}
