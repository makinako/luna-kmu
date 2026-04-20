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

#ifndef _SMALLOC_H_
#define _SMALLOC_H_

#include "p11.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _SMALLOC_C
#define _EXT
#else
#define _EXT extern
#endif

  _EXT  CK_VOID_PTR          smalloc(CK_ULONG uSize);
  _EXT  void                 sfree(CK_VOID_PTR sPtr, CK_ULONG uSize);

#undef _EXT

#ifdef __cplusplus
}
#endif

#endif // _SMALLOC_H_