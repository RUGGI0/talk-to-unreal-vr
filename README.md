# Virtual Reality

Il progetto usa Unreal Engine come interfaccia e un server Python sullo stesso PC. Questa base verifica lo scambio di testo via HTTP. Per ora `POST /query` restituisce il testo ricevuto con lo stato `echo`.

## Avvio del server

Da PowerShell, nella cartella del progetto, esegui le prime due righe per preparare l'ambiente su un nuovo PC. Usa l'ultima per avviare il server.

```powershell
py -3.10 -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r server\requirements.txt
.\.venv\Scripts\python.exe -m uvicorn server.main:app --host 127.0.0.1 --port 8000
```

Lascia il server aperto ed esegui la prova in un secondo terminale.

```powershell
Invoke-RestMethod 'http://127.0.0.1:8000/health'
Invoke-RestMethod -Method Post -Uri 'http://127.0.0.1:8000/query' -ContentType 'application/json' -Body '{"text":"Ciao Unreal"}'
```

La seconda richiesta restituisce `{"status":"echo","reply":"Ricevuto: Ciao Unreal"}`. Il server rifiuta un testo vuoto, assente o non testuale con HTTP 422. La [documentazione interattiva dell'API](http://127.0.0.1:8000/docs) è disponibile quando il server è avviato.

## Prima prova in Unreal

1. Installa Unreal Engine 5.6 dal Launcher e Visual Studio 2022 con **Game development with C++**, MSVC v143 14.38 e Windows SDK 10.0.22621.0 o superiore. Per UE 5.6, Epic indica Visual Studio 17.8 o superiore e raccomanda 17.14. [Configurazione C++ per UE 5.6](https://dev.epicgames.com/documentation/en-us/unreal-engine/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine?application_version=5.6)
2. Apri `VRProject/VRProject.uproject` e compila il modulo se l'editor lo richiede.
3. Apri o crea un livello. Nel suo Level Blueprint collega `Event BeginPlay` a `Send Local Text Query` con il testo `Ciao Unreal`. Collega `Completed` a `Print String` usando `Reply` e `Failed` a un altro `Print String` usando `Error`.
4. Con Python in esecuzione, premi **Play**. La prova riesce quando compare `Ricevuto: Ciao Unreal`.

## Requisiti della consegna

Il prototipo del corso deve accettare richieste scritte in italiano o inglese. Una risposta può eseguire un'azione in Unreal, chiedere un chiarimento, rifiutare una richiesta fuori ambito o chiedere conferma. Il progetto finale richiede da 5 a 8 azioni e una valutazione con almeno 20 richieste, di cui almeno 5 fuori ambito.

La tutor propone la lista della spesa o le istruzioni di montaggio come possibili scenari. Suggerisce embedding e reranker per scegliere l'azione e un piccolo modello locale via Ollama per estrarre i parametri. Il tutorial usa Unreal 5.6 e mostra uno scambio HTTP analogo a quello di questa base. [Tutorial di Gaia Lorenzoni](https://github.com/Gaia96/talk_to_Unreal)

VR e voce sono estensioni facoltative. Il passthrough del Quest è un'altra estensione proposta dalla tutor. Il corso richiede di dichiarare gli assistenti AI usati durante lo sviluppo. Per questa configurazione iniziale è stato usato Codex.
