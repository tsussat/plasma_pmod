# Tutoriels

- [Introduction](#introduction)
- [Configuration générale](#configuration-générale)
- [Première simulation](#première-simulation)
- [Compilation du bootloader et synthèse du bitstream Plasma](#compilation-du-bootloader-et-synthèse-du-bitstream-plasma)
- [Compilation du projet](#compilation-du-projet)
- [Port série et chargement du binaire du projet par UART](#port-série-et-chargement-du-binaire-du-projet-par-uart)
- [Projets](#projets)
	- [Buttons](#buttons)
	- [Switch/LEDs](#switchleds)
	- [Seven Segments](#seven-segments)
	- [I2C](#i2c)
	- [RGB OLED](#rgb-oled)

## Introduction

Cette série de tutoriels permet de prendre en main le processeur Plasma sur les cartes Nexys 4, au travers des grandes étapes incontournables :

1. Compilation du bootloader
1. Synthèse du bitstream Plasma (incluant le bootloader)
2. Compilation du projet
3. Programmation du bitstream Plasma par JTAG
4. Programmation du binaire du projet par UART

## Configuration générale

On va dans un premier temps configurer la machine hôte pour l'utilisation de *Vivado* en ligne de commande. On prendra soin d'entrer les commandes suivantes dans le terminal de travail (celui on on exécuter *make* par la suite) :

> PATH="$PATH:/net/ens/cleroux/gcc_mips/gnu_toolchain-1.5.4-Linux-i686/bin/"  
> PATH="$PATH:/opt/modelsim/modeltech/bin/:/opt/mentor/modelsim_se/10.3d/modeltech/linux_x86_64/"  
> PATH="$PATH:/opt/xilinx/vivado/2015.2/Vivado/2015.2/bin"  
> export PATH
>
> LM_LICENSE_FILE=2100@johnny  
> export LM_LICENSE_FILE

Il s'agit de spécifier les chemins d'accès aux outils utilisés ainsi que le serveur de licence pour Vivado.

Pour la suite, on se placera dans le dossier racine du projet Plasma, qui contient le *Makefile*.

## Première simulation

On va tout d'abord effectuer la simulation du design matériel et du projet de démonstration par défaut `hello`.

Il s'agit dans un premier temps de compiler l'application pour le processeur MIPS du Plasma puis de lancer la simulation, qui intègre le binaire du projet au sein du design. On effectue la première étape :
> make project

Puis on lance la simulation :
> make simulation

Une fois la simulation terminée, on voit apparaître sur l'écran la chaîne `Hello world !`.

**Astuce** : La première étape est automatiquement exécutée par l'appel à la cible `simulation`, on peut donc se contenter de la première étape.

**Astuce** : On peut sélectionner le projet à intégrer en passant `CONFIG_PROJECT` à la cible `simulation` de *make*, par exemple :
> make simulation CONFIG_PROJECT=buttons

## Compilation du bootloader et synthèse du bitstream Plasma

Le bootloader est le composant logiciel qui sera intégré au sein du design matériel du SoC Plasma pour lui permettre d'exécuter des applications sans synthétiser de bitstream spécifique à chaque projet. Il s'agit d'un composant en charge de lire le programme depuis le contrôleur UART (relié au travers de l'USB à l'ordinateur hôte), de le stocker en mémoire et de l'exécuter. Le bootloader est exécuté directement au démarrage par le processeur du Plasma et il est stocké en mémoire morte (ROM), où il ne peut plus être modifié (on parle de **BootROM**).

On peut compiler le bootloader avec la cible `boot_loader` de *make* :
> make boot_loader

Une fois la compilation terminée, on peut observer les fichiers temporaires créés dans le répertoire `OBJ/boot_loader/`. En particulier, c'est le fichier `code_bin.txt` présent dans ce répertoire qui sera utilisé pour la synthèse du Plasma.

On va alors effectuer les étapes permettant d'aboutir à la génération du bitstream dans Vivado:

1. Synthèse du design
2. Optimisation du design
3. Implémentation du design
4. Optimisation physique du design
5. Routage du design
6. Édition du bitstream

Ces étapes sont regroupées au travers de la cible `plasma` de *make* :
> make plasma

Par défaut, le bitstream est généré pour la cible Nexys 4-DDR. Pour générer le bitstream pour la cible Nexys 4, on spécifie `CONFIG_TARGET=nexys4` en argument à *make*:
> make plasma CONFIG_TARGET=nexys4

Cette étape, particulièrement longue, doit être répétée pour chaque modification apportée au design. Il n'est cependant pas nécessaire de la répéter pour des modifications apportées au projet logiciel, grâce à l'utilisation du bootloader.

**Astuce** : La première étape (compilation du bootloader) est appelée automatiquement quand nécessaire par la gestion des dépendances de la règle `plasma` et il est donc dans tous les cas suffisant d'appeler :
> make plasma

## Compilation du projet

On va maintenant compiler le projet que l'on souhaite exécuter sur le Plasma. Comme pour la simulation, le projet par défaut est `hello`.

On appelle *make* avec la cible `project` pour compiler le projet par défaut :
> make project

**Astuce** : On peut sélectionner le projet à compiler en passant `CONFIG_PROJECT` à la cible `project` de *make*, par exemple :
> make project CONFIG_PROJECT=buttons

**Astuce** : Les projets disposent de cibles dédiées que l'on peut également appeler directement, par exemple :
> make buttons

## Programmation du bitstream Plasma par JTAG

Le bitstream ayant été généré, on peut le programmer sur la carte en utilisant le *Hardware Manager* de *Vivado*. Le protocole utilisé pour configurer le circuit logique est le JTAG, qui est accessible depuis le port USB de la carte.

Pour cela, on sélectionne le fichier `BIN/plasma.bit` comme bitstream à charger.

Alternativement, on peut utiliser le logiciel *OpenOCD* qui permet de même de configurer le FPGA par JTAG et dont l'utilisation est intégrée par la commande :
> make flash

Une fois la programmation de la configuration logique terminée, il faut **impérativement** réinitialiser le CPU par un appui sur le bouton rouge marqué *CPU RESET* sur la carte. Un oubli peut laisser des signaux internes non-initialisés et causer des erreurs imprévisibles.

## Port série et chargement du binaire du projet par UART

La FPGA de la carte est maintenant correctement programmé et exécute le bootloader au démarrage.

On peut observer la sortie de l'exécution par le port série UART du Plasma, qui est mis à disposition au travers du connecteur USB. Pour cela, on ouvre un hyperterminal que l'on configure en `115200-8-N-1`.

Comme l'exécution a déjà débuté, aucun message ne s'affiche. Un appui sur le bouton *CPU RESET* de la carte permet de redémarrer l'exécution et ainsi d'afficher la sortie du bootloader.

Le bootloader est maintenant en attente de données depuis le port série. On va y envoyer le programme choisi, avec la cible `send` du *Makefile* :
> make send

On pourra au besoin ajuster la variable `CONFIG_SERIAL` pour indiquer un port série différent.

Une fois l'envoi termine, l'exécution du programme débute et on en observe la sortie sur le port série.

## Projets

### Buttons

### Switch/LEDs

### Seven Segments

### I2C

### RGB OLED
