__kernel void life_logic(__global const int *A, __global int *B, const unsigned int whsize) 
{
/* Get the index of the current element to be processed */
int id = get_global_id(0);

/* id to i j*/
int i = id / whsize;
int j = id % whsize;

/* check amount of neighbors */
int neighborsAmount = 0;

/* for positions 1,2,3,7,8,9 */
int x = i-1;
int y = j-1;

if ((x >= 0) && (x < whsize) && (y >= 0) && (y < whsize))
{
   int temp = mad(x, whsize, y);
   if (A[temp] == 1)
   {
      neighborsAmount++;
   }
}

x = i-1;
y = j;
if ((x >= 0) && (x < whsize ) && (y >= 0) && ( y < whsize))
{
   int temp = mad (x,whsize,y);
   if (A[temp] == 1)
   {
      neighborsAmount++;
   }
}

x = i-1;
y = j+1;
if ((x >= 0) && (x < whsize ) && (y >= 0) && ( y < whsize))
{
   int temp = mad (x,whsize,y);
   if (A[temp] == 1)
   {
      neighborsAmount++;
   }
}

x = i+1;
y = j-1;
if ((x >= 0) && (x < whsize ) && (y >= 0) && ( y < whsize))
{
   int temp = mad (x,whsize,y);
   if (A[temp] == 1)
   {
      neighborsAmount++;
   }
}

x = i+1;
y = j;
if ((x >= 0) && (x < whsize ) && (y >= 0) && ( y < whsize))
{
   int temp = mad (x,whsize,y);
   if (A[temp] == 1)
   {
      neighborsAmount++;
   }
}

x = i+1;
y = j+1;
if ((x >= 0) && (x < whsize ) && (y >= 0) && ( y < whsize))
{
   int temp = mad (x,whsize,y);
   if (A[temp] == 1)
   {
      neighborsAmount++;
   }
}

/* for position 4 */
x = i;
y = j-1;
if ((x >= 0) && (x < whsize ) && (y >= 0) && ( y < whsize))
{
   int temp = mad (x,whsize,y);
   if (A[temp] == 1)
   {
      neighborsAmount++;
   }
}

/* for position 6 */
x = i;
y = j+1;
if ((x >= 0) && (x < whsize ) && (y >= 0) && ( y < whsize))
{
   int temp = mad (x,whsize,y);
   if (A[temp] == 1)
   {
      neighborsAmount++;
   }
}

/* perform proper seeding */
int resultIdx = mad(i,whsize,j);

if ((neighborsAmount == 3) && (A[resultIdx] == 0)) /*if 3 neighbors and is not alive -> goes alive*/
{
   B[resultIdx] = 1;
}
else if (((neighborsAmount == 2) || (neighborsAmount == 3)) && (A[resultIdx] == 1)) /*if 2 or 3 neighbors and is alive -> stay alive*/
{
   B[resultIdx] = 1;
}
else /*dies*/
{   
   B[resultIdx] = 0;
}

/*end*/
}