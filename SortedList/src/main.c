#include "sorted-list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_SIZE 10
#define FLOAT_SIZE 10

/****************************************************/
// FUNCTION PROTOTYPES 
void printList(SortedListPtr, void (*)(void* ));

void testInt();
int compareInt(void * ,void * );
void destructInt( void * );
void printInt(void* );

void testFloat();
int compareFloat(void*, void*);
void destructFloat(void *);
void printFloat(void*);

void testRemoveAndIteratorConsitency();
/****************************************************/

int main(int argc, char** argv){
    printf("Test Integer List:\n");
    testInt();

    printf("\n\nTest Float List:\n");
    testFloat();
    
    printf("\n\nTest Remove and Iterator Consitency:\n");
    testRemoveAndIteratorConsitency();
    printf("\n");
    return 0;

}
void printList(SortedListPtr intList, void (*printType)(void* i)){
    SortedListIteratorPtr sli = SLCreateIterator(intList);
    // Print all element
    void* temp = SLGetItem(sli);
    while( temp!=NULL){
        printType(temp);
        temp= SLNextItem(sli);
    }
    SLDestroyIterator(sli);
}
//=======================================================================
void testInt(){
    // Create an integer list
    SortedListPtr intList = SLCreate(compareInt,destructInt);
    int intArr[INT_SIZE] = { 0, 1 , 2 , 3 , 4 , 5, 6, 7, 8, 9};
    int* intPtr[INT_SIZE];
    int i;
    // Allocate memory for integer pointer elements
    for(i = 0 ; i <  INT_SIZE; i++){
        intPtr[i] = (int*) malloc(sizeof(int)); 
        if(intPtr[i]==NULL){
            printf("Error: Cannot allocate memory\n");
            exit(-1);
        }else{
            *intPtr[i] = intArr[i];
            if(i==0){
                printf("Insert order: \n%d",intArr[i]);
            }else{
                printf("\t%d",intArr[i]);
            }
            SLInsert(intList,intPtr[i]); 
        }
    }
    printf("\nThe sorted integer list is:\n");
    printList(intList, printInt);
    SortedListIteratorPtr sli = SLCreateIterator(intList);
    SLNextItem(sli);
    // Destroy iterator and list
    SLDestroy(intList);
    SLDestroyIterator(sli);
    

}

int compareInt(void * int1, void * int2){
    int a = *((int*) int1);
    int b = *((int*) int2);
    if( a > b ){
        return 1;
    }else if(a<b){
        return -1;
    }else return 0;
}   

void destructInt(void* i){
    free( (int* ) i);
}
void printInt(void* i){
    if(i==NULL){
        printf("null\t");
    }else
        printf("%d\t",*((int*)i) );
} 
//=======================================================================
void testFloat(){
    // Create an floating number list
    SortedListPtr floatList = SLCreate(compareFloat,destructFloat);
    float floatArr[FLOAT_SIZE] = { 0.23, 11 , 2.3 , 0.3 , -14.23 , 5.1, 26.00, -7, 8.14, 9.21};
    float* floatPtr[FLOAT_SIZE];
    int i;
    // Allocate memory for float number pointer elements
    for(i = 0 ; i <  INT_SIZE; i++){
        floatPtr[i] = (float*) malloc(sizeof(float)); 
        if(floatPtr[i]==NULL){
            printf("Error: Cannot allocate memory\n");
            exit(-1);
        }else{
            *floatPtr[i] = floatArr[i];
            if(i==0){
                printf("Insert order: \n%.2f",floatArr[i]);
            }else{
                printf("\t%.2f",floatArr[i]);
            }
            SLInsert(floatList,floatPtr[i]); 
        }
    }
    printf("\nThe sorted float list is:\n");
    printList(floatList,printFloat); 
    SLDestroy(floatList);
}


int compareFloat(void * f1, void* f2){
    float a = *((float*) f1);
    float b = *((float*) f2);
    if(a>b){
        return 1;
    }else if(a<b){
        return -1;
    }else return 0;
}

void destructFloat(void* f){
    free((float*) f);
}
void printFloat(void* i){
    if(i==NULL)
        printf("null\t");
    else printf("%.02f\t",*((float*)i));
}
//=======================================================================

void testRemoveAndIteratorConsitency(){
    // Create an integer list
    SortedListPtr intList = SLCreate(compareInt,destructInt);
    int intArr[INT_SIZE] = { 0, 1 , 2 , 3 , 4 , 5, 6, 7, 8, 9};
    int* intPtr[INT_SIZE];
    int i;
    // Allocate memory for integer pointer elements
    for(i = 0 ; i <  INT_SIZE; i++){
        intPtr[i] = (int*) malloc(sizeof(int)); 
        if(intPtr[i]==NULL){
            printf("Error: Cannot allocate memory");
            exit(-1);
        }else{
            *intPtr[i] = intArr[i];
            SLInsert(intList,intPtr[i]); 
        }
    }

    // Create an iterator
    SortedListIteratorPtr sli = SLCreateIterator(intList);
    // Print all elements
    printf("Elements of the sorted list:\n");
    printList(intList, printInt);
    SLDestroyIterator(sli);

    sli = SLCreateIterator(intList);
    printf("\nNow, a new iterator created is pointing to: %d" , *(int*) SLGetItem(sli));
    // Remove elements
    for( i = 9 ; i>=6;i--){
        printf("\nRemove: "); printInt((void*)intPtr[i]);SLRemove(intList,intPtr[i]);
    }
    printf("\nElements of the sorted list:\n");
    printList(intList,printInt);
    printf("\nThe iterator is \"still\" pointing to: %d",*(int*) SLGetItem(sli));
    printf("\nAdvance the iterator using SLNextItem(). Now it's pointing to: %d",*(int*) SLNextItem(sli));

    // Destroy iterator and list
    printf("\n\nNow, we destroy the list.");
    SLDestroy(intList);
    printf("\nThe iterator can return %d using SLGetItem().",*(int*) SLGetItem(sli));
    printf("\nThe next element is : ");
    printInt( SLNextItem(sli) );
    printf("\nDestroy the iterator");   
    SLDestroyIterator(sli);
}


//=======================================================================
