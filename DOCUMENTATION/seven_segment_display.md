<!-- Auteur : Henri, Version 0.1 -->

Documentation : Afficheur sept segments
=======================================

La carte Nexys 4 est équipée de huit afficheurs sept segments, cablés en anode commune. Il est possible d'obtenir un retour d'information sur ces afficheurs. Le bloc VHDL ajouté à l'architecture du plasma pour la gestion des afficheurs sept segments est semblable aux blocs coprocesseur présents dans le PLASMA.

Fichiers VHDL :
---------------

Les différents fichiers VHDL qui décrivent la gestion des afficheurs sept segment sont les suivants :
- `plasma.vhd` dans lequel est instancié le bloc de gestion de l'afficheur sept segments, les entrées/sorties et signaux pilotant le bloc y sont cablés.
- `ctrl_7seg.vhd` bloc principal on les différents sous-blocs nécessaires à l'affichage sont cablés.
- `mod_7seg.vhd`
- `mux_7seg.vhd`

Adresse du bloc :
------------------
Pour intérgir avec les afficheurs sept-segments une adresse est réservée.
- `0x40000200` : adresse de l'entrée sur 32 bits, il faut écrire les données à cette adresse. Le *macro* associé à cette adresse est `SEVEN_SEGMENT`.


Schéma du bloc principal **ctrl_7seg.vhd** :
-------------------------------------------
<p align="center">
  <img src="SRC/ctrl_7seg.png">
</p>


Comment ça fonctionne ?
-----------------------
Il suffit d'écrire à l'adresse `SEVEN_SEGMENT`, la valeur en entrée sur 32 bits et elle sera affichée en sortie, en écriture héxadécimal, sur les 8 afficheurs sept-segments disponibles sur la Nexys 4.


Programme C :
-------------
Un programme d'exemple est fournit, il implémente un compteur allant de *0* à *2000* (*7DO* en hexadécimal). Le compteur s'incrémente toute les *100ms*. L'affichage de la valeur du compteur est fait sur les afficheurs sept-segment.

Ensuite le programme rentre dans un boucle infinie dans laquelle il affiche les 16 bits de données *switchs*, à la fois sur les quatres afficheurs de droite, et sur les quatres afficheurs de gauche.