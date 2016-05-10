#include <stdio.h>
#include <string.h>

#include "dvr.h"

// The distance table for node 2
struct distance_table dt2;

/***********************HELPERS***********************************/
/**
* Copies rows
*/
void copyRow2(int destination[], int source[]){
  int looper;
  for (looper = 0; looper < 4; looper++){
    destination[looper] = source[looper];
  }
}

void sendToNeighbour2(){
  //send distance table packet
  struct rtpkt packet;
  packet.sourceid = 2;

  int looper;
  for (looper = 0; looper < 4; looper++){
    packet.mincost[looper] = dt2.costs[2][looper];
  }

  packet.destid = 0;
  tolayer2(packet);

  packet.destid = 1;
  tolayer2(packet);

  packet.destid = 3;
  tolayer2(packet);
}
/**
 * Called by the simulation during start-up. Use this function to initialize
 * any data on node 2. You may not need to do anything with this function.
 */
void rtinit2() {
  //initialize distance table
  /* table formart
      0   1   2   3
    ---------------
  0| inf inf  inf inf
  1| inf inf inf inf
  2|  3   1   0   2
  3| inf inf inf inf
  */
  int table[4][4] = {
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {3, 1, 0, 2},
    {INFINITY, INFINITY, INFINITY, INFINITY}
  };

  memcpy(dt2.costs, table, 4 * 4 * sizeof(int));

  sendToNeighbour2();
}

/**
 * Called when node 2 receives a routing packet from one of its neighbors
 *
 * \param packet  A pointer to the packet data that was received.
 */
void rtupdate2(struct rtpkt* packet) {
  int sourceId = packet->sourceid;
  copyRow2(dt2.costs[sourceId], packet->mincost);

  int costToGivenNode = dt2.costs[2][sourceId];

  //calculate the distances
  int distanceTo0 = costToGivenNode + packet->mincost[0];
  int distanceTo1 = costToGivenNode + packet->mincost[1];
  int distanceTo3 = costToGivenNode + packet->mincost[3];
  //update table
  if(dt2.costs[2][0] > distanceTo0){
    dt2.costs[2][0] = distanceTo0;
    sendToNeighbour2();
  }
  if(dt2.costs[2][1] > distanceTo1){
    dt2.costs[2][1] = distanceTo1;
    sendToNeighbour2();
  }
  if(dt2.costs[2][3] > distanceTo3){
    dt2.costs[2][3] = distanceTo3;
    sendToNeighbour2();
  }
  int i;
  int j;
  for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++){
      printf("%d ", dt2.costs[i][j]);
    }
    printf("\n" );
  }
}
