/*
  Last changed Time-stamp: <2006-02-10 22:53:06 xtof>
  $Id: PSplot.cpp,v 1.1.1.1 2006/02/15 10:15:53 xtof Exp $
*/
#include <cmath>
#include <string>

#include "Utils.h"
#include "maxmatch.h"
#include "options.h"
extern "C" {
#include "utils.h"
}
  
// private function(s)
static std::string
CreateRGBString(double color_depth);
static std::string
PSTail(void);
static std::string
PSHead(std::string sequence, std::string outPath);

/**
 * Code to create a .ps dotplot template. Based on PS_dot.c
 */
void
PSColorPlot(std::string sequence, std::string outPath, double max_ratio)
{
  std::string sText;

  sText += PSHead(sequence, outPath);

  // for 3x2 matrix translation coordinates are
  // 55 575    320 575
  // 55 315    320 315
  // 55  50    320  55
  // add 2 div before dup in scale
  
  sText	+= " 72 216 translate\n";
  sText	+= " 72 6 mul len 1 add div dup scale\n";

  sText	+= " /Helvetica findfont 0.95 scalefont setfont\n";
  sText	+= " drawseq\n";
  sText	+= " 0.5 dup translate\n";
  sText	+= "% draw diagonal\n";
  sText	+= " 0.04 setlinewidth\n";
  sText	+= " 0 len moveto len 0 lineto stroke \n";
  sText	+= " drawgrid\n";
  sText	+= "% data starts here; x y size rgb-array\n";
  
  // Add the actual entries and their colours for the upper half of the matrix
  int l = sequence.size();
  for (int i=1; i<=l; i++) {
    for (int j=i; j<=l; j++) {
      double color_depth = mm_get_ratio(i,j)/max_ratio;
  
      // Round to two decimals
      color_depth = .01 * (int)floor(color_depth * 100 + .5);
      std::string rgb = CreateRGBString(color_depth);
      sText += " " + Str(j)
	+ " " + Str(i) + " " + Str(1) + " " + rgb + " " + "mylbox\n";
    }
  }
  
  // Draw a colour scale in the lower half of the matrix 
  for (int i=0; i<10; i++) {
    std::string rgb = CreateRGBString(.1 * i);
    sText += " " + Str(i+1) 
      + " " + Str(l-1) + " " + Str(1) + " "+rgb + " " + "mylbox\n";
  }

  sText += PSTail(); 

  StringToTextFile(sText, outPath);
}

/**
 * Colouring for the PS dotplot. blue always 1.0, red+green=1
 */
static std::string
CreateRGBString(double color_depth) {
  float red   = color_depth;
  float green = 1.0-color_depth;
  float blue  = 1.0;
  std::string rgb = "["+Str(red)+" "+Str(green)+" "+Str(blue)+"]";
  return (rgb);
}

static std::string
PSHead(std::string sequence, std::string outPath)
{
  std::string sText;
  std::string timestamp(time_stamp());
  std::string options(optionString());

  sText	+= "%!PS-Adobe-3.0 EPSF-3.0\n";
  sText	+= "%%Creator: kinwalker\n";
  sText	+= "%%CreationDate: "+timestamp;
  sText	+= "%%Title: Maximum Matching Color Plot\n";
  sText	+= "%%BoundingBox: 65 211 510 655\n";
  sText	+= "%%DocumentFonts: Helvetica\n";
  sText	+= "%%Pages: 1\n";
  sText	+= "%%EndComments\n";
  sText += "%Options: "+options+"\n";
  sText	+= "%%BeginProlog\n";
  sText	+= "/DPdict 100 dict def\n";
  sText	+= "DPdict begin\n";
  sText	+= " /logscale false def\n";
  sText	+= " /mylbox { % x y size [rgb] => -\n";
  sText	+= "  exch 4 2 roll\n";
  sText	+= "  len exch sub 1 add mybox\n";
  sText	+= " } bind def\n";
  sText	+= " /mybox { % [rgb] size x y box - draws box centered on x,y\n";
  sText	+= "  2 index 0.5 mul add            % x += 0.5\n";
  sText	+= "  exch 2 index 0.5 mul add exch  % x += 0.5\n";
  sText	+= "  newpath\n";
  sText	+= "  moveto\n";
  sText	+= "  dup neg 0 rlineto\n";
  sText	+= "  dup neg 0 exch rlineto\n";
  sText	+= "  0 rlineto\n";
  sText	+= "  closepath\n";
  sText	+= "  gsave\n";
  sText	+= "  aload pop\n";
  sText	+= "  setrgbcolor\n";
  sText	+= "  fill\n";
  sText	+= "  grestore\n";
  sText	+= " } bind def\n";
  sText	+= " /drawseq {\n";
  sText	+= "% print sequence along all 4 sides\n";
  sText	+= " [[0.7 -0.3 0 ]\n";
  sText	+= "  [0.7 0.7 len add 0]\n";
  sText	+= "  [-0.3 len sub -0.4 -90]\n";
  sText	+= "  [-0.3 len sub 0.7 len add -90]\n";
  sText	+= " ]{\n";
  sText	+= "   gsave\n";
  sText	+= "    aload pop rotate translate\n";
  sText	+= "    0 1 len 1 sub {\n";
  sText	+= "     dup 0 moveto\n";
  sText	+= "     sequence exch 1 getinterval\n";
  sText	+= "     show\n";
  sText	+= "    } for\n";
  sText	+= "   grestore\n";
  sText	+= "  } forall\n";
  sText	+= " } bind def\n";
  sText	+= " /drawgrid{\n";
  sText	+= "  0.01 setlinewidth\n";
  sText	+= "  len log 0.9 sub cvi 10 exch exp  % grid spacing\n";
  sText	+= "  dup 1 gt {\n";
  sText	+= "   dup dup 20 div dup 2 array astore exch 40 div setdash\n";
  sText	+= "  } { [0.3 0.7] 0.1 setdash } ifelse\n";
  sText	+= "  0 exch len {\n";
  sText	+= "   dup dup\n";
  sText	+= "   0 moveto\n";
  sText	+= "   len lineto \n";
  sText	+= "   dup\n";
  sText	+= "   len exch sub 0 exch moveto\n";
  sText	+= "   len exch len exch sub lineto\n";
  sText	+= "   stroke\n";
  sText	+= "  } for\n";
  sText	+= "  [] 0 setdash\n";
  sText	+= "  0.04 setlinewidth \n";
  sText	+= "  currentdict /cutpoint known {\n";
  sText	+= "   cutpoint 1 sub\n";
  sText	+= "   dup dup -1 moveto len 1 add lineto\n";
  sText	+= "   len exch sub dup\n";
  sText	+= "   -1 exch moveto len 1 add exch lineto\n";
  sText	+= "   stroke\n";
  sText	+= "  } if\n";
  sText	+= "  0.5 neg dup translate\n";
  sText	+= " } bind def\n";
  sText	+= "end\n";
  sText	+= "%%EndProlog\n";
  sText	+= "DPdict begin\n";
  sText	+= "% delete next line to get rid of title\n";
  sText	+= " 270 665 moveto /Helvetica findfont 14 scalefont ";
  sText	+= " setfont ("+outPath+") show\n";
  sText	+= " /sequence { ("+sequence+") } def\n";
  sText	+= " /len { sequence length } bind def\n";

  return (sText);
}  

static std::string
PSTail(void)
{
  std::string sText;
  sText += " showpage\n";
  sText += "end\n";
  sText += "%%EOF\n";

  return (sText);
}

// End of file
