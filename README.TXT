PhantomLimb 1.0
---------------

1. What is it?

   PhantomLimb is a little program to generate tones, and test your
   discrimination of frequency deltas.  I wrote it for a friend of
   mine who is suffering from tinnitus ("ringing in the ears") due 
   to an ear infection.  (the infection destroyed some nerve cells
   in his inner ear, and the lack of signals from the destroyed nerve
   cells is what causes the ringing--similar to the "phantom limb"
   sensations experienced by amputees.  Hence the name of this program)
   By practicing discriminating between the sound of the tinnitus
   and "nearby" sounds, he hopes to be able to train his brain to
   discriminate between the "phantom" sound and other, real sounds.

2. How do I use it?

   To launch PhantomLimb, just double click its icon.  Choose the
   base frequency and sound volume you like, either from the pull
   down menus or by entering them numerically into the text controls
   at the right.  Then click the check box next to "Audition" to
   hear your tone.

   Once you are satisfied with the tone, choose a delta frequency.
   Then click "test".  The computer will play two tones.  One tone
   will be your base frequency, and the other will either be
   (base+delta) or (base-delta).  When the tones are done playing,
   click either the "A" or "B" buttons, depending on which tone
   you think was the base frequency.  The computer will tell you
   whether or not you got it right.

   If you get tired of using the mouse to test yourself, you can
   use the T, A, and B keys as equivalents to clicking the Test, 
   A, and B buttons, respectively.

   If you run PhantomLimb from the Terminal, you will see a line
   printed to stdout for each test iteration.  You can save this
   as a record of your progress, if desired.

3. Who is responsible for this?

   PhantomLimb is derived from Ben Loftis' handy application,
   "Toner".  Toner is available on BeBits:  http://www.bebits.com/app/2034
   Thanks Ben for the useful code!

   Modifications to the code were done by Jeremy Friesner.
   My email address is jaf@lcsaudio.com, and my web page is
   http://www.lcscanada.com/jaf .  Feel free to email me
   with any comments or questions.

4. What's the licensing mumbo jumbo for this program?

   Both the application and its source code are hereby placed in the
   public domain.  Do whatever you like with it, but if you use it in 
   your program, please give a credit to Ben Loftis and I.

