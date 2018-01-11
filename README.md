# cmos

Per scaricare il codice andare nella cartella desiderata e lanciare il comando:

`git clone git@github.com:carlomt/cmos.git`

creera' una cartella
`cmos`
con dentro tutto il codice.

Per aggiornare all'ultima versione usare il comando:

`git pull`

dentro quella cartella.


Come convenzione i file con estensione cpp producono eseguibili, i file con estensione C sono le implementazioni delle classi.
Tutti gli eseguibili si compilano con make e il nome dell'eseguibile (con .x alla fine)

Tutti gli eseguibili hanno un help (eseguendoli senza opzioni o con -help)

Facendo source setthis.sh si aggiungono al path FileConverter e Riduzione, entrambi con l'autocompletion

Il pacchetto e' composto di 3 eseguibili

FileConverter.x coverte output CMOS txt in root files
i dati raw (in txt) sono su

`@bb8.roma1.infn.it:/home/cmos/data/`

(directory in sola lettura)

i root files conterranno istanze della classe Frame

Riduzione.x permette di settare molte variabili della riduzione da riga di comando (-help mostra tali opzioni)
produce un file root con un'istanza della classe SeedList che all'interno ha un array di seed (istanze della classe Seed)

esempio.x e' il codice di Amedeo che prende in input il file prodotto dalla riduzione e produce tutti i plot che vengono salvati in un file chiamato di default `analized.root`

CMOSDict.cxx e' il dizionario delle classi per ROOT (se usato in modalita' interattiva) si crea con

`make CMOSDict.cxx`

per usare le classi (ed i file con i dati) in modalita' interattiva di ROOT bisogna caricare la libreria libCMOS.so 

`.L libCMOS.so`

anche questa libreria si produce con:

`make libCMOS.so`

poi si possono usare tutte le classi in interattivo, ad esempio:

`root [0] .L libCMOS.so`

`root [1] Frame prova(480,640)`

`root [2] prova.ReadFile("capture0001.raw",true)`

`root [3] prova.Draw("colz")`

P.S.: Il codice di Amedeo e' estremamente verboso e forse andrebbe sistemato e aggiunto a setthis.sh
oltre che estremamente lento (almeno per bb8)

Il mio consiglio e' convertire alcuni file di dati in root su bb8 e copiarli sul tuo portatile con

`scp lgiuliano@bb8.roma1.infn.it:/pathdelfile/nomefile.root .`

e poi girare riduzione e analisi sul tuo portatile (facendo attenzione di avere l'ultima versione)


Per attivare gli output di debug ricompilare con la flag apposita:

`make clean`

`make libCMOS.so DEBUG=ON`

#############
Dopo la sessione di misure Y di gennaio @ Roma

Tempi dei vari programmi:
FileConverter:
- qualche secondo su 100 frame buio
- 40 secondi su 2000 frame segnale


#### WORKFLOW
- L'acquisizione produce:
AAAA-MM-GG_NomeSensore_note{num}.raw
(~600kB a frame -> 1.3 GB per acq 2000 frames)
- FileConverter prende tutti i .raw e produce:
AAAA-MM-GG_NomeSensore_note0000.root
(~550MB per 2000 frames, ~40 s su 2000 frames)
- PedAnalyzer.C prende in input questo root e produce:
- I due file di testo da trasforamare poi in .root (tramite FileConverter) che serviranno per girare riduzione (~2MB ciascuno per 2000 frame)
- Un file .root con tutti gli istogrammi e i vettori per usare eventualmente la macro di controllo stabilità (~100MB per 2000frame)
   -AAAA-MM-GG_NomeSensore_note0000_buio_{NframesConsiderati}.txt
   -AAAA-MM-GG_NomeSensore_note0000_noise_{NframesConsiderati}.txt
   -AAAA-MM-GG_NomeSensore_note0000_PostPed.root
   
   













