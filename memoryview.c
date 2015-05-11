/*
     * Memory view
     *
     * A memory view presents an address space (the "view") that maps onto
     * underlying memory buffers (the "memory").  This layer of indirection makes
     * it easy to access isolated pieces of memory as if they were adjacent.  For
     * example:
     *
     * Memory address   Name
     * 0x08000-0x0c000  BUFFER1
     * 0x10000-0x11000  BUFFER2
     *
     * BUFFER2 can be accessed as if it was located immediately after BUFFER1 using
     * a memory view:
     *
     * View offset      Name
     * 0x0000-0xc000    BUFFER1
     * 0xc000-0xd000    BUFFER2
     *
     * This technique is used in text, audio, and video editors to perform
     * insertions and deletions without copying data.  In this exercise you will
     * implement a simplified memory view.
     */
     
    #include <stdio.h>
    #include <assert.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>
     
    /* TODO You may include C Standard Library headers and define additional
     * structs.  You cannot change the function prototypes, the API is fixed.
     */
     
    /** A piece of memory */
    typedef struct {
        void *data;
        size_t len;
    } Buffer;
     
    typedef struct {
        /* TODO */
		Buffer *myArray;
		size_t arrayLen;
           
    } MemView;
     
    /**
     * Initialize @memview given an array of memory buffers.
     *
     * Returns: true on success, false on error
     */
    bool memview_init(MemView *memview,
                      const Buffer *buffers,
                      size_t nbuffers)
    {
        int i;
        if(memview == NULL || buffers == NULL)
        {
				return false;
		}
		
		memview->myArray = malloc(nbuffers*sizeof(Buffer));
		if(memview->myArray == NULL)
		{
			return false;
		}
		
		for (i = 0; i < nbuffers; i++)
		{
			memview->myArray[i].data = (void*)malloc(buffers[i].len);
			if(memview->myArray[i].data == NULL)
			{
				return false;
			}
			memcpy(memview->myArray[i].data, buffers[i].data, buffers[i].len);
			memview->myArray[i].len = buffers[i].len;
		}
		memview->arrayLen = nbuffers;
        return true; /* TODO */
    }
     
    /**
     * Free any allocated resources.
     */
    void memview_cleanup(MemView *memview)
    {
        /* TODO */
            if(memview == NULL)
            {
                return;
            }
			
			if(memview->myArray == NULL)
			{
				return;
			}
            int i;
     
            for (i = 0; i < memview->arrayLen; i++)
            {      
                    free(memview->myArray[i].data);
            }
			
			free(memview->myArray);
    }
     
    /**
     * Drop @nbytes from the start of the memory view.
     */
    void memview_discard_front(MemView *memview, size_t nbytes)
    {
		/* TODO */
		int i=0;
		void *newData = NULL, *temp=NULL;

		if(memview == NULL)
		{
			return;
		}
		
		if(memview->myArray == NULL)
		{
			return;
		}
	   
		if(nbytes == 0) =
		{
				printf("Error");
		}
		//newLocation = memview->myArray->len - nbytes;
		while(nbytes > 0 && memview->arrayLen > i)
		{
			if(nbytes < memview->myArray[i].len)
			{
				size_t nCopyBytes = memview->myArray[i].len - nbytes;
				newData = malloc(nCopyBytes);
				memcpy(newData, (char *)memview->myArray[i].data + nbytes, nCopyBytes);
				temp = memview->myArray[i].data ;
				memview->myArray[i].data = newData;
				free(temp);
				nbytes = 0;
			}
			else 
			{
				nbytes = nbytes - memview->myArray[i].len;
				free(memview->myArray[i].data);
				memview->myArray[i].len = 0;
			}
			i++;
		}
    }
     
    /**
     * Copy @len bytes starting at @offset to @data.
     *
     * @offset: the starting location in the view
     * @data: the copy destination
     * @len: the number of bytes to copy
     *
     * Returns: true on success, false on error
     */
    bool memview_read(MemView *memview, uint64_t offset, void *data, size_t len)
    {
		i = 0;
		uint64_t currentLength = memview->myArray[i].len;
		unit64_t currentDifference = offset;
		void *blockPtr = memview->myArray[i].data;
		unit64_t currentLocation;
		size_t newLen;
		
        if(memview == NULL)
		{
			return;
		}
		
		if(offset < 0)
		{
			return;
		}
		
		if(data == NULL)
		{
			return;
		}
		
		if(len <= 0)
		{
			return;
		}
		
		while(currentLength <= offset && currentDifference != 0)
		{
			currentLength = currentLength + memview->myArray[++i].len;
			currentDifference = offset - currentLength;
		}
		
		
		if(currentLength > offset && currentDifference < 0)
		{
			currentLocation = memview->myArray[i].len - abs(currentDifference);
			//currentDifference = abs(currentDifference);
			blockLen = memview->myArray[i].len;
			
			while(len > 0)
			{
				if(currentLocation < blockLen && ((char*)memview->myArray[i].data != NULL))
				{
					*data = (char*)blockPtr + currentLocation;
					(char*)blockPtr++;
					(char*)data++;
					len--;
				}
				else if(i < memview->myArray[i].len)
				{
					i++;
					blockPtr = memview->myArray[i].data;
					currentLocation = 0;
					blockLen = memview->myArray[i].len;
					*data = (char*)blockPtr + currentLocation;
					(char*)blockPtr++;
					(char*)data++;
					len--;
				}
				else
				{
					break;
				}
				
			}
			
		}
		
		
		
		return true; /* TODO */
    }
     
    /* This serves both as an API usage example and a test case. */
    int main(int argc, char **argv) {
        Buffer buffers[] = {
            {"hello", 5},
            {"world", 5},
            {"!", 1},
        };
        MemView memview;
        assert(memview_init(&memview,
                            buffers,
                            sizeof(buffers) / sizeof(buffers[0])));
     
        /* Read across one buffer boundary */
        char buf[7];
        assert(memview_read(&memview, 3, buf, 4));
        assert(buf[0] == 'l' &&
               buf[1] == 'o' &&
               buf[2] == 'w' &&
               buf[3] == 'o');
     
        /* Discard within first buffer */
        memview_discard_front(&memview, 2);
        assert(memview_read(&memview, 0, buf, 4));
        assert(buf[0] == 'l' &&
               buf[1] == 'l' &&
               buf[2] == 'o' &&
               buf[3] == 'w');
     
        /* Read across two buffer boundaries */
        assert(memview_read(&memview, 2, buf, 7));
        assert(buf[0] == 'o' &&
               buf[1] == 'w' &&
               buf[2] == 'o' &&
               buf[3] == 'r' &&
               buf[4] == 'l' &&
               buf[5] == 'd' &&
               buf[6] == '!');
     
        /* Read beyond end */
        assert(!memview_read(&memview, 9, buf, 1));
     
        /* Discard first buffer and into second */
        memview_discard_front(&memview, 4);
        assert(memview_read(&memview, 0, buf, 3));
        assert(buf[0] == 'o' &&
               buf[1] == 'r' &&
               buf[2] == 'l');
     
        memview_cleanup(&memview);
        return 0;
    }
