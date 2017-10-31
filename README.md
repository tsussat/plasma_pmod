# plasma_pmod

Ce dépôt git est dédié à la modification (matérielle et logicielle) du processeur softcore Plasma (disponible sur OpenCores.org) afin de pouvoir gérer des modules PMOD sur les cartes de développement Nexys-4 de Digilent.

Ce fichier permet de documenter le projet

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
