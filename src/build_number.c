/*
 * Name        : build_number.c
 * Author      : William "Amos" Confer
 * License     : Copyright (c) 2014 William "Amos" Confer
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a
 *    copy of this software and associated documentation files (the "Software"),
 *    to deal in the Software without restriction, including without limitation
 *    the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *    and/or sell copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *    DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned int VERSION_MAJOR        = 0;
const unsigned int VERSION_MINOR        = 8;
const char         VERSION_MODIFIER[]   = ""; /* e.g., "alpha", or "RC1" */

const unsigned int DEFAULT_BUILD_NUMBER = 1;

#define HEAD_LINE_CNT 54

const char head_str[HEAD_LINE_CNT][200] = {
"#ifndef _BUILD_NUMBER_H_",
"#define _BUILD_NUMBER_H_                     ",
"",
"/*******************************************/",
"/* DO NOT EDIT THIS FILE.  IT IS GENERATED */",
"/*******************************************/	",
"",
"/*",
" * Name        : build_number.h",
" * Author      : William \"Amos\" Confer",
" * Description : This header is the generated output of the build_number.c ",
" *    application.  The build_number application must be executed at least once ",
" *    prior to any SCM commits.  It increments the build number so bugs in any ",
" *    application including it can be traced from the appropriate point in the SCM ",
" *    history.",
" *",
" *    The major, minor, and modifier versions are also maintined by this application,",
" *    but they are not modified by it.  Project developers must change these values",
" *    in build_number.c whenever appropriate.  It would be common practice to reset",
" *    the build number to 0 at such a time (it is incremented to 1 and generated into",
" *    this header on execution), although that is completely up to the project team.",
" *",
" *    When this application is executed in a source directory lacking a build_number.h",
" *    header, a new one is generated with the major, minor, and modifier versions.",
" *    The generated file contains a build number initialized to 1, as well.",
" *",
" *    When forking or branches occur in an SCM history, it is possible for multiple",
" *    versions of the code to acquire identical build numbers, even though their content",
" *    is otherwise unique or were commited at different times.  Such situations are",
" *    the responsibility of each project to address.  Sorry :/  If I could do it in a ",
" *    cross-platform way that worked with most C compilers and SCMs, I would centralize",
" *    the numbering to guarantee uniqueness of numbers, but I've not come up with a way",
" *    to do that yet.",
" *",
" * License     : Copyright (c) 2014 William \"Amos\" Confer",
" *",
" *    Permission is hereby granted, free of charge, to any person obtaining a ",
" *    copy of this software and associated documentation files (the \"Software\"),",
" *    to deal in the Software without restriction, including without limitation",
" *    the rights to use, copy, modify, merge, publish, distribute, sublicense, ",
" *    and/or sell copies of the Software, and to permit persons to whom the",
" *    Software is furnished to do so, subject to the following conditions:",
" *",
" *    The above copyright notice and this permission notice shall be included in",
" *    all copies or substantial portions of the Software.",
" *",
" *    THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR",
" *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,",
" *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL ",
" *    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER",
" *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING ",
" *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER ",
" *    DEALINGS IN THE SOFTWARE.",
" */"
};


const char major_str[]        =
	"const unsigned int VERSION_MAJOR      = %u;\n";
const char minor_str[]        =
	"const unsigned int VERSION_MINOR      = %u;\n";
const char modifier_str[]     =
	"const char         VERSION_MODIFIER[] = \"%s\"; /* e.g., \"alpha\", or \"RC1\" */\n";
const char build_number_str[] =
	"const unsigned int BUILD_NUMBER       = %u;\n";


#define FOOT_LINE_CNT 5

const char foot_str[FOOT_LINE_CNT][200] = {
"/*******************************************/",
"/* DO NOT EDIT THIS FILE.  IT IS GENERATED */",
"/*******************************************/",
"",
"#endif /* _BUILD_NUMBER_H_ */"
};


const char *filename = "build_number.h";
FILE *build_file;

unsigned int build_number;

int generate_header() {
	int i;

	if(NULL == (build_file = fopen(filename, "w"))) {
		fprintf(stderr, "Could not open %s for writing\n", filename);
		return EXIT_FAILURE;
	}

	for(i = 0; i < HEAD_LINE_CNT; i++) {
		fprintf(build_file, "%s\n", head_str[i]);
	}

	fprintf(build_file, "\n");

	fprintf(build_file, major_str, VERSION_MAJOR);
	fprintf(build_file, minor_str, VERSION_MINOR);
	fprintf(build_file, modifier_str, VERSION_MODIFIER);

	fprintf(build_file, build_number_str, build_number);

	fprintf(build_file, "\n");

	for(i = 0; i < FOOT_LINE_CNT; i++) {
		fprintf(build_file, "%s\n", foot_str[i]);
	}

	fclose(build_file);

	printf("\n-------------------------\n");
	printf("Updated build number.  It is now...\n");
	printf("Version %u.%u", VERSION_MAJOR, VERSION_MINOR);
	if(strlen(VERSION_MODIFIER) > 0) {
		printf("_%s", VERSION_MODIFIER);
	}
	printf(" Build %u\n", build_number);
	printf("-------------------------\n\n");

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

	build_number = DEFAULT_BUILD_NUMBER;
    if(argc > 1) filename = argv[1] ;
	if(NULL != (build_file = fopen(filename, "r"))) {
		const int expected_col_num  = 40;
		const int expected_line_num = 59;

		char str[200];
		int i;

		/* read past all the head-matter */
		for(i = 1; i < expected_line_num; i++) {
			if(NULL == fgets(str, 200, build_file)) {
				fprintf(stderr, "Couldn't read line %d of %s\n", i, filename);
				fclose(build_file);
				return EXIT_FAILURE;
			}
		}

		/* get the old build number */
		if((NULL == fgets(str, 200, build_file)) ||
		   (0 >= sscanf(str + expected_col_num, "%u", &build_number))) {
			fprintf(stderr, "Couldn't read build number line of %s\n", filename);
			fclose(build_file);
			return EXIT_FAILURE;
		}

		build_number++;

		fclose(build_file);
	}

	return generate_header();
}
