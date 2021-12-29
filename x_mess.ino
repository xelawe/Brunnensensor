// An eigene Zisterne anpassen (zur Berechnung der Füllmenge)
const int A = 3140;                       // Grundfläche der Zisterne in cm^2 (d * d * 3,14 / 4)
const int maxFuellhoehe = 3040;           // Füllhöhe der Zisterne in mm

int atmDruck, messDruck, vergleichswert;
int hoehe = 0;
int volumen = 0;
int fuellstand = 0; 
int messSchritt, wassersaeule;
unsigned long messung, messTakt;
