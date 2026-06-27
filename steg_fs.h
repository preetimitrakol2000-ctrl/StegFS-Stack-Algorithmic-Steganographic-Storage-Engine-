#ifndef STEG_FS_H
#define STEG_FS_H

typedef struct {
    int pixel_index;
    float variance; // Measure of detail noise around this local node region
} PixelMetric;

typedef struct {
    PixelMetric* data;
    int size;
    int capacity;
} MinHeap;

// Structural functional declarations mapping configurations
MinHeap* create_heap(int capacity);
void insert_heap(MinHeap* heap, int pixel_idx, float variance);
PixelMetric extract_min(MinHeap* heap);
void free_heap(MinHeap* heap);

#endif // STEG_FS_H
