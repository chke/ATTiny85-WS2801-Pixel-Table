ATTiny85 WS2801 Pixel Table
===========================

This is the source code of my pixel ATTiny85 WS2801 Pixel Table, a table with a 7x7 LED pixel grid controlled by an ir remote.

Currently there are two of my remotes integrated in my code but you can use the code reader of Nathan Chantrell's project to read your own codes: 

https://github.com/nathanchantrell/TinyPCRemote

The Table hat several different modes for displaying controlled by the ir remote.

* Displaying a color wheel (keys 1 + 2)
* Loading and programming of single pixel colors (key 3 + 4)
* Clearing the displayed pixels (key 5)
* Displaying random pixels on the screen (keys 6 + 7)
* Display some pixels in the same color (keys 8 + 9)
* Print the next read ir code on the table (key 0)

The ir code is based on Nathan Chantrell's code of the TinyPCRemote

https://github.com/nathanchantrell/TinyPCRemote

The ATTiny85 library for bitbaning the WS2801 LEDs is based on Phillip Pearson's project

https://github.com/myelin/myelin-matrix-display/tree/master/attiny85_strand_tester

You may use this code under a CC Attribution license.
