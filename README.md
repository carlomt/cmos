# cmos Data Analysis Framework
*Latest update to Readme:
2018-03-19 by collamaf+amorusor*
Pacchetto software per l'analisi dei dati acquisiti dal sensore CMOS. Funziona sia con il sensore MTV011 che con il sensore MTV115.

## Come scaricare il codice:
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

Facendo `source setthis.sh` si aggiungono al path 'FileConverter', 'Riduzione' ed 'DataAnalysis', tutti con l'autocompletion.
Il pacchetto e' composto di 3 eseguibili:

### FileConverter.x
Coverte i file di output dell'acquisizione CMOS (*NomeFileNNNN.txt/.raw*) in root files (*NomeFileNNNN.root*), contenenti istanze della classe Frame. Tramite la flag `-s` si può selezionare il tipo di sensore: 1 per MTV011 (default) e 2 per MTV115
e.g.:

```
FileConverter DatiVari/AcquisizioneGEN/sorgenti/2018-01-10_MT9V115_90Y_Decay05/*.raw -s 2

```
### PedAnalyzer.C
Analizza i primi N frames (passabile da linea di comando, altrimenti li usa tutti) dell'acquisizione per calcolare piedistallo e noise di ogni pixel. Produce due file di testo (*NomeFileNNNN_buio.txt, NomeFileNNNN_noise.txt*) da mandare a file converter e un file root (*NomeFileNNNN_PostPed.root*) da usare per il confronto fra run di piedistallo con la macro (*PedCompare.C*). Il programma legge da solo il numero di righe e colonne, ed è quindi funzionante in automatico con entrambi i sensori.
e.g.

```
root -l
.L libCMOS.so
.L PedAnalyzer.C                                                                                                                                                     
c=new PedAnalyzer("DatiVari/RootBuoni/2018-01-10_MT9V115_90Y_Decay23_0000",100)
c->Loop() 

```
Poi per convertire i .txt in .root:

```
FileConverter DatiVari/PostRic/2018-01-10_MT9V115_90Y_Decay05_0000_buio_100.txt

```
### PedCompare.C
Confronta due run di piedistallo facendo la distribuzione della differenza pixel a pixel. Prende in input da linea di comando i due file (*NomeFileNNNN_PostPed?.root*) e produce un file .root con l'istogramma (*NomeFileNNNN_CheckDiff.root*). Questa macro ha senso solo con il sensore MTV115.
e.g.

```
root -l
.L PedCompare.C
PedCompare("2018-01-10_MT9V115_90Y_Decay03_0000_PostPed_100.root","2018-01-10_MT9V115_90Y_Decay05_0000_PostPed_100.root")

```
### Riduzione.x
Prende in input il file (*NomeFileNNNN.root*) insieme ai file di buio e piedistallo (*NomeFileNNNN_buio.root, NomeFileNNNN_noise.root*) ed esegue l'algoritmo di riduzione del file con la ricerca dei cluster. Permette di settare molte variabili della riduzione da riga di comando (ad esempio la soglia in unità di sigma, -help mostra tali opzioni). Produce un file root (*NomeFileNNNN_Reduced.root*) con un'istanza della classe SeedList che all'interno ha un array di seed (istanze della classe Seed). Produce anche il file contenente l'elenco dei bad pixels (*NomeFileNNNN_badpixel.txt*). Funziona con entrambi i sensori, grazie alla flag `-frameSize 480x640`.
e.g.

```
Riduzione DatiVari/PostRic/2018-01-10_MT9V115_90Y_Decay05_0000.root -noise DatiVari/PostRic/2018-01-10_MT9V115_90Y_Decay05_0000_noise_100.root -ped DatiVari/PostRic/2018-01-10_MT9V115_90Y_Decay05_0000_buio_100.root -t 7 -frameSize 488x648

```
### DataAnalysis.x 
Prende in input il file ridotto (*NomeFileNNNN_Reduced.root*) e il file dei bad pixels (*NomeFileNNNN_badpixel.txt*) e fornisce diversi plot che vengono salvati in un file root (*NomeFileNNNN_Analized.root*). Funziona con entrambi i sensori, grazie alla flag `-frameSize 480x640`.
e.g.

```
DataAnalysis DatiVari/PostRic/2018-01-10_MT9V115_90Y_Decay05_0000_Reduced.root -frameSize 488x648

```

### CMOSDict.cxx 
E il dizionario delle classi per ROOT (se usato in modalita' interattiva) si crea con

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

Per attivare gli output di debug ricompilare con la flag apposita:

`make clean`

`make libCMOS.so DEBUG=ON`

## Tempi di esecuzione dei vari programmi
### FileConverter:
   40 secondi per acquisizione di 2000 frames
### Riduzione
   ordine 30 secondi per acquisizione di 2000 frames (ma dipende da quanti cluster trova..)

## Dimensioni dei tipici file prodotti:
### *NomeFileNNNN.raw*
   Circa 1.3GB per 2000 frame (~600KB a frame)
### *NomeFileNNNN.root*
   Circa 550MB per 2000 frame
   
   ## Latest update to ReadMe:
   *2018-04-27 by collamaf*
   Ora è possibile far arrivare dalla simulazione Geant l'informazione sull'energia del primario che aveva generato il dato cluster attraverso Riduzione e DataAnalysis. In entrambi i casi c'è una flag ("-mc") in fase di esecuzione che va a cercare e riempire questa info, altrimenti funziona come prima





