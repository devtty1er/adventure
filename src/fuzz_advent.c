#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "advent.h"
#include "advdec.h"

#define printf(...) 
#define fprintf(...) 
#define fputs(...) 
#define fputc(...) 
#define fflush(...) 

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

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 2504 + 64) return 0;  // Need game state (2504) + minimum command input (64)

    initplay();
    
    const uint8_t *p = data;
    memcpy(&turns, p, sizeof(int)); p += sizeof(int);
    memcpy(&loc, p, sizeof(int)); p += sizeof(int);
    memcpy(&oldloc, p, sizeof(int)); p += sizeof(int);
    memcpy(&oldloc2, p, sizeof(int)); p += sizeof(int);
    memcpy(&newloc, p, sizeof(int)); p += sizeof(int);
    memcpy(cond, p, sizeof(int) * MAXLOC); p += sizeof(int) * MAXLOC;
    memcpy(place, p, sizeof(int) * MAXOBJ); p += sizeof(int) * MAXOBJ;
    memcpy(fixed, p, sizeof(int) * MAXOBJ); p += sizeof(int) * MAXOBJ;
    memcpy(visited, p, sizeof(int) * MAXLOC); p += sizeof(int) * MAXLOC;
    memcpy(prop, p, sizeof(int) * MAXOBJ); p += sizeof(int) * MAXOBJ;
    memcpy(&tally, p, sizeof(int)); p += sizeof(int);
    memcpy(&tally2, p, sizeof(int)); p += sizeof(int);
    memcpy(&limit, p, sizeof(int)); p += sizeof(int);
    memcpy(&lmwarn, p, sizeof(int)); p += sizeof(int);
    memcpy(&wzdark, p, sizeof(int)); p += sizeof(int);
    memcpy(&closing, p, sizeof(int)); p += sizeof(int);
    memcpy(&closed, p, sizeof(int)); p += sizeof(int);
    memcpy(&holding, p, sizeof(int)); p += sizeof(int);
    memcpy(&detail, p, sizeof(int)); p += sizeof(int);
    memcpy(&knfloc, p, sizeof(int)); p += sizeof(int);
    memcpy(&clock1, p, sizeof(int)); p += sizeof(int);
    memcpy(&clock2, p, sizeof(int)); p += sizeof(int);
    memcpy(&panic, p, sizeof(int)); p += sizeof(int);
    memcpy(dloc, p, sizeof(int) * DWARFMAX); p += sizeof(int) * DWARFMAX;
    memcpy(&dflag, p, sizeof(int)); p += sizeof(int);
    memcpy(dseen, p, sizeof(int) * DWARFMAX); p += sizeof(int) * DWARFMAX;
    memcpy(odloc, p, sizeof(int) * DWARFMAX); p += sizeof(int) * DWARFMAX;
    memcpy(&daltloc, p, sizeof(int)); p += sizeof(int);
    memcpy(&dkill, p, sizeof(int)); p += sizeof(int);
    memcpy(&chloc, p, sizeof(int)); p += sizeof(int);
    memcpy(&chloc2, p, sizeof(int)); p += sizeof(int);
    memcpy(&bonus, p, sizeof(int)); p += sizeof(int);
    memcpy(&numdie, p, sizeof(int)); p += sizeof(int);
    memcpy(&object1, p, sizeof(int)); p += sizeof(int);
    memcpy(&gaveup, p, sizeof(int)); p += sizeof(int);
    memcpy(&foobar, p, sizeof(int)); p += sizeof(int);

    data += 2504;
    size -= 2504;

    // Rest is commands
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
