#include <stdio.h>
#include <string.h>

#include "dvr.h"

// The distance table for node 1
struct distance_table dt1;

/***********************HELPERS***********************************/
/**
* Copies rows
*/
void copyRow1(int destination[], int source[]){
  int looper;
  for (looper = 0; looper < 4; looper++){
    destination[looper] = source[looper];
  }
}

void sendToNeighbour1(){
  //send distance table packet
  struct rtpkt packet;
  packet.sourceid = 1;

  int looper;
  for (looper = 0; looper < 4; looper++){
    packet.mincost[looper] = dt1.costs[1][looper];
  }

  packet.destid = 0;
  tolayer2(packet);

  packet.destid = 2;
  tolayer2(packet);
}
/**
 * Called by the simulation during start-up. Use this function to initialize
 * any data on node 1. You may not need to do anything with this function.
 */
void rtinit1() {
  //initialize distance table
  /* table formart
      0   1   2   3
    ---------------
  0| inf  1  inf inf
  1|  1   0   1  inf
  2| inf  1  inf inf
  3| inf inf inf inf
  */
  int table[4][4] = {
    {INFINITY, 1, INFINITY, INFINITY},
    {1, 0, 1, INFINITY},
    {INFINITY, 1, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY}
  };

  memcpy(dt1.costs, table, 4 * 4 * sizeof(int));

  sendToNeighbour1();
}

/**
 * Called when node 1 receives a routing packet from one of its neighbors
 *
 * \param packet  A pointer to the packet data that was received.
 */
void rtupdate1(struct rtpkt* packet) {

  int sourceId = packet->sourceid;
  copyRow1(dt1.costs[sourceId], packet->mincost);

  int costToGivenNode = dt1.costs[1][sourceId];

  //calculate the distances
  int distanceTo0 = costToGivenNode + packet->mincost[0];
  int distanceTo2 = costToGivenNode + packet->mincost[2];
  int distanceTo3 = costToGivenNode + packet->mincost[3];

  //update table
  if(dt1.costs[1][0] > distanceTo0){
    dt1.costs[1][0] = distanceTo0;
    sendToNeighbour1();
  }
  if(dt1.costs[1][2] > distanceTo2){
    dt1.costs[1][2] = distanceTo2;
    sendToNeighbour1();
  }
  if(dt1.costs[1][3] > distanceTo3){
    dt1.costs[1][3] = distanceTo3;
    sendToNeighbour1();
  }
  int i;
  int j;
  for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++){
      printf("%d ", dt1.costs[i][j]);
    }
    printf("\n" );
  }
}

/**
 * Handle an update to the cost of one of the links from this node. You only
 * need to implement this function if you complete the advanced assignment.
 * You must also set LINKCHANGE to 1 in dvr.h.
 *
 * \param linkid  The id of the node connected on the link that changed cost
 * \param newcost The new cost of this link
 */
void linkhandler1(int linkid, int newcost) {

}
