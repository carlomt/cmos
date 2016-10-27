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

P.S.: Il codice di Amedeo e' estremamente verboso e forse andrebbe sistemato e aggiunto a setthis.sh
oltre che estremamente lento (almeno per bb8)

Il mio consiglio e' convertire alcuni file di dati in root su bb8 e copiarli sul tuo portatile con

`scp lgiuliano@bb8.roma1.infn.it:/pathdelfile/nomefile.root .`

e poi girare riduzione e analisi sul tuo portatile (facendo attenzione di avere l'ultima versione)

