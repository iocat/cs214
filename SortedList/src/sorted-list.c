#include "sorted-list.h"
#include "stdlib.h"
#include <stdio.h>

typedef struct Node Node;
typedef Node* NodePtr;
/*
 *  Node_ is a struct that represents the linked list's nodes
 */
struct Node{
    // Count the number of pointer currently point to this node.
    // Default value should be 1 ( previus node points to this)
    // The iterator should keep track of this. 
    int ptrCount; 

    // The data that the node contains    
    void* data;
    
    // a pointer that point to the next node
    NodePtr next;
};
/*
 * nodeDestroy destroys the chosen node and every node linked to it if this current node has 
 * only one pointer pointing to it
 */
NodePtr nodeDestroy(NodePtr node, DestructFuncT destruct){
    if(node==NULL){
        return NULL;
    }else if ( node->ptrCount ==1) {
        if(destruct!= NULL){
            destruct(node->data);
        }
        NodePtr temp = node->next;
        free(node);
        return nodeDestroy(temp,destruct);
    }else{
        node->ptrCount--;
        return node;
    }
}

void point(NodePtr from, NodePtr to){
    from->next = to;
    if(to!=NULL)
        to->ptrCount++;
}
/*
 * NodeCreate creates a new node
 */
NodePtr nodeCreate(void* data){
    NodePtr temp = (NodePtr) malloc(sizeof(Node));
    if(temp == NULL){
        printf("Error: Cannot allocate memory for the node");
        exit(-1);
    }else{
        // Initialize the pointer counter
        temp->ptrCount=1;
        
        // Put the data into the node
        temp->data = data;
    
        // The next node is NULL
        temp->next = NULL;
        return temp;
    }
}

struct SortedList{
    // The compare function
    CompareFuncT compare;

    // The destruct function
    DestructFuncT destruct;

    // The first node of the singly linked list
    Node* first;

};

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
    SortedListPtr sl = (SortedListPtr) malloc(sizeof(struct SortedList ));
    if(sl == NULL){
        // SLCreate fails to create a new SortedListPtr
        return NULL;
    }else{
        // Assign the compare function
        sl->compare = cf;

        // Assign the destruct function
        sl->destruct = df;

        sl->first = NULL;
        return sl;
    }
}

void SLDestroy(SortedListPtr list){
    // Destroy all nodes that has less than 2 pointers point to it 
    //nodeDestroy(list->first,list->destruct); 
    while(list->first!=NULL)
        SLRemove(list, list->first->data);
    //Free memory that is allocated for the SortedList
    free(list);
       
}

int SLInsert(SortedListPtr list, void* newObj){
    if(list->first == NULL){
        list->first = nodeCreate(newObj);
        return 1;
    }else{
        NodePtr previous = NULL;
        NodePtr smallerNode = list->first;
        int comp ;
        while( (comp = list->compare(smallerNode->data,newObj))==1)  {
            if(smallerNode->next == NULL){
                // Reach the end of the list
                smallerNode->next = nodeCreate(newObj);
                return 1;       
            }else{
                previous = smallerNode;
                smallerNode = smallerNode->next;
            }
        }
        if(comp == 0){
            // Object is equal to an item in the list
            return 0;
        }else{
            if(previous == NULL){
                list->first = nodeCreate(newObj);
                list->first->next = smallerNode;  
            }else{
                // Create a new node and make the previous node of temp point to it
                previous->next = nodeCreate(newObj);
                // Make the new created node point to the smaller node 
                previous->next->next = smallerNode;
            }
            return 1;
        }
    }
}

int SLRemove(SortedListPtr list, void *newObj){
    if(list->first == NULL) {
        // Empty list
        return 0;
    } else{
          
        NodePtr curr = list->first;
        NodePtr prev = NULL;
       
        while(curr!=NULL && list->compare(curr->data, newObj)!=0){
            prev = curr;
            curr= curr->next;
        }
        if(curr == NULL){
            // Unfound node
            return 0 ;
        }else{
            if(prev == NULL){
                
                NodePtr temp = list->first;
                list->first->ptrCount--;
                list->first = list->first->next;
                if(list->first!=NULL){
                    list->first->ptrCount++;
                }
                if(temp->ptrCount == 0){
                    list->destruct(temp->data);
                    if(temp->next!=NULL){
                        temp->next->ptrCount--;
                    }                          
                    free(temp);            
                }
                
            }else{
                prev->next = curr->next;
                if(curr->next!=NULL){
                    curr->next->ptrCount++;
                }
                nodeDestroy(curr,list->destruct);
            }
            return 1;
        }       
    } 
}

struct SortedListIterator{

    NodePtr current;  

    DestructFuncT destruct;    

};
SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
    // Allocate memory for the iterator
    SortedListIteratorPtr sli = (SortedListIteratorPtr) malloc(sizeof(struct SortedListIterator));
    if(sli == NULL || list == NULL){
        // If the iterator is not allocated or the list is NULL
        return NULL;
    }else{
        // Point the current pointer to the first item of the list
        sli->current = list->first;
        // Increase the number of pointer points to it
        list->first->ptrCount++;
        //  copy the destruct function
        sli->destruct = list->destruct;
 
       return sli;
    }
}

void* SLNextItem(SortedListIteratorPtr iter){
    if(iter->current==NULL){
        // There is no next item in the linked list
        return NULL;
    }else if(iter->current->ptrCount == 1){
        // If the number of pointers point to the current node is 1
        //NodePtr nextNode = iter->current->next;
        // Destroy the current node
        iter->current = nodeDestroy(iter->current,iter->destruct);
        // Assign the current as the next one
        //iter->current  = nextNode;
    }else{
        // Decrease the pointer counter by one
        iter->current->ptrCount--;
        // Point the next node
        iter->current = iter->current->next;
    }
    // Increase the pointer counter by one.
    if(iter->current!=NULL)
        iter->current->ptrCount++;
    return SLGetItem(iter);
}

void* SLGetItem(SortedListIteratorPtr iter){
    if(iter->current==NULL){
        return NULL;    
    }else return iter->current->data;
}

void SLDestroyIterator(SortedListIteratorPtr iter){
    nodeDestroy(iter->current,iter->destruct);
    free(iter); 
}
