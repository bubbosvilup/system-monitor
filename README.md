# System Monitor

System Monitor è un'applicazione sviluppata in C++ utilizzando **SDL3** e **SDL_ttf** per visualizzare in tempo reale l'utilizzo di CPU e RAM, con un'interfaccia grafica minimale.

## 🌈** Cambiamenti**

✅ Il file Arial.ttf è ora incluso nel progetto, quindi non è più necessario scaricarlo manualmente.
✅ i dlls sono ora incluso nel progetto, quindi non è più necessario scaricarli manualmente.

## **📌 Funzionalità**

✅ Monitoraggio in tempo reale di CPU e RAM
✅ Visualizzazione delle informazioni su un'interfaccia grafica
✅ Grafici animati con barre colorate
✅ Mostra la quantità di RAM totale e utilizzata
✅ **TODO:** Aggiungere il rilevamento di CPU, GPU e sistema operativo

## **🛠️ Installazione**

### **1️⃣ Dipendenze necessarie**

- MinGW (GCC/G++)

### **2️⃣ Compilazione**

Esegui il seguente comando per compilare il progetto:

```sh
g++ main.cpp -o system_monitor.exe \
-I C:/SDL3-3.2.2/x86_64-w64-mingw32/include \
-I C:/SDL3_ttf-3.1.0/x86_64-w64-mingw32/include \
-L C:/SDL3-3.2.2/x86_64-w64-mingw32/lib \
-L C:/SDL3_ttf-3.1.0/x86_64-w64-mingw32/lib \
-lmingw32 -lSDL3 -lSDL3_ttf
```

### **3️⃣ Esecuzione**

Assicurati di avere le librerie SDL3.dll e SDL3_ttf.dll nella stessa cartella dell'eseguibile, poi esegui:

```sh
./system_monitor.exe
```

## **📌 TODO: Prossimi miglioramenti**

✅ **Migliorare l'interfaccia utente** per distinguere meglio CPU e RAM
✅ **Recuperare informazioni su CPU, GPU e Sistema Operativo**
✅ **Rendere il codice più modulare** per evitare funzioni troppo complesse
✅ **Gestire meglio le eccezioni e gli errori** in fase di esecuzione

## **📤 Contribuire**

Se vuoi contribuire, puoi clonare il repository e aprire una Pull Request su GitHub:

```sh
git clone https://github.com/bubbosvilup/system-monitor.git
```
