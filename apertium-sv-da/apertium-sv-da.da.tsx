<?xml version="1.0" encoding="iso-8859-1"?>
<!DOCTYPE tagger SYSTEM "../tagger.dtd">
<tagger name="danes">
<tagset>
   <def-label name="PREP" closed="true">
    <tags-item tags="pr"/>
  </def-label>
  <def-label name="PRNTONIC" closed="true">
    <tags-item tags="prn.tn.*"/>
  </def-label>
  <def-label name="PRNOBJ" closed="true">
    <tags-item tags="prn.obj.*"/>
  </def-label>
  <def-label name="PRNPOS" closed="true">
    <tags-item tags="prn.pos.*"/>
  </def-label>
  <def-label name="PRNREF" closed="true">
    <tags-item tags="prn.ref.*"/>
  </def-label>
  <def-label name="PRN" closed="true">
    <tags-item tags="prn.*"/>
  </def-label>
  <def-label name="ONLYPRN" closed="true">
    <tags-item tags="prn"/>
  </def-label> 
  <def-label name="NUM" closed="true">
    <tags-item tags="num"/>
  </def-label>
    <def-label name="CNJCOO" closed="true">
    <tags-item tags="cnjcoo"/>
  </def-label>
  <def-label name="CNJSUB" closed="true">
    <tags-item tags="cnjsub"/>
  </def-label>
  <def-label name="DETDEM" closed="true">
    <tags-item tags="det.dem.*"/>
  </def-label>
  <def-label name="DETIND" closed="true">
    <tags-item tags="det.ind.*"/>
  </def-label>
  <def-label name="DETDEF" closed="true">
    <tags-item tags="det.def.*"/>
  </def-label> 
  <def-label name="DETPOS" closed="true">
    <tags-item tags="det.pos.*"/>
  </def-label>
  <def-label name="INTERJECCIONS" closed="true">
    <tags-item tags="ij"/>
  </def-label>
  <def-label name="REL" closed="true">
    <tags-item tags="rel"/>
  </def-label>
  <def-label name="NOMSPROPIS">
    <tags-item tags="np.*"/>
  </def-label>
  <def-label name="NOMSNEUTRE">
    <tags-item tags="n.nt.*"/>
  </def-label>
  <def-label name="NOMSUTRE">
    <tags-item tags="n.ut.*"/>
  </def-label>
  <def-label name="ADV">
    <tags-item tags="adv.*"/>
  </def-label> 
  <def-label name="ONLYADV">
    <tags-item tags="adv"/>
  </def-label>
  <def-label name="CONJADV">
    <tags-item tags="cnjadv"/>
  </def-label>
  <def-label name="ADJ">
    <tags-item tags="adj.*"/>
  </def-label>
 <def-label name="VBHARINF" closed="true">
    <tags-item tags="vbhar.inf"/>
  </def-label>
  <def-label name="VBHARPRS" closed="true">
    <tags-item tags="vbhar.prs"/>
  </def-label>
  <def-label name="VBHARDA" closed="true">
    <tags-item tags="vbhar.da"/>
  </def-label>
  <def-label name="VBHARPRET" closed="true">
    <tags-item tags="vbhar.part"/>
  </def-label>
  <def-label name="VBERINF" closed="true">
    <tags-item tags="vber.inf"/>
  </def-label>
  <def-label name="VBERPRS" closed="true">
    <tags-item tags="vber.prs"/>
  </def-label>
  <def-label name="VBERDA" closed="true">
    <tags-item tags="vber.da"/>
  </def-label>
  <def-label name="VBERPRET" closed="true">
    <tags-item tags="vber.part"/>
  </def-label>  
   <def-label name="VBMODPRS" closed="true">
    <tags-item tags="vbmod.prs"/>
  </def-label>
  <def-label name="VBMODDA" closed="true">
    <tags-item tags="vbmod.da"/>
  </def-label>
  <def-label name="VBMODPRET" closed="true">
    <tags-item tags="vbmod.part"/>
  </def-label>
  <def-label name="VBMODINF" closed="true">
    <tags-item tags="vbmod.inf"/>
  </def-label>
   <def-label name="VBAUXPRS" closed="true">
    <tags-item tags="vbaux.prs"/>
  </def-label>
  <def-label name="VBAUXDA" closed="true">
    <tags-item tags="vbaux.da"/>
  </def-label>
  <def-label name="VBAUXINF" closed="true">
    <tags-item tags="vbaux.inf"/>
  </def-label>
  <def-label name="VBAUXPRET" closed="true">
    <tags-item tags="vbaux.part"/>
  </def-label>
   <def-label name="VBSPRS" closed="true">
    <tags-item tags="vbs.prs"/>
  </def-label>
  <def-label name="VBSDA" closed="true">
    <tags-item tags="vbs.da"/>
  </def-label>
  <def-label name="VBSPRET" closed="true">
    <tags-item tags="vbs.part"/>
  </def-label>  
  <def-label name="VBSINF" closed="true">
    <tags-item tags="vbs.inf"/>
  </def-label>  
   <def-label name="VBLEXPRS" closed="true">
    <tags-item tags="vblex.prs"/>
  </def-label>  
  <def-label name="VBLEXPRSGEN" closed="true">
    <tags-item tags="vblex.prs.gen"/>
  </def-label>
  <def-label name="VBLEXDA" closed="true">
    <tags-item tags="vblex.da"/>
  </def-label>
  <def-label name="VBLEXDAGEN" closed="true">
    <tags-item tags="vblex.da.gen"/>
  </def-label>
  <def-label name="VBLEXPART" closed="true">
    <tags-item tags="vblex.part"/>
  </def-label>
  <def-label name="VBLEXINF" closed="true">
    <tags-item tags="vblex.inf"/>
  </def-label> 
  <def-label name="VBLEXPARTGEN" closed="true">
    <tags-item tags="vblex.part.gen"/>
  </def-label>
  <def-label name="VBLEXINFGEN" closed="true">
    <tags-item tags="vblex.inf.gen"/>
  </def-label> 
  <def-label name="VBLEXIMP" closed="true">
    <tags-item tags="vblex.imp"/>
  </def-label>
</tagset>

  <forbid>
    <label-sequence>
      <label-item label="NOMNEUTRE"/>
      <label-item label="NOMNEUTRE"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMNEUTRE"/>
      <label-item label="NOMUTRE"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMUTRE"/>
      <label-item label="NOMUTRE"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMUTRE"/>
      <label-item label="NOMNEUTRE"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMNEUTRE"/>
      <label-item label="PRNOBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMUTRE"/>
      <label-item label="PRNOBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMNEUTRE"/>
      <label-item label="PRNTONIC"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMUTRE"/>
      <label-item label="PRNTONIC"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMNEUTRE"/>
      <label-item label="DETPOS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMUTRE"/>
      <label-item label="PRNPOS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="NOMNEUTRE"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="NOMUTRE"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="PREP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="PRNTONIC"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="DETPOS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="REL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="DETDEF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="PRNREF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="CNJSUB"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="CNJADV"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="DETIND"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="IJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNTONIC"/>
      <label-item label="ADJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJ"/>
      <label-item label="PRNOBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJ"/>
      <label-item label="CNJSUB"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJ"/>
      <label-item label="DETDEM"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJ"/>
      <label-item label="DETIND"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJ"/>
      <label-item label="DETDEF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJ"/>
      <label-item label="DETPOS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJ"/>
      <label-item label="REL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADV"/>
      <label-item label="PRNTN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADV"/>
      <label-item label="PRNOBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADV"/>
      <label-item label="IJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADV"/>
      <label-item label="PRNREF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PR"/>
      <label-item label="PRNTN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PR"/>
      <label-item label="CNJCOO"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PR"/>
      <label-item label="IJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PR"/>
      <label-item label="VBAUX"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PR"/>
      <label-item label="VBS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PR"/>
      <label-item label="VBER"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PR"/>
      <label-item label="VBHAR"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PR"/>
      <label-item label="VBLEX"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PR"/>
      <label-item label="VBMOD"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="NP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="PR"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="ADV"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="DETDEF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="PRNOBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="PRNREF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="PRN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="CNJCOO"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="CNJADV"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="CNJSUB"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="REL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="IJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="DETPOS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="DETIND"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEF"/>
      <label-item label="DETDEM"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNOBJ"/>
      <label-item label="VBAUX"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNOBJ"/>
      <label-item label="VBS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNOBJ"/>
      <label-item label="VBMOD"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNOBJ"/>
      <label-item label="VBER"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNOBJ"/>
      <label-item label="VBHAR"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNOBJ"/>
      <label-item label="VBLEX"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNREF"/>
      <label-item label="ADJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNREF"/>
      <label-item label="VBAUX"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNREF"/>
      <label-item label="VBS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNREF"/>
      <label-item label="VBMOD"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNREF"/>
      <label-item label="VBER"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNREF"/>
      <label-item label="VBHAR"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRNREF"/>
      <label-item label="VBLEX"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="NP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="PR"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="ADV"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="DETDEF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="PRNOBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="PRNREF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="PRN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="CNJCOO"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="CNJADV"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="CNJSUB"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="REL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="IJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="DETPOS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEM"/>
      <label-item label="DETIND"/>
    </label-sequence>    
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="NP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="PR"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="ADV"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="DETDEF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="PRNOBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="PRNREF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="PRN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="CNJCOO"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="CNJADV"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="CNJSUB"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="REL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="IJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETIND"/>
      <label-item label="DETPOS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBLEXDA"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBLEXPRS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBMODPRS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBMODDA"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBMODPRET"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBAUXPRS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBAUXDA"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBAUXPRET"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBERPRS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBERDA"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBERPRET"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBHARPRS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBHARDA"/>
    </label-sequence>
     <label-sequence>
      <label-item label="VBAUX"/>
      <label-item label="VBHARPRET"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="PRNTONIC"/>
    </label-sequence>
     <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBLEXPRS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBLEXDA"/>
    </label-sequence>
     <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBLEXPRET"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBMODPRS"/>
    </label-sequence>
     <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBMODDA"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBMODPRET"/>
    </label-sequence>
     <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBAUXPRS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBAUXDA"/>
    </label-sequence>
     <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBAUXPRET"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBSPRS"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBSDA"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBSPRET"/>
    </label-sequence>	
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBERPRS"/>
    </label-sequence>	
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBERDA"/>
    </label-sequence>	
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBERRPET"/>
    </label-sequence>	
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBHARPRS"/>
    </label-sequence>	
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBHARDA"/>
    </label-sequence>	
    <label-sequence>
      <label-item label="VBLEX"/>
      <label-item label="VBHARPRET"/>
    </label-sequence>	
    	
    </forbid>


</tagger>
