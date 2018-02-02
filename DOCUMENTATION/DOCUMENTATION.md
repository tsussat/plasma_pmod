# Documentation

<p align="center">
  <img src="SRC/plasma.gif">
</p>

- [Introduction](#introduction)
- [Prérequis](#prerequis)
	- [Configuration](#configuration)
	- [Ajout de nouveaux PMODs](#ajout-de-nouveaux-pmods)	
- [Manuel d'utilisation de certains PMODs](#manuel-dutilisation-de-certains-pmods)
	- [PMOD Oled-RGB: ](#pmod-oled-rgb)
		- [Module Charmap](#module-charmap)
		- [Module Terminal](#module-terminal)
		- [Module Bitmap](#module-bitmap)	
	- [Afficheur sept segments](#afficheur-sept-segments)
	- [Module de gestion de l'I2C](#module-de-gestion-de-li2c)


## Introduction
Cette documentation a pour objectif de détailler le principe de fonctionnement de l'architecture du processeur Plasma lors de son utilisation avec divers PMODs. Ce processeur a été instancié sur une puce FPGA *Artix 7* embarquée sur carte *NEXYS 4*. Il est basé sur une architecture RISC 32-bit softcore et est issu d'un projet open source: [**Plasma**](http://opencores.org/project,plasma). Il tourne à une fréquence d'horloge de 25 MHz. L'utilisation des PMODs repose sur une interface plus flexible codée en C et faisant abstraction du langage de description VHDL.

Ce travail a été réalisé dans le cadre d'un projet proposé par Camille Leroux en option SE et développé par Paul Kocialkowski, Henri Gouttard et Igor Papandinas.

<p align="center">
  <img src="SRC/architecture.gif">
</p>

## Prérequis
Ci-dessous sont listé les outils requis pour l'utilisation du processeur Plasma et les PMODs ou bien l'ajout de nouveaux.

### Configuration
Les outils nécessaire pour l'utilisation du processeur Plasma avec les PMODs sont les suivants:
  * Une carte de développement *NEXYS 4* pour y instancier le plasma et autres modules à utiliser.
  * Le logiciel `Vivado` pour le chargement du bitstream.

### Ajout de nouveaux PMODs
Les outils nécessaires pour l'ajout de nouveaux PMODs au processeur Plasma sont les suivants:
  * Les outils cross-compilés pour une architecture MIPS-elf: `mips-elf-gcc`, `mips-elf-as`, `mips-elf-ld`.
  * Le logiciel `Vivado` pour compiler le code VHDL.

Pour commencer il faut développer en VHDL le module correspondant au PMOD que l'on souhaite ajouter. On place ensuite le fichier .vhd de la description dans le répertoire *HDL/PLASMA/*.

Maintenant il faut instancier le module dans l'architecture du plasma. Dans le fichier **plasma.vhd**, on créé donc un component et son port map correspondant avec les signaux internes nécessaires (valid, reset, ...). Puis on définit de nouvelles adresses READ/WRITE (RW) et RESET (RST) que l'on associe aux signaux *valid* et *reset* précedement créé. Cela est fait dans la description VHDL au moment de déclarer les signaux de commandes en direction du port PCIe. Enfin dans le process *misc_proc*, pour le cas où les bits 28 à 30 de *cpu_address* valent "100", on déclare l'adresse de lecture pour notre module (la même que pour valid) à laquelle on associe le signal de sortie du module.

Une fois l'instanciation faite, il ne faut pas oublier de modifier le fichier **top_plasma.vhd** si nécéssaire pour y ajouter les connectiques des périphériques d'entrer et de sortie sur la carte *NEXYS 4*.

Ensuite il faut compiler l'ensemble à partir du *Makefile* dans le répertoire courant du projet (cf la partie détaillant le fonctionnement du fichier Makefile).

Pour terminer, si l'ensemble des instructions précédentes ont bien été suivi, il est maintenant possible d'intéragir dans un programme en C avec le PMOD à partir des adresses utilisées et des fonctions *MemoryWrite()* et *MemoryRead()* (cf les exemples donnés ci-dessous pour d'autres modules tel que le module *Charmap* du PMOD Oled-RGB).

**[`^        back to top        ^`](#)**

## Manuel d'utilisation de certains PMODs

### PMOD Oled-RGB

<p align="center">
  <img src="SRC/OLEDrgb.png" alt="Oled-RGB" height="250" width="250">
</p>

Le PMOD Oled-RGB permet l'affichage de caractères ASCII sous 8 lignes X 16 colonnes. Il permet aussi de réaliser un affichage Bitmap sous 96X64 avec 16 bits/pixel. Un module d'affichage de jusqu'à 4 courbes a également été instancié (non testé encore).
L'ajout des divers modules de ce PMOD au projet repose sur le travail de Mr. Bornat, détaillé à l'adresse suivante: http://bornat.vvv.enseirb.fr/wiki/doku.php?id=en202:pmodoledrgb.

Chaque module possède une adresse d'activation sur un bit (*oledXXXXXX_valid*) qui passe à '1' au moment d'une lecture ou d'une écriture à l'adresse correspondante au module.

**[`^        back to top        ^`](#)**

#### Module Charmap
Le module **Charmap** permet l'affichage sur l'écran Oled-RGB d'un caractère ASCII à une position donnée (ligne et colone).
Ce module du PMOD Oled-RGB est adressable aux adresses suivantes:
  * READ/WRITE: OLEDCHARMAP_RW    --> 0x400000A8
  * RESET:      OLEDCHARMAP_RST   --> 0x400000A0

Avant de pouvoir afficher un caractère, il vaut attendre que le module soit prêt à le recevoir. Cela ce traduit par l'activation du bit *Ready* de poids faible en sortie du module *Charmap*. Ainsi on lit à l'adresse *OLEDCHARMAP_RW* ce bit grâce à la fonction *MemoryRead()* et ce jusqu'à qu'il vaille '1'.
Ensuite une fois le module prêt, pour afficher un caractère, il suffit d'écrire à la bonne adresse (*OLEDCHARMAP_RW*) une trame de 32 bits contenant l'ensemble des informations. On utilise pour cela la fonction *MemoryWrite()*.
L'allure de la trame à envoyer est la suivante:
  * la valeur hexadecimal du caractère sur les bits 7 à 0.
  * la valeur hexadecimal de la ligne sur les bits 10 à 8.
  * la valeur hexadecimal de la colonne sur les bits 19 à 16.

Un exemple d'utilisation pour ce module est donné dans le fichier *main.c* du répertoire *C/rgb_oledcharmap/Sources/*.
Pour faciliter l'écriture de la trame à envoyé, une fonction printCar() a été mise en place, prenant en paramètre le caractère, la ligne et la colonne: *void printCar(char row, char col, char car)*.

**[`^        back to top        ^`](#)**

#### Module Terminal
Le module **Terminal** permet l'affichage sur l'écran Oled-RGB de caractères ASCII en prenant en charge la position. Il est donc plus adapté que le module *Charmap* pour écrire un buffer contenant plusieurs caractères.
Ce module du PMOD Oled-RGB utilise le précedent module *Charmap* et est adressable aux adresses suivantes:
  * READ/WRITE: OLEDTERMINAL_RW   --> 0x400000A4
  * RESET:      OLEDTERMINAL_RST  --> 0x400000AC

De la même manière que pour le module *Charmap*, avant de pouvoir afficher un caractère, il vaut attendre que le module soit prêt à le recevoir. Cela ce traduit par l'activation du bit *Ready* de poids faible en sortie du module *Terminal*. Ainsi on lit à l'adresse *OLEDTERMINAL_RW* ce bit grâce à la fonction *MemoryRead()* et ce jusqu'à qu'il vaille '1'.
Ensuite une fois le module prêt, pour afficher un caractère, il suffit d'écrire à la bonne adresse (*OLEDTERMINAL_RW*) une trame de 32 bits contenant l'ensemble des informations. On utilise pour cela la fonction *MemoryWrite()*.
L'allure de la trame à envoyer est la suivante:
  * la valeur hexadecimal du caractère sur les bits 7 à 0.
  * le bit 24 à '1' pour effacer entièrement l'écran en noir (couleur par défaut): *screen_clear*.

Un exemple d'utilisation pour ce module est donné dans le fichier *main.c* du répertoire *C/rgb_oledterminal/Sources/*.

**[`^        back to top        ^`](#)**

#### Module Bitmap
Le module **Bitmap** du PMOD Oled-RGB est adressable aux adresses suivantes:
  * READ/WRITE: OLEDBITMAP_RW     --> 0x400000B0
  * RESET:      OLEDBITMAP_RST    --> 0x400000B8

Pour ce module, il n'est pas nécessaire d'attendre que l'écran soit prêt avant de lui envoyer la trame. Pour colorier un pixel, il suffit d'écrire directement à la bonne adresse (*OLEDBITMAP_RW*) une trame de 32 bits contenant l'ensemble des informations. On utilise pour cela la fonction *MemoryWrite()*.
L'allure de la trame à envoyer est la suivante:
  * la valeur hexadecimal de la colonne sur les bits 6 à 0.
  * la valeur hexadecimal de la ligne sur les bits 13 à 8.
  * la valeur hexadecimal de la couleur du pixel sur les bits 16 à 31.
On peut cependant raccourcir à 8 bits la trame de la valeur de la couleur du pixel en modifiant la valeur de BPP dans la description VHDL (*plasma.vhd*). Cette valeur correspond à la profondeur colorimétrique et elle est définit par défaut à 16 bpp ce qui équivaut au mode *Highcolor*.

Un exemple d'utilisation pour ce module est donné dans le fichier *main.c* du répertoire *C/rgb_oledbitmap/Sources/*.

**[`^        back to top        ^`](#)**

### Afficheur sept segments

La carte Nexys 4 est équipée de huit afficheurs sept segments, cablés en anode commune. Il est possible d'obtenir un retour d'information sur ces afficheurs. Le bloc VHDL ajouté à l'architecture du plasma pour la gestion des afficheurs sept segments est semblable aux blocs coprocesseur présents dans le PLASMA.

#### Fichiers VHDL

Les différents fichiers VHDL qui décrivent la gestion des afficheurs sept segment sont les suivants :
- `plasma.vhd` dans lequel est instancié le bloc de gestion de l'afficheur sept segments, les entrées/sorties et signaux pilotant le bloc y sont cablés.
- `ctrl_7seg.vhd` bloc principal on les différents sous-blocs nécessaires à l'affichage sont cablés.
- `mod_7seg.vhd`
- `mux_7seg.vhd`

#### Adresse associée au module

Pour intérgir avec les afficheurs sept-segments une adresse est réservée.
- `0x40000200` : adresse de l'entrée sur 32 bits, il faut écrire les données à cette adresse. Le *macro* associé à cette adresse est `SEVEN_SEGMENT`.


#### Schéma du bloc principal **ctrl_7seg.vhd**

<p align="center">
  <img src="SRC/ctrl_7seg.png" width="400px">
</p>


#### Comment ça fonctionne ?

Il suffit d'écrire à l'adresse `SEVEN_SEGMENT`, la valeur en entrée sur 32 bits et elle sera affichée en sortie, en écriture héxadécimal, sur les 8 afficheurs sept-segments disponibles sur la Nexys 4.


#### Programme C

Un programme d'exemple est fournit, il implémente un compteur allant de *0* à *2000* (*7DO* en hexadécimal). Le compteur s'incrémente toute les *100ms*. L'affichage de la valeur du compteur est fait sur les afficheurs sept-segment.

Ensuite le programme rentre dans un boucle infinie dans laquelle il affiche les 16 bits de données *switchs*, à la fois sur les quatres afficheurs de droite, et sur les quatres afficheurs de gauche.

**[`^        back to top        ^`](#)**

### Module de gestion de l'I2C

Les nombreux PMOD *I2C* fournits par Digilent peuvent être interfacés facilement au processeur Plasma via le module *I2C*. Ce module est un hybride en matériel et logiciel. En effet, il est constitué de deux parties :
- Un bloc matériel décrit en *VHDL* qui permet de synchroniser et de gérer bit à bit les émissions/réceptions des signaux qui assurent la communication *I2C*: *SDA* pour les données et *SCL* pour l'horloge.
- Un programme écrit en C bas niveau, qui permet de gérer les séquences d'écriture et de lecture propres au protocole *I2C*.

#### Fichiers sources

Les différents fichiers VHDL qui décrivent la gestion des afficheurs sept segment sont les suivants :
- `plasma.vhd` dans lequel est instancié le bloc VHDL du module *I2C*.
- `i2c.vhd` bloc principal qui contient deux entités **i2c_clock** et **i2c_controller**.
- `i2c.h` fichier d'entête que contient les prototype des fonctions nécessaires pour l'établissement d'une communication *I2C*, ainsi que les macros des adresses et masques des  différents registres.
- `i2c.c` fichier C qui contient les fonctions qui permette de gérer les séquences d'écriture et de lecture du protocole *I2C*.

*NB: Le fichier `main.c` contient un programme d'exemple qui gère une communication avec le capteur PMOD compass*

#### Schéma des blocs de la partie VHDL du module *I2C*

<p align="center">
  <img src="SRC/i2cbloc.png">
</p>

#### Adresses associées au module

- `0x40000300`: adresse du registre qui contient l'adresse de l'esclave ciblé dans une communication *I2C*.
- `0x40000304`: adresse du registre de status du module *I2C*.
- `0x40000308`: adresse du registre de contrôle du module *I2C*.
- `0x4000030c`: adresse du registre de données du module *I2C*.

L'écriture ou la lecture sur l'une de ces adresses active le bloc VHDL du module *I2C*.

**[`^        back to top        ^`](#)**
