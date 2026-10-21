# Décompilation avec GPT-6 Luna — 21 octobre 2026

Trois agents utilisant explicitement `gpt-6-luna` ont travaillé sur MP5 USA
`GP5E01_00`, en compilant des variantes isolées. Base du lot : `e48f662`.

## Résultats

| Cible | Avant | Après | Statut final |
|---|---:|---:|---|
| `m547dll/game.c` | 17/18 fonctions identiques | 18/18 et données identiques | `Matching`, REL identique |
| `fn_1_5DC4`, 2 376 octets | 80,65151 % fuzzy | 100 % | unité ci-dessus intégrée |
| Destructeur `std::bad_exception`, 92 octets | 65,21739 % fuzzy | 100 % | runtime encore `NonMatching` |
| `SLSaveSdBExec`, 2 308 octets | 99,65338 % fuzzy | 99,7227 % | amélioration partielle |

Deux fonctions supplémentaires correspondent entièrement. La promotion de
`m547dll/game.c` intègre aussi ses 17 fonctions déjà correspondantes :
**17 956 octets supplémentaires sont liés réellement depuis les sources**.

Les corrections du mini-jeu rétablissent les états du contrôleur, les masques
de bits, un mouvement sinusoïdal et des appels omis. Les strides des modèles et
des paires de coordonnées reproduisent les accès originaux. Les valeurs,
l'ordre et le placement des données ont été reconstruits : `.text` 0x4624,
`.rodata` 0x160, `.data` 0xD0 et `.bss` 0x50. Le REL final est identique.

Le destructeur vide inline de `std::exception` permet de reproduire celui de
la classe dérivée. Les 13 fonctions de `Gecko_ExceptionPPC` correspondent,
mais le pool de littéraux et des métadonnées d'exception diffèrent encore.
Les promotions essayées échouaient au SHA-1 : elles ont été annulées. Aucun
contrôle ni fichier original n'a été modifié.

Le type signé de `boxOfs` dans `SLCheckSumBoxSet` améliore l'allocation des
registres après inlining dans `SLSaveSdBExec`. Les offsets utilisés restent
positifs et dans leur plage valide. Les variantes qui régressaient ont été
écartées.

## Progression globale

- Code correspondant : **2 847 268 / 9 481 492 octets — 30,03 %** (+2 468).
- Code lié depuis les sources : **1 977 336 / 9 481 492 — 20,85 %**.
- Unités intégrées : **347 / 881**, contre 346.
- Fonctions correspondantes : **7 856 / 21 157**, contre 7 854.

La comparaison et la liaison réelle restent distinctes. Les deux unités non
promues ne sont pas comptées comme des TU nouvellement intégrées.

## Validation

Les trois objets touchés ont été supprimés puis recompilés. `ninja -j 4`
reconstruit le DOL et les REL et termine par **135 files OK**. La comparaison
des scores de toutes les fonctions avant/après ne montre aucune régression.
Le port Windows compile également avec ses avertissements existants ; son
exécution et sa jouabilité n'ont pas été testées.

Variantes, rapports et journaux sont conservés localement sous
`build/luna-2026-10-21/`. Les fichiers du jeu et les objets extraits restent
exclus de la publication.
