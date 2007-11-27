<?xml version="1.0" encoding="UTF-8"?>
<tagger name="romanes">
<tagset>
  <def-label name="INTNOM" closed="true">
    <tags-item tags="prn.itg.*"/>
  </def-label> 
  <def-label name="INTADV" closed="true">
    <tags-item tags="adv.itg"/>
  </def-label>
  <def-label name="PREADV" closed="true">
    <tags-item tags="preadv"/>
  </def-label>


  <def-label name="SACNJSUB" closed="true">
    <tags-item lemma="sä" tags="cnjsub"/>  
  </def-label><!--acompaña sempre subjuntiu -->

<def-label name="MAIADV" closed="true">     
    <tags-item lemma="mai" tags="adv"/>
  </def-label>
 
  <def-label name="ADV">
    <tags-item tags="adv"/>
  </def-label>
  

  <def-label name="CNJSUBS" closed="true">
    <tags-item tags="cnjsub"/>
  </def-label>
  <def-label name="CNJCOORD" closed="true">
    <tags-item tags="cnjcoo"/>
  </def-label>
  <def-label name="CNJADV">
    <tags-item tags="cnjadv"/>  
  </def-label>


  <def-label name="DETDEFM" closed="true">
    <tags-item tags="det.def.m.*"/>
  </def-label>
  <def-label name="DETDEFMN" closed="true">
    <tags-item  tags="det.def.mn.*"/>
  </def-label>
  <def-label name="DETDEFF" closed="true">
    <tags-item  tags="det.def.f.*"/>
  </def-label>
  <def-label name="DETDEFFN" closed="true">
    <tags-item  tags="det.def.fn.*"/>
  </def-label>
  


  <def-label name="DETM" closed="true">
    <tags-item tags="det.*.m.*"/>
  </def-label>
  <def-label name="DETMN" closed="true">
    <tags-item tags="det.*.mn.*"/>
  </def-label>
  <def-label name="DETF" closed="true">
    <tags-item tags="det.*.f.*"/>
  </def-label>
  <def-label name="DETFN" closed="true">
    <tags-item tags="det.*.fn.*"/>
  </def-label>
  <def-label name="DETMF" closed="true">
    <tags-item tags="det.*.mf.*"/>
  </def-label>
  <def-label name="DETMFN" closed="true">
    <tags-item tags="det.*.mfn.*"/>
  </def-label>
  <def-label name="INTERJ">
    <tags-item tags="ij"/>
  </def-label>
 <def-label name="ACRONIMON">
    <tags-item tags="n.acr.*"/>
  </def-label>
 
<!--  <def-label name="NOMF">
    <tags-item tags="n.f.*"/>
  </def-label>
  <def-label name="NOMM">
    <tags-item tags="n.m.*"/>
  </def-label>
  <def-label name="NOMN">
    <tags-item tags="n.nt.*"/>
  </def-label> -->

 <def-label name="ANTROPONIM">
    <tags-item tags="np.ant.*"/>
  </def-label>

 <def-label name="NOMNOMSG">
    <tags-item tags="n.*.sg.nom.*"/>
    <tags-item tags="n.*.sg.nom"/>
    <tags-item tags="np.*.sg"/>
    <tags-item tags="np.*.sg.nom.*"/>
    <tags-item tags="np.loc.*.sg"/>
    <tags-item tags="n.*.nom.*.sg"/>
  </def-label>

 <def-label name="NOMDGSG">
    <tags-item tags="n.*.sg.dg.*"/>
    <tags-item tags="n.*.sg.dg"/>
    <tags-item tags="np.*.sg"/>
    <tags-item tags="np.*.sg.dg.*"/>
    <tags-item tags="n.*.dg.*.sg"/>
  </def-label>

<def-label name="NOMNOMPL">
    <tags-item tags="n.*.pl.nom.*"/>
    <tags-item tags="n.*.pl.nom"/>
    <tags-item tags="np.*.pl"/>
    <tags-item tags="np.*.pl.nom.*"/>
    <tags-item tags="np.loc.*.pl"/>
    <tags-item tags="n.*.nom.*.pl"/>
  </def-label>

 <def-label name="NOMDGPL">
    <tags-item tags="n.*.pl.dg.*"/>
    <tags-item tags="n.*.pl.dg"/>
    <tags-item tags="np.*.pl"/>
    <tags-item tags="np.*.pl.dg.*"/>
    <tags-item tags="n.*.dg.*.pl"/>
  </def-label>

<def-label name="NOMNOMSP">
    <tags-item tags="n.*.sp.nom.*"/>
    <tags-item tags="n.*.sp.nom"/>
    <tags-item tags="np.*.sp.nom.*"/>
    <tags-item tags="np.loc.*.sp"/>
    <tags-item tags="n.*.nom.*.sp"/>
  </def-label>

 <def-label name="NOMDGSP">
    <tags-item tags="n.*.sp.dg.*"/>
    <tags-item tags="n.*.sp.dg"/>
    <tags-item tags="np.*.sp.dg.*"/>
    <tags-item tags="n.*.dg.*.sp"/>
  </def-label>

<!--  <def-label name="TOPONIM">
    <tags-item tags="np.loc"/>
  </def-label>
  <def-label name="NPALTRES">
    <tags-item tags="np.al"/>
  </def-label> -->


  <def-label name="NUM" closed="true">
    <tags-item tags="num.*"/>
    <tags-item tags="num"/>
  </def-label>
  
  <def-label name="PREDET" closed="true">
    <tags-item tags="predet.*"/>
  </def-label>  
 <!--<def-label name="PREPA" closed="true">
    <tags-item lemma="a" tags="pr"/>
  </def-label> --><!--Preguntar a Delia si fa falta. -->
  <def-label name="PREP" closed="true">
    <tags-item tags="pr"/>
  </def-label>
<!--  <def-label name="PRNTNNT" closed="true">
    <tags-item tags="prn.tn.nt"/>
  </def-label> -->
  <def-label name="PRNTN" closed="true">
    <tags-item tags="prn.tn.*"/>
  </def-label>
  <def-label name="PRNENCREF" closed="true">
    <tags-item tags="prn.enc.ref.*"/>
  </def-label>
  <def-label name="PRNPROREF" closed="true">
    <tags-item tags="prn.pro.ref.*"/>
  </def-label>
     <def-label name="PRNENC" closed="true">
    <tags-item tags="prn.enc.*"/>
  </def-label>
  <def-label name="PRNPRO" closed="true">
    <tags-item tags="prn.pro.*"/>
  </def-label>

  
  <def-label name="VLEXINF">
    <tags-item tags="vblex.inf"/>
  </def-label>
  <def-label name="VLEXGER">
    <tags-item tags="vblex.ger"/>
  </def-label>
  <def-label name="VLEXPARTPI">
    <tags-item tags="vblex.pp.*"/>
  </def-label>
  <def-label name="VLEXPFCI">
    <tags-item tags="vblex.pri.*"/>
    <tags-item tags="vblex.cni.*"/>
     <tags-item tags="vblex.fti.*"/>
  </def-label>
  <def-label name="VLEXIPI">
    <tags-item tags="vblex.pii.*"/>
    <tags-item tags="vblex.pper.*"/>
  </def-label>
  <def-label name="VLEXSUBJ">
    <tags-item tags="vblex.prs.*"/>
  </def-label>
   <def-label name="VLEXIMP">
    <tags-item tags="vblex.imp.*"/>
  </def-label>

 
  <def-label name="VAUX" closed="true">
    <tags-item tags="vaux.*"/>
  </def-label> 
 

  <def-label name="VAVEAPFCI" closed="true">
    <tags-item tags="vbavea.pri.*"/>
    <tags-item tags="vbavea.cni.*"/>
  </def-label>




<!--    <def-label name="AVEALEX" closed="true">
    <tags-item lemma="avea" tags="vblex.*"/>
  </def-label> -->

<!--  <def-label name="VREALEX" closed="true">
    <tags-item lemma="vrea" tags="vblex.*"/>
  </def-label>

    <def-label name="VREAMOD" closed="true">
    <tags-item lemma="vrea" tags="vbmod.*"/>
  </def-label> -->


  
  <def-label name="VMODALINF" closed="true">
    <tags-item tags="vbmod.inf"/>
  </def-label>
  <def-label name="VMODALGER" closed="true">
    <tags-item tags="vbmod.ger"/>
  </def-label>
  <def-label name="VMODALPARTPI" closed="true">
    <tags-item tags="vbmod.pp.*"/>
  </def-label>
  <def-label name="VMODALPFCI" closed="true">
    <tags-item tags="vbmod.pri.*"/>
  </def-label>
  <def-label name="VMODALIPI" closed="true">
    <tags-item tags="vbmod.pii.*"/>
    <tags-item tags="vbmod.pper.*"/>
  </def-label>
  <def-label name="VMODALSUBJ" closed="true">
    <tags-item tags="vbmod.prs.*"/>
  </def-label>
  <def-label name="VMODALIMP" closed="true">
    <tags-item tags="vbmod.imp.*"/>
  </def-label>
  


  <!--  <def-label name="ADJF">           PENSAR SI POSEM GÈNERES, TAMBÉ HO HEM DE FER EN ELS NOMS
    <tags-item tags="adj.f.*"/>
    <tags-item tags="adj.ind.f.*"/>
  </def-label>
  <def-label name="ADJM">
    <tags-item tags="adj.m.*"/>    
    <tags-item tags="adj.ind.m.*"/>
  </def-label>
  <def-label name="ADJMF">
    <tags-item tags="adj.mf.*"/>    
    <tags-item tags="adj.ind.mf.*"/>
  </def-label>
  
   <def-label name="ADJFN">
    <tags-item tags="adj.fn.*"/>
    <tags-item tags="adj.ind.fn.*"/>
  </def-label>
  <def-label name="ADJMFN">
    <tags-item tags="adj.mfn.*"/>    
    <tags-item tags="adj.ind.m.*"/>
  </def-label>
  <def-label name="ADJMN">
    <tags-item tags="adj.mn.*"/>    
    <tags-item tags="adj.ind.mn.*"/>
  </def-label> -->



<def-label name="ADJNOMSG"> 
    <tags-item tags="adj.*.sg.nom.*"/>
    <tags-item tags="adj.*.sg.nom"/>
  </def-label>
  <def-label name="ADJNOMPL">
    <tags-item tags="adj.*.pl.nom.*"/> 
    <tags-item tags="adj.*.pl.nom"/> 
  </def-label>
  <def-label name="ADJDGSG">
    <tags-item tags="adj.*.sg.dg.*"/> 
    <tags-item tags="adj.*.sg.dg"/>  
  </def-label>
  
   <def-label name="ADJDGPL">
    <tags-item tags="adj.*.pl.dg.*"/>
    <tags-item tags="adj.*.pl.dg"/>
  </def-label>



<!--  <def-label name="ADJINT" closed="true">
    <tags-item tags="adj.itg.*"/>
  </def-label> -->
  <def-label name="ADJPOS_NOMSG" closed="true">
    <tags-item tags="adj.pos.*.sg.nom"/>
  </def-label>
  <def-label name="ADJPOS_DGSG" closed="true">
    <tags-item tags="adj.pos.*.sg.dg"/>
  </def-label>
  <def-label name="ADJPOS_NOMPL" closed="true">
    <tags-item tags="adj.pos.*.pl.nom"/>
  </def-label>
  <def-label name="ADJPOS_DGPL" closed="true">
    <tags-item tags="adj.pos.*.pl.dg"/>
  </def-label>

  <def-label name="RELNOM" closed="true">
    <tags-item tags="rel.an.*.nom"/>
    <tags-item tags="rel.an.nom.*"/>
    <tags-item tags="rel.an.nt"/>
  </def-label>

 <def-label name="RELDG" closed="true">
    <tags-item tags="rel.an.dg.*"/>
  </def-label>


  <def-label name="RELNN" closed="true">  
    <tags-item tags="rel.nn.*"/>
  </def-label>
  
<!--  <def-label name="CAREREL" closed="true">
    <tags-item lemma="care" tags="rel.*"/>  
  </def-label> -->
  
  <def-label name="RELADV" closed="true">
    <tags-item tags="rel.adv"/>
  </def-label>

<!--Mireia: això ja no m'ho he mirat, caldria repassar-ho, també les prohibicions amb enclítics -->

  <def-mult name="GERLEXPRNENC">
    <sequence>
      <label-item label="VLEXGER"/>
      <label-item label="PRNENC"/>
   </sequence>
    <sequence>
      <label-item label="VLEXGER"/>
      <label-item label="PRNENC"/>
      <label-item label="PRNENC"/>
   </sequence>
  </def-mult>
  
  <def-mult name="PPLEXPRNENC">
    <sequence>
      <label-item label="VLEXPARTPI"/>
      <label-item label="PRNENC"/>
   </sequence> 
    </def-mult> 
  <def-mult name="IMPLEXPRNENC">
    <sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="PRNENC"/>
   </sequence>
    <sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="PRNENC"/>
      <label-item label="PRNENC"/>
   </sequence>
  </def-mult>

  <def-mult name="IMPAVEAPRNENC" closed="true">
    <sequence>
      <label-item label="VAVEAIMP"/>
      <label-item label="PRNENC"/>
    </sequence>
    <sequence>
      <label-item label="VAVEAIMP"/>
      <label-item label="PRNENC"/>
      <label-item label="PRNENC"/>
    </sequence>
  </def-mult>
  <def-mult name="GERAVEAPRNENC" closed="true">
    <sequence>
      <label-item label="VAVEAGER"/>
      <label-item label="PRNENC"/>
    </sequence>
    <sequence>
      <label-item label="VAVEAGER"/>
      <label-item label="PRNENC"/>
      <label-item label="PRNENC"/>
    </sequence>
  </def-mult>
  <def-mult name="GERMODPRNENC" closed="true">
    <sequence>
      <label-item label="VMODALGER"/>
      <label-item label="PRNENC"/>
    </sequence>
    <sequence>
      <label-item label="VMODALGER"/>
      <label-item label="PRNENC"/>
      <label-item label="PRNENC"/>
    </sequence>
  </def-mult>

</tagset>

  <forbid>

   <label-sequence>
      <label-item label="DETDEFM"/>
      <label-item label="PRNPRO"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEFF"/>
      <label-item label="PRNPRO"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEFMN"/>
      <label-item label="PRNPRO"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEFFN"/>
      <label-item label="PRNPRO"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEFM"/>
      <label-item label="PRNPROREF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEFF"/>
      <label-item label="PRNPROREF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEFMN"/>
      <label-item label="PRNPROREF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETDEFFN"/>
      <label-item label="PRNPROREF"/>
    </label-sequence>
   <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="ADJNOMSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="ADJNOMPL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="ADJDGSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="ADJDGPL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXPFCI"/>
      <label-item label="ADJNOMSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXPFCI"/>
      <label-item label="ADJNOMPL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXPFCI"/>
      <label-item label="ADJDGSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXPFCI"/>
      <label-item label="ADJDGPL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXINF"/>
      <label-item label="ADJNOMSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXINF"/>
      <label-item label="ADJNOMPL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXINF"/>
      <label-item label="ADJDGSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXINF"/>
      <label-item label="ADJDGPL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIPI"/>
      <label-item label="ADJNOMSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIPI"/>
      <label-item label="ADJNOMPL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIPI"/>
      <label-item label="ADJDGSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIPI"/>
      <label-item label="ADJDGPL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXGER"/>
      <label-item label="ADJNOMSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXGER"/>
      <label-item label="ADJNOMPL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXGER"/>
      <label-item label="ADJDGSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXGER"/>
      <label-item label="ADJDGPL"/>
    </label-sequence> 
     <label-sequence>
      <label-item label="VLEXPARTPI"/>
      <label-item label="ADJNOMSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXPARTPI"/>
      <label-item label="ADJNOMPL"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXPARTPI"/>
      <label-item label="ADJDGSG"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXPARTPI"/>
      <label-item label="ADJDGPL"/>
    </label-sequence>
    
   <label-sequence>
      <label-item label="NOMDGSG"/>
      <label-item label="NOMNOMSG"/>
    </label-sequence>
 <label-sequence>
      <label-item label="NOMDGPL"/>
      <label-item label="NOMNOMPL"/>
    </label-sequence>

  
    <label-sequence>
      <label-item label="ANTROPONIM"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
<!--    <label-sequence>
      <label-item label="TOPONIM"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NPALTRES"/>
      <label-item label="VLEXIMP"/>
    </label-sequence> -->
      
    <label-sequence>
      <label-item label="ANTROPONIM"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
  <!--  <label-sequence>
      <label-item label="TOPONIM"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NPALTRES"/>
      <label-item label="VMODALIMP"/>
    </label-sequence> -->
    <label-sequence>
      <label-item label="ANTROPONIM"/>
      <label-item label="IMPLEXPRNENC"/>
    </label-sequence>
 <!--   <label-sequence>
      <label-item label="TOPONIM"/>
      <label-item label="IMPLEXPRNENC"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NPALTRES"/>
      <label-item label="IMPLEXPRNENC"/>
    </label-sequence> -->
    <label-sequence>
      <label-item label="ANTROPONIM"/>
      <label-item label="IMPMODPRNENC"/>
    </label-sequence>
  <!--  <label-sequence>
      <label-item label="TOPONIM"/>
      <label-item label="IMPMODPRNENC"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NPALTRES"/>
      <label-item label="IMPMODPRNENC"/>
    </label-sequence> -->
   
<!--    <label-sequence>
      <label-item label="ANTROPONIM"/>
      <label-item label="NOMM"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ANTROPONIM"/>
      <label-item label="NOMMN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="TOPONIM"/>
      <label-item label="NOMM"/>
    </label-sequence>
    <label-sequence>
      <label-item label="TOPONIM"/>
      <label-item label="NOMMN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NPALTRES"/>
      <label-item label="NOMM"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NPALTRES"/>
      <label-item label="NOMMN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ANTROPONIM"/>
      <label-item label="NOMF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ANTROPONIM"/>
      <label-item label="NOMFN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="TOPONIM"/>
      <label-item label="NOMF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="TOPONIM"/>
      <label-item label="NOMFN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NPALTRES"/>
      <label-item label="NOMF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NPALTRES"/>
      <label-item label="NOMFN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ANTROPONIM"/>
      <label-item label="NOMMF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="TOPONIM"/>
      <label-item label="NOMMF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NPALTRES"/>
      <label-item label="NOMMF"/>
    </label-sequence> -->
   
   
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="VLEXPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="VLEXIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="VLEXSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="IMPLEXPRNENC"/>
    </label-sequence>
      
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="VAVEAPFCI"/>
    </label-sequence>
 
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="VMODALPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREPA"/>
      <label-item label="VLEXPARTPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="AVEALEX"/>
      <label-item label="VLEXPARTPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="VMODALSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="IMPMODPRNENC"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VLEXPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VLEXPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VLEXIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VLEXIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VLEXSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VLEXSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VLEXGER"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VLEXGER"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VAVEAPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VMODALPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VMODALPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VMODALIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VMODALIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VMODALSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VMODALSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VMODALGER"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VMODALGER"/>
    </label-sequence>
 <!--  <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VREAMOD"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VREAMOD"/>
    </label-sequence>
   <label-sequence>
      <label-item label="DETM"/>
      <label-item label="VREALEX"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="VREALEX"/>
    </label-sequence> -->

    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VLEXPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VLEXPFCI"/>
    </label-sequence>
    
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VLEXIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VLEXIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VLEXSUBJ"/>
    </label-sequence>
   <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VLEXSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VLEXGER"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VLEXGER"/>
    </label-sequence>
    
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VAVEAPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VAVEAPFCI"/>
    </label-sequence>

   
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VMODALPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VMODALPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VMODALIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VMODALIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VMODALSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VMODALSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VMODALGER"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VMODALGER"/>
    </label-sequence>
<!--   <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VREAMOD"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VREAMOD"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="VREALEX"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="VREALEX"/>
    </label-sequence> -->
       <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="VLEXPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VLEXPFCI"/>
    </label-sequence>

    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VLEXIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="VLEXIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VLEXSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="VLEXSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VLEXGER"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="VLEXGER"/>
    </label-sequence>

    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VAVEAPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VAVEAPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VMODALPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="VMODALPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VMODALIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="VMODALIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VMODALSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="VMODALSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VMODALGER"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="VMODALGER"/>
    </label-sequence>
 <!--   <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VREAMOD"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="VREALEX"/>
    </label-sequence> -->
    <label-sequence>
      <label-item label="CNJSUBS"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="SACNJSUB"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
  <!--  <label-sequence> 
      <label-item label="CAREREL"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>  -->
     <label-sequence>
      <label-item label="RELNOM"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    
    <label-sequence>
      <label-item label="CNJSUBS"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="SACNJSUB"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
   <!-- <label-sequence>
      <label-item label="CAREREL"/>
      <label-item label="VMODALIMP"/>
    </label-sequence> -->
   <label-sequence>
      <label-item label="RELNOM"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="CNJSUBS"/>
      <label-item label="IMPLEXPRNENC"/>
    </label-sequence>
    <label-sequence>
      <label-item label="SACNJSUB"/>
      <label-item label="IMPLEXPRNENC"/>
    </label-sequence>
  <!--  <label-sequence>
      <label-item label="CAREREL"/>
      <label-item label="IMPLEXPRNENC"/>
    </label-sequence> -->
  <label-sequence>
      <label-item label="RELNOM"/>
      <label-item label="IMPLEXPRNENC"/>
    </label-sequence>
   
    <label-sequence>
      <label-item label="CNJSUBS"/>
      <label-item label="IMPMODPRNENC"/>
    </label-sequence>
    <label-sequence>
      <label-item label="SACNJSUB"/>
      <label-item label="IMPMODPRNENC"/>
    </label-sequence>
  <!--  <label-sequence>
      <label-item label="CAREREL"/>
      <label-item label="IMPMODPRNENC"/>
    </label-sequence> -->
    <label-sequence>
      <label-item label="RELNOM"/>
      <label-item label="IMPMODPRNENC"/>
    </label-sequence>
  
  
  
  

<!--    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="NOMF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETM"/>
      <label-item label="NOMFN"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="NOMM"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="NOMMN"/>
    </label-sequence> -->
    
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VLEXPARTPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXINF"/>
      <label-item label="VLEXPARTPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXGER"/>
      <label-item label="VLEXPARTPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXPFCI"/>
      <label-item label="VLEXPARTPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIPI"/>
      <label-item label="VLEXPARTPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXSUBJ"/>
      <label-item label="VLEXPARTPI"/>
    </label-sequence>
    
    
    
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VLEXPFCI"/>
    </label-sequence>
    
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VLEXIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VLEXSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="IMPLEXPRNENC"/>
    </label-sequence>
  
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VAVEAPFCI"/>
    </label-sequence>
 <!--   <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VAVEAIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VAVEASUBJ"/>
    </label-sequence> -->
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VMODALPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VMODALIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VMODALSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    
<!--    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VREAMOD"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VLEXIMP"/>
      <label-item label="VREALEX"/>
    </label-sequence> -->
   
    
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VLEXPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VMODALIMP"/><!--MG: l'he afegit, completar la resta de vmodalimp -->
      <label-item label="VLEXPARTPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VLEXIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VLEXSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VLEXIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="IMPLEXPRNENC"/>
    </label-sequence>
    
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VAVEAPFCI"/>
    </label-sequence>
 <!--   <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VAVEAIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VAVEASUBJ"/>
    </label-sequence> -->
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VMODALPFCI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VMODALIPI"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VMODALSUBJ"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="VMODALIMP"/>
    </label-sequence>
    <label-sequence>
      <label-item label="VMODALIMP"/>
      <label-item label="IMPMODPRNENC"/>
    </label-sequence>
<!--    <label-sequence>      AIXÒ TÉ SENTIT EN ROMANÈS?
      <label-item label="DETM"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETF"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETFN"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMF"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMN"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETMFN"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREP"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREDET"/>
      <label-item label="SENT"/>
    </label-sequence> -->

    <label-sequence>
      <label-item label="REL"/>
      <label-item label="SENT"/>
    </label-sequence>
<!--  <label-sequence>
      <label-item label="RELCINE"/>
      <label-item label="SENT"/>
    </label-sequence> --> 
    <label-sequence>
      <label-item label="RELADV"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="CNJCOORD"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="CNJSUBS"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="CNJADV"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PREPDET"/>
      <label-item label="SENT"/>
    </label-sequence>
<!--    <label-sequence>
      <label-item label="BINEPREADV"/>
      <label-item label="SENT"/>
    </label-sequence> -->
<!--    <label-sequence>
      <label-item label="FOARTEADV"/>
      <label-item label="INTERJ"/>
    </label-sequence>    -->
  </forbid>
  
   <enforce-rules>
    <enforce-after label="vaux">
      <label-set>
        <label-item label="VLEXINF"/>
      <!--  <label-item label="VAVEAINF"/>-->
        <label-item label="VMODALINF"/>
      </label-set>
    </enforce-after>
      <enforce-after label="VAVEAPFCI">
      <label-set>
        <label-item label="VLEXPARTPI"/>
        <label-item label="VMODALPARTPI"/>
      </label-set>
    </enforce-after>
 </enforce-rules>



  <preferences>
   <prefer tags="vblex.pri.p3.pl"/>
  </preferences>




















</tagger>
