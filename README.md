robprog
=======

This repository hosts my adaptation of [ezProgressBar](http://ezprogressbar.sourceforge.net/)
to use the C++11 timer facilities.  Right now, I've only modified the and included
the ETA progress bar variant here.  I've also modified the interface slightly.
I may add more capabilities in the future, but right now, this is what it is,
a simple and effective progress bar for C++11 applications using the builtin language
facilities for timing.

Changelog
=========

(ezProgressBar v2.0.1) => (RobProg v0.1)
----------------------------------------

* Initial commit of the project.
* Moved timing facilities over to C++11 using std::chrono.
* Removed some platfor specific (WIN32) code.
* Removed methods that are no longer necessary.
* Changed interface slightly (reset now takes the "new" total for the
  progress bar, since the member variable 'n' is now private).

RobProb v0.2
------------

* Changed the display of the progress bar from '######' to '=====>' (
  eventually, this will be fully configurable)
* If compiled with HAVE_ANSI_TERM, the progress portion of the bar
  will print in green text.  Right now this just uses ANSI terminal codes.
  In the future, the color will be configurable and the detection of the
  terminal type will (hopefully) be more automated.