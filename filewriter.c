/*
 * Checksummed file writer
 *
 * This module collects data buffers until they should be written out to a
 * file, prefixed by a checksum.
 *
 * Typical usage is:
 *
 *   ChecksumWriter writer;
 *   cwriter_init(&writer);
 *   while (...) {
 *       buf = malloc(len);
 *       ...read len bytes into buf...
 *       cwriter_add_buf(&writer, buf, len);
 *   }
 *   cwriter_finish(&writer, stdout);
 *
 * The ChecksumWriter holds onto the buffers and finally emits the checksum,
 * followed by the buffers themselves.
 *
 * The checksum algorithm is simple: unsigned addition modulo 256 of each byte
 * in the buffer.
 *
 * The output written by cwriter_finish() is laid out like this:
 * [checksum byte][buf1][buf2]...[bufn]
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	char *data;
	size_t len;
	void *nextNode;
}Buffer;

typedef struct {
    /* TODO */
	Buffer *listHead; 
	size_t len;
} ChecksumWriter;

/**
 * cwriter_init:
 * @writer: The writer instance.
 *
 * Initialize a ChecksumWriter so it is ready to be used in cwriter_add_buf()
 * and cwriter_finish() calls.
 */
void cwriter_init(ChecksumWriter *writer)
{
    /* TODO */
	writer->listHead = NULL;
	writer->len = 0;
}

/**
 * cwriter_add_buf:
 * @writer: The writer instance.
 * @buf: The buffer to add.
 * @len: The length of the buffer, in bytes.
 *
 * Append a buffer to the ChecksumWriter.
 *
 * This function does not copy buf, instead the caller passes ownership of @buf
 * to this function.
 */
void cwriter_add_buf(ChecksumWriter *writer, void *buf, size_t len)
{
    /* TODO */
	Buffer *myNode, *currPtr;
	myNode= (Buffer*)malloc(sizeof(Buffer));
	myNode->data = (char*)buf;
	myNode->len = len;
	myNode->nextNode = NULL;
	
	if(writer->listHead == NULL)
	{
		writer->listHead = myNode;
	}
	else
	{
		currPtr = writer->listHead;
		while(currPtr->nextNode != NULL)
		{
			currPtr = (Buffer*)currPtr->nextNode;
		}
		currPtr->nextNode = (void*)myNode;
	}
}

/**
 * cwriter_cleanup:
 * @writer: The writer instance.
 * @fp: The output file.
 *
 * Write the checksum followed by the buffers themselves to @fp.
 *
 * This function frees resources.  To use the ChecksumWriter again, call
 * cwriter_init() first.
 *
 * Returns: true on success, false if writing to file failed
 */
bool cwriter_finish(ChecksumWriter *writer, FILE *fp)
{
    /* TODO */
	int i;
	char checksum = 0;
	
	if(writer == NULL){
		return false;
	}
	
	if(fp == NULL){
		return false;
	}
	Buffer *currPtr = writer->listHead;
	Buffer *tempPtr = NULL;
	
	while (currPtr) {
		for (i = 0; i < currPtr->len; i++){
			checksum = (checksum + currPtr->data[i])%256;
		}
		currPtr = (Buffer*)currPtr->nextNode;
	}

	fputc(checksum, fp);
	
	currPtr = writer->listHead;

	while (currPtr) {
		for(i = 0; i < currPtr->len; i++){
			fputc(currPtr->data[i], fp);
		}
		tempPtr = currPtr;
		currPtr = (Buffer*)currPtr->nextNode;
		free(tempPtr);
	}
}
