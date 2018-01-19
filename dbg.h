#ifndef __dbg_h__   //Check incase you include the pakage twice
#define __dbg_h__


#include <stdio.h>  //include the libraries that you need to make it work
#include <errno.h>
#include <string.h>

#ifdef NDEBUG  //recompiles so that all of the debug errors are removed
#define debug(M, ...)//if debug is defined then no debug messages will appear, so you can turn them on and off
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)//VA_ARGS lets you use whaterver else you need here
#endif

#define clean_errno() (errno ==0 ? "None" : strerror(errno))// is used to get the error message in a readable way

//end user error messages, cant be compiled out
#define log_err(M, ...) fprintf(stderr, "[ERROR](%s:Function: %s :%d: errno: %s) " M "\n", __FILE__,__FUNCTION__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN](%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

//check if A is true, if not then it goes to the functions error
#define check(A, M, ...) if((!A)){ log_err(M, ##__VA_ARGS__); errno=0; goto error;}

//placed in parts of the code that should never run  then jumps to the function cleanup
#define sentinel(M,...) {log_err(M, ##__VA_ARGS__); errno=0; goto error;}

//checks to make sure a pointer is valid
#define check_mem(A) check((A), "Out of memory.")

//Checks and handles an error, if it is common then it doesnt report it 
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error;}

#endif

