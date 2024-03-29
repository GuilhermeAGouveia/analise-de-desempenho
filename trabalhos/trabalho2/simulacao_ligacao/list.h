/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lista.h
 * Author: Guilherme Gouveia
 *
 * Created on January 26, 2021, 11:32 AM
 */

#ifndef LIST_H
#define LIST_H

typedef enum {COLETA_DADOS, CHEGADA_WEB, SERVICO_WEB, CHEGADA_LIGACAO, SERVICO_LIGACAO, NOVA_CHAMADA, FIM_CHAMADA} EventType;

typedef struct Event {
	EventType type;
	double time;
} Event;


typedef struct MinHeap MinHeap;
struct MinHeap {
    Event* arr;
    // Current Size of the Heap
    int size;
    // Maximum capacity of the heap
    int capacity;
};


int parent(int i);
int left_child(int i);
int right_child(int i);
void swap(int* a, int* b);
int get_min(MinHeap* heap);
MinHeap* init_minheap(int capacity);
MinHeap* insert_minheap(MinHeap* heap, Event element);
MinHeap* delete_element(MinHeap* heap, int index);
Event extract_minheap(MinHeap* heap);
void print_heap(MinHeap* heap);
void free_minheap(MinHeap* heap);

#endif /* LIST_H */