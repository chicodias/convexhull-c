// Iterative C program to do merge sort on linked list 
// based on  https://www.geeksforgeeks.org/c-program-for-merge-sort-for-linked-lists/

#include "lista.h" 
#include "primitivas.h"
#include <stdlib.h>

#define swap(type, i, j) {type t = i; i = j; j = t;}

/* Function to calculate length of linked list */
int length(ANGS* current) 
{ 
    int count = 0; 
    while (current != NULL) { 
        current = current->prox; 
        count++; 
    } 
    return count; 
} 
  
/* Merge function of Merge Sort to Merge the two sorted parts 
   of the Linked List. We compare the prox value of start1 and  
   current value of start2 and insert start2 after start1 if  
   it's smaller than prox value of start1. We do this until 
   start1 or start2 end. If start1 ends, then we assign prox  
   of start1 to start2 because start2 may have some elements 
   left out which are greater than the last value of start1.  
   If start2 ends then we assign end2 to end1. This is necessary 
   because we use end2 in aANGSther function (mergeSort function)  
   to determine the prox start1 (i.e) start1 for prox 
   iteration = end2->prox */
void merge(ANGS** start1, ANGS** end1,  
          ANGS** start2, ANGS** end2) 
{ 
  
    // Making sure that first ANGS of second 
    // list is higher. 
    ANGS* temp = NULL; 
    if ((*start1)->ang > (*start2)->ang) { 
        swap(ANGS*,*start1, *start2); 
        swap(ANGS* ,*end1, *end2); 
    } 
  
    // Merging remaining 
    ANGS* astart = *start1, *aend = *end1; 
    ANGS* bstart = *start2, *bend = *end2; 
    ANGS* bendprox = (*end2)->prox; 
    while (astart != aend && bstart != bendprox) { 
        if (astart->prox->ang > bstart->ang) { 
            temp = bstart->prox; 
            bstart->prox = astart->prox; 
            astart->prox = bstart; 
            bstart = temp; 
        } 
        astart = astart->prox; 
    } 
    if (astart == aend) 
        astart->prox = bstart; 
    else
        *end2 = *end1; 
} 
  
/* MergeSort of Linked List 
   The gap is initially 1. It is incremented as  
   2, 4, 8, .. until it reaches the length of the  
   linked list. For each gap, the linked list is  
   sorted around the gap.  
   The prevend stores the address of the last node after 
   sorting a part of linked list so that it's prox node 
   can be assigned after sorting the succeeding list.  
   temp is used to store the prox start1 because after  
   sorting, the last node will be different. So it  
   is necessary to store the address of start1 before  
   sorting. We select the start1, end1, start2, end2 for  
   sorting. start1 - end1 may be considered as a list  
   and start2 - end2 may be considered as aANGSther list  
   and we are merging these two sorted list in merge  
   function and assigning the starting address to the  
   previous end address. */
void mergeSort(ANGS** head) 
{ 
    if (*head == NULL) 
        return; 
    ANGS* start1 = NULL, *end1 = NULL; 
    ANGS* start2 = NULL, *end2 = NULL; 
    ANGS* prevend = NULL; 
    int len = length(*head); 
  
    for (int gap = 1; gap < len; gap = gap*2) { 
        start1 = *head; 
        while (start1) { 
  
            // If this is first iteration 
            boolean isFirstIter = 0; 
            if (start1 == *head) 
                isFirstIter = 1; 
  
            // First part for merging 
            int counter = gap; 
            end1 = start1; 
            while (--counter && end1->prox) 
                end1 = end1->prox; 
  
            // Second part for merging 
            start2 = end1->prox; 
            if (!start2) 
                break; 
            counter = gap; 
            end2 = start2; 
            while (--counter && end2->prox) 
                end2 = end2->prox; 
  
            // To store for prox iteration. 
            ANGS *temp = end2->prox; 
  
            // Merging two parts. 
            merge(&start1, &end1, &start2, &end2); 
  
            // Update head for first iteration, else 
            // append after previous list 
            if (isFirstIter) 
                *head = start1; 
            else
                prevend->prox = start1; 
  
            prevend = end2; 
            start1 = temp; 
        } 
        prevend->prox = start1; 
    } 
} 
    
  
/* Given a reference (pointer to    
   pointer) to the head of a list   
   and an int, push a new node on   
   the front of the list. */
void push(ANGS** head_ref,  
          int new_data)  
{  
    ANGS* new_node = malloc(sizeof(ANGS));  
    new_node->ang = new_data;  
    new_node->prox = (*head_ref);  
    (*head_ref) = new_node;  
}    