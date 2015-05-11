/*
 * Peer manager
 *
 * The peer manager keeps track of neighbour nodes in a peer-to-peer
 * network.  This module might be used as part of a BitTorrent, BitCoin,
 * or Tor client.
 *
 * Every time there is activity from a peer, the peer manager is told about it.
 * The peer manager remembers the n most recently active unique peers.
 *
 * When we wish to communicate, the peer manager can select a random
 * peer.
 *
 * Example usage:
 *
 *   PeerManager mgr;
 *   peermgr_init(&mgr, 8);
 *
 *   peermgr_saw_peer(&mgr, 1234, time(NULL));
 *   peermgr_saw_peer(&mgr, 5432, time(NULL));
 *
 *   PeerId random_peer;
 *   if (peermgr_pick_random_peer(&mgr, &random_peer)) {
 *       printf("chosen peer = %" PRIu64 "\n", random_peer);
 *   }
 *
 *   peermgr_cleanup(&mgr);
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/* TODO You may include additional C standard library headers and define
 * additional structs.  You cannot change the function prototypes, the API is
 * fixed.
 */

/** A unique identifier for a peer */
typedef uint64_t PeerId;

typedef struct{
	PeerId peerId;
	time_t timestamp;
	void *nextPeer;
}Peer;

typedef struct {
    /* TODO */
	Peer *listHead; 
	size_t listLen;
	size_t maxPeers;
}PeerManager;

/**
 * new node memory allocation and initialization 
 */
struct Peer* newNode(PeerId peer, time_t timestamp)
{
	Peer *newNode = (Peer*)malloc(sizeof(Peer));
	newNode->peerId = peer;
	newNode->nextPeer = NULL;
	newNode->timestamp = timestamp;
	return newNode;
}
 
/**
 * Initialize @mgr so that it can track up to @maxpeers peers.
 */
void peermgr_init(PeerManager *mgr, unsigned int maxpeers)
{
    /* TODO */
	if(mgr == NULL){
		return;
	}
	mgr->listHead = NULL;
	mgr->listLen = 0;
	mgr->maxPeers = maxpeers;
	/* Intializes random number generator */
	srand(3);
	

}

/**
 * Free any resources allocated by @mgr.
 */
void peermgr_cleanup(PeerManager *mgr)
{
    /* TODO */
	if(mgr == NULL){
		return;
	}
	
	if(mgr->listLen == 0){
		return;
	}
	
	Peer *currPtr = mgr->listHead;
	
	while(currPtr){
		mgr->listHead = (Peer*)mgr->listHead->nextPeer;
		free(currPtr);
		currPtr = mgr->listHead;
	}
}

/**
 * Update peer information with new activity from @peer at time @timestamp.
 * The peer manager retains a fixed number of unique peers with the most recent
 * timestamps.  The maximum number of peers to remember was set in
 * peermgr_init().
 *
 * If the maximum number of peers to remember has been reached, it may be
 * necessary to forget about the peer with the oldest timestamp so that there
 * is space for the newer peer.
 */
 
void peermgr_saw_peer(PeerManager *mgr, PeerId peer, time_t timestamp)
{
	/* TODO */
	Peer *currPtr = NULL, *lowestTimePtr=NULL, lastPtr = NULL;
	
	if(mgr == NULL){
		return;
	}
	
	if(mgr->listHead == NULL){
		mgr->listHead = newNode(peer, timestamp);
		mgr->listLen = 1;
	}
	else{
		currPtr = mgr->listHead;
		lastPtr = currPtr;
		lowestTimePtr = mgr->listHead;
		while(currPtr)
		{
			/*case1 : peer id is already in the list, you just need to update the time stamp */
			if(currPtr->peerId == peer){
				currPtr->timestamp = timestamp;
				return;
			}
			
			if(currPtr->timestamp < lowestTimePtr->timestamp){
				lowestTimePtr = currPtr;
			}
			
			lastPtr = currPtr;
			currPtr = (Peer*)currPtr->nextPeer;
		}
		
		/*case2 : peer id is not in the and list length is less then maxPeers*/
		if(mgr->listLen < mgr->maxPeers)
		{
			lastPtr->nextPeer = (void*)newNode(peer, timestamp);
			mgr->listLen = mgr->listLen + 1;
		}
		else
		{	
			lowestTimePtr->peerId = peer;
			lowestTimePtr->timestamp = timestamp;
		}
	}
}

/**
 * Select a peer at random and store its identifier in @peer.
 *
 * Returns: true on success
 */
bool peermgr_pick_random_peer(PeerManager *mgr, PeerId *peer)
{
    /* TODO use random(3) and assume a seed has already been set */
	int randomPeerInd, i=0;

	if(peer == NULL){
		return false;
	}
	
	if(mgr == NULL){
		return false;
	}
	
	if(mgr->listLen == 0){
		return false;
	}
	
	Peer *currPtr = mgr->listHead;
	
	randomPeerInd = rand()%(mgr->listLen - 1);
	
	while(i < randomPeerInd)
	{
		currPtr = (Peer*)currPtr->nextPeer;
		i++;
	}
	//currPtr = currPtr->nextPeer;
	*peer = currPtr->peerId;
	return true;
}
