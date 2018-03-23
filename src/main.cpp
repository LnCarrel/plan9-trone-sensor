/*
 * Enable relay when object is closed
 *  Use a Sparkfun IR sensor (Sharp GP2Y0A02YK0F)
 */

#include <Arduino.h>

#define IR_SENSOR_PIN 4
#define RELAY_OUTPUT 8
#define LEVEL 150
#define TEMPS_ON 5000
#define TEMPS_OFF 2500
#define TEMPS_AFFICHAGE 100

unsigned long temps_au_changement_etat_precedent = 0;
unsigned long temps_a_l_affichage_precedent = 0;

typedef enum {
  kChercheBonneValeur,
  kSortieOn,
  kSortieOff
} MachineEtatTrucImportantEnum;

//Déclaration d'une variable (nom : DansQuelEtatJeSuis) qui est de type MachineEtatEnum
MachineEtatTrucImportantEnum DansQuelEtatJeSuis;

typedef enum {
  kAfficheRien,
  kAfficheValeur
} MachineEtatAfficheurEnum;

MachineEtatAfficheurEnum DansQuelEtatEstLAffichage;

void setup()
{
  // initialize digital pin RELAY_OUTPUT as an output.
  pinMode(RELAY_OUTPUT, OUTPUT);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //Turn low outputs (LED & RELAY)
  digitalWrite(RELAY_OUTPUT, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  //Defini le 1er etat de la machine d'état
  DansQuelEtatJeSuis = kChercheBonneValeur;
  DansQuelEtatEstLAffichage = kAfficheRien;

  Serial.begin(9600);
}

void loop()
{
  //Read analog input value
  int val = analogRead(IR_SENSOR_PIN);

  unsigned long now = millis();

  //Transition "Cherche bonne valeur" à "ON"
  if ((val > LEVEL) && (DansQuelEtatJeSuis == kChercheBonneValeur)) {
    //Sauvegarde le temps actuel
    temps_au_changement_etat_precedent = now;

    //Affichage d'information
    Serial.print("Event level!");
    Serial.print(" (val : ");
    Serial.print(val);
    Serial.println("), Start timer ON");

    //Enclenchement de la sortie
    digitalWrite(RELAY_OUTPUT, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);

    DansQuelEtatJeSuis = kSortieOn;
  }

  //Transition "ON" à "OFF"
  if ((now > (temps_au_changement_etat_precedent + TEMPS_ON)) && (DansQuelEtatJeSuis == kSortieOn)) {
    //Sauvegarde le temps actuel
    temps_au_changement_etat_precedent = now;

    //Affichage d'information
    Serial.println("End timer ON, start timer OFF");

    //Déclenchement de la sortie
    digitalWrite(RELAY_OUTPUT, LOW);
    digitalWrite(LED_BUILTIN, LOW);

    DansQuelEtatJeSuis = kSortieOff;
  }

  //Transition "OFF" à "Cherche bonne valeur"
  if ((now > (temps_au_changement_etat_precedent + TEMPS_OFF)) && (DansQuelEtatJeSuis == kSortieOff)) {
    //Affichage d'information
    Serial.println("End timer OFF");
    //Serial.print(" / val : ");
    //Serial.println(val);

    DansQuelEtatJeSuis = kChercheBonneValeur;
  }

  //Transition "AffichageRien" à "AffichageValeur"
  if ((now > (temps_a_l_affichage_precedent + TEMPS_AFFICHAGE)) && (DansQuelEtatEstLAffichage == kAfficheRien)) {
    temps_a_l_affichage_precedent = now;

    DansQuelEtatEstLAffichage = kAfficheValeur;
  }

  //Transition "AffichageValeur" à "AffichageRien"
  if (DansQuelEtatEstLAffichage == kAfficheValeur) {
    Serial.print("Ma valeur est a ");
    Serial.println(val);

    DansQuelEtatEstLAffichage = kAfficheRien;
  }
}
