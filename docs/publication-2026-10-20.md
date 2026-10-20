# Analyse et publication — 20 octobre 2026

## Projet analysé

`G:\decomp\MP5`, branche locale `port/pal-gp5p01-config`, base avant
publication `a44bcfd`. Cette base contient 460 commits absents de `origin/main`
(`7b3163e`). Le travail comprend une décompilation GameCube de MP5 et un port
natif utilisant Aurora, SDL3, MusyX et libco.

## Résultats vérifiés

| Mesure USA `GP5E01_00` | Résultat |
|---|---:|
| Code correspondant | 2 844 800 / 9 481 492 octets — 30,00 % |
| Code lié depuis les sources | 1 959 380 / 9 481 492 octets — 20,67 % |
| Unités complètes | 346 / 881 |
| Fonctions correspondantes | 7 854 / 21 157 |
| Code DOL correspondant | 85,50 % |
| Code REL correspondant | 17,61 % |
| Reconstructions SHA-1 identiques | 135 / 135 |

Le DOL reconstruit a pour SHA-1
`ba946b5ba8d6c9e6e6c378f8e3215cbb8ae5ffc9`. La réussite du contrôle du DOL et des
134 REL ne signifie pas une décompilation terminée : les unités inachevées sont
encore liées depuis leurs objets extraits.

Le port Windows a compilé avec MSVC : exécutable, DOL natif et 47 modules REL
activés. Le démarrage et la jouabilité n'ont pas été testés. Des avertissements
restent présents, notamment des variables non initialisées dans m519 et un
chemin sans valeur de retour dans m524. Le statut `Equivalent` est un statut de
construction du port, pas une preuve de correction de son comportement.

La configuration européenne `GP5P01_00` reste expérimentale. Les 135 fichiers
originaux locaux correspondent aux empreintes déclarées, mais une reconstruction
PAL identique n'a pas été validée. Le manifeste `build.sha1`, auparavant vide,
contient maintenant les 135 empreintes attendues pour éviter un contrôle vide.

## Préparation de la publication

- Sources existantes et quatre nouveaux fichiers source référencés par
  `configure.py`, plus l'en-tête m571, inclus dans la publication.
- Configuration PAL, documentation de progression et outils de mesure utiles
  conservés ; ressources et images du jeu exclues.
- Cinq blocs d'assembleur expérimentaux, copiés depuis des désassemblages et
  rejetés par MWCC, retirés de la fin de `world04.c`. L'unité reste
  `NonMatching`. Le fichier complet original a été sauvegardé localement dans
  `build/publication-2026-10-20/world04.c.unpublished` ; les autres brouillons
  locaux restent présents.
- MusyX pointe vers le commit public
  `dc56c921528821209a8b53029114e5443435f40d`. Les cinq modifications locales sont
  enregistrées dans `patches/musyx-mp5.patch`, dont l'application inverse a été
  contrôlée. Aucun push vers le dépôt tiers n'est nécessaire.
- L'historique des 460 commits locaux a été inspecté par liste des chemins :
  sources et documentation, aucun DOL, REL, image disque ni objet compilé.
- Les fichiers originaux et produits de compilation sont exclus de Git.

## Vérifications exécutées

```text
python configure.py --version GP5E01_00 --dtk build/tools/dtk.exe
  --objdiff build/tools/objdiff-cli.exe --sjiswrap build/tools/sjiswrap.exe
  --binutils build/binutils --compilers build/compilers
ninja -j 4
# 135 files OK
python tools/avancement.py
python tools/build_port.py --jobs 4
```

La première commande est présentée sur trois lignes pour la lisibilité et doit
être exécutée sur une seule ligne. Outils utilisés : DTK 1.5.1, objdiff-cli 3.3.1,
Python 3.13.14 et Ninja 1.13.2. Les journaux complets de cette analyse sont
conservés localement dans `build/publication-2026-10-20/`.
