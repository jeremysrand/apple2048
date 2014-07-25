Apple2048
==========

This is an implementation of the game "2048" for the Apple // series.  It
requires an enhanced Apple //e, Apple //c or Apple //gs.  The binary is a
ProDOS system file.

Instructions about how to play are displayed on launch.  You can bring up the
instructions at any time by pressing the 'h' key.


Implementation Notes:
=====================

The game logic is in game.c and I believe it reproduces the basic behaviours of
existing 2048 implementations.  The code for animating tiles is mostly still
written in C in anim.c and is messy.  There are lots of opportunities to
improve performance in animations.

The way animations work is:
   * Text page one is copied to text page two.
   * Text page two is displayed.
   * The next frame is prepared on text page one.
   * Text page one is displayed
   * Repeat all of the above for the next frame.

The copying of page one to page two is implemented in assembly.  There is a
problem with using text page two.  The Apple //GS has a "bug" where shadowing
of text page two from bank $00 to bank $E0 is not implemented.  This lead to
the creation of the "Alternate Display Mode" CDA which did this shadowing in
software at a performance cost.  Because I am using page two, early versions
required "Alternate Display Mode" to be on.

But, I worked around this in software.  At startup, the code detects if it is
running on a GS.  If so, it choses to use a different implementation of the
assembly routine to copy from page one to page two.  This implementation
switches to native 16-bit mode and does a copy from $000400 (text page one)
to $E00800 (text page two) right into the $E0 bank.  This avoids the need for
shadowing $00 to $E0 and it now works as expected on a GS even with "Alternate
Display Mode" off.


Possible Improvements:
======================

   * Make the animation code not suck so much.
   * Optimize the tile drawing code
   * Wait for the VBL interval before switching text pages to avoid any tearing.
