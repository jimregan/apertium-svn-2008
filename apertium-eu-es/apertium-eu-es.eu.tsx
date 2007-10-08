<?xml version="1.0" encoding="iso-8859-1"?>
<tagger name="euskera">

<tagset>

  <def-label name="ezanedin" closed="true">
    <tags-item lemma="ezan" tags="ADL.*"/>
    <tags-item lemma="edin" tags="ADL.*"/>
    <tags-item lemma="edin" tags="ADT.*"/>
  </def-label>
   <def-label name="ukan" closed="true">
    <tags-item lemma="ukan" tags="vbsint.*"/>
    <tags-item lemma="ukan" tags="ADT.*"/>
  </def-label> 
  <def-label name="izan" closed="true">
    <tags-item lemma="izan" tags="vbsint.*"/>
    <tags-item lemma="izan" tags="ADT.*"/>
    <tags-item tags="ADL.*"/> <!--treure, provisional tagger -->
  </def-label>
 <def-label name="hori" closed="true">
    <tags-item lemma="hori" tags="adj.izo"/>
  </def-label> 
  <def-label name="PP">
    <tags-item tags="vblex.pp"/>
  </def-label> 
 <def-label name="PPper">
    <tags-item tags="vbper.pp"/>
  </def-label>
 <def-label name="PADV">
    <tags-item tags="vblex.padv"/>
  </def-label> 


   <def-label name="GER">
    <tags-item tags="vblex.ger"/>
   </def-label>
     <def-label name="GERper">
    <tags-item tags="vbper.ger"/>
   </def-label>
   <def-label name="INF">
    <tags-item tags="vblex.inf"/>
  <!--  <tags-item tags="vblex"/>treure, provisional perquè el tagger ho agafi tot -->
   </def-label>
<!--   <def-label name="INFper">
    <tags-item tags="vbper.inf"/>
   </def-label> -->
   <def-label name="PFUT">
    <tags-item tags="vblex.pfut"/>
   </def-label>
   <def-label name="PFUTper">
    <tags-item tags="vbper.pfut"/>
   </def-label>
   <def-label name="VBIZEN">
    <tags-item tags="vblex.izen"/>
  </def-label>

<def-label name="VBSINT">
    <tags-item tags="vbsint.*"/>
    <tags-item tags="ADT.*"/>
  </def-label>

<def-label name="VBPER"><!--la resta de temps, per ara tots junts -->
    <tags-item tags="vbper.*"/>
  </def-label>
<def-label name="altreverbs">
    <tags-item tags="ATZ.ADI.*"/>
    <tags-item tags="ATZ.vblex.*"/>
    <tags-item tags="ADI.*"/>
  </def-label>


 <def-label name="NOMA">
    <tags-item lemma="*a" tags="n"/>
  </def-label> 



  <def-label name="NOM">
    <tags-item tags="n"/> 
    <tags-item tags="n.acr"/> 
  </def-label>


 <def-label name="NP">
    <tags-item tags="np.*"/>
  </def-label>

<def-label name="DET" closed="true">
    <tags-item tags="det.art.*"/>
    <tags-item lemma="bat" tags="num.sg"/>
    <tags-item tags="det.ind.*"/>
    <tags-item tags="det.dem.*"/>
    <tags-item tags="det.dem"/>
    <tags-item tags="ATZ.DET.BAN"/>
  </def-label>


<def-label name="DETINDIZL" closed="true">
    <tags-item tags="det.indizl.*"/>
  </def-label>


<!--<def-label name="DET_ORD" closed="true">
    <tags-item tags="DET.ORD"/>
    <tags-item tags="DET.NOLARR"/>
  </def-label> -->

<def-label name="ADJIZOA">
    <tags-item lemma="*a" tags="adj.izo"/>
  </def-label>

<def-label name="ADJIZO">
    <tags-item tags="adj.izo"/>
  </def-label>

<def-label name="ADJIZLA">
    <tags-item lemma="*a" tags="adj.izl"/>
  </def-label>

<def-label name="ADJIZL">
    <tags-item tags="adj.izl"/>
  </def-label>

<def-label name="PRNTN" closed="true">
    <tags-item tags="prn.tn.*"/>
    <tags-item tags="prn.pers.*"/>
    <tags-item tags="prn.ind.*"/>
    <tags-item tags="prn.itg.*"/>
    <tags-item tags="IOR.ELK"/>

  </def-label>

<def-label name="NUM" closed="true">
    <tags-item tags="num.*"/>
    <tags-item tags="num"/>
  </def-label>

<def-label name="KO" closed="true">
    <tags-item lemma="ko" tags="post"/>
    <tags-item tags="post.ko"/>
  </def-label> 

<def-label name="POST" closed="true">
    <tags-item tags="post"/>
</def-label>

<def-label name="SPOST">
    <tags-item tags="spost"/>
  </def-label>

<def-label name="PREADV">
    <tags-item tags="preadv"/>
  </def-label>
<def-label name="ADV">
    <tags-item tags="adv"/>
    <tags-item tags="ADB"/>
    <tags-item tags="ADB.*"/>
  </def-label>
<def-label name="ADVGEN">
    <tags-item tags="adv.gen"/>
  </def-label>
 <def-label name="CNJSUBS" closed="true">
    <tags-item tags="cnjsub"/>
    <tags-item tags="LOT.*"/>
  </def-label>
  <def-label name="CNJCOORD" closed="true">
    <tags-item tags="cnjcoo"/>
  </def-label>
  <def-label name="CNJADV">
    <tags-item tags="cnjadv"/>
  </def-label>
  <def-label name="CNJLOC">
    <tags-item tags="cnjloc"/>
  </def-label>


 <def-label name="GRA" closed="true">
    <tags-item tags="GRA.*"/>
    <tags-item tags="gra.*"/>
  </def-label>



  <def-label name="INTERJ">
    <tags-item tags="ij"/>
  </def-label>
  <def-label name="ANTROPONIM">
    <tags-item tags="np.ant.*"/>
  </def-label>
  <def-label name="TOPONIM">
    <tags-item tags="np.loc.*"/>
  </def-label>
  <def-label name="NPALTRES">
    <tags-item tags="np.al.*"/>
  </def-label>

  <def-label name="PREP" closed="true">
    <tags-item tags="pr"/>
  </def-label>


 <!-- <def-label name="RELADV" closed="true">
    <tags-item tags="rel.adv"/>
  </def-label> -->



<def-label name="REL" closed="true">
    <tags-item tags="rel"/>
  </def-label>
<def-label name="PRT" closed="true">
    <tags-item tags="PRT.*"/>
    <tags-item tags="PRT"/>
  </def-label>
<def-label name="BST" closed="true">
    <tags-item tags="BST"/>
  </def-label>

<def-mult name="NOMA_DET">
    <sequence>
      <label-item label="NOMA"/>
      <tags-item tags="det.art.sg"/>
    </sequence>
  </def-mult>

<def-mult name="ADJIZOA_DET">
    <sequence>
      <label-item label="ADJIZOA"/>
      <tags-item tags="det.art.sg"/>
    </sequence>
  </def-mult>

<def-mult name="DETERG" closed="true">
    <sequence>
      <tags-item tags="det.art.sg"/>
      <tags-item lemma="k" tags="post"/>
    </sequence>
  </def-mult>


<def-mult name="DETEN" closed="true"><!--per a  casar-lo amb 'en' superlatiu -->
    <sequence>
      <tags-item tags="det.art.pl"/>
      <tags-item lemma="en" tags="post"/>
    </sequence>
  </def-mult>

<def-mult name="BANA" closed="true">
    <sequence>
      <tags-item lemma="bat" tags="num.sg"/>
      <tags-item lemma="na" tags="ATZ.DET.BAN"/>
    </sequence>
  </def-mult>

<def-mult name="BANAKA" closed="true">
    <sequence>
      <tags-item lemma="bat" tags="num.sg"/>
      <tags-item lemma="na" tags="ATZ.DET.BAN"/>
      <tags-item lemma="ka" tags="ATZ.ADB.ADOARR"/>
    </sequence>
  </def-mult>

<def-mult name="NOMKO"><!--per a casar-lo amb adjectius en -ko? -->
    <sequence>
      <tags-item tags="n"/>
      <label-item label="KO"/>
    </sequence>
  </def-mult>

<def-mult name="ADVKO">
    <sequence>
      <label-item label="ADV"/>
      <label-item label="KO"/>
    </sequence>
  </def-mult>

<def-mult name="SPOSTKO">
    <sequence>
      <label-item label="SPOST"/>
      <label-item label="KO"/>
    </sequence>
  </def-mult>

<def-mult name="VERBREL">
    <sequence>
      <label-item label="VBSINT"/>
      <label-item label="REL"/>
    </sequence>
  </def-mult>

<def-mult name="PRGEN"><!--perquè 'zuen' = VERBREL/PRGEN  --> 
    <sequence>
      <tags-item tags="prn.pers.*"/>
      <tags-item lemma="en" tags="post"/>
    </sequence>
  </def-mult>

<def-mult name="NOM_POST"><!--afegit 28/09/07 -->
    <sequence>
      <tags-item tags="n"/>
      <label-item label="POST"/>
    </sequence>
  </def-mult>

<def-mult name="ADJIZO_POST"><!--afegit 28/09/07 -->
    <sequence>
      <tags-item tags="adj.izo"/>
      <label-item label="POST"/>
    </sequence>
  </def-mult>
<!--<def-mult name="DETPOST" closed="true">
    <sequence>
      <tags-item tags="det.art.*"/>
      <tags-item tags="post"/>
    </sequence>
  </def-mult>

<def-mult name="DETKO" closed="true">
    <sequence>
      <tags-item tags="det.art.*"/>
      <tags-item tags="post.*"/>
    </sequence>
  </def-mult>-->


</tagset>

 <forbid>
    <label-sequence>
      <label-item label="NUM"/>
      <label-item label="INF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="DETINDIZL"/>
      <label-item label="INF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJIZLA"/>
      <label-item label="INF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJIZL"/>
      <label-item label="INF"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJIZLA"/>
      <label-item label="DET"/>
    </label-sequence>
    <label-sequence>
      <label-item label="ADJIZL"/>
      <label-item label="DET"/>
    </label-sequence>
    <label-sequence>
      <label-item label="INF"/>
      <label-item label="DET"/>
    </label-sequence> 
    <label-sequence>
      <label-item label="INF"/>
      <label-item label="ADJIZOA"/>
    </label-sequence>  
    <label-sequence>
      <label-item label="INF"/>
      <label-item label="ADJIZOA_DET"/>
    </label-sequence> 
    <label-sequence>
      <label-item label="INF"/>
      <label-item label="ADJIZO"/>
    </label-sequence>  
    <label-sequence>
      <label-item label="INF"/>
      <label-item label="NOM"/>
    </label-sequence> 
    <label-sequence>
      <label-item label="INF"/>
      <label-item label="NOMA"/>
    </label-sequence>
    <label-sequence>
      <label-item label="INF"/>
      <label-item label="NOMA_DET"/>
    </label-sequence>
    <label-sequence><!--joan den -->
      <label-item label="INF"/>
      <label-item label="VERBREL"/>
    </label-sequence>
    <label-sequence><!--joan da -->
      <label-item label="INF"/>
      <label-item label="VBSINT"/>
    </label-sequence>
    <label-sequence><!--joan da -->
      <label-item label="INF"/>
      <label-item label="ukan"/>
    </label-sequence>
    <label-sequence><!--joan da -->
      <label-item label="INF"/>
      <label-item label="izan"/>
    </label-sequence>
    <label-sequence>
      <label-item label="INF"/>
      <label-item label="SENT"/>
    </label-sequence>
    <label-sequence>
      <label-item label="GRA"/>
      <label-item label="SPOST"/>
    </label-sequence> 
    <label-sequence>
      <label-item label="GRA"/>
      <label-item label="ADVGEN"/>
    </label-sequence> 
    <label-sequence><!--? correcte? (etorri zuen) -->
      <label-item label="PP"/>
      <label-item label="PRGEN"/>
    </label-sequence> 
    <label-sequence>
      <label-item label="SENT"/>
      <label-item label="ADJIZOA"/>
    </label-sequence>
    <label-sequence><!--. Ezker abertzalea -->
      <label-item label="SENT"/>
      <label-item label="ADJIZO"/>
    </label-sequence>
    <label-sequence><!--ui no sé, només una prova -->
      <label-item label="NOMA"/>
      <label-item label="NOMA"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMA_DET"/>
      <label-item label="NOMA"/>
    </label-sequence>
    <label-sequence>
      <label-item label="NOMA"/>
      <label-item label="NOMA_DET"/>
    </label-sequence>

<!--    <label-sequence>
      <label-item label="PRT"/>
      <label-item label="NOM"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRT"/>
      <label-item label="NOMA"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRT"/>
      <label-item label="NOMA_DET"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRT"/>
      <label-item label="NOMKO"/>
    </label-sequence>
    <label-sequence>
      <label-item label="PRT"/>
      <label-item label="NOM_POST"/>
    </label-sequence> -->


 </forbid>

 <enforce-rules>
   <enforce-after label="hori">
      <label-set>
        <label-item label="DET"/>
      </label-set>
    </enforce-after>
   
   <enforce-after label="PRT">
      <label-set>
        <label-item label="ukan"/>
        <label-item label="izan"/>
        <label-item label="VBSINT"/>
      </label-set>
    </enforce-after>

     <enforce-after label="INF">
      <label-set>
        <label-item label="ezanedin"/>
      </label-set>
    </enforce-after>

 </enforce-rules>

  <preferences><!--nago, etc. -->
   <prefer tags="vbsint.pri.*"/>
   <prefer tags="vbsint.pii.*"/>
  </preferences>

</tagger>
