# A-Drawing-Panel
 A basic drawing program which can draw some basic geometries and show texts. The drawing program have a graphical user interface via which users can define colors, select geometry types, chose fonts, and draw objects.

## Implemented features

### The program opens a window and allows user to:
* Draws 2D objects with different properties in window. Objects can be: polygons, circles, lines, points, text, etc.
* Include menus in the program.(Hierarchical menus)
* Handle keyboard, mouse,window and motion events in the program.
* By using the menu system, the drawing program allows users to change colors, line width, point size, fill-mode, etc.
* By using the events, users can control the drawing, for example, defining vertex positions for polygons.
* Save the contents of color buffer by using idle events(or timer). (Save)
* Copy the saved color buffer contents while a display event occurs. (Load)
* Allow users to type in strings.
* Draw grid-lines to show positions and sizes of objects.
* Decrease canvas quality.
* Reverse canvas color.
* Change color to black and white.

### Build environment:
DEV C++, Using other methods to build may have some bugs.
