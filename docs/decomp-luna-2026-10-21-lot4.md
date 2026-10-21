# Quatrième lot GPT-6 Luna — 21 octobre 2026

Trois agents `gpt-6-luna` poursuivent la décompilation USA `GP5E01_00` à partir
du commit `6392672`. Les variantes sont compilées isolément ; l’intégration
est contrôlée avec le rapport entier et les empreintes du jeu reconstruit.

## Neuf nouvelles fonctions correspondantes

| Cible | Octets | Avant fuzzy | Après |
|---|---:|---:|---:|
| `safDll/SafBallMatHook` | 1 036 | 99,938225 % | 100 % |
| `m511Dll/fn_1_F2C8` | 436 | 99,944954 % | 100 % |
| `m574Dll/fn_1_34B0` | 596 | 99,59731 % | 100 % |
| Six wrappers de `mgtourDll` | 140 | 99,8–99,833336 % | 100 % |

`SafBallMatHook` utilise cinq matrices locales de 48 octets : leur ordre de
déclaration reproduit désormais les emplacements de pile attendus. Dans
`fn_1_F2C8`, une matrice 4×4 de 64 octets est remplacée par le type `Mtx`,
soit 3×4 et 48 octets ; cela rétablit la taille originale de la pile.

Dans `fn_1_34B0`, le masque `0xE0000000` est remplacé par `~2`. L’instruction
originale `rlwinm r0,r0,0,31,29` conserve tous les bits sauf le bit 1 : le
nouveau masque rétablit aussi le comportement observé.

Le header partagé de `mgtourDll` corrige deux tailles de tableaux réservés :
`MGTFlow.padEA` passe de cinq à sept éléments `s16`, et `MGT_MODEWORK.order`
de quinze à onze octets. Les champs suivants retrouvent leurs offsets. Six
fonctions deviennent exactes : `fn_1_660`, `fn_1_7E4`, `fn_1_83C`, `fn_1_88C`,
`fn_1_96C`, `fn_1_984`. Aucun second type local incompatible n’est introduit.

Les sept variantes ciblées de `m518dll/m518.c` n’amélioraient pas la pile,
les registres ou les opérations flottantes ; elles ne sont pas intégrées.
`m530Dll/player.c` conserve ses 22 fonctions identiques, mais deux constantes
doubles provenant du `sqrtf` inline des headers ajoutent 16 octets à `.rodata`.
Les cinq variantes d’inclusions testées ne les éliminent pas. La TU n’est pas
promue et ces essais ne sont pas intégrés.

La promotion exploratoire de `m510dll/player.c` échouait au SHA-1 et a été
annulée. Aucune empreinte attendue ni aucun original n’a été modifié.

## Progression et validation

- Code correspondant : **2 852 708 / 9 481 492 octets — 30,09 %**, soit +2 208.
- Fonctions correspondantes : **7 882 / 21 157**, soit +9.
- Code lié depuis les sources : **1 983 280 / 9 481 492 — 20,92 %**, inchangé.
- Unités intégrées : **351 / 881**, inchangé.

Les gains portent sur des fonctions au sein de TU encore partielles ; aucune
nouvelle TU n’est déclarée entièrement intégrée. La comparaison des scores de
toutes les fonctions ne montre aucune régression. Le DOL et les 134 REL
reconstruits passent les **135 contrôles SHA-1**. Le port Windows compile ;
son exécution n’a pas été testée. Variantes et rapports restent localement
sous `build/luna-2026-10-21/lot4/`.
