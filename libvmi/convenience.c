/*
 * The LibVMI Library is an introspection library that simplifies access to 
 * memory in a target virtual machine or in a file containing a dump of 
 * a system's physical memory.  LibVMI is based on the XenAccess Library.
 *
 * Copyright (C) 2010 Sandia National Laboratories
 * Author: Bryan D. Payne (bpayne@sandia.gov)
 */

#include "libvmi.h"
#include "private.h"
#include <stdarg.h>
#include <stdlib.h>

/* updates the errno value to the given value */
void vmi_set_errno (int error){
// do nothing for now... not using errno just yet
//    errno = error;
}

#ifndef VMI_DEBUG
/* Nothing */
#else
void dbprint(char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}
#endif

/* prints an error message to stderr */
void errprint (char* format, ...){
    va_list args;
    fprintf(stderr, "VMI_ERROR: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

/* prints a warning message to stderr */
void warnprint (char* format, ...){
    va_list args;
    fprintf(stderr, "VMI_WARNING: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

int vmi_report_error (vmi_instance_t instance, int error, int error_type){
    int ret = 0;

    /* determine how to set the return value */
    if (instance->error_mode == VMI_FAILHARD){
        ret = VMI_FAILURE;
    }
    else if (instance->error_mode == VMI_FAILSOFT){
        if (error_type == VMI_ECRITICAL){
            ret = VMI_FAILURE;
        }
        else{
            ret = VMI_SUCCESS;
        }
    }
    else{
        dbprint("BUG: invalid mode\n");
        ret = VMI_FAILURE;
    }

    /* report to errno */
    if (VMI_FAILURE == ret){
        vmi_set_errno(error);
    }

    /* return the return value to be used by the library */
    return ret;
}

void *safe_malloc_ (size_t size, char const *file, int line)
{
    void *p = malloc(size);
    if (NULL == p){
        errprint("malloc %lu bytes failed at %s:%d\n", (unsigned long)size, file, line);
        exit(EXIT_FAILURE);
   }
   return p;
}

unsigned long get_reg32 (reg_t r)
{
    return (unsigned long) r;
}