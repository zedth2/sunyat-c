

#include <stdlib.h>
#include <string.h> //Only needed for strcmp

#include "sunyat.h"

int main (int argc, char *argv []) {
    if (argc < 2) 	{
		//printf (ERR_BAD_USAGE);
		return EXT_ERR_NO_FILE_ARG;
	}
    int debug = 0, save_state = 0, i = 0 ;
    //printf("here1\n") ;
    for(i = 0 ; i < argc ; i++) {
        //printf("here2 %s \n", argv[i]) ;
        if(0 == strcmp(argv[i], SAVE_STATE_SWITCH)) {
            printf("save state set.\n") ;
            save_state = 1 ;
        } else if (0 == strcmp(argv[i], DEBUGGER_SWITCH)) {
            printf("debug set.\n") ;
            debug = 1 ;
        }
    }

    return start_sunyat(argv[1], save_state, debug) ;
}
