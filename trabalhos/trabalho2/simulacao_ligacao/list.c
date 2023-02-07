#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int get_min(MinHeap *heap)
{
    double min = 1000000000000.0;
    int minIndex = 0;

    for (int i = 0; i < heap->size; i++)
    {
        if (heap->arr[i].time < min)
        {
            min = heap->arr[i].time;
            minIndex = i;
        }
    }
    return minIndex;
}

MinHeap *init_minheap(int capacity)
{
    MinHeap *minheap = (MinHeap *)calloc(1, sizeof(MinHeap));
    minheap->arr = (Event *)calloc(capacity, sizeof(Event));
    minheap->capacity = capacity;
    minheap->size = 0;
    return minheap;
}

MinHeap *insert_minheap(MinHeap *heap, Event element)
{
    // Inserts an element to the min heap
    // We first add it to the bottom (last level)
    // of the tree, and keep swapping with it's parent
    // if it is lesser than it. We keep doing that until
    // we reach the root node. So, we will have inserted the
    // element in it's proper position to preserve the min heap property
    if (heap->size == heap->capacity)
    {
        fprintf(stderr, "Cannot insert %lf. Heap is already full!\n", element.time);
        return heap;
    }
    // We can add it. Increase the size and add it to the end
    heap->size++;
    heap->arr[heap->size - 1] = element;

   
    return heap;
}


MinHeap *delete_element(MinHeap *heap, int index)
{
    // Deletes an element, indexed by index
    // Ensure that it's lesser than the current root
    for (int i = index; i < heap->size - 1; i++)
    {
        heap->arr[i] = heap->arr[i + 1];
    }

    heap->size--;

    return heap;
}

Event extract_minheap(MinHeap *heap)
{
    // Extracts the minimum element, and returns it
    // We simply call delete_minimum() and return the
    // root element
    if (!heap || heap->size == 0)
        return (Event){-1, -1};
    int index = get_min(heap);
    Event min = heap->arr[index];
    delete_element(heap, index);
    return min;
}

void print_heap(MinHeap *heap)
{
    // Simply print the array. This is an
    // inorder traversal of the tree
    printf("Min Heap:\n");
    for (int i = 0; i < heap->size; i++)
    {
        printf("(%c, %lf) -> ", heap->arr[i].type, heap->arr[i].time);
    }
    printf("\n");
}

void free_minheap(MinHeap *heap)
{
    if (!heap)
        return;
    free(heap->arr);
    free(heap);
}
