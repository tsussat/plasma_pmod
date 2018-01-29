# Documentation

![Plasma](SRC/plasma.gif)


## Introduction
Cette documentation a pour objectif de détailler le principe de fonctionnement de l'architecture du processeur Plasma lors de son utilisation avec divers PMODs. Ce processeur a été instancié sur une puce FPGA *Artix 7* embarquée sur carte *NEXYS 4*. Il est basé sur une architecture RISC 32-bit softcore et est issu d'un projet open source: [**Plasma**](http://opencores.org/project,plasma). L'utilisation des PMODs repose sur une interface plus flexible codée en C et faisant abstraction du langage de description VHDL.

Ce travail a été réalisé dans le cadre d'un projet proposé par Camille Leroux en option SE et développé par Paul Kocialcowski, Henri Gouttard et Igor Papandinas.

![Architecture](SRC/architecture.gif)

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

## Manuel d'utilisation de certains PMODs

### PMOD Oled-RGB

![OledRGB](SRC/OledRGB.png)

Le PMOD Oled-RGB permet l'affichage de caractères ASCII sous 8 lignes X 16 colonnes. Il permet aussi de réalisé un affichage Bitmap sous 96X64 avec 16 bits/pixel. Un module d'affichage de jusqu'à 4 courbes a également été instancié (non testé encore).
L'ajout des divers modules de ce PMOD au projet repose sur le travail de Mr. Bornat détaillé à l'adresse suivante: http://bornat.vvv.enseirb.fr/wiki/doku.php?id=en202:pmodoledrgb.
