# Avancement de la décompilation

Généré le 2026-09-23 09:37 par `tools/avancement.py` depuis `build/GP5E01_00/report.json`.
Ne pas éditer à la main : relancer `ninja` puis le script.

| mesure | valeur |
|---|---|
| code identique | **23.56 %** (2 183.6 / 9 266.8 Ko) |
| code lié depuis les sources | 18.65 % |
| fonctions identiques | 5071 / 21231 |
| unités complètes | 315 / 881 |

Statuts des unités dans `configure.py` : **M** `Matching` (identique, liée), **E** `Equivalent` (complète, pas identique : entre dans le port), **N** `NonMatching` (incomplète ou absente : hors du port).

## Par famille

| famille | modules | code | identique | reste |
|---|---|---|---|---|
| DOL (moteur et bibliothèques) | 1 | 1 697.5 Ko | 82.3 % | 300.3 Ko |
| Mini-jeux m5xx | 76 | 4 493.9 Ko | 3.0 % | 4 358.3 Ko |
| Mini-jeux repris de MP4 (m4xx) | 1 | 77.0 Ko | 100.0 % | 0.0 Ko |
| Plateaux du mode Histoire (w0Xs) | 7 | 210.1 Ko | 7.3 % | 194.7 Ko |
| Plateaux (w0X, w10, w20) | 9 | 383.2 Ko | 41.4 % | 224.4 Ko |
| Modes (md*, mg*) | 12 | 1 033.6 Ko | 20.1 % | 826.1 Ko |
| Système, menus, résultats, histoire, Super Duel, E3 | 22 | 1 371.5 Ko | 14.0 % | 1 179.3 Ko |

## Par module

### DOL (moteur et bibliothèques)

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `main` | 1 697.5 Ko | 82.3 % | 3635/6158 | 85.1 % | 220/4/86 | en cours |

### Mini-jeux m5xx

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `m501Dll` | 65.1 Ko | 1.3 % | 16/319 | 1.3 % | 2/0/9 | en cours |
| `m502dll` | 55.7 Ko | 0.0 % | 0/123 | 0.0 % | 0/0/9 | à faire |
| `m503Dll` | 56.1 Ko | 0.0 % | 0/148 | 0.0 % | 0/0/8 | à faire |
| `m504dll` | 74.4 Ko | 0.3 % | 3/116 | 0.3 % | 1/0/10 | en cours |
| `m505Dll` | 72.4 Ko | 0.0 % | 0/19 | 0.0 % | 0/0/1 | à faire |
| `m506Dll` | 59.0 Ko | 52.8 % | 52/96 | 87.5 % | 1/2/0 | fait (E) |
| `m507dll` | 33.7 Ko | 100.0 % | 47/47 | 100.0 % | 2/0/0 | fait |
| `m508Dll` | 57.4 Ko | 0.0 % | 0/245 | 0.0 % | 0/0/2 | à faire |
| `m509Dll` | 76.1 Ko | 1.3 % | 18/284 | 1.3 % | 2/0/8 | en cours |
| `m510dll` | 53.1 Ko | 0.4 % | 3/106 | 0.4 % | 1/0/12 | en cours |
| `m511Dll` | 116.4 Ko | 0.5 % | 11/439 | 0.5 % | 1/0/16 | en cours |
| `m512Dll` | 68.3 Ko | 0.9 % | 11/310 | 0.9 % | 1/0/9 | en cours |
| `m513Dll` | 83.4 Ko | 0.7 % | 11/368 | 0.7 % | 1/0/15 | en cours |
| `m514Dll` | 38.3 Ko | 0.0 % | 0/127 | 0.0 % | 0/0/6 | à faire |
| `m515Dll` | 57.7 Ko | 0.0 % | 0/84 | 0.0 % | 0/0/1 | à faire |
| `m516Dll` | 35.8 Ko | 0.0 % | 0/113 | 0.0 % | 0/0/7 | à faire |
| `m517Dll` | 112.2 Ko | 0.5 % | 11/420 | 0.5 % | 1/0/14 | en cours |
| `m518dll` | 58.5 Ko | 0.0 % | 0/122 | 0.0 % | 0/0/1 | à faire |
| `m519dll` | 65.5 Ko | 0.0 % | 0/142 | 0.0 % | 0/0/1 | à faire |
| `m520Dll` | 39.7 Ko | 0.0 % | 0/29 | 0.0 % | 0/0/1 | à faire |
| `m521Dll` | 99.2 Ko | 1.0 % | 14/365 | 1.0 % | 2/0/9 | en cours |
| `m522Dll` | 86.2 Ko | 0.7 % | 11/398 | 0.7 % | 1/0/8 | en cours |
| `m523Dll` | 38.5 Ko | 0.0 % | 0/111 | 0.0 % | 0/0/2 | à faire |
| `m524Dll` | 23.9 Ko | 0.0 % | 0/76 | 0.0 % | 0/0/1 | à faire |
| `m525Dll` | 49.8 Ko | 0.0 % | 0/18 | 0.0 % | 0/0/1 | à faire |
| `m526Dll` | 58.9 Ko | 100.0 % | 184/184 | 100.0 % | 2/0/0 | fait |
| `m527Dll` | 52.7 Ko | 0.0 % | 0/16 | 0.0 % | 0/0/1 | à faire |
| `m528Dll` | 51.4 Ko | 0.0 % | 0/103 | 0.0 % | 0/0/1 | à faire |
| `m529dll` | 29.2 Ko | 0.0 % | 0/54 | 0.0 % | 0/0/1 | à faire |
| `m530Dll` | 38.2 Ko | 0.0 % | 0/133 | 0.0 % | 0/0/4 | à faire |
| `m531Dll` | 69.9 Ko | 2.3 % | 20/276 | 2.3 % | 3/0/11 | en cours |
| `m532Dll` | 78.2 Ko | 0.8 % | 11/414 | 0.8 % | 1/0/10 | en cours |
| `m533dll` | 32.8 Ko | 0.0 % | 0/72 | 0.0 % | 0/0/2 | à faire |
| `m534Dll` | 29.3 Ko | 0.0 % | 0/94 | 0.0 % | 0/0/6 | à faire |
| `m535Dll` | 47.7 Ko | 0.0 % | 0/19 | 0.0 % | 0/0/1 | à faire |
| `m536Dll` | 65.5 Ko | 0.9 % | 11/328 | 0.9 % | 1/0/9 | en cours |
| `m537Dll` | 93.2 Ko | 0.6 % | 11/382 | 0.6 % | 1/0/9 | en cours |
| `m538Dll` | 44.5 Ko | 0.0 % | 0/89 | 0.0 % | 0/0/1 | à faire |
| `m539Dll` | 68.5 Ko | 0.9 % | 11/333 | 0.9 % | 1/0/11 | en cours |
| `m540Dll` | 86.1 Ko | 0.0 % | 0/226 | 0.0 % | 0/0/1 | à faire |
| `m541Dll` | 105.7 Ko | 0.0 % | 0/92 | 0.0 % | 0/0/3 | à faire |
| `m542dll` | 37.2 Ko | 0.0 % | 0/58 | 0.0 % | 0/0/3 | à faire |
| `m543Dll` | 65.7 Ko | 0.0 % | 0/91 | 0.0 % | 0/0/3 | à faire |
| `m544Dll` | 53.0 Ko | 0.0 % | 0/93 | 0.0 % | 0/0/3 | à faire |
| `m545Dll` | 42.3 Ko | 0.0 % | 0/72 | 0.0 % | 0/0/3 | à faire |
| `m546Dll` | 57.0 Ko | 0.0 % | 0/95 | 0.0 % | 0/0/3 | à faire |
| `m547dll` | 45.4 Ko | 0.0 % | 0/60 | 0.0 % | 0/0/3 | à faire |
| `m548dll` | 36.3 Ko | 0.0 % | 0/56 | 0.0 % | 0/0/3 | à faire |
| `m549Dll` | 82.5 Ko | 0.0 % | 0/109 | 0.0 % | 0/0/4 | à faire |
| `m550dll` | 33.0 Ko | 0.0 % | 0/67 | 0.0 % | 0/0/3 | à faire |
| `m551Dll` | 93.6 Ko | 0.0 % | 0/108 | 0.0 % | 0/0/3 | à faire |
| `m552dll` | 49.2 Ko | 0.0 % | 0/86 | 0.0 % | 0/0/5 | à faire |
| `m553Dll` | 66.0 Ko | 0.0 % | 0/94 | 0.0 % | 0/0/3 | à faire |
| `m554dll` | 34.1 Ko | 0.0 % | 0/60 | 0.0 % | 0/0/3 | à faire |
| `m555Dll` | 110.1 Ko | 0.0 % | 0/102 | 0.0 % | 0/0/3 | à faire |
| `m559dll` | 31.8 Ko | 0.0 % | 0/80 | 0.0 % | 0/0/1 | à faire |
| `m560dll` | 29.7 Ko | 0.0 % | 0/71 | 0.0 % | 0/0/2 | à faire |
| `m561dll` | 38.8 Ko | 0.0 % | 0/78 | 0.0 % | 0/0/2 | à faire |
| `m562dll` | 188.5 Ko | 0.0 % | 0/225 | 0.0 % | 0/0/22 | à faire |
| `m563dll` | 34.5 Ko | 0.0 % | 0/96 | 0.0 % | 0/0/2 | à faire |
| `m564Dll` | 45.1 Ko | 0.0 % | 0/139 | 0.0 % | 0/0/4 | à faire |
| `m565dll` | 63.4 Ko | 0.3 % | 3/112 | 0.3 % | 1/0/10 | en cours |
| `m566Dll` | 57.7 Ko | 0.0 % | 0/17 | 0.0 % | 0/0/1 | à faire |
| `m567Dll` | 38.9 Ko | 0.0 % | 0/55 | 0.0 % | 0/0/1 | à faire |
| `m568Dll` | 81.6 Ko | 0.7 % | 11/358 | 0.7 % | 1/0/12 | en cours |
| `m569Dll` | 39.5 Ko | 0.0 % | 0/115 | 0.0 % | 0/0/5 | à faire |
| `m570Dll` | 23.9 Ko | 0.0 % | 0/89 | 0.0 % | 0/0/2 | à faire |
| `m571dll` | 36.9 Ko | 0.0 % | 0/87 | 0.0 % | 0/0/1 | à faire |
| `m572dll` | 40.8 Ko | 0.0 % | 0/88 | 0.0 % | 0/0/1 | à faire |
| `m573dll` | 34.3 Ko | 0.0 % | 0/77 | 0.0 % | 0/0/1 | à faire |
| `m574Dll` | 37.0 Ko | 0.0 % | 0/120 | 0.0 % | 0/0/6 | à faire |
| `m575dll` | 60.7 Ko | 0.3 % | 3/118 | 0.3 % | 1/0/13 | en cours |
| `m576Dll` | 59.8 Ko | 0.0 % | 0/84 | 0.0 % | 0/0/3 | à faire |
| `m577Dll` | 46.4 Ko | 0.0 % | 0/74 | 0.0 % | 0/0/3 | à faire |
| `m579Dll` | 96.9 Ko | 0.6 % | 11/408 | 0.6 % | 1/0/13 | en cours |
| `m580Dll` | 73.8 Ko | 0.0 % | 0/179 | 0.0 % | 0/0/8 | à faire |

### Mini-jeux repris de MP4 (m4xx)

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `m433Dll` | 77.0 Ko | 100.0 % | 127/127 | 100.0 % | 3/0/0 | fait |

### Plateaux du mode Histoire (w0Xs)

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `w01sDll` | 58.5 Ko | 0.0 % | 0/70 | 0.0 % | 0/0/1 | à faire |
| `w02sDll` | 41.4 Ko | 0.0 % | 0/62 | 0.0 % | 0/0/1 | à faire |
| `w03sDll` | 11.2 Ko | 32.4 % | 13/22 | 66.2 % | 0/1/0 | fait (E) |
| `w04sDll` | 54.8 Ko | 0.0 % | 0/59 | 0.0 % | 0/0/1 | à faire |
| `w05sDll` | 13.3 Ko | 9.3 % | 8/19 | 66.9 % | 0/1/0 | fait (E) |
| `w06sDll` | 9.2 Ko | 100.0 % | 23/23 | 100.0 % | 1/0/0 | fait |
| `w07sDll` | 21.5 Ko | 6.1 % | 15/31 | 64.6 % | 0/1/0 | fait (E) |

### Plateaux (w0X, w10, w20)

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `w01Dll` | 66.2 Ko | 100.0 % | 72/72 | 100.0 % | 1/0/0 | fait |
| `w02Dll` | 72.5 Ko | 1.0 % | 12/73 | 41.0 % | 0/1/0 | fait (E) |
| `w03Dll` | 35.6 Ko | 8.2 % | 11/40 | 55.5 % | 0/1/0 | fait (E) |
| `w04Dll` | 78.7 Ko | 3.4 % | 22/66 | 31.2 % | 0/1/0 | fait (E) |
| `w05Dll` | 47.6 Ko | 100.0 % | 33/33 | 100.0 % | 1/0/0 | fait |
| `w06Dll` | 35.2 Ko | 100.0 % | 47/47 | 100.0 % | 1/0/0 | fait |
| `w07Dll` | 45.2 Ko | 2.9 % | 14/52 | 56.3 % | 0/1/0 | fait (E) |
| `w10Dll` | 1.0 Ko | 100.0 % | 8/8 | 100.0 % | 1/0/0 | fait |
| `w20Dll` | 1.2 Ko | 100.0 % | 8/8 | 100.0 % | 1/0/0 | fait |

### Modes (md*, mg*)

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `mdbeachDll` | 114.3 Ko | 0.0 % | 0/124 | 0.0 % | 0/0/3 | à faire |
| `mdcardDll` | 74.8 Ko | 0.0 % | 0/126 | 0.0 % | 0/0/3 | à faire |
| `mdminiDll` | 132.9 Ko | 0.0 % | 0/157 | 0.0 % | 0/0/3 | à faire |
| `mdomakeDll` | 47.7 Ko | 0.0 % | 0/98 | 0.0 % | 0/0/3 | à faire |
| `mdoptionDll` | 109.3 Ko | 0.0 % | 0/175 | 0.0 % | 0/0/4 | à faire |
| `mdpartyDll` | 157.8 Ko | 100.0 % | 163/163 | 100.0 % | 3/0/0 | fait |
| `mdselDll` | 49.7 Ko | 100.0 % | 103/103 | 100.0 % | 3/0/0 | fait |
| `mdstoryDll` | 73.9 Ko | 0.0 % | 0/136 | 0.0 % | 0/0/3 | à faire |
| `mgfreeDll` | 54.7 Ko | 0.0 % | 0/71 | 0.0 % | 0/0/2 | à faire |
| `mgmatchDll` | 91.7 Ko | 0.0 % | 0/125 | 0.0 % | 0/0/3 | à faire |
| `mgtourDll` | 64.8 Ko | 0.0 % | 0/112 | 0.0 % | 0/0/3 | à faire |
| `mgwarsDll` | 62.2 Ko | 0.0 % | 0/121 | 0.0 % | 0/0/3 | à faire |

### Système, menus, résultats, histoire, Super Duel, E3

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `actmanDLL` | 2.4 Ko | 100.0 % | 13/13 | 100.0 % | 1/0/0 | fait |
| `bootDll` | 10.6 Ko | 100.0 % | 11/11 | 100.0 % | 1/0/0 | fait |
| `carddll` | 321.2 Ko | 0.0 % | 0/178 | 0.0 % | 0/0/6 | à faire |
| `decathlonDll` | 43.5 Ko | 100.0 % | 81/81 | 100.0 % | 3/0/0 | fait |
| `e3bootdll` | 18.0 Ko | 100.0 % | 13/13 | 100.0 % | 1/0/0 | fait |
| `e3setupdll` | 47.6 Ko | 0.0 % | 0/41 | 0.0 % | 0/0/4 | à faire |
| `fileseldll` | 44.3 Ko | 100.0 % | 45/45 | 100.0 % | 4/0/0 | fait |
| `instDll` | 29.2 Ko | 100.0 % | 24/24 | 100.0 % | 4/0/0 | fait |
| `meschkdll` | 1.8 Ko | 34.7 % | 5/6 | 81.8 % | 0/0/0 | en cours |
| `motchkDll` | 7.7 Ko | 13.3 % | 5/9 | 90.1 % | 0/1/0 | fait (E) |
| `partyresultdll` | 79.8 Ko | 10.2 % | 14/52 | 10.2 % | 3/0/2 | en cours |
| `rescardDll` | 68.1 Ko | 0.0 % | 0/123 | 0.0 % | 0/0/3 | à faire |
| `resultDll` | 55.3 Ko | 6.5 % | 8/42 | 6.5 % | 3/0/3 | en cours |
| `safDll` | 15.7 Ko | 16.8 % | 9/15 | 80.3 % | 0/3/0 | fait (E) |
| `sd00dll` | 209.0 Ko | 0.0 % | 0/281 | 0.0 % | 0/0/10 | à faire |
| `sdroomdll` | 160.7 Ko | 0.0 % | 0/162 | 0.0 % | 0/0/9 | à faire |
| `selmenuDll` | 19.9 Ko | 100.0 % | 25/25 | 100.0 % | 1/0/0 | fait |
| `sequencedll` | 2.6 Ko | 100.0 % | 9/9 | 100.0 % | 1/0/0 | fait |
| `staffDll` | 10.2 Ko | 7.6 % | 5/8 | 33.0 % | 0/1/0 | fait (E) |
| `storymodedll` | 41.9 Ko | 0.0 % | 0/50 | 0.0 % | 0/0/2 | à faire |
| `storyresultdll` | 15.7 Ko | 30.9 % | 6/15 | 83.3 % | 0/3/0 | fait (E) |
| `systemDll` | 166.2 Ko | 0.0 % | 0/286 | 0.0 % | 0/0/6 | à faire |
