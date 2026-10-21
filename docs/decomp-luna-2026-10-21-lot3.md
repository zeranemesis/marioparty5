# Troisième lot GPT-6 Luna — 21 octobre 2026

Trois agents `gpt-6-luna` poursuivent la décompilation USA `GP5E01_00` à partir
du commit `d16bb4f`. Leurs variantes sont compilées séparément, puis le rapport
complet et les empreintes du jeu reconstruit contrôlent l’intégration.

## Résultats

`m537Dll/game_master.cpp` est promue `Matching` : ses onze fonctions étaient
déjà correspondantes, mais ses données et son initialisation ne suffisaient
pas à reproduire le REL. Les paramètres de l’ombre sont désormais stockés dans
l’ordre original, avec le mot de remplissage final. Des lectures `volatile`
empêchent la création de constantes anonymes supplémentaires. L’entrée `.ctors`
référence `fn_1_888C`, qui initialise les limites avant la première image.
La TU ajoute **1 284 octets liés depuis les sources** et le REL est identique.

Dans `m511Dll/score.cpp`, un champ manquant à l’offset 0x0C décalait le pointeur
de minuterie et les champs suivants. Le rétablir fait correspondre entièrement
quatre fonctions : `fn_1_15DE8`, `fn_1_15E1C`, `fn_1_15E48`, `fn_1_15EA0`,
soit **232 octets**. Quatre autres fonctions s’améliorent partiellement.

Dans `dolphin/os/OSThread.c`, `__OSGetEffectivePriority` consulte correctement
le premier thread en attente sur le mutex pour l’héritage de priorité.
L’ordre des opérandes des vérifications de liens de `CheckThreadQueue` est
rétabli. Ces deux fonctions deviennent identiques, soit **216 octets**.
`OSExitThread` suit désormais le test de détachement et les accès de liste du
binaire ; son score monte de 99,40351 % à 99,5614 %. Deux fonctions qui
incorporent du code modifié s’améliorent légèrement. L’unité reste `NonMatching`.

Les essais sur les branchements de `m570Dll/player.c` n’apportaient pas de gain
sans régression ; ils n’ont pas été intégrés. Une promotion exploratoire de
`m516Dll/score.c` échouait au SHA-1 et a été entièrement annulée.

## Structure d’entrée m504

Le header partagé plaçait les champs de lissage après une zone réservée, au
lieu de les intégrer à cette zone. `INPUTWORK` faisait 0x34 octets et
`INPUTMAIN` 0x50 ; le binaire exige respectivement **0x24 et 0x40 octets**.
Les champs de lissage sont replacés à 0x18 et 0x20 dans `INPUTWORK`. Aucun
second type local incompatible n’est introduit.

Cette correction suffit à faire correspondre les quatre fonctions de
`m504dll/input.c`, dont trois nouvelles fonctions exactes, ainsi que quatre
fonctions de `m504dll/player.c`. La TU `input` est promue `Matching`, soit
**1 068 octets liés depuis les sources**. Le REL reconstruit est identique.

Dans `com.c`, le lissage utilise désormais ses champs flottants, puis écrit
les octets de stick dans `INPUTMAIN`. `fn_1_BEF0` progresse de 49,635803 % à
81,049385 %. Les parties encore différentes de cette routine restent à traiter.

## Progression et validation

- Code correspondant : **2 850 500 / 9 481 492 octets — 30,06 %**, soit +1 976.
- Fonctions correspondantes : **7 873 / 21 157**, soit +13.
- Code lié depuis les sources : **1 983 280 / 9 481 492 — 20,92 %**, soit +2 352.
- Unités intégrées : **351 / 881**, soit +2.

La comparaison des scores de toutes les fonctions ne montre aucune régression.
Le DOL et les 134 REL reconstruits passent les **135 contrôles SHA-1**.
Le port Windows compile également ; son exécution n’a pas été testée.
Les empreintes attendues et les originaux n’ont pas été modifiés. Les rapports
et variantes restent localement sous `build/luna-2026-10-21/lot3/`.
