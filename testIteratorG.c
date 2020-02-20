/*
  testIteratorG.c 
  Client to test Generic Iterator 
  
  Written by: Ashesh Mahidadia
  Date: April 2018 
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "iteratorG.h"
#include "positiveIntType.h"
#include "stringType.h" 

#define MAXARRAY 5

/* Helper Functions Below */

/* Returns 1 if marks >= 50, 0 otherwise  */
int passMarks(void *marks){
  return (*((int *) marks) >= 50); 
  
  /* Easy to understand below .. 
     int *ip = (int *) marks;
     if(*ip >= 50) { return 1; }
     else { return 0; } 
  */
}

/* Returns 1 if str starts with "jo" */
int prefixJo(void *str){
  return (strncmp("jo", (char *) str, 2) == 0) ; 
}

/* A function to print a string from a void pointer */
void prnStr(void *vp){
  assert(vp != NULL);
  printf(" %s", (char *) vp );  	
}

/* A function to print an integer from a void pointer */
void prnInt(void *vp){
  assert(vp != NULL);
  //printf(" %d", *((int *) vp) );
  printf(" %d", *((int *) vp) );  	
}

/* Prints previous element using the given function 'fp'
   examples: prnPrev(it1, prnInt); prnPrev(it2, prnStr);
*/
void prnPrev(IteratorG it, void (*fp) (void *p) ){
  void *prevP = previous(it);
  assert(prevP != NULL);
  printf("> Previous value is: "); 
  fp(prevP);
  printf("\n"); 
}

/* Prints next element using the given function 'fp'
   examples:   prnNext(it1, prnInt); prnNext(it2, prnStr);
*/
void prnNext(IteratorG it, void (*fp) (void *p) ){
  void *nextP = next(it);
  assert(nextP != NULL);
  printf("> Next value is: "); 
  fp(nextP);
  printf("\n"); 
}

/* Prints elements of 'it' from current to last position 
   using the given function 'fp'. The current position 
   of 'it' will change to the end of the list.
   examples: prnIt(it1, prnInt); prnIt(it2, prnStr);
*/
void prnIt(IteratorG it, void (*fp) (void *p) ){
  int count = 0;
  while(hasNext(it)){
    void *nextP = next(it);	
    count++;
    if(count > 1) { printf(", "); }
    fp(nextP); 		
  }
  printf("\n"); 
}


/* Few Tests Below  */

void test1(){
  printf("\n--====  Test-01       ====------\n");
  IteratorG it1 = newIterator(positiveIntCompare, positiveIntNew, positiveIntFree);
  int a[MAXARRAY] = { 25, 78, 6, 82 , 11};
  //int a[1] = {1};
  //printf("NULL");
  for(int i=0; i<MAXARRAY; i++){
    int result = add(it1 , &a[i]);
    printf("> Inserting %d: %s \n", a[i], (result==1 ? "Success" : "Failed") );
  }
  reset(it1);
  prnIt(it1, prnInt);
 
 
 //bunch of test
 /*
  //executed from the beginning of the list, prints the distances and the next element in the list until it reaches the end 
  int k;
  printf("Distance from start: %d\n", distanceFromStart(it1));
  printf("Distance to end: %d\n", distanceToEnd(it1));
  while(1){
      k = hasNext(it1);
      if(k == 0) break;
      prnNext(it1, prnInt);
      printf("Distance from start: %d\n", distanceFromStart(it1));
      printf("Distance to end: %d\n", distanceToEnd(it1));
      k = hasNext(it1);
      printf("Has next?: %s\n", (k==1? "Yes" : "No"));
      if(k == 0) break;

  }
  printf("\n");
  reset(it1);
  prnIt(it1, prnInt); //prnIt puts curs back to the end of the list
  printf("\n");
  //executed from the end, prints each previous value and the distances until the start of the list
  while(1){
     prnPrev(it1, prnInt);
     k = hasPrevious(it1);
     printf("Has prev?: %d\n", k);
     printf("Distance from start: %d\n", distanceFromStart(it1));
     printf("Distance to end: %d\n", distanceToEnd(it1));
     if(k == 0) break;
 }
 
 
 printf("\n\n ----Test set() form the start of the list----\n");
 //test set() from the start of the list
 printf("Since we are at the very start of the list, set should return 0\n");
 reset(it1);
 int newVal0 = 66;
 int result1 = set(it1, &newVal0);
  printf("> Set value: %d ; return val: %d \n", newVal0,  result1 );
 
 printf("so traverse the list\n");
 prnNext(it1, prnInt);
 result1 = set(it1, &newVal0);
 printf("> Set value: %d ; return val: %d \n", newVal0,  result1 );
 reset(it1);
 prnIt(it1, prnInt);
 
 
 
 printf("\n\n -----Test set() form the end of the list-----\n");
 //test set() from the very end of the list
 //prints entire list
 printf("(pushes the cursor to the very end of the list)\n");
 prnIt(it1, prnInt);
 printf("(traverses back from the very end of list)\n");
 prnPrev(it1, prnInt);
 printf("(traverses back to the end of the list)\n");
 prnNext(it1, prnInt);
 printf("(traverses back from the very end of list)\n");
 prnPrev(it1, prnInt);
  int newVal1 = 55;
  result1 = set(it1, &newVal1);
  printf("> Set value: %d ; return val: %d \n", newVal1,  result1 );
  reset(it1);
  prnIt(it1, prnInt);
 
  
  printf("\n\n -----Test delete() from the beginning of the list----\n");
  //TEST THE DELETE FUNCTION
  reset(it1);
  printf("We are currently at the beginning of the list so, delete should return 0\n");
  result1 = del(it1);
  printf("> Return value: %d \n", result1 );
  prnIt(it1, prnInt);
  reset(it1);
  printf("Traverse forward two spaces\n");
  prnNext(it1, prnInt);
  prnNext(it1, prnInt);
  result1 = del(it1);
  printf("> Return value: %d \n", result1 );
  result1 = del(it1);
  printf("> Return value: %d \n", result1 );
  reset(it1);
  prnIt(it1, prnInt);
  result1 = del(it1);
  printf("> Return value: %d \n", result1 );
  reset(it1);
  prnIt(it1, prnInt);
  
  freeIt(it1);
  //RECREATE IT1
   printf("\n");
   printf("go ahead and re-create original it1\n");
   it1 = newIterator(positiveIntCompare, positiveIntNew, positiveIntFree);
   //a[MAXARRAY] = {25, 78, 6, 82 , 11};
  //int a[1] = {1};
  //printf("NULL");
  for(int i=0; i<MAXARRAY; i++){
    int result = add(it1 , &a[i]);
    printf("> Inserting %d: %s \n", a[i], (result==1 ? "Success" : "Failed") );
  }
  prnNext(it1, prnInt);
  prnNext(it1, prnInt);
  prnNext(it1, prnInt);
  prnNext(it1, prnInt);
  prnNext(it1, prnInt);
  printf("Has next?: %s\n", (hasNext(it1)? "Yes" : "No"));
  
  printf("Distance from start: %d\n", distanceFromStart(it1));
  printf("Distance to end: %d\n", distanceToEnd(it1));
  //return;
  reset(it1);
  prnIt(it1, prnInt);*/
  freeIt(it1);	
  printf("--====  End of Test-01 ====------\n");
}

void test2(){
  printf("\n--====  Test-02       ====------\n");
  IteratorG it1 = newIterator(positiveIntCompare, positiveIntNew, positiveIntFree);
  int a[MAXARRAY] = { 72, 14, 62, 8, 93};
  for(int i=0; i<MAXARRAY; i++){
    int result = add(it1 , &a[i]); 
    printf("> Inserting %d: %s \n", a[i], (result==1 ? "Success" : "Failed") );
  }
  int k;
  while(1){
      k = hasNext(it1);
      if(k == 0) break;
      printf("Distance from start: %d\n", distanceFromStart(it1));
      //printf("Distance to end: %d\n", distanceToEnd(it1));
      prnNext(it1, prnInt);
      printf("Has next?: %s\n", (k==1? "Yes" : "No"));

  }
  prnIt(it1, prnInt);
  
  while(1){
     prnPrev(it1, prnInt);
     k = hasPrevious(it1);
     printf("Has prev?: %d\n", k);
     printf("Distance from start: %d\n", distanceFromStart(it1));
     printf("Distance to end: %d\n", distanceToEnd(it1));
     if(k == 0) break;
 }
  reset(it1);
  prnIt(it1, prnInt);
  
  printf("Advance the cursor two positions then reverse the list\n");
  reset(it1);
  prnNext(it1, prnInt);
  prnNext(it1, prnInt);
  
  int newVal1 = 1011;
  int result1 = set(it1, &newVal1);
  printf("> Set value: %d ; return val: %d \n", newVal1,  result1 );
  //reset(it1);
  //prnIt(it1, prnInt);
  
  printf("add another node to have an even number of nodes, then try reverse\n");
  int zebow = 27;
  result1 = add(it1 , &zebow); 
  printf("> Inserting %d: %s \n", zebow, (result1==1 ? "Success" : "Failed"));
  reset(it1);
  prnIt(it1, prnInt);
  reverse(it1);
  //printf("print previous \n");
  //prnPrev(it1, prnInt);
  printf("reset the list then set new and print it out (cursor is infront of setted number\n");
  //reset(it1);
  //prnIt(it1, prnInt);
  
  newVal1 = 55;
  result1 = set(it1, &newVal1);
  printf("> Set value: %d ; return val: %d \n", newVal1,  result1 );
  prnIt(it1, prnInt);
  //prnPrev(it1, prnInt);
  //result1 = del(it1); 
  
  
  
  //print the list as a whole
  reset(it1);
  prnIt(it1, prnInt);
  /*while(hasNext(it1)){
     printf("{%d}-><-", it1->curs->data);
     it1->curs = it1->curs->next;
  }*/
  
  //prnIt(it1, prnInt);
  
  //freeIt(it1);
  
  printf("--====  End of Test-02 ====------\n");

}

void test3(){
  printf("\n--====  Test-03       ====------\n");
  IteratorG it1 = newIterator(positiveIntCompare, positiveIntNew, positiveIntFree);
  int a[MAXARRAY] = { 04, 54, 15, 12, 34};
  for(int i=0; i<MAXARRAY; i++){
    int result = add(it1 , &a[i]); 
    printf("> Inserting %d: %s \n", a[i], (result==1 ? "Success" : "Failed") );
  }
  
  reset(it1);
  printf("> it1 (after reset): \n");
  prnIt(it1, prnInt);
  reset(it1);
  
  IteratorG advIt1 = advance(it1, 4);
  printf("> advance(it1, 4) returns: \n");
  prnIt(advIt1, prnInt);
  
  IteratorG advIt2 = advance(it1, -3);
  printf("> advance(it1, -3) returns: \n");
  prnIt(advIt2, prnInt);
  
  printf("> In 'it1', ");
  prnPrev(it1, prnInt);
      
  freeIt(it1);
  freeIt(advIt1);
  freeIt(advIt2);

  printf("--====  End of Test-03 ====------\n");
}

void test4(){
  printf("\n--====  Test-04       ====------\n");
  IteratorG it1 = newIterator(positiveIntCompare, positiveIntNew, positiveIntFree);
  int a[MAXARRAY] = { 97, 10, 61, 73, 47};
  for(int i=0; i<MAXARRAY; i++){
    int result = add(it1 , &a[i]); 
    printf("> Inserting %d: %s \n", a[i], (result==1 ? "Success" : "Failed") );
  }

  reset(it1);
  printf("> it1 (after reset): \n");
  prnIt(it1, prnInt);
  reset(it1);
  
  IteratorG findIt1 = find(it1, passMarks);
  printf("> find(it1, passMarks) returns: \n");
  prnIt(findIt1, prnInt);
  //test of findIt1
  printf("----Playin with out new list------ \n");
  reset(findIt1);
  prnNext(findIt1, prnInt);
  prnNext(findIt1, prnInt);
  prnPrev(findIt1, prnInt);
  del(findIt1);
  reset(findIt1);
  prnIt(findIt1, prnInt);
  reset(findIt1);
  prnNext(findIt1, prnInt);
  int eddies_mark = 2;
  set(findIt1, &eddies_mark);
  reset(findIt1);
  prnIt(findIt1, prnInt);
  
  
  printf("---Back to using our original list----\n ");
  printf("\n");
  printf("> In 'it1', \n");
  prnNext(it1, prnInt);
  prnNext(it1, prnInt);
  int delResult = del(it1);
  printf("\n> del(it1) results in the following and returns %d: \n", delResult );
  reset(it1);
  printf("> it1 (after reset): \n");
  prnIt(it1, prnInt);

  freeIt(it1);
  freeIt(findIt1);

  printf("--====  End of Test-04 ====------\n");
}

void test5(){
  printf("\n--====  Test-05       ====------\n");
  IteratorG it = newIterator(stringCompare, stringNew, stringFree);
  
  char *strA[MAXARRAY];
  strA[0] = strdup("joe");
  strA[1] = strdup("rita");
  strA[2] = strdup("john");
  strA[3] = strdup("abby"); 
  strA[4] = strdup("peter"); 
  
  int j;
  for(j=0; j<MAXARRAY; j++){
    int result = add(it , strA[j]); 
    printf("> Inserting %s: %s \n", strA[j], (result==1 ? "Success" : "Failed") );
  }

  prnNext(it, prnStr);
  prnNext(it, prnStr);
  prnPrev(it, prnStr);
  
  char *newStr1 = "sydney";
  int result2 = set(it, newStr1);
  printf("> Set value: %s ; return val: %d \n", newStr1,  result2 );  
  
  prnPrev(it, prnStr);

  freeIt(it);
  printf("--====  End of Test-05 ====------\n");
}


void test6(){
  printf("\n--====  Test-06       ====------\n");
  IteratorG it = newIterator(stringCompare, stringNew, stringFree);
  
  char *strA[MAXARRAY];
  strA[0] = strdup("kylie");
  strA[1] = strdup("lian");
  strA[2] = strdup("john");
  strA[3] = strdup("arnav"); 
  strA[4] = strdup("joe"); 
  
  int j;
  for(j=0; j<MAXARRAY; j++){
    int result = add(it , strA[j]); 
    printf("> Inserting %s: %s \n", strA[j], (result==1 ? "Success" : "Failed") );
  }

  reset(it);
  printf("> it (after reset): \n");
  prnIt(it, prnStr);
  reset(it);

  IteratorG advIt3 = advance(it, 3);
  printf("> advance(it, 3) returns: \n");
  prnIt(advIt3, prnStr);
 
  IteratorG advIt4 = advance(it, -2);
  printf("> advance(it, -2) returns: \n");
  prnIt(advIt4, prnStr);

  reset(it);
  printf("> it (after reset): \n");
  prnIt(it, prnStr);
  reset(it);

  IteratorG findit = find(it, prefixJo);
  printf("> find(it, prefixJo) returns: \n");
  prnIt(findit, prnStr);
  printf("\n> In 'it',  \n");
  prnNext(it, prnStr);
  prnNext(it, prnStr);

  freeIt(it);
  freeIt(findit);
  freeIt(advIt3);
  freeIt(advIt4);
  printf("--====  End of Test-06 ====------\n\n");
}


void test7(){
  printf("\n--====  Test-07       ====------\n");
  IteratorG it1 = newIterator(positiveIntCompare, positiveIntNew, positiveIntFree);
  int a[3] = { 97, 10, 3};
  for(int i=0; i<3; i++){
    int result = add(it1 , &a[i]); 
    printf("> Inserting %d: %s \n", a[i], (result==1 ? "Success" : "Failed") );
  }
  int added = 101;
  int result1 = set(it1, &added);
  printf("> Set value: %d ; return val: %d \n", added,  result1 );
  
  reset(it1);
  prnIt(it1, prnInt);
  reset(it1);
  //prnNext(it1, prnInt);
  //prnNext(it1, prnInt);
  
  reverse(it1);
  added = 101;
  result1 = set(it1, &added);
  printf("> Set value: %d ; return val: %d \n", added,  result1 );
  //add(it1, &added);
  reset(it1);
  prnIt(it1, prnInt);
    
    
  printf("--====  End of Test-07 ====------\n\n");
}
void test8(){
  printf("\n--====  Test-08       ====------\n");
  IteratorG it1 = newIterator(positiveIntCompare, positiveIntNew, positiveIntFree);
  int a[9] = { 97, 10, 11, 15, 29, 1234, 37, 1, 543};
  for(int i=0; i<9; i++){
    int result = add(it1 , &a[i]); 
    printf("> Inserting %d: %s \n", a[i], (result==1 ? "Success" : "Failed") );
  }
  
  reset(it1);
  prnIt(it1, prnInt);
  
  printf("Move the cursor back twice \n");
  prnPrev(it1, prnInt);
  prnPrev(it1, prnInt);
  prnPrev(it1, prnInt);
  prnPrev(it1, prnInt);
  prnPrev(it1, prnInt);
  printf("Reverse the list\n");
  reverse(it1);
  
  
  prnPrev(it1, prnInt);
  
  
  printf("Advance the list -3 places ");
  IteratorG advIt1 = advance(it1, -3);
  printf("> advance(it1, 4) returns: \n");
  prnIt(advIt1, prnInt);
  reset(advIt1);
  
  
  
  /*printf("Using the advance portion of the list\n");
  printf("Add another node\n");
   int i = 999999;
   int result = add(advIt1 , &i); 
   printf("> Inserting %d: %s \n", i, (result==1 ? "Success" : "Failed") );
   reset(advIt1);
   prnIt(advIt1, prnInt);
   reset(advIt1);
   printf("Move forward twice, then delete a node\n");
   prnNext(advIt1, prnInt);
   prnNext(advIt1, prnInt);
   del(advIt1);
   reset(advIt1);
   prnIt(advIt1, prnInt);
   
   printf("Advance -2 on the advanced list\n");
  IteratorG advIt2 = advance(advIt1, -3);
  printf("> advance(it1, 4) returns: \n");
  reset(advIt2);
   prnIt(advIt2, prnInt);
  printf("Reset then print\n");
  reset(it1);
  prnIt(it1, prnInt);*/
  
  
   printf("--====  End of Test-08 ====------\n\n");
}
  
  
int main(int argc, char *argv[])
{
  /* The code in this file is provided in case you find it difficult 
     to understand how to test your generic iterator. In case you have 
     difficulty understanding a generic ADT, please study SetG example 
     we discussed in the lectures in detail. You may also want to 
     (again) watch the lectures on this topic and/or see a tutor 
     during one of the Help Sessions (see the class web page).

     The code below only tests basic operations, however make sure 
     that you do test all the required functions properly by adding more 
     tests in this or another file. 

     Remove comments below as required. The expected output for each test is in 
     the file 'expected_output.txt'.
  */
  
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  
  return EXIT_SUCCESS;
  
}
