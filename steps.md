# Assignment #02 – Smart Radar System: guida al ragionamento

> Punto di partenza: lo scheletro `sweeping-system` del Lab 08.
> Questa guida **non** contiene la soluzione: per ogni passo trovi cosa fare, dove guardare nel codice e le domande a cui rispondere prima di scrivere.

Abbreviazioni dei percorsi usati sotto:
- `SW` = [`../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src)
- `JAVA` = [`../lab-activities/lab-activity-08/sweeping-system/java/SweepingSystem/src/esiot/sweeping_system`](../lab-activities/lab-activity-08/sweeping-system/java/SweepingSystem/src/esiot/sweeping_system)

---

## Step 0 – Setup del progetto

- [ ] Crea la struttura della consegna:
  ```
  assignment-02/
  ├── radar-subsystem-unit/     ← copia di sweeping-system/arduino/sweeping-system
  ├── console-subsystem-unit/   ← copia di sweeping-system/java/SweepingSystem
  └── doc/
  ```
- [ ] Apri `radar-subsystem-unit` con PlatformIO e verifica che compili **senza modifiche**.
  - In [`platformio.ini`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/platformio.ini) ci sono già `TimerOne` (scheduler), `LiquidCrystal_I2C` (display) ed `EnableInterrupt` (interrupt sui pin).

**Struttura che erediti** (la mantieni così):

| Cartella | Ruolo | File chiave |
|---|---|---|
| `kernel/` | infrastruttura: scheduler, task, seriale, log | `Scheduler.cpp`, `Task.h`, `MsgService.*`, `Logger.*` |
| `devices/` | driver dei componenti (interfaccia + implementazione) | `Sonar`, `ServoMotorImpl`, `ButtonImpl`, `Led`, `Pot`, `TempSensorTMP36`/`LM35` |
| `model/` | `HWPlatform` (crea i device) e `Context` (stato condiviso) | `HWPlatform.cpp`, `Context.h` |
| `tasks/` | la logica: un task = una FSM sincrona | `SweepingTask.cpp`, `BlinkingTask.cpp` |

---

## Step 1 – Hardware e configurazione

**Dove guardare:** [`SW/config.h`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/config.h), [`SW/model/HWPlatform.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/model/HWPlatform.cpp), [`SW/main.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/main.cpp)

- [ ] Elenca i componenti e assegna un pin a ciascuno in `config.h`: 3 pulsanti, 2 LED, servo, sonar (trig, echo), pot, temp, LCD (I2C usa A4/A5).
- [ ] Aggiungi i device mancanti in `HWPlatform`, cioè i membri e i getter, come per `getButton()` e `getMotor()`.
- [ ] Testa l'hardware **prima** della logica: attiva `#define __TESTING_HW__` in `main.cpp` ed estendi `HWPlatform::test()`. Questo è l'unico posto dove i `delay()` sono tollerati.

**Domande:**
1. Quale pin dai a **START**? *Indizio: su Arduino Uno solo i pin 2 e 3 hanno interrupt esterni (INT0/INT1), e servono per risvegliare dal deep sleep (Step 7).*
2. Perché il servo usa `ServoTimer2` e non la libreria `Servo` standard? *Indizio: quale timer usa lo scheduler in `kernel/Scheduler.cpp`?*
3. Cosa restituisce `Potentiometer::getValue()` e come lo converti in un angolo 0–180? Guarda [`SW/devices/Pot.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/devices/Pot.cpp).

---

## Step 2 – Le FSM su carta (prima del codice!)

**Dove guardare:** [`doc/sweeping-system-FSMs.png`](../lab-activities/lab-activity-08/sweeping-system/doc/sweeping-system-FSMs.png) come esempio di formato.

- [ ] Disegna la FSM di sistema con gli stati `IDLE`, `WORKING`, `PRE_ALARM` e `ALARM`.
- [ ] Per ogni transizione scrivi **evento/condizione → azione**. Rileggi la specifica riga per riga:
  - START, STOP, RESET (pulsanti);
  - distanza in [D1, D2] → pre-allarme; distanza < D1 → allarme;
  - uscita dal pre-allarme quando nessun oggetto è più vicino di D2.
- [ ] Il diagramma finisce nel report: usa draw.io, come nel lab.

**Domande:**
1. Basta **una** FSM o ne servono più di una, che collaborano? Nello sweeping-system lo `SweepingTask` decide lo stato e il `BlinkingTask` lo *legge* dal `Context`.
2. Lo STOP vale in ogni stato? Rileggi: "*when the system is working normally*…"
3. In `ALARM` il radar continua a misurare? La specifica non lo dice: decidi tu e scrivilo nel report.
4. Da `PRE_ALARM` si può passare direttamente ad `ALARM`?

---

## Step 3 – Decomposizione in task

**Dove guardare:** [`SW/kernel/Task.h`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/kernel/Task.h), [`SW/kernel/Scheduler.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/kernel/Scheduler.cpp), [`SW/main.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/main.cpp)

Task candidati (decidi tu come accorparli o dividerli):

| Task candidato | Responsabilità | Simile a |
|---|---|---|
| Radar / Sweeping | muove il servo settore per settore, gestisce lo stato di sistema | `SweepingTask` |
| Detection | legge il sonar e classifica la distanza (niente, [D1,D2], <D1) | — |
| Alarm LED | LED ALARM spento / fisso / lampeggiante in base allo stato | `BlinkingTask` |
| Display | aggiorna l'LCD | — |
| Comm | invia lo stato e i settori alla console (e opzionalmente riceve comandi) | `Logger` / `MsgService` |
| Manual control | in ALARM, servo ← potenziometro | — |

**Regole da ricordare (Lab 08):**
- Ogni `tick()` deve essere **breve e non bloccante**: niente `delay()` e niente cicli di attesa.
- Il periodo di ogni task è un **multiplo del periodo base** (`sched.init(...)`).
- Il periodo si sceglie in base alla dinamica: pulsanti ~50 ms; un LCD non ha bisogno di aggiornarsi ogni 50 ms.

**Domande:**
1. Quale periodo dai a ciascun task? Giustificalo nel report.
2. ⚠️ Guarda [`SW/devices/Sonar.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/devices/Sonar.cpp): `getDistance()` usa `pulseIn(..., timeOut)`, che **blocca** fino al timeout. Quanto vale il timeout nel caso peggiore? È compatibile col periodo base? Come scegli `maxTime` nel costruttore?
3. Il Display e il Comm devono essere task separati o possono stare insieme?
4. Un task può essere **disattivato** quando non serve (`setActive(false)`, vedi `scheduler-example-dyn`)? Per esempio il controllo manuale fuori da ALARM.

---

## Step 4 – Il `Context` condiviso

**Dove guardare:** [`SW/model/Context.h`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/model/Context.h) e `Context.cpp`

È la "lavagna" su cui i task si parlano: uno scrive, gli altri leggono al proprio tick.

- [ ] Elenca le informazioni condivise. Spunti:
  - stato del sistema (enum al posto dei due bool `started`/`stopped`);
  - settore corrente (1..N) e angolo;
  - distanza misurata per ogni settore (array di N elementi);
  - eventi "one-shot", come un comando arrivato dalla console.
- [ ] Per ogni campo, chiediti **chi lo scrive** (idealmente un solo task) e **chi lo legge**.

**Domande:**
1. Se due task scrivono lo stesso campo, cosa può andare storto?
2. Ti serve sapere "lo stato è appena cambiato"? Pensa a Display e Comm, che dovrebbero inviare solo le novità.

---

## Step 5 – Il pattern FSM dentro un task

**Dove guardare:** [`SW/tasks/SweepingTask.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/tasks/SweepingTask.cpp) e [`SW/tasks/BlinkingTask.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/tasks/BlinkingTask.cpp)

Ingredienti da riusare:
- `enum { ... } state;` + `switch (state)` in `tick()`
- `setState(s)`: cambia stato, salva il timestamp e imposta `justEntered`
- `checkAndSetJustEntered()`: azioni **di ingresso** (eseguite una volta sola)
- `elapsedTimeInState()`: attese **senza bloccare** (vedi lo stato `STARTING`)

**Differenza col lab:** lo sweeping-system muove il servo in modo **continuo** (`dt/FWD_TIME*180`). Qui invece il radar si muove **per settori**.

**Domande:**
1. Come traduci "velocità S1" in qualcosa di misurabile? *Indizio: tempo di permanenza in un settore, oppure gradi per tick.* Cosa cambia passando a S2?
2. Ogni quanti gradi c'è un settore (180/N)? Misuri all'**arrivo** nel settore, dopo che il servo si è assestato?
3. Fine corsa a 180°: come gestisci l'inversione avanti/indietro? Lo sweeping-system ha `SWEEPING_FWD`/`SWEEPING_BWD`.
4. Il punto aperto del Lab 08 ("evitare lo stop immediato", `toBeStopped`) ti serve per lo STOP?

---

## Step 6 – Temperatura → velocità del suono

**Dove guardare:** `SW/devices/Sonar.h` (`setTemperature`, `getSoundSpeed`), `SW/devices/TempSensorTMP36.cpp`, `TempSensorLM35.cpp`

- [ ] Scegli il sensore che hai (TMP36 o LM35: formule diverse!).
- [ ] Passa la temperatura al sonar con `setTemperature()`.

**Domande:**
1. La temperatura cambia lentamente: con che frequenza ha senso leggerla? Serve un task apposito o basta farlo in un task esistente con un contatore?

---

## Step 7 – Deep sleep in IDLE

**Dove guardare:** la funzione vuota `wakeUp()` in [`SW/model/HWPlatform.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/model/HWPlatform.cpp); la libreria `EnableInterrupt` (oppure `attachInterrupt`); `<avr/sleep.h>` (`set_sleep_mode(SLEEP_MODE_PWR_DOWN)`, `sleep_enable()`, `sleep_mode()`, `sleep_disable()`)

**Domande:**
1. In power-down il Timer1 si ferma: lo scheduler è "congelato". Chi chiama lo sleep, e in che punto? Dentro un `tick()`, alla fine di `schedule()` o in `loop()`?
2. Prima di dormire devi spegnere qualcosa? Pensa al LED ON, al servo (`off()`) e alla seriale (svuotala con `Serial.flush()` prima di dormire).
3. Al risveglio la pressione di START deve essere "consumata" dalla FSM: come eviti che venga persa o letta due volte?
4. Cosa succede ai `millis()`/`stateTimestamp` dopo lo sleep?

---

## Step 8 – Display LCD I2C

**Libreria:** `LiquidCrystal_I2C` (già in `platformio.ini`). Indirizzo tipico 0x27 o 0x3F.

- [ ] Crea un device `Lcd` (interfaccia + impl), nello stile degli altri device, e aggiungilo a `HWPlatform`.

**Domande:**
1. `lcd.clear()` + riscrittura a ogni tick provoca sfarfallio ed è lento (I2C): come aggiorni **solo quando cambia** qualcosa?
2. Cosa mostri in 2×16 caratteri? Per esempio stato, settore e oggetto/distanza.

---

## Step 9 – Protocollo seriale Arduino ↔ PC

**Dove guardare:** [`SW/kernel/Logger.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/kernel/Logger.cpp) (prefisso `lo:`), [`SW/kernel/MsgService.cpp`](../lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/kernel/MsgService.cpp) (`sendMsg`, `receiveMsg`, `serialEvent`, `Pattern`), [`JAVA/MonitoringAgent.java`](../lab-activities/lab-activity-08/sweeping-system/java/SweepingSystem/src/esiot/sweeping_system/MonitoringAgent.java) (prefisso `sw:`)

L'idea è già impostata: ogni riga ha un **prefisso** che dice di che tipo è (`lo:` = log, `sw:` = dato applicativo).

- [ ] Progetta il formato dei messaggi **prima** di scriverli. Per esempio:
  - un messaggio per lo stato;
  - un messaggio per il settore (numero, oggetto sì/no, distanza).
- [ ] Mettilo nel report (tabella dei messaggi).

**Domande:**
1. Formato testuale con separatori (es. `:`/`;`) o altro? Cosa è più facile da fare il parsing in Java?
2. La seriale a 115200 baud è veloce ma non infinita: quanti messaggi al secondo invii? Ogni `Serial.println` lungo allunga il `tick()`.
3. *(Opzionale)* Per i comandi PC → Arduino (TAKE CONTROL, posizione, RESET, STOP, RELEASE): quale task li legge con `MsgService.isMsgAvailable()`/`receiveMsg()`? Come entra "remote control" nella FSM dello Step 2?

---

## Step 10 – Console (Java)

**Dove guardare:** `JAVA/`
- `DashboardLauncher.java`: entry point. **Cambia `portName`** (su Linux sarà tipo `/dev/ttyACM0`).
- `SerialCommChannel.java` / `CommChannel.java`: lettura e scrittura su seriale (libreria jssc in `lib/`).
- `MonitoringAgent.java`: **thread** che legge di continuo le righe e le smista in base al prefisso.
- `DashboardView.java`, `LogView.java`: GUI Swing.
- `DashboardController.java`: collega le azioni della GUI (bottoni) all'invio di messaggi.

È il pattern **MVC**: per i dubbi su Java/Swing puoi usare la skill `oopskill`.

- [ ] Fai girare lo scheletro così com'è collegato all'Arduino del lab e verifica di vedere i log.
- [ ] Nel ramo `APP_PREFIX` di `MonitoringAgent.run()`, fai il parsing dei tuoi messaggi e aggiorna la view.

**Domande:**
1. Come rappresenti gli N settori nella GUI? Basta una tabella con settore, oggetto e distanza.
2. ⚠️ Swing non è thread-safe: `MonitoringAgent` è un thread separato. Come aggiorni la GUI in modo sicuro? *Indizio: `SwingUtilities.invokeLater`.*

---

## Step 11 – Ordine di sviluppo consigliato

Procedi per **incrementi testabili**: ogni passo compila e gira.

1. [ ] Test HW di tutti i componenti (`__TESTING_HW__`)
2. [ ] FSM disegnate (Step 2) + `Context` definito (Step 4)
3. [ ] IDLE ↔ WORKING con START/STOP, LED ON (ancora senza sleep)
4. [ ] Sweep per settori a velocità S1 (solo servo, log su seriale)
5. [ ] Sonar + temperatura: misura per settore, log della distanza
6. [ ] PRE_ALARM (S2 + LED fisso) → ALARM (LED lampeggiante + pot) → RESET
7. [ ] LCD
8. [ ] Protocollo seriale + console Java
9. [ ] Deep sleep in IDLE
10. [ ] *(Opzionale)* controllo remoto dalla console

## Checklist consegna

- [ ] `assignment-02.zip` con `radar-subsystem-unit/`, `console-subsystem-unit/`, `doc/`
- [ ] Report: scelte progettuali, task e periodi, **diagrammi FSM**, protocollo seriale, valori scelti (N, S1, S2, D1, D2…)
- [ ] Schema del circuito (TinkerCad / Fritzing)
- [ ] Video dimostrativo (o link OneDrive UniBo)
