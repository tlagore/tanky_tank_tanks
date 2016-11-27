#include <stdio.h>
#include <stdlib.h>
#include "events.h"

void randomize_terrain(struct Terrain terrain[]);
int test_collision();


struct Model ts =
	{
		{	
			/* Tanks */
			/* Tank 1 @ x = 96, y = 300 */
			{ 96, 300, 0, 159, 32, 16, 100, 0, 0, 0 },

			/* Tank 2 @ x = 543, y = 300 */			
			{ 543, 300, 480, 639, 32, 16, 100, 0, 0, 0} 
		},
		{
			/* Staging areas */
		
			{ 0, 159, 317 }, 
			/* Staging area for Tank 1 from x: 0.159 y: 317 */
			{ 480, 639, 317 } 
			/* Staging area for Tank 2 from x: 480.639 y: 317 */
		},
		
		{
			/* Terrain Objects */
			
			{ 0, 0, 0, 8}, /* Terrain object 1 */
			{ 0, 0, 0, 8}, /* Terrain object 2 */
			{ 0, 0, 0, 8}, /* Terrain object 3 */
			{ 0, 0, 0, 8}, /* Terrain object 4 */
			{ 0, 0, 0, 8}, /* Terrain object 5 */
			{ 0, 0, 0, 8}, /* Terrain object 6 */
			{ 0, 0, 0, 8}, /* Terrain object 7 */
			{ 0, 0, 0, 8}, /* Terrain object 8 */
			{ 0, 0, 0, 8}, /* Terrain object 9 */
			{ 0, 0, 0, 8}, /* Terrain object 10 */
			{ 0, 0, 0, 8}, /* Terrain object 11 */
			{ 0, 0, 0, 8}, /* Terrain object 12 */
			{ 0, 0, 0, 8}, /* Terrain object 13 */
			{ 0, 0, 0, 8}, /* Terrain object 14 */
			{ 0, 0, 0, 8}, /* Terrain object 15 */
			{ 0, 0, 0, 8}, /* Terrain object 16 */
			{ 0, 0, 0, 8}, /* Terrain object 17 */
			{ 0, 0, 0, 8}, /* Terrain object 18 */
			{ 0, 0, 0, 8}, /* Terrain object 19 */
			{ 0, 0, 0, 8}, /* Terrain object 20 */
			{ 0, 0, 0, 8}, /* Terrain object 21 */
			{ 0, 0, 0, 8}, /* Terrain object 22 */
			{ 0, 0, 0, 8}, /* Terrain object 23 */
			{ 0, 0, 0, 8}, /* Terrain object 24 */
			{ 0, 0, 0, 8}, /* Terrain object 25 */
			{ 0, 0, 0, 8}, /* Terrain object 26 */
			{ 0, 0, 0, 8}, /* Terrain object 27 */
			{ 0, 0, 0, 8}, /* Terrain object 28 */
			{ 0, 0, 0, 8}, /* Terrain object 29 */
			{ 0, 0, 0, 8}, /* Terrain object 30 */
			{ 0, 0, 0, 8}, /* Terrain object 31 */
			{ 0, 0, 0, 8}, /* Terrain object 32 */
			{ 0, 0, 0, 8}, /* Terrain object 33 */
			{ 0, 0, 0, 8}, /* Terrain object 34 */
			{ 0, 0, 0, 8}, /* Terrain object 35 */
			{ 0, 0, 0, 8}, /* Terrain object 36 */
			{ 0, 0, 0, 8}, /* Terrain object 37 */
			{ 0, 0, 0, 8}, /* Terrain object 38 */
			{ 0, 0, 0, 8}, /* Terrain object 39 */
			{ 0, 0, 0, 8} /* Terrain object 40 */
		},
		
		/* Missile */
		{ 0, 0, 0, 0, 8, 8} /* Missile Object, uninstantiated except for
								width/height */
	};
	
int main()
{
	int i;
	int collision;
	
	
	printf ("Tank 1 x: %d\nTank 1 y: %d\nTank 1 leftB: %d\nTank 1 rightB: %d\n",
		ts.tanks[0].x, ts.tanks[0].y, 
		ts.tanks[0].leftBoundary, ts.tanks[0].rightBoundary);

	ts.tanks[0].deltaX = 3;
	
	
	/* Move right test */
	printf ("\n\nMove right test:\nTank old x: %u. Press return.\n", ts.tanks[0].x);
	getchar();
	event_tank_move(&ts.tanks[0]);
	printf ("Tank new x: %u <- should be +3\n ", ts.tanks[0].x);
	
	/* Move left test  */
	ts.tanks[0].deltaX = -5;
	printf ("\n\nMove left test:\nTank old x: %d. Press return.\n", ts.tanks[0].x);
	getchar();
	event_tank_move(&ts.tanks[0]);
	printf ("Tank new x: %d <- should be -5\n ", ts.tanks[0].x);
	
	
	/* Randomize terrain test */
	printf("Next test, randomize terrain.  Press return.");
	getchar();
	
	randomize_terrain(ts.terrain);
	printf("Randomizing terrain...\n");
	
	for (i = 0; i < 40; i++)
		printf ("terrain x: %d terrain y: %d terrain height: %d\n", ts.terrain[i].x, ts.terrain[i].y, ts.terrain[i].height);
		
		
	printf("Next test, move tank past left boundary\n(should note no change in x coordinate).  Press return.");
	getchar();
	
	ts.tanks[0].x = 0;
	ts.tanks[0].deltaX = -3;
	
	printf ("Tank x: %u\nTank deltaX: %d\n", ts.tanks[0].x, ts.tanks[0].deltaX);
	printf ("Tank leftBoundary: %d\n", ts.tanks[0].leftBoundary);
	printf ("Press return.");
	getchar();
	
	printf("Attempting move...\n");
	event_tank_move (&ts.tanks[0]);
	
	printf ("Tank x: %d\n", ts.tanks[0].x);
	getchar();
	
	ts.tanks[0].x = 126;
	ts.tanks[0].deltaX = 4;
	printf ("Next test, move tank past right boundary\n(should note no change in x coordinate). Press return");
	getchar();
	printf ("Tank x: %u\nTank deltaX: %d\n", ts.tanks[0].x, ts.tanks[0].deltaX);
	printf ("Tank rightBoundary: %d\n", ts.tanks[0].rightBoundary);

	printf("Press return...");
	getchar();
	
	printf("Attempting move...\n");
	event_tank_move(&ts.tanks[0]);
	
	printf ("Tank x: %d\n", ts.tanks[0].x);
	getchar();
	
	printf ("Collision test: Should hit a tank!\n");
	printf ("Press return...");
	getchar();
	
	ts.missile.x = ts.tanks[0].x;
	ts.missile.y = ts.tanks[0].y;
	
	test_collision();
	
	ts.missile.x = ts.terrain[2].x;
	ts.missile.y = ts.terrain[2].y;
	
	printf ("Collision test: Should hit terrain!\n");
	printf ("%d <- height of terrain that it should collide with\n", ts.terrain[2].height);
	printf ("Press return...");
	getchar();
	test_collision();
	printf ("%d <- height after collision\n", ts.terrain[2].height);
	
	ts.missile.x = 3;
	ts.missile.deltaX = -40;
	
	printf ("Collision test: Should hit the wall boundary!\n");
	printf ("Press return...");
	getchar();
	test_collision();
	
	printf ("Tests completed. Press return to quit.");
	getchar();
	return 0;
	
}
/*


*/
int test_collision()
{
	int collision = event_move_missile(ts.terrain, &ts.missile, ts.tanks);
	if (collision == 1)
	{
		printf ("You hit something! Collision code: 1.  A tank was struck!\n");
		printf ("Tank 1 HP: %d\n", ts.tanks[PLAYER_ONE].health);
		printf ("Tank 2 HP: %d\n", ts.tanks[PLAYER_TWO].health);
	}else if (collision == 2)
		printf ("You hit something! Collision code: 2.  Destructible terrain was hit!\n");
	else if (collision == 3)
		printf ("You exited the screen boundaries! Collision code 3.\n");
	else
		printf ("Nothing hit!\n");
		
	printf ("Test finished. Press return to continue.\n");
	
	return collision;
}

void randomize_terrain(struct Terrain terrain[])
{
	int i, change, toggle;
	UINT16 x = TERRAIN_START;
	UINT16 y = LOWEST_TERRAIN_POINT;

	terrain[0].x = x;			/* First terrain block is static */
	terrain[0].y = y;			/* To avoid weird clipping */
	
	
	
	terrain[39].x = TERRAIN_FINISH;
	terrain[39].y = LOWEST_TERRAIN_POINT;	
	
	for (i = 1; i < 39; i++)
	{
		x += terrain[i].width;
	
		change = (rand() & 15);
		toggle = (rand() & 127);
		terrain[i].x = x;
		terrain[i].y = toggle < 64 ? 
					(y-change): (y + change);
					
		terrain[i].height = (LOWEST_TERRAIN_POINT - terrain[i].y) > 0 ? 
				LOWEST_TERRAIN_POINT - terrain[i].y : 1;
	
		y = i < 20 ? y -= 10 : y += 10;
	}
}					

