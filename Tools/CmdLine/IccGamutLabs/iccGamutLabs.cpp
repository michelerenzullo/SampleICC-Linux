/*
    File:       iccGamutLabs.cpp

    Contains:   Console app to output a sequense of Lab values inside
		the gamut as defined by the gamt tag.

    Version:    V1

    Copyright:  © see below
*/

/*
 * The ICC Software License, Version 0.2
 *
 *
 * Copyright (c) 2003-2015 The International Color Consortium. All rights 
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
// -Initial implementation by Max Derhak 4-8-2015
//
//////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <math.h>
#include "IccCmm.h"

int main(int argc, char* argv[])
{
  int nArg = 1;

  if (argc<=7) {
    printf("Usage: iccGamutLabs profile rel-abs_flag num_hue hue_start chroma_inc lightness_inc lightness_start\n");
    printf("  where\n");
    printf("   rel-abs_flag determines if relative (=0) or absolute (=1) Lab values should be output\n");
	  printf("   num_hue is number of equal spaced hue steps\n");
	  printf("   hue_start is the starting hue value\n");
	  printf("   chroma_inc is spacing between chroma values\n");
	  printf("   lightness_inc is spacing between lightness values\n");
	  printf("   lightness_start is the starting lightness value\n");
    return -1;
  }

  bool bAbsolute = atoi(argv[2])!=0;
  int nHueSteps = atoi(argv[3]);
	icFloatNumber hueStart = atof(argv[4]);
	icFloatNumber chromaInc = atof(argv[5]);
	icFloatNumber lightnessInc = atof(argv[6]);
	icFloatNumber lightnessStart = atof(argv[7]);

  CIccProfile *pProfile = OpenIccProfile(argv[1]);

  if (!pProfile) {
    printf("Unable to open '%s'\n", argv[1]);
    return -2;
  }

  icFloatNumber mediaXYZ[3], D50XYZ[3], Lab[3], XYZ[3];
  if (bAbsolute) {
    CIccTag *pTag= pProfile->FindTag(icSigMediaWhitePointTag);
    if (pTag && pTag->GetType()==icSigXYZType) {
      CIccTagXYZ *pWhitePtTag = (CIccTagXYZ*)pTag;
      icXYZNumber *mxyz = pWhitePtTag->GetXYZ(0);
      mediaXYZ[0] = icFtoD(mxyz->X);
      mediaXYZ[1] = icFtoD(mxyz->Y);
      mediaXYZ[2] = icFtoD(mxyz->Z);

      Lab[0] = 100;
      Lab[1] = 0;
      Lab[2] = 0;

      icLabtoXYZ(D50XYZ, Lab);
    }
    else {
      printf("Unable to find/use Media White point tag for Absolute processing\n");
      return -3;
    }
  }

  CIccCmm cmm(icSigLabData, icSigGamutData, false);
  if (cmm.AddXform(pProfile, icRelativeColorimetric, icInterpTetrahedral, icXformLutGamut, false)!=icCmmStatOk) {
    printf("Unable to add '%s' to cmm\n", argv[1]);
    return -4;
  }
  if (cmm.Begin()!=icCmmStatOk) {
    printf("Unable to begin profile apply\n");
    return -5;
  }
  
  icFloatNumber pixel[16];

  icFloatNumber hue, hueStep;
  icFloatNumber Lval, Cval;
  icFloatNumber L, a, b;

  hueStep = 360.0f /(icFloatNumber)nHueSteps;

  printf("L*\ta*\tb*\n");
  //Add achromatic first
  for (Lval = lightnessStart; Lval<=100.0; Lval+=lightnessInc) {
    pixel[0] = Lval;
    pixel[1] = 0.0;
    pixel[2] = 0.0;
    icLch2Lab(pixel);
    if (!bAbsolute) {
      L=pixel[0];
      a=pixel[1];
      b=pixel[2];
    }
    else {
      icLabtoXYZ(XYZ, pixel);
      XYZ[0] *= mediaXYZ[0] / D50XYZ[0];
      XYZ[1] *= mediaXYZ[1] / D50XYZ[1];
      XYZ[2] *= mediaXYZ[2] / D50XYZ[2];
      icXYZtoLab(Lab, XYZ);
      L=Lab[0];
      a=Lab[1];
      b=Lab[2];
    }
    icLabToPcs(pixel);

    cmm.Apply(pixel,pixel);

    if (pixel[0]<0.01) {
      printf("%.2f\t%.2f\t%.2f\n", L, a, b);
    }
  }
  //Now add chromatic points
  for (hue=0.0; hue<360.0; hue += hueStep) {
    for (Lval = lightnessStart; Lval<=100.0; Lval+=lightnessInc) {
		  for (Cval = chromaInc; Cval<181; Cval += chromaInc) {
				pixel[0] = Lval;
				pixel[1] = Cval;
				pixel[2] = hue+hueStart;
				icLch2Lab(pixel);
        if (!bAbsolute) {
          L=pixel[0];
          a=pixel[1];
          b=pixel[2];
        }
        else {
          icLabtoXYZ(XYZ, pixel);
          XYZ[0] *= mediaXYZ[0] / D50XYZ[0];
          XYZ[1] *= mediaXYZ[1] / D50XYZ[1];
          XYZ[2] *= mediaXYZ[2] / D50XYZ[2];
          icXYZtoLab(Lab, XYZ);
          L=Lab[0];
          a=Lab[1];
          b=Lab[2];
        }
				icLabToPcs(pixel);

				cmm.Apply(pixel,pixel);

				if (pixel[0]<0.01) {
				  printf("%.2f\t%.2f\t%.2f\n", L, a, b);
				}
			}
    }
  }

  return 0;
}

