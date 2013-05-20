Assignment 1, NAME: Eric Shih, UID: 303917006

Here is the list of interactions that I implemented:
  Press 1: display Sierpinski Gasket in red
  Press 2: display Sierpinski Gasket in blue
  Press 3: display Fractal Tree in current color
  Press r or R: Rotate object clockwise (can keep rotating)
  Press q, 'Q', ESC to leave

Requirements 1, 2, 3, and 4 are all implemented.

For extra credit,

1. Vertex shader takes color as input, and fragment shader will take that information and generate the color. The implementation is application based using floats to represent color data.

2. Press 2, the color of Sierpinski Gasket is changed from red to blue.

3. Another fractal (tree fractal) is implemented. Press 3 to display the fractal. I recursively generate the tree fractal as follows.
  (a) Set two vertices as the trunk of the tree.
  (b) Generate two branches (left and right branches) on top of the line segment. A parameter is set to determine the angle between two branches. Angle randomized between 30 and 120. 
  (c) Recursively generate two branches on top of the branch generated previously.

4. Rotation of objects is implemented. Press r or R to rotate objects clockwise.



