/*
Compilation instructions: "make SHELL=cmd" -> saves the executable in the bin/Debug folder.
                          F*ing sucks that I can't build using nob tbh.

Commands:
    S: [S]ave current state.
    Q: [Q]uit and save current image.
    Z: [notZ]Undo the last drawn label.
    ESC: Save the image and close the program.

To draw a rectangle simply click once. Click again to fix it in place. There is no need to hold the mouse.
*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "raylib.h"

#define MAX_NEURONS 1000
#define MAX_FILENAME_SIZE 300
#define MIN_WIDTH 640
#define MIN_HEIGHT 480

typedef enum {
    DRAWING,
    IDLE
} STATE;

typedef struct labels {
    Rectangle rect[MAX_NEURONS]; 
    int occupation;                 // Points to the first empty object;
} Labels;

Rectangle computeRect(Vector2 mouse, Vector2 prev_pos);
int saveRects(char *save_file, Labels labels, Vector2 img_size);
char *basename(char *path);

int main () {
    STATE state = IDLE;

    Vector2 mousePos = {0};
    Vector2 rectPos = {0};
    Rectangle currentRect = {0};

    Labels labels;
    labels.occupation = 0;

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    InitWindow(MIN_WIDTH, MIN_HEIGHT, "Welcome! Please drag & drop your image.");
    SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);

    Texture img = {0};
    Vector2 img_size = {0};
    int dropped = 0;

    int scale = 1;
    char loadedFile[MAX_FILENAME_SIZE];

	while (!WindowShouldClose())	
	{

        if (IsFileDropped()){
            FilePathList files = LoadDroppedFiles();

            TextCopy(loadedFile, files.paths[0]);
            img = LoadTexture(loadedFile);
            img_size = (Vector2){img.width, img.height};
            Vector2 screenS = {GetScreenWidth(), GetScreenHeight()};
            SetWindowSize(img_size.x * (screenS.x / img_size.x), img_size.y * (screenS.y / img_size.y));
            scale = (int)fmin(screenS.x / img_size.x, screenS.y / img_size.y);

            char titleImage[strlen(loadedFile) + 2];
            memcpy(titleImage, loadedFile, strlen(loadedFile) + 1);
            SetWindowTitle(basename(titleImage));
            
            UnloadDroppedFiles(files);
            dropped = 1;
        }

        // Unload the current image and wait for the user to drop a new one.
        if (IsKeyPressed(KEY_Q)) {
            saveRects(loadedFile, labels, img_size);
            img = (Texture){0};
            labels = (Labels){0};
            labels.occupation = 0;
            dropped = 0;
            SetWindowTitle("Please, drag & drop your image.");
        }
        
        BeginDrawing();

		ClearBackground(BLACK);

        if (0 == dropped) { EndDrawing(); continue; }

        DrawTextureEx(img, (Vector2){0, 0}, 0, scale, WHITE); 

        if (IsKeyPressed(KEY_Z)) {
            if (labels.occupation != 0) {
                labels.occupation--;
            };
        }
        
        if (state == DRAWING) {
            mousePos = GetMousePosition();
            currentRect = computeRect(mousePos, rectPos);
            DrawRectangleLinesEx(currentRect, 1.f , RED );
        }
		
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (state == IDLE) {
                rectPos = GetMousePosition();
                state = DRAWING;
            } else {
                labels.rect[labels.occupation] = currentRect;
                if (labels.occupation != MAX_NEURONS) {
                    labels.occupation++;
                    state = IDLE;
                }
            }
        }

        for (int i = 0; i < labels.occupation; i++) {
            DrawRectangleLinesEx(labels.rect[i], 1.f , DARKBLUE );
        }

		EndDrawing();

        if (IsKeyPressed(KEY_S)) {
            saveRects(loadedFile, labels, img_size);
        }

	}

	UnloadTexture(img);

	CloseWindow();

    saveRects(loadedFile, labels, img_size);

	return 0;
}

Rectangle computeRect(Vector2 mousePos, Vector2 rectPos) {
    /*
     * Takes care of flipping the coordinates for the vertices of the rectangle so they are
     * rendered correctly.
     */
    Vector2 topLeft = {fmin(mousePos.x, rectPos.x), fmin(mousePos.y, rectPos.y)};
    Vector2 rectSize = {fabs(mousePos.x - rectPos.x), fabs(mousePos.y - rectPos.y)};
    return (Rectangle){topLeft.x, topLeft.y, rectSize.x, rectSize.y};
}

char *basename(char *path) {
    /*
     * This function implements extracting the basename without the extension.
     */
    char *last_match = path;
    while (*path != '\0') {
        if (*path == '\\') {
            last_match = path;
        }
        path++;
    }
    int ft_length = 0;
    char *extension = last_match;
    while (*extension != '.' && *extension != '\0') {
        extension++;
    }
    
    last_match++;
    *extension = '\0';
    return last_match;
}

int file_exists(char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { return false; }
    fclose(f);
    return true;
}


int saveRects(char *save_file, Labels labels, Vector2 img_size) {
    /*
     * Saves the specs of the rectangles on the screen.
     *  Generates a file with the same name of the image ending in .txt where each line consists of
     *  class center.x center.y w h
     */
    char ext[5] = ".txt";
    char *save_name = strncat(basename(save_file), ext, strlen(ext)); 

    FILE *save_path = fopen(save_name, "wb");
    if (save_path == NULL) {
        fprintf(stderr, "saveRects() -> Failed to open <%s> for writing ", save_name); 
        return 1;
    }

    int W = img_size.x; int H = img_size.y;
    int written = 0;
    for (int i = 0; i < labels.occupation; i++) {
        Rectangle currRect = labels.rect[i];
        float centerX = (currRect.x + currRect.width/2)/W;
        float centerY = (currRect.y + currRect.height/2)/H;

        char temp_buf[100] = {0};
        snprintf(temp_buf, sizeof(temp_buf), "0 %f %f %f %f\n", centerX, centerY, currRect.width/W, currRect.height/H);
        written = fwrite(temp_buf, strlen(temp_buf), 1, save_path);
        assert (written == 1 && "Rectangle line not copied in its entire length, terminating...");
    }

    fclose(save_path);
    
    return 0;
}
