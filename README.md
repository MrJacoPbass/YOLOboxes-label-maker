# YOLOboxes-label-maker_v0.1
Simple very minimal program that allows the user to draw boxes on an image, saving them on a .txt file in the yolo format

# Build
This has been developed from the raylib quickstart <https://github.com/raylib-extras/raylib-quickstart>, there are build instructions there if this doesn't work.
Haven't been tested everywhere, only on Win.
  * cd into the repository after cloning it.
  * `make SHELL=cmd`

# Usage
To start using it simply drag an image onto the window. Then start drawing, simple.
Note: This is designed to be used with 640x480 images. Depending on the resolution you might want to go to the source and ad
apt the values. 
  Search for MIN_WIDTH and MIN_HEIGHT.

<img width="647" height="514" alt="Captura de pantalla 2026-01-29 190452" src="https://github.com/user-attachments/assets/3923e44b-2899-4530-b9dc-06cccb53f9ef" />

## Commands:
   * S: [S]ave current state.
   * Q: [Q]lose and save current image.
   * Z: [notZ]ndo the last drawn label.
   * ESC: Save the image and close the program.

To draw a rectangle simply click once. Click again to fix it in place. There is no need to hold the mouse.
The .txt is generated in the same folder as the executable.

There is no .tif support. Please convert your .tif files to a supported type, recommended types are JPG and PNG.


# Unsupported operations (for the moment)
 * Controlling zoom, linewidth or colours.
 * Eraser mode. If you want to erase boxes, they are simply popped out of a buffer. Hit `Z` as many times as necessary to erase the target one.
 * Modifying already drawn boxes.
 * Multiple classes.
   
# Known issues
 * If the path to the file contains strange characters the file might not be loaded correctly. Avoid accents.

___________________________________________________________________________________________________________________________________________________________________
## Please, check Raylib at <https://github.com/raysan5/raylib>
