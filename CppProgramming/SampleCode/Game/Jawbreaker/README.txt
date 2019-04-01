--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

           | | __ ___      _| |__  _ __ ___  __ _| | _____ _ __
        _  | |/ _` \ \ /\ / / '_ \| '__/ _ \/ _` | |/ / _ \ '__|
       | |_| | (_| |\ V  V /| |_) | | |  __/ (_| |   <  __/ |
        \___/ \__,_| \_/\_/ |_.__/|_|  \___|\__,_|_|\_\___|_|

         A version of Jawbreaker in less than 128 lines of code

--------------------------------------------------------------------------------

# Summary
#---------

  Last night, while playing on the so-called Jawbreaker game in my Pocket PC,
  I wonder how hard it would be to do a C++ version of this game.
  That was a good excuse to start programming !
  I use the CImg Library (http://cimg.sourceforge.net/) to help managing graphics
  and user events. This is an open-source library that has the advantage of being
  multi-platform, so the game should compile on other architectures as well.
  The result fits in less than 128 lines of source code !

# Author
#--------

  David Tschumperle  ( http://www.greyc.ensicaen.fr/~dtschump/ )

# License
#---------

 The source code of this Jawbreaker game is distributed under the CeCiLL license
 (see file 'Licence_CeCILL_V2-en.txt').
 This License is a Free-Software license, compatible with the GPL license.

# Directory structure :
#--------------------

  The directory Jawbreaker/ is organized as follows :

  - jawbreaker.cpp             : The C++ source file of the Jawbreaker game.
  - jawbreaker.exe             : The compiled version for Windows 32 (using Dev-cpp).
  - jawbreaker.jpg             : A screenshot of the game.
  - CImg.h                     : The CImg library (this lib fits in one single header file).
  - Licence_CeCILL_V2-en.txt   : The CeCiLL license governing the use of the jawbreaker.cpp file.
  - README.txt                 : This file.

# How to compile ?
#-----------------

  Basically, you need to open a 'console-based' project using your favorite C++ IDE,
  then you need to link with the GDI32 library. This lib is very standard and is the
  low-level library responsible for all windows operations, so you have it on your
  system, if you have installed the Microsoft SDK.

------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------
