/*
Copyright(c) 2017 Cedric Jimenez

This file is part of Nano-STL.

Nano-STL is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Nano-STL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Nano-STL.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NANO_STL_CONF_H
#define NANO_STL_CONF_H


#if (__cplusplus < 201103L)
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#ifndef NULL
#define NULL 0
#endif // NULL
#define nullptr NULL

#else // __cplusplus
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#endif // __cplusplus

namespace nano_stl
{

/** \brief Define the maximum size of a container in number of elements */
typedef uint32_t nano_stl_size_t;

}


//////////////////////////////// Options /////////////////////////////////


/* Containers configuration options */

/** \brief Enable the use of iterators (increase code size) */
#define NANO_STL_ITERATORS_ENABLED                      0





/* Memory management configuration options */

/** \brief No dynamic memory allocation will be allowed */
#define NANO_STL_DYNAMIC_MEMORY_ALLOCATION_DISABLED     0
/** \brief Dynamic memory allocation will be performed by C malloc/free functions */
#define NANO_STL_DYNAMIC_MEMORY_ALLOCATION_MALLOC       1
/** \brief Dynamic memory allocation will be performed by C++ standard new/delete implementation or by another library */
#define NANO_STL_DYNAMIC_MEMORY_ALLOCATION_STANDARD     2

/** \brief Memory management configuration
 *         Valid values are:
 *         NANO_STL_DYNAMIC_MEMORY_ALLOCATION_DISABLED
 *         NANO_STL_DYNAMIC_MEMORY_ALLOCATION_MALLOC
 *         NANO_STL_DYNAMIC_MEMORY_ALLOCATION_STANDARD
 */
#define NANO_STL_DYNAMIC_MEMORY_ALLOCATION      NANO_STL_DYNAMIC_MEMORY_ALLOCATION_DISABLED




/* Lib C configuration options 

    Default is to use internal Nano-STL fonctions which are highly portable, 
    usually small in code size but not very efficient in terms of performances.

    Uncomment the following headers and change the function names in the macro definitions
    if you want to use standard functions from your compiler librairies.
*/

/*  
#if (__cplusplus < 201103L)
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#else // __cplusplus
#include <cstring>
#include <cstdlib>
#include <cstdio>
#endif // __cplusplus
*/

#include "nano-stl-libc.h"

/** \brief Memset macro definition */
#define NANO_STL_MEMSET(dst, val, size) NANO_STL_LIBC_Memset((dst), (val), (size))

/** \brief Memcpy macro definition */
#define NANO_STL_MEMCPY(dst, src, size) NANO_STL_LIBC_Memcpy((dst), (src), (size))

/** \brief Memcmp macro definition */
#define NANO_STL_MEMCMP(s1, s2, size) NANO_STL_LIBC_Memcmp((s1), (s2), (size))

/** \brief Strncmp macro definition */
#define NANO_STL_STRNCMP(s1, s2, size) NANO_STL_LIBC_Strncmp((s1), (s2), (size))

/** \brief Strnlen macro definition */
#define NANO_STL_STRNLEN(s, maxlen) NANO_STL_LIBC_Strnlen((s), (maxlen))

/** \brief Strncat macro definition */
#define NANO_STL_STRNCAT(dest, src, size) NANO_STL_LIBC_Strncat((dest), (src), (size))

/** \brief Strncpy macro definition */
#define NANO_STL_STRNCPY(dest, src, size) NANO_STL_LIBC_Strncpy((dest), (src), (size))

/** \brief Vsnprintf macro definition */
#define NANO_STL_VSNPRINTF(str, n, format, ap) NANO_STL_LIBC_Vsnprintf((str), (n), (format), (ap))

/** \brief Snprintf macro definition */
#define NANO_STL_SNPRINTF(str, n, format, ...) NANO_STL_LIBC_Snprintf((str), (n), (format), ##__VA_ARGS__)

/** \brief Atoi macro definition */
#define NANO_STL_ATOI(str) NANO_STL_LIBC_Atoi((str))

/** \brief Itoa macro definition */
#define NANO_STL_ITOA(value, str, base) NANO_STL_LIBC_Itoa((value), (str), (base))

/** \brief Atof macro definition */
#define NANO_STL_ATOF(str) NANO_STL_LIBC_Atof((str))


#endif // NANO_STL_CONF_H
