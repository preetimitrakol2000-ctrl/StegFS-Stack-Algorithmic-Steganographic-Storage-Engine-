#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "steg_fs.h"

// Swaps two heap elements
void swap_nodes(PixelMetric* a, PixelMetric* b) {
    PixelMetric temp = *a;
    *a = *b;
    *b = temp;
}

// Standard Min-Heapify traversal down the tree structure
void min_heapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->data[left].variance < heap->data[smallest].variance)
        smallest = left;

    if (right < heap->size && heap->data[right].variance < heap->data[smallest].variance)
        smallest = right;

    if (smallest != idx) {
        swap_nodes(&heap->data[idx], &heap->data[smallest]);
        min_heapify(heap, smallest);
    }
}

// Instantiates the Min-Heap space
MinHeap* create_heap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->data = (PixelMetric*)malloc(capacity * sizeof(PixelMetric));
    return heap;
}

// Inserts a new pixel index metric evaluation into the Priority Queue
void insert_heap(MinHeap* heap, int pixel_idx, float variance) {
    if (heap->size == heap->capacity) return;

    heap->size++;
    int i = heap->size - 1;
    heap->data[i].pixel_index = pixel_idx;
    heap->data[i].variance = variance;

    // Traverse up to fix structural violations
    while (i != 0 && heap->data[(i - 1) / 2].variance > heap->data[i].variance) {
        swap_nodes(&heap->data[i], &heap->data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Extracts the absolute lowest variance element out of the root node space
PixelMetric extract_min(MinHeap* heap) {
    if (heap->size <= 0) {
        PixelMetric empty = {-1, -1.0f};
        return empty;
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->data[0];
    }

    PixelMetric root = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    min_heapify(heap, 0);

    return root;
}

void free_heap(MinHeap* heap) {
    if (!heap) return;
    free(heap->data);
    free(heap);
}
