#include <stdio.h>
#include <string.h>

#include "dvr.h"

// The distance table for node 0
struct distance_table dt0;

/***********************HELPERS***********************************/
/**
* Copies rows
*/
void copyRow(int destination[], int source[]){
  int looper;
  for (looper = 0; looper < 4; looper++){
    destination[looper] = source[looper];
  }
}

void sendToNeighbour0(){
  //send distance table packet
  struct rtpkt packet;
  packet.sourceid = 0;
  //memcpy(packet.mincost, table[1], 4 * sizeof(int));


  int looper;

  for (looper = 0; looper < 4; looper++){
    packet.mincost[looper] = dt0.costs[0][looper];
  }
  for (looper = 1; looper < 4; looper++){
    packet.destid = looper;
    tolayer2(packet);
  }
}
/**
 * Called by the simulation during start-up. Use this function to initialize
 * any data on node 0. You may not need to do anything with this function.
 */
void rtinit0() {
  //initialize distance table
  /* table formart
      0   1   2   3
    ---------------
  0|  0   1   3   7
  1| inf inf inf inf
  2| inf inf inf inf
  3| inf inf inf inf
  */
  int table[4][4] = {
    {0, 1, 3, 7},
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY}
  };

  memcpy(dt0.costs, table, 4 * 4 * sizeof(int));

  sendToNeighbour0();
}

/**
 * Called when node 0 receives a routing packet from one of its neighbors
 *
 * \param packet  A pointer to the packet data that was received.
 */
void rtupdate0(struct rtpkt* packet) {

  int sourceId = packet->sourceid;
  copyRow(dt0.costs[sourceId], packet->mincost);

  int costToGivenNode = dt0.costs[0][sourceId];
  //calculate the distances
  int distanceTo1 = costToGivenNode + packet->mincost[1];
  int distanceTo2 = costToGivenNode + packet->mincost[2];
  int distanceTo3 = costToGivenNode + packet->mincost[3];
  //update table
  if(dt0.costs[0][1] > distanceTo1){
    dt0.costs[0][1] = distanceTo1;
    sendToNeighbour0();
  }

  if(dt0.costs[0][3] > distanceTo3){
    dt0.costs[0][3] = distanceTo3;
    sendToNeighbour0();
  }

  if(dt0.costs[0][2] > distanceTo2){
    dt0.costs[0][2] = distanceTo2;
    sendToNeighbour0();
  }



  printdt(0, &dt0);

  int i;
  int j;
  for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++){
      printf("%d ", dt0.costs[i][j]);
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
void linkhandler0(int linkid, int newcost) {

}
