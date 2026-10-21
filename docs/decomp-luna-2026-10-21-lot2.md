# Deuxième lot GPT-6 Luna — 21 octobre 2026

Trois agents `gpt-6-luna` poursuivent la décompilation USA `GP5E01_00` à partir
du commit `54623a9`. Les variantes sont compilées isolément, puis intégrées et
comparées à l’ensemble du rapport précédent.

## Fonctions reconstruites

- `msmSysDelGroupBase` : 760 octets, de 99,63158 % à 100 %. La portée locale du
  pointeur de groupe dans les deux boucles de la première branche reproduit
  l’allocation des registres. Les boucles de l’autre branche restent inchangées.
- `PSVECNormalize` : 68 octets, de 94,11765 % à 100 %. Ajout du retour `blr`
  manquant dans le bloc assembleur déjà présent.
- `C_VECReflect` : 212 octets, de 90,9434 % à 100 %. Décomposition des calculs
  pour reproduire l’ordre des opérations flottantes et leurs registres.
- `C_VECHalfAngle` : 216 octets, de 72,03704 % à 100 %. Sens du test et copie
  par mots de la branche dégénérée conformes au code original.

Les douze fonctions de `dolphin/mtx/vec.c` correspondent maintenant entièrement.
Le score de comparaison des instructions ne suffit cependant pas à promouvoir
une unité : la liaison doit aussi reproduire ses données et ses relocalisations.

Les essais sur `__construct_array` et `msmSysLoadGroup` n’apportaient aucun gain ;
ils n’ont pas été intégrés.

## Unités intégrées

`OdemuExi2/DebuggerDriver.c` avait déjà ses 14 fonctions correspondantes. Leur
ordre dans la section de code était cependant inversé : la comparaison par
symbole masquait ce problème. Le réordonnancement des définitions dans la
branche USA rétablit la section `.text` exacte, sans changer les corps.
L’unité est promue `Matching` : **2 688 octets supplémentaires liés depuis les
sources**, avec validation du DOL final.

`dolphin/mtx/vec.c` est également promue `Matching` : **904 octets liés depuis
les sources**. Le retrait de `<math.h>` élimine deux constantes doubles inutiles.
Une constante zéro globale, chargée à travers un pointeur `volatile` dans le
test de `C_VECHalfAngle`, empêche le compilateur d’en créer une deuxième copie
et rétablit l’ordre exact `[0, 0.5, 3, 2]` des 16 octets de `.sdata2`.
Le DOL reconstruit avec cette unité passe son SHA-1 original.

## Progression et validation

- Code correspondant : **2 848 524 / 9 481 492 octets — 30,04 %**, soit +1 256.
- Fonctions correspondantes : **7 860 / 21 157**, soit +4.
- Code lié depuis les sources : **1 980 928 / 9 481 492 — 20,89 %**.
- Unités intégrées : **349 / 881**, soit +2.

La comparaison des scores de toutes les fonctions ne montre aucune régression.
Le DOL et les 134 REL reconstruits passent les **135 contrôles SHA-1**.
Le port Windows compile également ; son exécution n’a pas été testée.

La promotion de `m575dll/tuboeff.c` échouait au SHA-1 malgré un score de code
à 100 % : elle a été annulée. Cette unité garde douze octets supplémentaires
dans son pool de constantes. Aucune empreinte attendue ni aucun original n’a
été modifié. Les rapports et variantes restent localement sous
`build/luna-2026-10-21/lot2/`.
