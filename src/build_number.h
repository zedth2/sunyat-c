#ifndef _BUILD_NUMBER_H_
#define _BUILD_NUMBER_H_                     

/*******************************************/
/* DO NOT EDIT THIS FILE.  IT IS GENERATED */
/*******************************************/	

/*
 * Name        : build_number.h
 * Author      : William "Amos" Confer
 * Description : This header is the generated output of the build_number.c 
 *    application.  The build_number application must be executed at least once 
 *    prior to any SCM commits.  It increments the build number so bugs in any 
 *    application including it can be traced from the appropriate point in the SCM 
 *    history.
 *
 *    The major, minor, and modifier versions are also maintined by this application,
 *    but they are not modified by it.  Project developers must change these values
 *    in build_number.c whenever appropriate.  It would be common practice to reset
 *    the build number to 0 at such a time (it is incremented to 1 and generated into
 *    this header on execution), although that is completely up to the project team.
 *
 *    When this application is executed in a source directory lacking a build_number.h
 *    header, a new one is generated with the major, minor, and modifier versions.
 *    The generated file contains a build number initialized to 1, as well.
 *
 *    When forking or branches occur in an SCM history, it is possible for multiple
 *    versions of the code to acquire identical build numbers, even though their content
 *    is otherwise unique or were commited at different times.  Such situations are
 *    the responsibility of each project to address.  Sorry :/  If I could do it in a 
 *    cross-platform way that worked with most C compilers and SCMs, I would centralize
 *    the numbering to guarantee uniqueness of numbers, but I've not come up with a way
 *    to do that yet.
 *
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

const unsigned int VERSION_MAJOR      = 0;
const unsigned int VERSION_MINOR      = 8;
const char         VERSION_MODIFIER[] = ""; /* e.g., "alpha", or "RC1" */
const unsigned int BUILD_NUMBER       = 124;

/*******************************************/
/* DO NOT EDIT THIS FILE.  IT IS GENERATED */
/*******************************************/

#endif /* _BUILD_NUMBER_H_ */
