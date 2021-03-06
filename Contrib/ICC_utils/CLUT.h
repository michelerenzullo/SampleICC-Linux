/*
  File:       CLUT.cpp

  Contains:   originally part of iccCreateCLUTProfile command-line tool:
  create and write CLUT tag data

  Version:    V1

  Copyright:  ? see below
*/

/*
 * The ICC Software License, Version 0.2
 *
 *
 * Copyright (c) 2003-2010 The International Color Consortium. All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. In the absence of prior written permission, the names "ICC" and "The
 *    International Color Consortium" must not be used to imply that the
 *    ICC organization endorses or promotes products derived from this
 *    software.
 *
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE INTERNATIONAL COLOR CONSORTIUM OR
 * ITS CONTRIBUTING MEMBERS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the The International Color Consortium. 
 *
 *
 * Membership in the ICC is encouraged when this software is used for
 * commercial purposes. 
 *
 *  
 * For more information on The International Color Consortium, please
 * see <http://www.color.org/>.
 *  
 * 
 */

////////////////////////////////////////////////////////////////////// 
// HISTORY:
//
// -Initial implementation by Joseph Goldstone spring 2006
//
//////////////////////////////////////////////////////////////////////

#ifndef __DEFINED_CLUT_H__
#define __DEFINED_CLUT_H__


#include <string>
#include <cstdlib>

#include "IccUtil.h"
#include "IccTagLut.h"

#include "CAT.h"

class CLUT
{
public:

  CLUT() {}
 
  void
  loadInputShaperLUTs(CIccTagCurve** inputShaperLUTs,
                      const std::string& inputShaperFilename) const;
  
  CIccTagLut16*
  makeAToBxTag(const unsigned int edgeN,
               const icFloatNumber* const rawXYZ,
               const icFloatNumber* const flare,
               const icFloatNumber* const illuminant,
               const CAT* const CATToPCS,
               const icFloatNumber inputShaperGamma,
               const std::string& inputShaperFilename,
               const icFloatNumber* const mediaWhite,
               const bool LABPCS);
  
  static
  void
  measuredXYZToAdaptedXYZ(icFloatNumber* const adaptedXYZ,
                          const icFloatNumber* const measuredXYZ,
                          const icFloatNumber* const flare,
                          const icFloatNumber illuminantY,
                          const CAT* CATToPCS);
  
  static
  void
  adaptedXYZToMeasuredXYZ(icFloatNumber* const measuredXYZ,
                          const icFloatNumber* const adaptedXYZ,
                          const icFloatNumber* const flare,
                          const icFloatNumber illuminantY,
                          const CAT* invCATToPCS);
  
  void
  Iterate(IIccCLUTExec* pExec);
  
private:
  CIccCLUT* m_innerCLUT;
};

#endif
