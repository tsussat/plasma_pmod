# Documentation

<p align="center">
  <img src="SRC/plasma.gif">
</p>

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
Les outils nécessaire pour l'ajout de nouveaux PMODs au processeur Plasma sont les suivants:
* Les outils cross-compilés pour une architecture MIPS-elf: `mips-elf-gcc`, `mips-elf-as`, `mips-elf-ld`.
* Le logiciel `Vivado` pour compiler le code VHDL.


## Utilisation

## Ajouter un PMOD supplémentaire

## PMOD Oled-RGB

<p align="center">
  <img src="SRC/OLEDrgb.png">
</p>

Le PMOD Oled-RGB permet l'affichage de caractères ASCII sous 8 lignes X 16 colonnes. Il permet aussi de réalisé un affichage Bitmap sous 96X64 avec 16 bits/pixel. Un module d'affichage de jusqu'à 4 courbes a également été instancié (non testé encore).
L'ajout des divers modules de ce PMOD au projet repose sur le travail de Mr. Bornat détaillé à l'adresse suivante: http://bornat.vvv.enseirb.fr/wiki/doku.php?id=en202:pmodoledrgb.

## Afficheur sept segments

La carte Nexys 4 est équipée de huit afficheurs sept segments, cablés en anode commune. Il est possible d'obtenir un retour d'information sur ces afficheurs. Le bloc VHDL ajouté à l'architecture du plasma pour la gestion des afficheurs sept segments est semblable aux blocs coprocesseur présents dans le PLASMA.

### Fichiers VHDL :

Les différents fichiers VHDL qui décrivent la gestion des afficheurs sept segment sont les suivants :
- `plasma.vhd` dans lequel est instancié le bloc de gestion de l'afficheur sept segments, les entrées/sorties et signaux pilotant le bloc y sont cablés.
- `ctrl_7seg.vhd` bloc principal on les différents sous-blocs nécessaires à l'affichage sont cablés.
- `mod_7seg.vhd`
- `mux_7seg.vhd`

### Adresse associé au module :

Pour intérgir avec les afficheurs sept-segments une adresse est réservée.
- `0x40000200` : adresse de l'entrée sur 32 bits, il faut écrire les données à cette adresse. Le *macro* associé à cette adresse est `SEVEN_SEGMENT`.


### Schéma du bloc principal **ctrl_7seg.vhd** :

<p align="center">
  <img src="SRC/ctrl_7seg.png" width="400px">
</p>


### Comment ça fonctionne ?

Il suffit d'écrire à l'adresse `SEVEN_SEGMENT`, la valeur en entrée sur 32 bits et elle sera affichée en sortie, en écriture héxadécimal, sur les 8 afficheurs sept-segments disponibles sur la Nexys 4.


### Programme C :

Un programme d'exemple est fournit, il implémente un compteur allant de *0* à *2000* (*7DO* en hexadécimal). Le compteur s'incrémente toute les *100ms*. L'affichage de la valeur du compteur est fait sur les afficheurs sept-segment.

Ensuite le programme rentre dans un boucle infinie dans laquelle il affiche les 16 bits de données *switchs*, à la fois sur les quatres afficheurs de droite, et sur les quatres afficheurs de gauche.

## Module de gestion de l'I2C :

Les nombreux PMOD *I2C* fournits par Digilent peuvent être interfacés facilement au processeur Plasma via le module *I2C*. Ce module est un hybride en matériel et logiciel. En effet, il est constitué de deux parties :
- Un bloc matériel décrit en *VHDL* qui permet de synchroniser et de gérer bit à bit les émissions/réceptions des signaux qui assurent la communication *I2C*: *SDA* pour les données et *SCL* pour l'horloge. 
- Un programme écrit en C bas niveau, qui permet de gérer les séquences d'écriture et de lecture propres au protocole *I2C*.

### Fichiers sources :

Les différents fichiers VHDL qui décrivent la gestion des afficheurs sept segment sont les suivants :
- `plasma.vhd` dans lequel est instancié le bloc VHDL du module *I2C*.
- `i2c.vhd` bloc principal qui contient deux entités **i2c_clock** et **i2c_controller**.
- `i2c.h` fichier d'entête que contient les prototype des fonctions nécessaires pour l'établissement d'une communication *I2C*, ainsi que les macros des adresses et masques des  différents registres.
- `i2c.c` fichier C qui contient les fonctions qui permette de gérer les séquences d'écriture et de lecture du protocole *I2C*.

*NB: Le fichier `main.c` contient un programme d'exemple qui gère une communication avec le capteur PMOD compass*

### Schéma des blocs de la partie VHDL du module *I2C*:

<p align="center">
  <img src="SRC/i2cbloc.png">
</p>

### Adresses associées au module :

- `0x40000300`: adresse du registre qui contient l'adresse de l'esclave ciblé dans une communication *I2C*.
- `0x40000304`: adresse du registre de status du module *I2C*.
- `0x40000308`: adresse du registre de contrôle du module *I2C*.
- `0x4000030c`: adresse du registre de données du module *I2C*.

L'écriture ou la lecture sur l'une de ces adresses active le bloc VHDL du module *I2C*.



