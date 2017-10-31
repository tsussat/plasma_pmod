# Journal de bord du projet SE

Encadrants : Camille Leroux, Yannick Bornat
Etudiants : Henri Gouttard, Igor Papandinas, Paul Kociallowski

Ce fichier permet de lister les tâches effectuées lors des séances des projets.



# Compte Rendu de la séance du 27/10

1.  Lors de cette séance, Yannick nous a présenté les PMODs disponibles dont la liste est la suivante (non exhaustive):
    - PMOD GPS (yb: aucun dispo)
    - PMOD Joystick
    - PMOD CMPS (Boussole)
    - PMOD OLED RGB (yb: quel mode de fonctionement souhaité ?)
    - PMOD OLED (yb: déconseillé, car moins utilisé dans les projets futurs)
    - PMOD TMP3 (température)
    - PMOD UART
    - PMOD RF2 (communication RF/Zigbee)
    - PMOD Micro
    - PMOD Sonar (mesure de distance par ultrasons)
    - PMOD ALS (lumière ambiante)
    - PMOD I2S (sortie audio stereo HQ)
    - PMOD Gyroscope
    - PMOD Horloge Temps Réel
    - PMOD DA3 (convertisseur Numérique/Analogique)
    - PMOD Clavier Hexadecimal
Il y a egalement l'ensemble des possibilités de la Nexys4/Nexys4DDR :
    - boutons/leds
    - afficheurs 7 segments
    - audio Jack PWM
    - Sortie VGA
    - capteur de température de carte / ON-chip
    - Carte SD
    - Microphone MEMs
    - Clavier/souris USB
    - accès à la PSRAM (Nexys4) ou SDRAM (Nexys4DDR)
    - Ethernet


    Il nous a ensuite présenté le principe de fonctionnement de l'architecture sur le FPGA et les différentes parties: Interface    Bus/FSM/Interface API. L'objectif est de développer une interface de communication entre le PMOD et la FSM qui soit la plus générique possible. Elle doit être flexible en permettant la prise en charge de plusieurs PMODs et couvrir divers besoins. Pour un PMOD donné, il faut réfléchir à quelles fonctions sont accessibles et avec quelles addresses.
Les trois bus de communication disponibles sont SPI, UART et I2C.

Idée de projet: Plutôt que de développer une application précise, nous envisageons de nous concentrer sur le développement matériel des drivers pour les périphériques. L'idée serait de faire une application vitrine en commençant par utiliser les PMODs OLED RGB, Clavier et un capteur au choix. Ainsi l'utilisateur pourra communiquer directement avec la carte. Lorsque que le capteur est connecté, l'appui sur une touche génère de l'information sur l'écran Oled.


2. Compromis Software/Hardware
    - Développement du PMOD Oled sur couche logicielle et du PMOD Clavier sur couche matérielle ?
        - yb: Le clavier nécessite un balayage régulier, je penche pour une FSM hard puisqu'il n'y a pas d'IT sur le plasma. Je ne pense pas qu'il y ait de timer non plus. ça peut-être utile de rajouter un module Timer.
    - Pour afficher du texte, un compromis doit être trouvé :
        - Où stocker la police de caractères ? ROM ? inclue dans le Soft ?
        - Utilisation d'un module dédié et spécifique (moins de ressources nécessaires), ou module générique Bitmap et mise en forme en soft ?
    - Yannick n'a pas de priorité sur l'ordre des PMODs utilisés, voir avec Camille si il en a.
        - YB: faire attention quand même à commencer par des modules abordables. L'utilisation du PMOD OLED (RGB ou pas) nécessite au moins un timer matériel à côté


3. Organisation et répartition du travail futur
    - Une division par PMODS (OledRGB/Clavier/Capteur)?
    - Une dision plus au niveau technique (VHDL/PLASMA)?


4. Communication
    - Pour les comptes rendus des séances, vous préférez les mails ou des fichiers sur le GitHub ?
        - YB: personnellement, GitHub me va bien.


5. Objectifs pour la prochaine séance du 10/11
    - Répondre aux problématiques précédentes
    - Regarder comment connecter les leds/switchs/boutons de la carte Nexys sur le bus système

6. (YB) : organisation de la doc et des fichiers.
    - pour moi, mélanger les comptes rendus et les descriptions techniques dans le même fichier n'a pas de sens. Définir une hiérarchie pour la documentation.
    - structure du projet : j'ai rajouté un dossier HDL/HW_controller pour les modules matériels qui ne font pas partie du Plasma. Pour l'instant, il ne contient qu'un module de maître de bus I2C (developpement avancé, mais pas définitif). Ce dépot n'est que temporaire en attendant qu'on se définisse une organisation. Pour éviter que ça devienne la foire, il faut se définir des règles pour l'arborescence des sources qui seront écrites à l'avenir (et ajouter cette description dans la doc)


# Compte Rendu de la séance du 20/10

1. Présentation du processeur Plasma par C. Leroux : architecture du CPU, ISA custom, Co-processeurs, ...)
2. Mise en place des outils (Vivado, Modelsim, gcc, ...)

NB: Il faudrait mettre à jour le makefile pour qu'on puisse choisir les outils que l'on veut utiliser, notamment pour la simulation (Modelsim, Vivado, GHDL, ...)









