#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "circularBuffer.h"

#define DEBUG 1

/*
Implement a circular buffer by OSX gcc

The operations required are:
- create a buffer with specific size. Assuming each element is an int
- put at the tail. When the CB is full, ignore the operation and return -1
- get from the head. When CM is empty, ignore the operation and return -1
- return the count. 
- delete a buffer.
*/



/*
	create circular buffer with given size to the passin circularBuffer_t pointer
*/
int createCircularBuffer ( circularBuffer_t *CBPtr, int size) {
	if (CBPtr != 0) {
		CBPtr->buffer = malloc (sizeof (int)* size);
		
		if (CBPtr->buffer != 0) {
			memset (CBPtr->buffer, 0, size * sizeof(int)); //clear the buffer
			CBPtr->bufferEnd = CBPtr->buffer + size;
			CBPtr->head = CBPtr->buffer;
			CBPtr->tail = CBPtr->buffer;
			CBPtr->count = 0;
			CBPtr->size = size;
		
			if (DEBUG) {
				printf ("%s()==>buffer: %p, bufferEnd: %p\n",__FUNCTION__, CBPtr->buffer, 
					CBPtr->bufferEnd);
			}
		} else {
			if (DEBUG) {
				printf ("%s()==>Error: Null CBPtr->buffer\n",__FUNCTION__);
			}			
			return -1;
		}
	} else {
		if (DEBUG) {
			printf ("%s()==>Error: Null CBPtr\n",__FUNCTION__);
		}
		return -1;
	}
	
	return 0;
}


/*
	Free the buffer of the pass-in circularBuffer_t pointer
*/
int freeCircularBuffer (circularBuffer_t *CBPtr) {
	if (CBPtr == 0) {
		if (DEBUG) {
			printf ("%s()==>: null CBPtr\n",__FUNCTION__);
		}
		return -1;
	} else {
		free (CBPtr->buffer);
		return 0;
	}
}

/* 
	put an int to the circular buffer tail pointer, if the buffer is full,
	ignore the put and return -1, otherwise add to tail, return 0;
*/
int put (circularBuffer_t *CBPtr, int value) {
	
	if (CBPtr == 0) {
		if (DEBUG) {
			printf ("%s()==>: null CBPtr\n",__FUNCTION__);
		}
		return -1;
	}
	//check if buffer is full
	if ((CBPtr->count !=0)&&(CBPtr->size == CBPtr->count)) {
		if (DEBUG) {
			printf ("%s()==>buffer overflow, value:%d ignored\n",__FUNCTION__, value);
		}
		return -1;
	}
	
	if (DEBUG) {
		printf ("%s()==>%d\n", __FUNCTION__,value);
	}
	
	//insert the value to tail
	*CBPtr->tail++ = value;
	
	//if reach the end of the buffer, move tail to buffer start
    if(CBPtr->tail == CBPtr->bufferEnd) {
       CBPtr->tail = CBPtr->buffer;
    }
    CBPtr->count++;
    
    return 0;

}

/* 
	get the int from circular buffer head pointer, place it to the pass-in pointer.
	empty buffer, return -1, otherwise return 0;
*/
int get (circularBuffer_t *CBPtr, int *value) {
	if (CBPtr == 0) {
		if (DEBUG) {
			printf ("%s()==>error: null CBPtr\n",__FUNCTION__);
		}
		return -1;
	}	
	if (CBPtr->count == 0) {
		if (DEBUG) {
	    	printf ("%s()==>error: empty buffer\n",__FUNCTION__);
	    }
		return -1;
	}
	
	//get value from head
	*value = *CBPtr->head++;
	
	if (DEBUG) {
		printf ("%s()==>%d\n",__FUNCTION__, *value);
	}
	
	//if reach the end of the buffer, move head to buffer start
    if(CBPtr->head == CBPtr->bufferEnd) {
       CBPtr->head = CBPtr->buffer;
    }
    
    CBPtr->count--;

    return 0;
}

/*
	get the number of the elements in the buffer, -1: error
*/
int getCircularBufferCount (circularBuffer_t  *CBPtr) {
	if (CBPtr == 0) {
		if (DEBUG) {
			printf ("%s()==>error: null CBPtr\n",__FUNCTION__);
		}
		return -1;
	} else {	
		return CBPtr->count;
	}
}

/*
	help utility to dump circular buffer
*/
int dumpCB (circularBuffer_t *CBPtr) {
	if (DEBUG) {
		int i;
		if (CBPtr == 0) {
			printf ("%s()==>error: null CBPtr\n",__FUNCTION__);
			return -1;
		} else {
			printf ("circular buffer dump:\n");
			for (i=0; i < CBPtr->size; i++) {
				printf ("%d: %d\n",i, CBPtr->buffer[i] );
			}

			printf ("count: %d, head: %lu, tail:%lu\n", getCircularBufferCount (CBPtr), 
				(CBPtr->head - CBPtr->buffer), 
				(CBPtr->tail - CBPtr->buffer));
		}
	}
	return 0;
}

/*
    main
*/
int main (void) {

	circularBuffer_t *cbptr;
	int *getVal;
   
    getVal = malloc (sizeof (int));
    
    //test 0 null pointer test
    createCircularBuffer(0, 5);
    
    //init the circular buffer of 5 elements hard code to 5 elements for testing purpose
    cbptr = malloc (sizeof (circularBuffer_t));     
    createCircularBuffer(cbptr, 5);
    
    dumpCB(cbptr);
    
    //test 1: test put(), insert 6 elements, test overflow
    put (cbptr, 1);
    put (cbptr, 2);
    put (cbptr, 3);
    dumpCB(cbptr);
    put (cbptr, 4);
    put (cbptr, 5);
    dumpCB(cbptr);
    put (cbptr, 6);
    dumpCB(cbptr);
    
    
    //test 2 test get (), also test moving tail to buffer start, test overflow again
    get (cbptr, getVal);

    dumpCB(cbptr);
    
    put (cbptr, 7);
    dumpCB(cbptr);
    put (cbptr, 8);
    dumpCB(cbptr);
    
	//test 3 test get() and alos testing empty buffer check
	get (cbptr, getVal);
    dumpCB(cbptr);
    
    get (cbptr, getVal);
    get (cbptr, getVal);
    dumpCB(cbptr);
    get (cbptr, getVal);
    get (cbptr, getVal);
    dumpCB(cbptr);
    get (cbptr, getVal);
    dumpCB(cbptr);
    
    //final test, after buffer is empty, few more put and get
    put (cbptr,9);
    put (cbptr,10);
    dumpCB(cbptr);
    get (cbptr, getVal);
    dumpCB(cbptr);
    
    freeCircularBuffer (cbptr);
    free (cbptr);
    free (getVal);
    
    return 0;
}
