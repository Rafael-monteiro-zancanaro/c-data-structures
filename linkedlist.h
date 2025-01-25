/*
    * Implementation of LinkedList in Pure C - By Rafael Monteiro Zancanaro.

    * This header file contains all Basic functions that handle linked lists. Feel free to use it on your projects :)
    * All the implementation's details are in the header of the function, that includes its purpose and asymptotic notation (Big O notation)
*/
#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

/*
    * This directive conditional assure functions that have return cannot be ignored.
    * This helps to avoid memory leaks caused by negligence from programmer.
    * Unfortunately, this is only available for GNU C Compiler (GCC) Users for now.
    * There is an open improvement to make this available for other Compilers:
    * See the discussion at: https://github.com/Rafael-monteiro-zancanaro/c-data-structures/issues/2
*/
#ifndef __GNUC__
    #define __attribute__(x) /*NOTHING*/
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>


/*
    * WARNINGS_ALLOWED: Directive to print warnings on stderr

    * Change this boolean if you want to receive warnings of possible errors.
*/
#define WARNINGS_ALLOWED false

/*
    * LIST_HEAD: Directive that represents the first element of list
*/
#define LIST_HEAD 0

/*
    * LIST_TAIL: Directive that represents the last element of list

    * This attribute is checked inside handling functions and replaced by the current size of list.
*/
#define LIST_TAIL INT64_MAX

/*
    * Cell_t: The list cell type

    * This type acts like a container that both store the wanted data and points to the next Cell
    * i.e: Consider the following representation of Cell
    * 
    * int someValue = 5;
    * Cell_t nextCell = ... ;// Some intialization here
    * Cell_t current = {
    *   .value = &someValue,
    *   .next = &nextCell;
    * }  
    * 
    * This is a cell that contains a pointer to the 'someValue' variable 
    * and points to the Cell 'nextCell'
*/
typedef struct Cell_t
{
    void *value;
    struct Cell_t *next;
} Cell_t;

/*
    * LinkedList_t: The concrete LinkedList type.

    * This type represents the concrete type of the linked list data structure
    * Warning: Is HIGHLY recommendable to DON'T change this structure directly, instead of,
    * use the methods of this header to handle the linked list and avoid undefined behavior.
*/
typedef struct LinkedList_t
{
    struct Cell_t *head;
    uint32_t currentSize;
} LinkedList_t;

/*
    * __Aux_Counter: Utilitary Internal Counter for loops

    * You dont need to use this on your custom functions, 
    * it is used as a internal facility to improve/maintain the functions of this header 
*/
struct __Aux_Counter
{
    struct Cell_t *pivot;
    uint_fast32_t currentPosition;
};

/*
    * Warning: Print the possible error on stderr
    
    * This function is handled by WARNINGS_ALLOWED directive. 
    * See 'WARNINGS_ALLOWED' for more details.
    
    * Other details:
    *   Time complexity: O(1)
*/
void Warning(const char *message)
{
    fprintf(stderr, "Warning: %s.\n", message);
    fprintf(stdout, "If you want to disable warning messages, set WARNINGS_ALLOWED as false\n");
}

/*
    * Pprint: Print the list data on stdout

    * It runs all over the list printing the values.
    * Its important to say that the 'values' described here are the POINTER addresses.
    * i.e, Considering the following code:
    * 
    * int value1, value2, value3 = 2;
    * LinkedList_t *list = (LinkedList_t *) malloc(sizeof(LinkedList));
    * 
    * PushBack(list, &value1); // Check out 'PushBack' for more details.
    * PushBack(list, &value2);
    * PushBack(list, &value3);
    * Pprint(list);
    * Free(list); // Check out 'Free' for more details.
    * 
    * Will result on something like this on stdout:
    * List size: 3
    * [{ value = (0x5...), index = 0 }, { value = (0xd...), index = 1 }, { value = (0xf...), index = 2 }]

    * Other Details:
    *   Time Complexity: Θ(n) 
*/
void Pprint(struct LinkedList_t *list)
{
    struct __Aux_Counter counter = {.currentPosition = 0, .pivot = list->head};
    printf("List size: %d\n", list->currentSize);
    printf("[");
    while (counter.pivot != NULL)
    {
        printf(
            "{ value = %p, index = %ld }%s",
            counter.pivot->value,
            counter.currentPosition,
            counter.pivot->next == NULL ? "]\n" : ", ");

        counter.pivot = counter.pivot->next;
        ++counter.currentPosition;
    }
}

/*
    * Empty: Check if a list doesnt contains elements

    * This verification checks if HEAD is NULL and if currentSize size is NULL
    * so be careful when change lists internal data, especially on this second value
    
    * Other Details:
    *   Time Complexity: Θ(1)
*/
bool Empty(struct LinkedList_t *list)
{
    return list->head == NULL && list->currentSize == 0;
}

/*
    * PushBack: Store an element in the end of list

    * This method internally creates a new Cell with the value given, and stores it as the last element 
    * of the list, and points its Cell structure next pointer as NULL
 
    * Other Details
    *   Time Complexity: O(n)
*/
void PushBack(struct LinkedList_t *list, void *value)
{
    struct Cell_t *currentCell = (struct Cell_t *)malloc(sizeof(struct Cell_t));
    currentCell->value = value;
    currentCell->next = NULL;
    if (list->head == NULL)
    {
        list->head = currentCell;
        list->currentSize = 1;
        return;
    }

    struct __Aux_Counter counter = {.pivot = list->head, .currentPosition = 0};
    while (counter.pivot->next != NULL)
    {
        counter.pivot = counter.pivot->next;
        counter.currentPosition++;
    }

    #if WARNINGS_ALLOWED
        if (value == NULL)
        {
            char warn[100] = "\0";
            sprintf(warn, "NULL element insert on position %ld, this could be a inconsistency\n", counter.currentPosition);
            Warning(warn);
        }
    #endif

    counter.pivot->next = currentCell;
    list->currentSize++;
}

/*
    * PushFront: Store an element in the start of list

    * This method internally creates a new Cell with the value given, and stores it as the list head 
    * and points its Cell structure next pointer as old list head
 
    * Other Details
    *   Time Complexity: Θ(1)
*/
void PushFront(struct LinkedList_t *list, void *value)
{
    struct Cell_t *currentCell = (struct Cell_t *)malloc(sizeof(struct Cell_t));
    currentCell->value = value;
    currentCell->next = NULL;

    if (list->head == NULL)
    {
        list->head = currentCell;
        list->currentSize = 1;
        return;
    }

    #if WARNINGS_ALLOWED
        if (value == NULL)
        {
            char warn[100] = "\0";
            sprintf(warn, "NULL element insert on list's head, this could be a inconsistency\n");
            Warning(warn);
        }
    #endif

    currentCell->next = list->head;
    list->head = currentCell;
    list->currentSize++;
}

/*
    * Free: Deallocate list resources

    * This method runs over all list cells and freeds they and the list itself after it.
    * Is very recommendable to Free a list after use it or in the end of 
    * its scope to prevent memory leaks. 
 
    * Other Details
    *   Time Complexity: Θ(n)
*/
void Free(struct LinkedList_t *list)
{
    if (list->head == NULL) {
        free(list);
        return;
    }
    struct __Aux_Counter counter = {.currentPosition = 0, .pivot = list->head};
    while (counter.pivot->next != NULL)
    {
        struct Cell_t *current = counter.pivot;
        counter.pivot = counter.pivot->next;
        free(current);
    }
    list->currentSize = 0;
    free(list);
}

/*
    * RemoveAt: Remove element on desired position

    * This method runs over the Cells until the internal counter be equal to the desired position.
    * It is best-effort algorithm, this means it will not break the execution if abnormal circustances happen,
    * like removal of empty list or a invalid index. Instead of this, it will make nothing.
 
    * Other Details
    *   Time Complexity: O(n)
*/
void RemoveAt(struct LinkedList_t *list, size_t position)
{
    if (position == LIST_TAIL) {
        position = list->currentSize - 1;
    }

    if (Empty(list)) {
        #if WARNINGS_ALLOWED
            char warn[100] = "\0";
            Warning("Warning: Tried to remove from an empty list");
        #endif
        return;
    }

    if (position < 0 || position > list->currentSize - 1)
    {
        #if WARNINGS_ALLOWED
            char warn[100] = "\0";
            sprintf(warn, "Warning: Tried to remove invalid index (%ld)", position);
            Warning(warn);
        #endif
        return;
    }

    struct Cell_t *beforeTarget = NULL;
    struct Cell_t *afterTarget = NULL;
    struct __Aux_Counter counter = {.currentPosition = 0, .pivot = list->head};
    while (counter.currentPosition != position) {
        beforeTarget = counter.pivot;
        counter.pivot = counter.pivot->next;
        ++counter.currentPosition;
    }
    afterTarget = counter.pivot->next;
    beforeTarget->next = afterTarget;
    free(counter.pivot);
}

/*
    * RemoveAt: Insert element on desired position

    * This method runs over the Cells until the internal counter be equal to the desired position.
    * It is best-effort algorithm, this means it will not break the execution if abnormal circustances happen,
    * like a invalid index. Instead of this, it will make nothing.
 
    * Other Details
    *   Time Complexity: O(n)
*/
void InsertAt(struct LinkedList_t *list, size_t position, void *value)
{
    if (position == LIST_TAIL) {
        position = list->currentSize - 1;
    }

    struct Cell_t *currentCell = (struct Cell_t *)malloc(sizeof(struct Cell_t));
    currentCell->value = value;
    currentCell->next = NULL;
    if (position < 0 || position > list->currentSize - 1)
    {
        #if WARNINGS_ALLOWED
            char warn[100] = "\0";
            sprintf(warn, "Warning: Tried to insert on invalid index (%ld)", position);
            Warning(warn);
        #endif
        return;
    }

    if (Empty(list) && position != 0) {
        #if WARNINGS_ALLOWED
            Warning("Warning: Tried to insert from an empty list");
        #endif
        return;
    }

    if (position == 0) {
        PushFront(list, value);
        return;
    }

    if (position == list->currentSize) {
        PushBack(list, value);
        return;
    }

    struct Cell_t *beforeTarget = NULL;
    struct __Aux_Counter counter = {.currentPosition = 0, .pivot = list->head};
    while (counter.currentPosition != position) {
        beforeTarget = counter.pivot;
        counter.pivot = counter.pivot->next;
        ++counter.currentPosition;
    }
    beforeTarget->next = currentCell;
    currentCell->next = counter.pivot;
    list->currentSize++;
}

/*
    * GetAt: Retrieve element of desired position

    * This method runs over the Cells until the internal counter be equal to the desired position,
    * when it reaches the position, returns the element.
    * If the position is an invalid index, it will return NULL.

    * Other Details
    *   Time Complexity: O(n)
    *   For GNU C Compiler (GCC) Users: 'warn_unused_result' flag. Return can't be ignored
*/
__attribute__((warn_unused_result)) struct Cell_t* GetAt(struct LinkedList_t *list, size_t position)
{
    if (position == LIST_TAIL) {
        position = list->currentSize - 1;
    }

    if (position < 0 || position > list->currentSize - 1) {
        #if WARNINGS_ALLOWED
            char warn[100] = "\0";
            sprintf(warn, "Warning: Tried to get on invalid index (%ld)", position);
            Warning(warn);
        #endif
        return NULL;
    }

    if (Empty(list) && position != 0) {
        #if WARNINGS_ALLOWED
            Warning("Warning: Tried to get from an empty list");
        #endif
        return NULL;
    }

    if (position == 0) {
        return list->head;
    }

    struct __Aux_Counter counter = {.currentPosition = 0, .pivot = list->head};
    while (counter.currentPosition != position) {
        counter.pivot = counter.pivot->next;
        ++counter.currentPosition;
    }

    return counter.pivot;
}

/*
    * Reserve: Create a new list with N empty cells

    * This method creates a new list and inserts empty cells (Cells with NULL value pointer)
    * until the internal counter reaches the 'positions' variable valueW

    * Other Details
    *   Time Complexity: O(n)
    *   For GNU C Compiler (GCC) Users: 'warn_unused_result' flag. Return can't be ignored
*/
__attribute__((warn_unused_result)) struct LinkedList_t* Reserve(size_t positions)
{
    struct LinkedList_t *list = (struct LinkedList_t *) malloc(sizeof(struct LinkedList_t));
    list->head = (struct Cell_t *) malloc(sizeof(struct Cell_t));
    struct __Aux_Counter counter = {.currentPosition = 0, .pivot = list->head};
    while (counter.currentPosition != positions - 1) {
        counter.pivot->next = (struct Cell_t *) malloc(sizeof(struct Cell_t));
        counter.pivot = counter.pivot->next;
        counter.pivot->value = NULL;
        ++counter.currentPosition;
    }
    list->currentSize=positions;
    return list;
}

/*
    * DryNulls: Remove NULL value cells from list

    * This method removes all Cells with internal value pointer set as NULL, and return a new List with this values removed
    * Warning: This method Freeds the old List, so, be careful

    * Other Details
    *   Time Complexity: O(n)
    *   For GNU C Compiler (GCC) Users: 'warn_unused_result' flag. Return can't be ignored
*/
__attribute__((warn_unused_result)) struct LinkedList_t* DryNulls(struct LinkedList_t *list)
{
    struct __Aux_Counter counter = {.currentPosition = 0, .pivot = list->head};
    struct LinkedList_t *newList = (struct LinkedList_t *) malloc(sizeof(struct LinkedList_t)); 
    while (counter.currentPosition != list->currentSize) {
        if (counter.pivot->value != NULL) {
            PushBack(newList, counter.pivot->value);
        }
        counter.pivot = counter.pivot->next;
        ++counter.currentPosition;
    }
    Free(list);
    return newList;
}

/*
    * RetrieveFront: Get the First Element of List

    * This method unbinds the first element from the Linked List, so
    * if you want to just access it, use GetAt(list, LIST_HEAD) (See 'GetAt' for more details)
    
    * Other Details:
    *   Time Complexity: Θ(1)
    *   For GNU C Compiler (GCC) Users: 'warn_unused_result' flag. Return can't be ignored
*/
__attribute__((warn_unused_result)) struct Cell_t* RetrieveFront(struct LinkedList_t *list)
{
    if (Empty(list)) {
        #if WARNINGS_ALLOWED
            Warning("Tried to retrieve from empty list.");
        #endif
        return NULL;
    }

    Cell_t *firstCell = list->head;
    list->head = list->head->next;
    firstCell->next = NULL;
    list->currentSize--;
    return firstCell;
}

/*
    * RetrieveBack: Get the Last Element of List

    * This method unbinds the last element from the Linked List, so
    * if you want to just access it, use GetAt(list, LIST_TAIL) (See 'GetAt' for more details)
    
    * Other Details:
    *   Time Complexity: Θ(n)
    *   For GNU C Compiler (GCC) Users: 'warn_unused_result' flag. Return can't be ignored
*/
__attribute__((warn_unused_result)) struct Cell_t* RetrieveBack(struct LinkedList_t *list)
{
    if (Empty(list)) {
        #if WARNINGS_ALLOWED
            Warning("Tried to retrieve from empty list.");
        #endif
        return NULL;
    }
    struct Cell_t *beforeTarget = NULL;
    struct __Aux_Counter counter = {.currentPosition = 0, .pivot = list->head};

    while (counter.pivot->next != NULL) {
        beforeTarget = counter.pivot;
        counter.pivot = counter.pivot->next;
        ++counter.currentPosition;
    }

    beforeTarget->next = NULL;
    counter.pivot->next = NULL;
    list->currentSize--;
    return counter.pivot;
}

#endif //__LINKED_LIST_H__