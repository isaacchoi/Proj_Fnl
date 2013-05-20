Assignment 2, NAME: Eric Shih, UID: 303917006

Here is the list of interactions that I implemented:
  Press i, m: Move camera forward/backward
  Press j, k: Move camera left/right
  Press Left Arrow, Right Arrow: Change azimuth left/right
  Press Up Arrow, Down Arrow: Change altitude up/down
  Press w, n: Change Horizontal Field of View by 5 degrees (w adds, n subtracts)
  Press c: Change color of cubes (only works twice)
  Press SPACE: Reset view
  Press q, 'Q', ESC to leave

Requirements 1, 2, 3, 4, and 6 are all implemented.
Requirement 5 is partially implemented (no crosshairs).
Requirement 3 has a limitation, but works fine.

For extra credit,
Only 2 was attempted, but could not be completed because of the same limitation in requirement 3. 

Notes:
Calling init() refreshes the cube (and changes the color and scaling) but the program will crash after init() is called more than 3 times: 1 initial call and 2 subsequent calls through the pressing of 'c' will work, but the program will crash upon the next init() call.


