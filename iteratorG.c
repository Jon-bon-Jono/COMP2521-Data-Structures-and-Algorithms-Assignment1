/* iteratorG.c
   Generic Iterator implementation, using doubly linked list

   Written by: Jonathan Williams z5162987
   Date: April 2018


*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "iteratorG.h"
#include <unistd.h> 
#include <math.h>

typedef struct Node {
   void* data;
   struct Node* prev;
   struct Node* next;
  
} Node;

typedef struct IteratorGRep {
   Node* curs;  //curs is used to keep track of the cursor, it will be infront of the cursor at all times
   
   //empty nodes used to keep track of the cursor when at position 0, 1, n and n+1   
   Node* mtstart; 
   Node* mtend;
   
   ElmCompareFp cmpElm;
   ElmNewFp newElm;
   ElmFreeFp freeElm;

} IteratorGRep;


IteratorG newIterator(ElmCompareFp cmpFp, ElmNewFp newFp, ElmFreeFp freeFp){
   IteratorG newIt;
   newIt = malloc(sizeof(struct IteratorGRep));
   assert (newIt != NULL);
   //establish an 'empty' newIterator with nodes mtend and mtstart, curs will point at mtstart
   newIt->mtstart = malloc(sizeof(struct Node));
   newIt->mtstart->data = NULL;
   newIt->mtend = malloc(sizeof(struct Node));
   newIt->mtend->data = NULL;       

   newIt->mtend->next = NULL;
   newIt->mtstart->prev = NULL;
    
   newIt->mtstart->next = newIt->mtend;
   newIt->mtend->prev = newIt->mtstart;
   newIt->curs = newIt->mtend;
   //at this moment newIt should look like:  {mtstart}-> ^ <-{mtend}(<-curs)
   
   newIt->cmpElm = cmpFp;
   newIt->newElm = newFp;
   newIt->freeElm = freeFp;
   return newIt;

}

int  add(IteratorG it, void *vp){
   Node* new = malloc(sizeof(Node));
   if(new == NULL){
      fprintf(stderr, "Error -- unable to add new node");
      return 0;
   }
   new->data = it->newElm(vp);
   
   //insert new node into the list
   it->curs->prev->next = new;
   new->prev = it->curs->prev;
   it->curs->prev = new;
   new->next = it->curs;
   
   //ensure that the cursor is behing it->curs
   it->curs = new;
   
   return 1;
   
}
int  hasNext(IteratorG it){
   //if it->curs is pointing to mtend
   if(it->curs->next == NULL) return 0;
   else return 1;
}
int  hasPrevious(IteratorG it){
   if(it->curs->prev == it->mtstart) return 0;
   else return 1;
}
void *next(IteratorG it){
   if(hasNext(it)){
      it->curs = it->curs->next;
      return it->curs->prev->data;
   }
   return NULL;
}
void *previous(IteratorG it){
   if(hasPrevious(it)){
      it->curs = it->curs->prev;
      return it->curs->data;
   }
   return NULL;
}
int  del(IteratorG it){
   if(hasPrevious(it)){
      //unplug node
      Node* tmp = it->curs->prev;
      tmp->prev->next = it->curs;
      it->curs->prev = tmp->prev;
      
      //free node
      it->freeElm(tmp);
      return 1;
   }
   //else no previous element to delete
   return 0;
}
int  set(IteratorG it, void *vp){
   if(hasPrevious(it)){
      it->curs->prev->data = vp;
      return 1;
   }
   return 0;
}
IteratorG advance(IteratorG it, int n){
   IteratorG advancenew = newIterator(it->cmpElm, it->newElm, it->freeElm);
   int count;
   //first determine the sign of n
   if(n > 0){
      //if we can't move forward n places, return NULL
      if(distanceToEnd(it) < n) return NULL;
      
      for(count = 1; count <= n; count++){
         add(advancenew, it->curs->data); //add nodes until count = n
         it->curs = it->curs->next;
      }
      reverse(advancenew); //reverse the order since add() places the a new node prev to the cursor
      reset(advancenew); //move the cursor to the start of the reversed list
      return advancenew; 
   }else if(n < 0){
      //if we can't move backwards n places, return NULL
      if(distanceFromStart(it) < abs(n)) return NULL;

      for(count = 1; count <= abs(n); count++){
         add(advancenew, it->curs->prev->data); //add nodes until count = abs(n)
         it->curs = it->curs->prev;
      }
      reverse(advancenew); 
      reset(advancenew); 
      return advancenew;
   }else{ //n == 0
      return advancenew; //return an empty list
   }
   return NULL;
}
void reverse(IteratorG it){
   //reverse begins with pointers pointing at the first and last nodes
   //each iteration the nodes are swapped and there pointers gradually come to the centre of the list
   
   Node* LHS = it->mtstart->next; //point LHS to the first node in the list
   //keep pointers for the nodes either side of LHS
   Node* LHSnext;// = LHS->next 
   Node* LHSprev;// = LHS->prev;
   
   Node* RHS = it->mtend->prev; //point RHS to the last node in the list
   //keep pointers for the node either side of RHS
   Node* RHSnext;// = RHS->next; 
   Node* RHSprev;// = RHS->prev;
   
   Node* tmp; //used for swapping it->curs
   int edge_curs_set = 0; //used to indicate whether a cursor at either ends has already been moved
   
   //if the cursor points infront of the first node we set it to the correct position at the end of the list
   if(it->curs == it->mtstart->next){
      it->curs = it->mtend;
      edge_curs_set = 1;
   }

   //the loop will iterate until both LHS and RHS point to the same node
   //if LHS and RHS are adjacent they are swapped and the loop breaks
   while(LHS != RHS){
      //reset pointers relative to where LHS and RHS are in this iteration
      LHSnext = LHS->next; 
      LHSprev = LHS->prev;
      RHSnext = RHS->next;
      RHSprev = RHS->prev;
      
      //SWAPPING THE POSITIONS OF LHS AND RHS:
      //1). if LHS and RHS are adjacent, you can skip some steps
      if(LHS->next == RHS && RHS->prev == LHS){
        //{LHSprev}--><--{LHS}--><--{RHS}--><--{RHSnext}
        LHS->next = RHSnext;
        RHSnext->prev = LHS;
        RHS->prev = LHSprev;
        LHSprev->next = RHS;
        LHS->prev = RHS;
        RHS->next = LHS;
        //{LHSprev}--><--{RHS}--><--{LHS}--><--{RHSnext}
        //now swap which nodes LHS and RHS point to
         tmp = LHS;
         LHS = RHS;
         RHS = tmp;
         break;
      //2). if LHS and RHS are adjacent, more steps are required
      }else{
        LHS->prev = RHSprev;  //--{RHSprev}<--{LHS}--><--{LHSnext}--
        RHSprev->next = LHS;  //--{RHSprev}--><--{LHS}--><--{LHSnext}--

        LHS->next = RHSnext;  //--{RHSprev}--><--{LHS}-->{RHSnext}--
        //printf("THis is where things go wrong\n");
        RHSnext->prev = LHS;  //--{RHSprev}--><--{LHS}--><--{RHSnext}--

        RHS->prev = LHSprev;  //--{LHSprev}<--{RHS}--><--{RHSnext}--
        LHSprev->next = RHS;  //--{LHSprev}--><--{RHS}--><--{RHSnext}--

        RHS->next = LHSnext;  //--{LHSprev}--><--{RHS}-->{LHSnext}--
        LHSnext->prev = RHS;  //--{LHSprev}--><--{RHS}--><--{LHSnext}--
        
        //now swap which nodes LHS and RHS point to
        tmp = LHS;
        LHS = RHS;
        RHS = tmp;
      }
            
      if(LHS == it->curs){ 
         it->curs = RHS;
      }else if(RHS == it->curs){
         it->curs = LHS;
      }
      //move LHS forward and move RHS backwards 
      LHS = LHS->next;
      RHS = RHS->prev;
   }
   
   //conditions necessary when the cursor is at the end of the list and hasn't already been moved
   if(it->curs == it->mtend && edge_curs_set == 0){
      it->curs = it->mtstart->next;
   }
	return;
}
IteratorG find(IteratorG it, int (*fp) (void *vp) ){
   IteratorG findsnew = newIterator(it->cmpElm, it->newElm, it->freeElm);
   //if the cursor is at the end of the list, return the empty list
   if(!hasNext(it)) return findsnew;
   Node* tmp = it->curs;
   while(1){
      if(fp(it->curs->data)){ //if fp returns 1 add a new node with curs->data
         add(findsnew, it->curs->data);
      }
      //if curs is at the last data entry node break
      if(it->curs->next == it->mtend) break;
      it->curs = it->curs->next;
   }
   it->curs = tmp;
   reverse(findsnew);
   reset(findsnew);
   return findsnew;
}

int distanceFromStart(IteratorG it){
   int dist = 0;
   Node* tmp = it->curs;
   while(tmp->prev != it->mtstart){
      dist++;
      tmp = tmp->prev;
   }
   return dist;

}
int distanceToEnd(IteratorG it){
   int dist = 0;
   Node* tmp = it->curs;
   while(it->curs->next != NULL){
      dist++;
      it->curs = it->curs->next;
   }
   it->curs = tmp;
   return dist;
}
void reset(IteratorG it){
   while(hasPrevious(it)){
      it->curs = it->curs->prev;
   }
   return;
}
void freeIt(IteratorG it){
   reset(it);
   while(1){
      it->freeElm(it->curs->prev);
      if(!hasNext(it)) break;
      it->curs = it->curs->next;
   }
	return;
}






