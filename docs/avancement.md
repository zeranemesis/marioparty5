# Avancement de la décompilation

Généré le 2026-10-21 16:36 par `tools/avancement.py` depuis `build/GP5E01_00/report.json`.
Ne pas éditer à la main : relancer `ninja` puis le script.

| mesure | valeur |
|---|---|
| code identique | **30.09 %** (2 785.8 / 9 259.3 Ko) |
| code lié depuis les sources | 20.92 % |
| fonctions identiques | 7882 / 21157 |
| unités complètes | 351 / 881 |

Statuts des unités dans `configure.py` : **M** `Matching` (identique, liée), **E** `Equivalent` (complète, pas identique : entre dans le port), **N** `NonMatching` (incomplète ou absente : hors du port).

## Par famille

| famille | modules | code | identique | reste |
|---|---|---|---|---|
| DOL (moteur et bibliothèques) | 1 | 1 690.0 Ko | 85.6 % | 243.5 Ko |
| Mini-jeux m5xx | 76 | 4 493.9 Ko | 12.5 % | 3 931.9 Ko |
| Mini-jeux repris de MP4 (m4xx) | 1 | 77.0 Ko | 100.0 % | 0.0 Ko |
| Plateaux du mode Histoire (w0Xs) | 7 | 210.1 Ko | 47.2 % | 111.0 Ko |
| Plateaux (w0X, w10, w20) | 9 | 383.2 Ko | 42.5 % | 220.3 Ko |
| Modes (md*, mg*) | 12 | 1 033.6 Ko | 21.2 % | 814.1 Ko |
| Système, menus, résultats, histoire, Super Duel, E3 | 22 | 1 371.5 Ko | 16.0 % | 1 152.6 Ko |

## Par module

### DOL (moteur et bibliothèques)

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `main` | 1 690.0 Ko | 85.6 % | 3723/6084 | 87.9 % | 238/6/66 | en cours |

### Mini-jeux m5xx

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `m501Dll` | 65.1 Ko | 4.8 % | 38/319 | 7.7 % | 2/2/7 | en cours |
| `m502dll` | 55.7 Ko | 2.6 % | 16/123 | 3.0 % | 0/0/9 | en cours |
| `m503Dll` | 56.1 Ko | 5.1 % | 30/148 | 11.6 % | 0/2/6 | en cours |
| `m504dll` | 74.4 Ko | 8.0 % | 28/116 | 14.6 % | 2/1/8 | en cours |
| `m505Dll` | 72.4 Ko | 35.8 % | 16/19 | 97.5 % | 0/1/0 | fait (E) |
| `m506Dll` | 59.0 Ko | 54.5 % | 58/96 | 94.5 % | 1/2/0 | fait (E) |
| `m507dll` | 33.7 Ko | 100.0 % | 47/47 | 100.0 % | 2/0/0 | fait |
| `m508Dll` | 57.4 Ko | 3.2 % | 57/245 | 9.1 % | 0/0/2 | en cours |
| `m509Dll` | 76.1 Ko | 2.7 % | 35/284 | 4.1 % | 2/1/7 | en cours |
| `m510dll` | 53.1 Ko | 7.3 % | 18/106 | 12.6 % | 2/1/10 | en cours |
| `m511Dll` | 116.4 Ko | 3.7 % | 58/439 | 18.7 % | 1/11/5 | en cours |
| `m512Dll` | 68.3 Ko | 6.4 % | 48/310 | 12.0 % | 1/2/7 | en cours |
| `m513Dll` | 83.4 Ko | 2.2 % | 38/368 | 7.1 % | 1/0/15 | en cours |
| `m514Dll` | 38.3 Ko | 8.2 % | 25/127 | 8.9 % | 1/0/5 | en cours |
| `m515Dll` | 57.7 Ko | 2.8 % | 21/84 | 3.2 % | 0/0/1 | en cours |
| `m516Dll` | 35.8 Ko | 32.0 % | 48/113 | 44.0 % | 1/0/6 | en cours |
| `m517Dll` | 112.2 Ko | 2.7 % | 50/420 | 5.1 % | 1/0/14 | en cours |
| `m518dll` | 58.5 Ko | 63.9 % | 113/122 | 89.6 % | 0/1/0 | fait (E) |
| `m519dll` | 65.5 Ko | 87.6 % | 140/142 | 99.8 % | 0/1/0 | fait (E) |
| `m520Dll` | 39.7 Ko | 1.5 % | 13/29 | 57.9 % | 0/1/0 | fait (E) |
| `m521Dll` | 99.2 Ko | 3.3 % | 42/365 | 7.5 % | 3/2/6 | en cours |
| `m522Dll` | 86.2 Ko | 3.4 % | 53/398 | 5.6 % | 1/1/7 | en cours |
| `m523Dll` | 38.5 Ko | 1.4 % | 7/111 | 1.4 % | 0/0/2 | en cours |
| `m524Dll` | 23.9 Ko | 43.4 % | 42/76 | 92.4 % | 0/1/0 | fait (E) |
| `m525Dll` | 49.8 Ko | 34.7 % | 13/18 | 54.3 % | 0/1/0 | fait (E) |
| `m526Dll` | 58.9 Ko | 100.0 % | 184/184 | 100.0 % | 2/0/0 | fait |
| `m527Dll` | 52.7 Ko | 1.4 % | 6/16 | 23.9 % | 0/1/0 | fait (E) |
| `m528Dll` | 51.4 Ko | 2.2 % | 17/103 | 2.2 % | 0/0/1 | en cours |
| `m529dll` | 29.2 Ko | 46.7 % | 40/54 | 93.0 % | 0/1/0 | fait (E) |
| `m530Dll` | 38.2 Ko | 20.1 % | 37/133 | 20.1 % | 0/0/4 | en cours |
| `m531Dll` | 69.9 Ko | 8.7 % | 78/276 | 11.3 % | 6/0/8 | en cours |
| `m532Dll` | 78.2 Ko | 1.5 % | 27/414 | 2.1 % | 1/0/10 | en cours |
| `m533dll` | 32.8 Ko | 1.6 % | 8/72 | 2.2 % | 0/0/2 | en cours |
| `m534Dll` | 29.3 Ko | 1.2 % | 5/94 | 1.4 % | 0/0/6 | en cours |
| `m535Dll` | 47.7 Ko | 1.3 % | 6/19 | 38.2 % | 0/0/1 | en cours |
| `m536Dll` | 65.5 Ko | 9.0 % | 80/328 | 12.4 % | 2/1/7 | en cours |
| `m537Dll` | 93.2 Ko | 2.7 % | 37/382 | 2.8 % | 2/0/8 | en cours |
| `m538Dll` | 44.5 Ko | 39.4 % | 62/89 | 84.2 % | 0/1/0 | fait (E) |
| `m539Dll` | 68.5 Ko | 7.7 % | 89/333 | 23.9 % | 1/6/5 | en cours |
| `m540Dll` | 86.1 Ko | 41.6 % | 183/226 | 57.7 % | 0/0/1 | en cours |
| `m541Dll` | 105.7 Ko | 0.6 % | 21/92 | 0.6 % | 0/0/3 | en cours |
| `m542dll` | 37.2 Ko | 3.3 % | 15/58 | 4.3 % | 0/0/3 | en cours |
| `m543Dll` | 65.7 Ko | 1.2 % | 24/91 | 1.2 % | 0/0/3 | en cours |
| `m544Dll` | 53.0 Ko | 2.2 % | 16/93 | 2.2 % | 0/0/3 | en cours |
| `m545Dll` | 42.3 Ko | 3.8 % | 16/72 | 3.8 % | 0/0/3 | en cours |
| `m546Dll` | 57.0 Ko | 2.2 % | 15/95 | 2.2 % | 0/0/3 | en cours |
| `m547dll` | 45.4 Ko | 40.5 % | 24/60 | 40.5 % | 1/0/2 | en cours |
| `m548dll` | 36.3 Ko | 4.5 % | 16/56 | 4.5 % | 0/0/3 | en cours |
| `m549Dll` | 82.5 Ko | 1.0 % | 15/109 | 2.7 % | 0/0/4 | en cours |
| `m550dll` | 33.0 Ko | 2.6 % | 10/67 | 2.6 % | 0/0/3 | en cours |
| `m551Dll` | 93.6 Ko | 0.5 % | 14/108 | 0.7 % | 0/0/3 | en cours |
| `m552dll` | 49.2 Ko | 4.3 % | 17/86 | 5.5 % | 0/0/5 | en cours |
| `m553Dll` | 66.0 Ko | 0.8 % | 11/94 | 1.0 % | 0/0/3 | en cours |
| `m554dll` | 34.1 Ko | 5.2 % | 22/60 | 5.7 % | 0/0/3 | en cours |
| `m555Dll` | 110.1 Ko | 3.7 % | 28/102 | 4.3 % | 0/0/3 | en cours |
| `m559dll` | 31.8 Ko | 4.5 % | 12/80 | 75.2 % | 0/1/0 | fait (E) |
| `m560dll` | 29.7 Ko | 0.7 % | 5/71 | 0.7 % | 0/0/2 | en cours |
| `m561dll` | 38.8 Ko | 4.0 % | 9/78 | 10.8 % | 0/0/2 | en cours |
| `m562dll` | 188.5 Ko | 3.7 % | 37/225 | 9.5 % | 1/0/21 | en cours |
| `m563dll` | 34.5 Ko | 46.9 % | 73/96 | 94.6 % | 0/2/0 | fait (E) |
| `m564Dll` | 45.1 Ko | 2.2 % | 12/139 | 2.2 % | 0/0/4 | en cours |
| `m565dll` | 63.4 Ko | 2.3 % | 13/112 | 2.3 % | 1/0/10 | en cours |
| `m566Dll` | 57.7 Ko | 24.9 % | 12/17 | 81.2 % | 0/1/0 | fait (E) |
| `m567Dll` | 38.9 Ko | 0.6 % | 6/55 | 0.6 % | 0/0/1 | en cours |
| `m568Dll` | 81.6 Ko | 2.5 % | 38/358 | 7.0 % | 1/1/11 | en cours |
| `m569Dll` | 39.5 Ko | 2.2 % | 10/115 | 5.4 % | 0/0/5 | en cours |
| `m570Dll` | 23.9 Ko | 21.7 % | 33/89 | 46.8 % | 0/0/2 | en cours |
| `m571dll` | 36.9 Ko | 30.9 % | 54/87 | 83.7 % | 0/1/0 | fait (E) |
| `m572dll` | 40.8 Ko | 4.5 % | 17/88 | 75.1 % | 0/1/0 | fait (E) |
| `m573dll` | 34.3 Ko | 0.1 % | 2/77 | 0.1 % | 0/0/1 | en cours |
| `m574Dll` | 37.0 Ko | 36.1 % | 48/120 | 41.8 % | 0/1/5 | en cours |
| `m575dll` | 60.7 Ko | 10.9 % | 23/118 | 16.8 % | 1/2/11 | en cours |
| `m576Dll` | 59.8 Ko | 0.6 % | 7/84 | 2.5 % | 0/0/3 | en cours |
| `m577Dll` | 46.4 Ko | 3.6 % | 12/74 | 3.7 % | 1/1/1 | en cours |
| `m579Dll` | 96.9 Ko | 4.5 % | 79/408 | 6.9 % | 1/2/11 | en cours |
| `m580Dll` | 73.8 Ko | 4.0 % | 24/179 | 4.1 % | 1/0/7 | en cours |

### Mini-jeux repris de MP4 (m4xx)

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `m433Dll` | 77.0 Ko | 100.0 % | 127/127 | 100.0 % | 3/0/0 | fait |

### Plateaux du mode Histoire (w0Xs)

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `w01sDll` | 58.5 Ko | 100.0 % | 70/70 | 100.0 % | 1/0/0 | fait |
| `w02sDll` | 41.4 Ko | 1.6 % | 14/62 | 54.5 % | 0/0/1 | en cours |
| `w03sDll` | 11.2 Ko | 32.4 % | 13/22 | 75.2 % | 0/1/0 | fait (E) |
| `w04sDll` | 54.8 Ko | 41.9 % | 49/59 | 55.4 % | 0/0/1 | en cours |
| `w05sDll` | 13.3 Ko | 16.9 % | 12/19 | 92.2 % | 0/1/0 | fait (E) |
| `w06sDll` | 9.2 Ko | 100.0 % | 23/23 | 100.0 % | 1/0/0 | fait |
| `w07sDll` | 21.5 Ko | 8.7 % | 19/31 | 86.6 % | 0/1/0 | fait (E) |

### Plateaux (w0X, w10, w20)

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `w01Dll` | 66.2 Ko | 100.0 % | 72/72 | 100.0 % | 1/0/0 | fait |
| `w02Dll` | 72.5 Ko | 1.9 % | 16/73 | 74.8 % | 0/1/0 | fait (E) |
| `w03Dll` | 35.6 Ko | 9.2 % | 14/40 | 62.2 % | 0/1/0 | fait (E) |
| `w04Dll` | 78.7 Ko | 5.7 % | 34/66 | 51.0 % | 0/1/0 | fait (E) |
| `w05Dll` | 47.6 Ko | 100.0 % | 33/33 | 100.0 % | 1/0/0 | fait |
| `w06Dll` | 35.2 Ko | 100.0 % | 47/47 | 100.0 % | 1/0/0 | fait |
| `w07Dll` | 45.2 Ko | 5.8 % | 23/52 | 84.7 % | 0/1/0 | fait (E) |
| `w10Dll` | 1.0 Ko | 100.0 % | 8/8 | 100.0 % | 1/0/0 | fait |
| `w20Dll` | 1.2 Ko | 100.0 % | 8/8 | 100.0 % | 1/0/0 | fait |

### Modes (md*, mg*)

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `mdbeachDll` | 114.3 Ko | 1.2 % | 13/124 | 1.2 % | 0/0/3 | en cours |
| `mdcardDll` | 74.8 Ko | 1.4 % | 12/126 | 1.4 % | 0/0/3 | en cours |
| `mdminiDll` | 132.9 Ko | 2.3 % | 27/157 | 3.7 % | 0/0/3 | en cours |
| `mdomakeDll` | 47.7 Ko | 1.9 % | 17/98 | 2.1 % | 0/0/3 | en cours |
| `mdoptionDll` | 109.3 Ko | 1.0 % | 15/175 | 1.1 % | 0/0/4 | en cours |
| `mdpartyDll` | 157.8 Ko | 100.0 % | 163/163 | 100.0 % | 3/0/0 | fait |
| `mdselDll` | 49.7 Ko | 100.0 % | 103/103 | 100.0 % | 3/0/0 | fait |
| `mdstoryDll` | 73.9 Ko | 1.0 % | 15/136 | 1.3 % | 0/0/3 | en cours |
| `mgfreeDll` | 54.7 Ko | 0.2 % | 3/71 | 1.9 % | 0/0/2 | en cours |
| `mgmatchDll` | 91.7 Ko | 2.0 % | 30/125 | 3.6 % | 0/0/3 | en cours |
| `mgtourDll` | 64.8 Ko | 0.7 % | 12/112 | 1.2 % | 0/0/3 | en cours |
| `mgwarsDll` | 62.2 Ko | 2.2 % | 30/121 | 4.7 % | 0/1/2 | en cours |

### Système, menus, résultats, histoire, Super Duel, E3

| module | code | identique | fonctions | fuzzy | unités M/E/N | état |
|---|---|---|---|---|---|---|
| `actmanDLL` | 2.4 Ko | 100.0 % | 13/13 | 100.0 % | 1/0/0 | fait |
| `bootDll` | 10.6 Ko | 100.0 % | 11/11 | 100.0 % | 1/0/0 | fait |
| `carddll` | 321.2 Ko | 1.6 % | 25/178 | 2.1 % | 0/0/6 | en cours |
| `decathlonDll` | 43.5 Ko | 100.0 % | 81/81 | 100.0 % | 3/0/0 | fait |
| `e3bootdll` | 18.0 Ko | 100.0 % | 13/13 | 100.0 % | 1/0/0 | fait |
| `e3setupdll` | 47.6 Ko | 3.7 % | 4/41 | 4.4 % | 2/0/2 | en cours |
| `fileseldll` | 44.3 Ko | 100.0 % | 45/45 | 100.0 % | 4/0/0 | fait |
| `instDll` | 29.2 Ko | 100.0 % | 24/24 | 100.0 % | 4/0/0 | fait |
| `meschkdll` | 1.8 Ko | 34.7 % | 5/6 | 98.4 % | 0/0/0 | en cours |
| `motchkDll` | 7.7 Ko | 13.3 % | 5/9 | 97.4 % | 0/1/0 | fait (E) |
| `partyresultdll` | 79.8 Ko | 11.1 % | 19/52 | 11.6 % | 3/0/2 | en cours |
| `rescardDll` | 68.1 Ko | 2.0 % | 21/123 | 3.3 % | 0/0/3 | en cours |
| `resultDll` | 55.3 Ko | 11.8 % | 19/42 | 12.8 % | 3/0/3 | en cours |
| `safDll` | 15.7 Ko | 23.3 % | 10/15 | 94.4 % | 0/3/0 | fait (E) |
| `sd00dll` | 209.0 Ko | 1.9 % | 24/281 | 3.7 % | 1/0/9 | en cours |
| `sdroomdll` | 160.7 Ko | 3.1 % | 30/162 | 7.5 % | 0/0/9 | en cours |
| `selmenuDll` | 19.9 Ko | 100.0 % | 25/25 | 100.0 % | 1/0/0 | fait |
| `sequencedll` | 2.6 Ko | 100.0 % | 9/9 | 100.0 % | 1/0/0 | fait |
| `staffDll` | 10.2 Ko | 7.6 % | 5/8 | 78.4 % | 0/1/0 | fait (E) |
| `storymodedll` | 41.9 Ko | 0.6 % | 4/50 | 0.8 % | 0/0/2 | en cours |
| `storyresultdll` | 15.7 Ko | 47.1 % | 8/15 | 92.0 % | 0/3/0 | fait (E) |
| `systemDll` | 166.2 Ko | 1.2 % | 34/286 | 1.5 % | 0/0/6 | en cours |
