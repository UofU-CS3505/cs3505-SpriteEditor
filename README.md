# Sprite Editor

A beginner-friendly pixel-art and animation editor built with C++ and Qt, developed by Team BitCats for CS 3505 at the University of Utah.

Create a sprite at any size, draw it frame by frame, preview the animation, and save or export your work.

## Features

* **Drawing & Editing Tools:** Pen, brush, and eraser on a visible pixel grid, with RGBA color selection (including transparency) and a custom color picker.
* **Frame Management:** Add, delete, and select frames, with a scaled thumbnail of each frame in a side list.
* **Animation Preview:** Looping playback with an adjustable frames-per-second control, plus a real-size preview that shows the sprite at its actual dimensions.
* **Undo / Redo & Clear:** Undo and redo for edits, and a clear button for the current frame.
* **Color Replacement:** Replace one color with another across every frame.
* **Resize Canvas:** Change the size of an existing sprite while keeping pixels that still fit.
* **Save, Load & Export:** Projects are saved as JSON `.ssp` files and validated on load; sprites can be exported as an image.

## Tech Stack

* C++17 and Qt (Qt Widgets, Qt Designer `.ui` files, `QTimer` for playback)
* Qt JSON classes for the `.ssp` file format
* Built with qmake via `bitcats-sprite-maker.pro`

## Project Structure

| File(s) | Responsibility |
|---------|----------------|
| `pixel.*` | A single RGBA pixel |
| `frame.*` | A grid of pixels representing one animation frame |
| `spriteproject.*` | Project state: frames, size, FPS, undo/redo stacks, resize, color replacement, JSON save/load |
| `mainwindow.*` | Qt UI, tools, mouse interaction, and preview playback |

## Save File Format

Projects are stored as JSON with the `.ssp` extension. Each frame is a grid of RGBA pixels:

```json
{
  "width": 2,
  "height": 2,
  "fps": 4,
  "frames": [ ... ]
}
```

## Building and Running

1. Install [Qt](https://www.qt.io/download) with Qt Creator.
2. Open `bitcats-sprite-maker.pro` in Qt Creator.
3. Configure the project with a desktop kit, then build and run.

A video demonstration is available in the `Video/` folder.

## Team

| Member | Main contributions |
|--------|--------------------|
| Aspen Tobler | Undo/redo, frame management (add/delete/select, frame list thumbnails), clear button, FPS control, preview, image export, code documentation |
| Emily Wang | Project setup, pixel and frame data structures, save/load |
| Shaoze Yang | Draw and erase functionality, animation preview |
| Xinmiao Li | Color selection with transparency, color replacement |
| Helen Zhang | Main window UI and mouse interaction, canvas resize, animation preview |

University of Utah, CS 3505
