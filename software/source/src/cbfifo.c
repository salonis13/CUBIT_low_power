/****************************************************************************
 * @file cbfifo.c : 
 * @brief : This file contains circular buffer FIFO implementation. User has 
 *          Enqueue, Dequeue, Length of Fifo and its capacity as APIs
 * 
 * @author : Rajat Chaple
 * @date : Feb 8 2021
 * @compiler : gcc (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
 ****************************************************************************/

#include <stdlib.h>  // for size_t
#include "cbfifo.h"


typedef struct cbfifo_s{
    uint8_t array_buf[CBFIFO_SIZE];
    uint32_t write_pointer;
    uint32_t read_pointer;
    uint32_t length;
    bool is_full;
}cbfifo_t;

cbfifo_t cbfifo_transmit;
cbfifo_t cbfifo_receive;


/*----------------------------------------------------------------------------
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity. (refer cbfifo.h for additional details)
 ----------------------------------------------------------------------------*/
size_t cbfifo_enqueue(cbfifo_t* cbfifo, void *buf, size_t nbyte)
{
    if(buf == NULL)
    {
        return -1;
    }
    uint8_t* data = buf;
    for(int i=0; i< nbyte; i++)
    {   
        //check for full and return number of enqueued elements if true
        if(cbfifo->is_full == true)
        {
            return i;
        }
        *(cbfifo->array_buf + cbfifo->write_pointer) = *(data + i);
        // Increamenting write pointer
        cbfifo->write_pointer = (cbfifo->write_pointer + 1) % cbfifo_capacity(cbfifo);
        // Increamenting length
        cbfifo->length++;
        //Check if circular buffer cbFifo is full
        if(cbfifo->length >= cbfifo_capacity(cbfifo))
        {
            cbfifo->is_full = true;
        }
        else
        {
            cbfifo->is_full = false;
        }
        
    }

    return nbyte;
}


/*----------------------------------------------------------------------------
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 * (refer cbfifo.h for additional details)
 ---------------------------------------------------------------------------- */
size_t cbfifo_dequeue(cbfifo_t* cbfifo, void *buf, size_t nbyte)
{
    uint8_t* data = buf;
    if(buf == NULL)
    {
        return -1;
    }
    for(int i=0; i< nbyte; i++)
    {
        //check for empty buffer and return number of dequeued elements if true
        if(cbfifo_length(cbfifo) <= 0)
        {
            return i;
        }
        //increamenting read pointer
        *(data + i) = *(cbfifo->array_buf + cbfifo->read_pointer);
        cbfifo->read_pointer = (cbfifo->read_pointer + 1) % cbfifo_capacity(cbfifo);
        //decreamenting FIFO length
        cbfifo->length--;
    }

    if(cbfifo->length < cbfifo_capacity(cbfifo))
    {
        cbfifo->is_full = false;
    }
    return nbyte;
}

/*----------------------------------------------------------------------------
 * Returns the number of bytes currently on the FIFO. 
 * (refer cbfifo.h for additional details)
 ----------------------------------------------------------------------------*/
size_t cbfifo_length(cbfifo_t* cbfifo)
{
    return (size_t)cbfifo->length;
}


/*----------------------------------------------------------------------------
 * Returns the FIFO's capacity
 * (refer cbfifo.h for additional details)
 *---------------------------------------------------------------------------- */
size_t cbfifo_capacity(cbfifo_t* cbfifo)
{
    return sizeof(cbfifo->array_buf)/sizeof(cbfifo->array_buf[0]);
}

