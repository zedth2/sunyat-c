

#include <stdlib.h>
#include "sunyat.h"

int main (int argc, char *argv []) {


	if (argc != 2) 	{
		//printf (ERR_BAD_USAGE);
		return EXT_ERR_NO_FILE_ARG;
	}

    return start_sunyat(argv[1], 0, 0) ;
}
