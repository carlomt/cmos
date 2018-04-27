#!/bin/bash
for i in {01..25}
do
    for j in 09 10 11
    do
	    
    varA="/home/rose/CMOSworld/datiRoot/sorgenti/VariazioneTempo/2018-01-$j"
    varB="_MT9V115_90Y_Decay$i"
    varC="_0000"
    varD=$varA$varB$varC
    echo $varD
    root -b -l <<EOF
.L libCMOS.so
.L PedAnalyzer.C
c=new PedAnalyzer("$varD", 100);
   c->Loop();
.q;
EOF
    done
done
[code]#!/bin/bash

root -l file.root << EOF
ntuple->Draw(“whatever”)
c1->SaveAs(“whatever.eps”)
EOF[/code]
