#ifndef SORTED_LIST_H
#define SORTED_LIST_H
#include <stdlib.h>

//======Prototypes for User-Defined Functions==========
/*
 * Your list is used to store data items of an unknown type, which you need to sort.
 * Since the type is opaque to you, you do not know how to directly compare the data.
 *
 * You can presume though that a user will supply your code with a comparator function
 * that knows how to compare the data being sorted, but your code must do the rest
 * of the bookkeeping in a generic manner.
 *
 * The comparator function will take pointers to two data items and will return -1 if the 1st 
 * is smaller, 0 if the two are equal, and 1 if the 2nd is smaller.
 *
 * The user will also supply a destruct function will take a pointer to a single data item
 *	and knows how to deallocate it. If the data item does not require deallocation, the user's
 *  destruct function will do nothing.
 */

typedef int (*CompareFuncT)( void *, void * );
typedef void (*DestructFuncT)( void * );
/*
 * Sorted list type that will hold all the data to be sorted.
 */
struct SortedList;
typedef struct SortedList* SortedListPtr;


/*
 * SLCreate creates a new, empty, 'SortedList'.
 *
 * SLCreate's parameters will be a comparator (cf) and destructor (df) function.
 *   Both the comparator and destructor functions will be defined by the user as per
 *     the prototypes above.
 * 
 * SLCreate must return NULL if it does not succeed, and a non-NULL SortedListPtr
 *   on success.
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df);


/*
 * SLDestroy destroys a SortedList, freeing all dynamically-allocated memory.
 */
void SLDestroy(SortedListPtr list);

/*
 * SLInsert inserts a given data item 'newObj' into a SortedList while maintaining the
 *   order and uniqueness of list items.
 * 
 * SLInsert should return 1 if 'newObj' is not equal to any other items in the list and
 *   was successfully inserted.
 * SLInsert should return 0 if 'newObj' is equal to an item already in the list or it was
 *   not successfully inserted
 *
 */

int SLInsert(SortedListPtr list, void *newObj);

/*
 * SLRemove should remove 'newObj' from the SortedList in a manner that
 *   maintains list order.
 *
 * SLRemove should return 1 on success, and 0 on failure.
 */

int SLRemove(SortedListPtr list, void *newObj);

/*
 * SortListIterator allows a SortedList to be easily 'walked' through,
 *  item by item using repeated calls to 'SLNextItem'.
 * Each 'SLNextItem' call to a SortedListIterator should result in the 
 *  next consecutive item in a SortedList, from the beginning to the end.
 * A SortedListIterator provides a one-way traversal through all of a SortedList
 */
struct SortedListIterator;
typedef struct SortedListIterator* SortedListIteratorPtr;


/*
 * SLCreateIterator creates a SortedListIterator for the SortedList pointed to by 'list'.
 *
 * SLCreateIterator should return a non-NULL SortedListIteratorPtr on success.
 * SLCreateIterator should return a NULL SortedListIteratorPtr if it could not
 *  construct a SortedListIterator, or if the SortedListPtr parameter 'list' is NULL.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list);


/*
 * SLDestroyIterator destroys a SortedListIterator pointed to by parameter 'iter'.
 *
 * SLDestroyIterator should destroy the SortedListIterator, but should NOT
 *  affect the list used to create the SortedListIterator in any way.
 */

void SLDestroyIterator(SortedListIteratorPtr iter);



//===1.2: SortedList Iterator Get/Next Operations

/*
 * SLNextItem returns a pointer to the data associated with the
 *  next item in the SortedListIterator's list
 *
 * SLNextItem should return a NULL if all of iter's elements have
 *  been iterated through.
 */

void * SLNextItem(SortedListIteratorPtr iter);


/*
 * SLGetItem should return a pointer to the current data item
 *   The current data item is the item that was most recently returned by SLNextItem
 *   SLGetItem should not alter the data item that SortedListIterator currently refers to
 *
 * SLGetItem should return a NULL pointer if the SortedListIterator has been advanced
 *  all the way through its list.
 *
 */

void * SLGetItem( SortedListIteratorPtr iter );

#endif
