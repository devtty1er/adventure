
#include <err.h>
#include <stdio.h> /* drv = 1.1st file 2.def 3.A	*/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <stdint.h>

#include "advent.h"  /* #define preprocessor equates	*/
#include "advword.h" /* definition of "word" array	*/
#include "advcave.h" /* definition of "cave" array	*/
#ifndef BUILTIN
#include "advtext.h" /* definition of "text" arrays	*/
#endif
#include "advdef.h"

#define setmem(l, s, c) memset(l, c, s)

// #define printf(...) 
// #define fprintf(...) 
// #define fputs(...) 
// #define fputc(...) 
// #define fflush(...) 


FILE *fopen(const char *pathname, const char *mode) { return NULL; }
int fclose(FILE *fp) { return 0; }
void saveadv(void) { return; }

static char g_input[4096];
static size_t g_pos = 0;

char *fgets(char *s, int size, FILE *stream) {
    if (stream != stdin || g_pos >= sizeof(g_input)) 
        return NULL;
    
    size_t i;
    for (i = 0; i < (size_t)(size-1) && g_pos < sizeof(g_input); i++) {
        s[i] = g_input[g_pos++];
        if (s[i] == '\n') {
            i++;
            break;
        }
    }
    if (i == 0) return NULL;
    s[i] = '\0';
    return s;
}

/*
Game state size calculation:
turns       = sizeof(int) = 4
loc         = sizeof(int) = 4
oldloc      = sizeof(int) = 4
oldloc2     = sizeof(int) = 4
newloc      = sizeof(int) = 4
cond[MAXLOC]= sizeof(int) * 140 = 560
place[MAXOBJ]= sizeof(int) * 100 = 400
fixed[MAXOBJ]= sizeof(int) * 100 = 400
visited[MAXLOC]= sizeof(int) * 140 = 560
prop[MAXOBJ]= sizeof(int) * 100 = 400
tally       = sizeof(int) = 4
tally2      = sizeof(int) = 4
limit       = sizeof(int) = 4
lmwarn      = sizeof(int) = 4
wzdark      = sizeof(int) = 4
closing     = sizeof(int) = 4
closed      = sizeof(int) = 4
holding     = sizeof(int) = 4
detail      = sizeof(int) = 4
knfloc      = sizeof(int) = 4
clock1      = sizeof(int) = 4
clock2      = sizeof(int) = 4
panic       = sizeof(int) = 4
dloc[DWARFMAX]= sizeof(int) * 7 = 28
dflag       = sizeof(int) = 4
dseen[DWARFMAX]= sizeof(int) * 7 = 28
odloc[DWARFMAX]= sizeof(int) * 7 = 28
daltloc     = sizeof(int) = 4
dkill       = sizeof(int) = 4
chloc       = sizeof(int) = 4
chloc2      = sizeof(int) = 4
bonus       = sizeof(int) = 4
numdie      = sizeof(int) = 4
object1     = sizeof(int) = 4
gaveup      = sizeof(int) = 4
foobar      = sizeof(int) = 4
Total: 2504 bytes

Minimum input size: 2504 (state) + 64 (commands) = 2568 bytes
*/

void initplay(void)
{
	turns = 0;

	/* initialize location status array */
	setmem(cond, (sizeof(int)) * MAXLOC, 0);
	scanint(&cond[1], "5,1,5,5,1,1,5,17,1,1,");
	scanint(&cond[13], "32,0,0,2,0,0,64,2,");
	scanint(&cond[21], "2,2,0,6,0,2,");
	scanint(&cond[31], "2,2,0,0,0,0,0,4,0,2,");
	scanint(&cond[42], "128,128,128,128,136,136,136,128,128,");
	scanint(&cond[51], "128,128,136,128,136,0,8,0,2,");
	scanint(&cond[79], "2,128,128,136,0,0,8,136,128,0,2,2,");
	scanint(&cond[95], "4,0,0,0,0,1,");
	scanint(&cond[113], "4,0,1,1,");
	scanint(&cond[122], "8,8,8,8,8,8,8,8,8,");

	/* initialize object locations */
	setmem(place, (sizeof(int)) * MAXOBJ, 0);
	scanint(&place[1], "3,3,8,10,11,0,14,13,94,96,");
	scanint(&place[11], "19,17,101,103,0,106,0,0,3,3,");
	scanint(&place[23], "109,25,23,111,35,0,97,");
	scanint(&place[31], "119,117,117,0,130,0,126,140,0,96,");
	scanint(&place[50], "18,27,28,29,30,");
	scanint(&place[56], "92,95,97,100,101,0,119,127,130,");

	/* initialize second (fixed) locations */
	setmem(fixed, (sizeof(int)) * MAXOBJ, 0);
	scanint(&fixed[3], "9,0,0,0,15,0,-1,");
	scanint(&fixed[11], "-1,27,-1,0,0,0,-1,");
	scanint(&fixed[23], "-1,-1,67,-1,110,0,-1,-1,");
	scanint(&fixed[31], "121,122,122,0,-1,-1,-1,-1,0,-1,");
	scanint(&fixed[62], "121,-1,");

	/* initialize default verb messages */
	scanint(actmsg, "0,24,29,0,33,0,33,38,38,42,14,");
	scanint(&actmsg[11], "43,110,29,110,73,75,29,13,59,59,");
	scanint(&actmsg[21], "174,109,67,13,147,155,195,146,110,13,13,");

	/* initialize various flags and other variables */
	setmem(visited, (sizeof(int)) * MAXLOC, 0);
	setmem(prop, (sizeof(int)) * MAXOBJ, 0);
	setmem(&prop[50], (sizeof(int)) * (MAXOBJ - 50), 0xff);
	wzdark = closed = closing = holding = detail = 0;
	limit = 100;
	tally = 15;
	tally2 = 0;
	newloc = 1;
	loc = oldloc = oldloc2 = 3;
	knfloc = 0;
	chloc = 114;
	chloc2 = 140;
	/*	dloc[DWARFMAX-1] = chloc				*/
	scanint(dloc, "0,19,27,33,44,64,114,");
	scanint(odloc, "0,0,0,0,0,0,0,");
	dkill = 0;
	scanint(dseen, "0,0,0,0,0,0,0,");
	clock1 = 30;
	clock2 = 50;
	panic = 0;
	bonus = 0;
	numdie = 0;
	daltloc = 18;
	lmwarn = 0;
	foobar = 0;
	dflag = 0;
	gaveup = 0;
	saveflg = 0;
}

void scanint(int *pi, char *str)
{
	while (*str) {
		if ((sscanf(str, "%d,", pi++)) < 1)
			bug(41);      /* failed before EOS	*/
		while (*str++ != ',') /* advance str pointer	*/
			;
	}
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 64) return 0;  // Need minimum command input (64)

    initplay();
    
    // Commands come directly from input data
    memset(g_input, 0, sizeof(g_input));
    memcpy(g_input, data, size < sizeof(g_input) ? size : sizeof(g_input));
    g_pos = 0;

    // Run turns until input exhausted
    while (g_pos < sizeof(g_input)) {
        turn();
        if (closed || numdie >= MAXDIE || gaveup)
            break;
    }
    return 0;
}
