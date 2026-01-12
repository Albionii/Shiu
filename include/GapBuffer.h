#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

#include <stddef.h>


typedef struct {
    char *buffer;
    size_t capacity;
    size_t gap_left;
    size_t gap_right;
} GapBuffer;

// The "Menu" of available functions
GapBuffer* gb_create(size_t initial_capacity);
void gb_insert(GapBuffer *gb, const char *input);
void gb_move_cursor(GapBuffer *gb, size_t position);
void gb_backspace(GapBuffer *gb);
void gb_render(GapBuffer *gb);
void gb_visual_debug(GapBuffer *gb); // Your cool arrow function!
void gb_cleanup(GapBuffer *gb);

#endif