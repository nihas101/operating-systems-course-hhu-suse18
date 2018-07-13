#ifndef __QUEUE_H__
#define __QUEUE_H__

// Element of a queue
typedef struct queue_node {
	// Pointer to next element in the queue
	struct queue_node* next;
	// Value/Data of the queue element
	int value;
} queue_node_t;

// Queue data structure
typedef struct queue {
	// Head of the linked list
	struct queue_node* head;
	// Tail of the linked list
	struct queue_node* tail;
	// Max capacity of the queue
	int capacity;
	// Current size of the queue. size <= capacity, always
	int size;
} queue_t;

// Create a new queue with max capacity by allocating it on the heap
// @param capacity Max capacity for the queue (must be positive non zero)
// @return Pointer to an empty allocated queue with defined capacity
struct queue* queue_new(int capacity);

// Delete a previously allocated queue
// This deletes the queue and all of it's elements
// @param queue Queue to delete
void queue_delete(queue_t* queue);

// Add an element to the back of the queue if there is still space
// @param queue Queue to add the element to
// @param Value to add to the back of the queue
// @return Size of the queue after the new value was added or -1
// 		   if element could not be added because the queue is full
int queue_push_back(queue_t* queue, int value);

// Remove an element from the front of the queue
// @param queue Queue to remove the element from
// @param ret_value Pointer to an int to write the returning value to
// @return Size of the queue after the remove of the element or -1 if the
//         queue is empty and no element was removed
int queue_pop_front(queue_t* queue, int* ret_value);

void queue_print(struct queue* queue);

#endif
