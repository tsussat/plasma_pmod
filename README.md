# plasma_pmod

Ce dépôt git est dédié à la modification (matérielle et logicielle) du processeur softcore Plasma (disponible sur OpenCores.org) afin de pouvoir gérer des modules PMOD sur les cartes de développement Nexys-4 de Digilent.

Ce fichier permet de documenter le projet


# Compte Rendu de la séance du 27/10

1.  Lors de cette séance, Yannick nous a présenté les PMODs disponibles dont la liste est la suivante (non exhaustive):
    - PMOD GPS
    - PMOD Joystick
    - PMOD Compas
    - PMOD OLED RGB
    - PMOD OLED
    - PMOD TMP3
    - PMOD USB/UART
    - PMOD RF2
    - PMOD Micro
    - PMOD Sonar
    - PMOD ALS
    - PMOD RTCC
    - PMOD I2S
    - PMOD Gyroscope
    - PMOD Horloge Temps Réel
    - PMOD DA3
    - PMOD Clavier
    
    Il nous a ensuite présenté le principe de fonctionnement de l'architecture sur le FPGA et les différentes parties: Interface    Bus/FSM/Interface API. L'objectif est de développer une interface de communication entre le PMOD et la FSM qui soit la plus générique possible. Elle doit être flexible en permettant la prise en charge de plusieurs PMODs et couvrir divers besoins. Pour un PMOD donné, il faut réfléchir à quelles fonctions sont accessibles et avec quelles addresses.
Les trois bus de communication disponibles sont SPI, UART et I2C.

Idée de projet: Plutôt que de développer une application précise, nous envisageons de nous concentrer sur le développement matériel des drivers pour les périphériques. L'idée serait de faire une application vitrine en commençant par utiliser les PMODs OLED RGB, Clavier et un capteur au choix. Ainsi l'utilisateur pourra communiquer directement avec la carte. Lorsque que le capteur est connecté, l'appui sur une touche génère de l'information sur l'écran Oled.


2. Compromis Software/Hardware
    - Développement du PMOD Oled sur couche logicielle et du PMOD Clavier sur couche matérielle ?
    - Stockage de la police du texte sur une ROM externe ou dans un grand tableau pour faire du bitmap direct ?
    - Notion de flexibilité VGA/Oled (partie software complète, juste le bus change) ?
    - Yannick n'a pas de priorité sur l'ordre des PMODs utilisés, voir avec Camille si il en a.


3. Organisation et répartition du travail futur
    - Une division par PMODS (OledRGB/Clavier/Capteur)?
    - Une dision plus au niveau technique (VHDL/PLASMA)?


4. Communication
    - Pour les comptes rendus des séances, vous préférez les mails ou des fichiers sur le GitHub ?


5. Objectifs pour la prochaine séance du 10/11
    - Répondre aux problématiques précédentes
    - Regarder comment connecter les leds/switchs/boutons de la carte Nexys sur le bus système




# Executer un hello world sur le Plasma implémenté sur la Nexys 4


Pour lancer le hello world:

1. Dans le fichier makefile, s'assurer que la variable `MY_PROJECT` a bien la valeur `hello` : `MY_PROJECT=hello`

2. Se placer dans le repertoire plasma_pmod : `cd plasma_mod`

3. Faire la compilation du fichier C : ./C/hello/Sources/main.c : `make soft`

4. Faire l'analyse et l'élaboration des fichiers VHDL sous Vivado : `make vhdl`

5. Lancer la simulation logique sous vivado : `make simu`

    Vous devez voir "Hello world !" s'afficher dans le terminal

6. Changer la valeur de la variable `MY_PROJECT` dans le `makefile` : `MY_PROJECT=boot_loader`

7. Lancer la synthèse, et le placement routage : `make fpga`

8. Ouvrir un hyperterminal, le configurer en `115200-8-N-1`

9. Télécharger le fichier `top_plasma.bit` sur la Nexys 4 (Ouvrer le hardware manager de Vivado)

    Vous devez voir un message généré par le programme de boot qui s'affiche. Si vous ne le voyez pas, appuyez sur le bouton poussoir central de la Nexys 4.

10. Changer de nouveau la valeur de la variable `MY_PROJECT` dans le `makefile` : `MY_PROJECT=hello`

11. Envoyer le programme au Plasma via l'UART : `make send`

    Vous devez voir le message Hello world ! s'afficher dans l'hyperterminal
