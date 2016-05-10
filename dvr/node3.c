#include <stdio.h>
#include <string.h>

#include "dvr.h"

// The distance table for node 3
struct distance_table dt3;

/***********************HELPERS***********************************/
/**
* Copies rows
*/
void copyRow3(int destination[], int source[]){
  int looper;
  for (looper = 0; looper < 4; looper++){
    destination[looper] = source[looper];
  }
}

void sendToNeighbour3(){
  //send distance table packet
  struct rtpkt pk;
  pk.sourceid = 3;
  int looper;
  for (looper = 0; looper < 4; looper++){
    pk.mincost[looper] = dt3.costs[3][looper];
  }

  pk.destid = 0;
  tolayer2(pk);

  pk.destid = 2;
  tolayer2(pk);
}

/**
 * Called by the simulation during start-up. Use this function to initialize
 * any data on node 3. You may not need to do anything with this function.
 */
void rtinit3() {
  //initialize distance table
  /* table formart
      0   1   2   3
    ---------------
  0| inf inf  inf inf
  1| inf inf inf inf
  2| inf inf inf inf
  3|  7  inf  2   0
  */
  int table[4][4] = {
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {INFINITY, INFINITY, INFINITY, INFINITY},
    {7, INFINITY, 2, 0}
  };

  memcpy(dt3.costs, table, 4 * 4 * sizeof(int));



  sendToNeighbour3();

}

/**
 * Called when node 3 receives a routing packet from one of its neighbors
 *
 * \param packet  A pointer to the packet data that was received.
 */
void rtupdate3(struct rtpkt* packet) {
  int sourceId = packet->sourceid;
  copyRow3(dt3.costs[sourceId], packet->mincost);

  int costToGivenNode = dt3.costs[3][sourceId];

  //calculate the distances
  int distanceTo0 = costToGivenNode + packet->mincost[0];
  int distanceTo1 = costToGivenNode + packet->mincost[1];
  int distanceTo2 = costToGivenNode + packet->mincost[2];
  //update table
  if(dt3.costs[3][0] > distanceTo0){
    dt3.costs[3][0] = distanceTo0;
    sendToNeighbour3();
  }
  if(dt3.costs[3][1] > distanceTo1){
    dt3.costs[3][1] = distanceTo1;
    sendToNeighbour3();
  }
  if(dt3.costs[3][2] > distanceTo2){
    dt3.costs[3][2] = distanceTo2;
    sendToNeighbour3();
  }
  int i;
  int j;
  for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++){
      printf("%d ", dt3.costs[i][j]);
    }
    printf("\n" );
  }
}
