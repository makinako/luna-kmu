/****************************************************************************\
*
* This file is part of the "Luna KMU" tool.
*
* The "KMU" tool is provided under the MIT license (see the
* following Web site for further details: https://mit-license.org/ ).
*
* Author: Kim O'Sullivan
*
* Copyright © 2026 Makina Pty Ltd (assigned to Thales Group)
*
\****************************************************************************/

#define _SMALLOC_C

#ifdef OS_WIN32
#include <windows.h>
#else
#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#endif

#include <stdlib.h>
#include "smalloc.h"

/*
    FUNCTION:        CK_VOID_PTR smalloc(CK_ULONG uSize)
*/
CK_VOID_PTR smalloc(CK_ULONG uSize)
{
  CK_VOID_PTR sPtr = NULL;

  if (uSize == 0)
  {
    return NULL;
  }

#ifdef OS_WIN32
  sPtr = VirtualAlloc(NULL, uSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (sPtr != NULL)
  {
    if (!VirtualLock(sPtr, uSize))
    {
      VirtualFree(sPtr, 0, MEM_RELEASE);
      return NULL;
    }
  }
#else
  sPtr = mmap(NULL, uSize, PROT_READ | PROT_WRITE,
    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  if (sPtr == MAP_FAILED)
  {
    return NULL;
  }

  if (mlock(sPtr, uSize) != 0)
  {
    munmap(sPtr, uSize);
    return NULL;
  }
#endif

  return sPtr;
}

/*
    FUNCTION:        CK_VOID sec_free(CK_VOID_PTR sPtr, CK_ULONG uSize)
*/
void sfree(CK_VOID_PTR sPtr, CK_ULONG uSize)
{
  if (sPtr == NULL || uSize == 0)
  {
    return;
  }

#ifdef OS_WIN32
  SecureZeroMemory(sPtr, uSize);
  VirtualUnlock(sPtr, uSize);
  VirtualFree(sPtr, 0, MEM_RELEASE);
#else
  /* Use explicit_bzero to ensure the compiler doesn't optimize out the wipe */
  explicit_bzero(sPtr, uSize);
  munlock(sPtr, uSize);
  munmap(sPtr, uSize);
#endif
}