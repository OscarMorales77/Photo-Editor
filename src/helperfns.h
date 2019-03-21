#ifndef HELPERFNS_H
#define HELPERFNS_H

#include "console.h"
#include "gwindow.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "gbufferedimage.h"
#include "gevents.h"

using namespace std;

static const int    WHITE = 0xFFFFFF;
static const int    BLACK = 0x000000;
static const int    GREEN = 0x00FF00;



/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to open the image file 'filename'.
 *
 * This function returns true when the image file was successfully
 * opened and the 'img' object now contains that image, otherwise it
 * returns false.
 */
static bool openImageFromFilename(GBufferedImage &img, string filename) {
    try { img.load(filename); }
    catch (...) { return false; }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to save the image file to 'filename'.
 *
 * This function returns true when the image was successfully saved
 * to the file specified, otherwise it returns false.
 */
static bool saveImageToFilename(const GBufferedImage &img, string filename) {
    try {
        img.save(filename);
    } catch (...) {
        return false;
    }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Waits for a mouse click in the GWindow and reports click location.
 *
 * When this function returns, row and col are set to the row and
 * column where a mouse click was detected.
 */
static void getMouseClickLocation(int &row, int &col) {
    GMouseEvent me;
    do {
        me = getNextEvent(MOUSE_EVENT);
    } while (me.getEventType() != MOUSE_CLICKED);
    row = me.getY();
    col = me.getX();
}


#endif // HELPERFNS_H
