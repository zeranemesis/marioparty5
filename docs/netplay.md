# Jeu en ligne (netplay)

MP5 Native permet de jouer à plusieurs machines (PC Windows, Android), chacune avec sa manette, en réseau local ou
par Internet. Le code est dans `src/port/netplay.cpp` (session, entrées, audio, désync) et `src/port/ui/netplay.cpp`
(réglages et lobby).

## Principe : lockstep sur les manettes

Comme le netplay de Dolphin, chaque machine exécute **tout le jeu**. Seules les manettes circulent sur le réseau.

- **Démarrage commun.** Le lobby a lieu avant le boot du jeu. Quand l'hôte lance la partie, il envoie à chacun :
  - la graine de `frand` ;
  - ses cartes mémoire (dossiers GCI `<config>/<région>/Card A|B`, copiés chez le client dans `<config>/netplay/`
    sans toucher sa propre sauvegarde) ;
  - les réglages qui changent la logique du jeu : langue, Quick Transform, triches, déblocages, modes speedrun et
    enregistrement ;
  - l'attribution des ports manette.

  À la connexion, l'hôte vérifie que le build est le même (version, plateforme, architecture) et que le disque est
  de la même région.
- **Une image = un appel de `PadReadVSync`.** C'est le tick du jeu, une fois par `VIWaitForRetrace`. Une manette lue
  à l'image *f* devient l'entrée de l'image *f + délai*. Les clients envoient leurs manettes à l'hôte. L'hôte
  renvoie à tous chaque image complète (topologie en étoile). Le jeu ne voit que des manettes identiques partout.
- **Désynchronisation.** Toutes les 60 images, chaque machine calcule un CRC32 de `GwCommon`, `GwSystem`,
  `GwPlayer`, `GwPlayerConf` et de la graine. L'hôte compare les sommes. En cas d'écart, chaque machine écrit
  `netplay_desync_<image>_<rôle>.bin` (l'état haché) dans son dossier de config, et le log affiche `DESYNC`. Toutes
  les 3600 images, l'hôte écrit `frame N: in sync`.

### Ce qui a été rendu déterministe

| Source | Correction en netplay |
|---|---|
| Graine `frand` (horloge) | Imposée par l'hôte |
| Synthé MusyX sur son propre thread, cadencé par la carte son, alors que le jeu lit l'état des sons (~90 appels : `HuAudFXStatusGet`, `msm*GetNumPlay`…) | MusyX rend exactement 1/60 s par image sur le thread du jeu (`sndPCRender`, sans horloge murale). Son thread n'est pas démarré. |
| Vidéos THP, qui suivent l'horloge audio | Elles suivent le compteur d'images (le mode était déjà prévu dans `thp_player.cpp`) |
| Lectures disque « vitesse GameCube » (horloge temps réel) | Forcées en mode instantané |
| Pause à la perte du focus, turbo | Forcés désactivés |
| Horloge du jeu (`OSGetTick`/`OSGetTime`), qui sert à amorcer des générateurs de mini-jeux (m537) et à minuter des attentes | Tout le code du jeu est compilé avec `OSGetTick=Mp5Net_OSGetTick` : en session, l'heure de l'hôte au départ plus 1/60 s par image (plus 10 µs par lecture, pour que les attentes dans une même image se terminent) |
| Format de la fenêtre : le facteur écran large sert au tri des objets visibles (`ObjCullCheck`), et un modèle tiré d'un côté seulement exécute son hook de dessin | Réglages d'aspect de l'hôte imposés, et taille de sa fenêtre figée pour toute la session |
| Hooks d'effets de capsule rejoués dans chaque passe d'ombres, donc plus souvent à 144 i/s (images interpolées) qu'à 60 | Pas de mise à jour de l'effet pendant une passe d'ombres interpolée |

### PC ↔ Android

Le PC (MSVC, x64) et Android (clang, arm64) ne calculent pas tout de la même façon. Corrigé :

| Différence | Correction |
|---|---|
| Disposition des structures à champs de bits (`GwCommon`, `GwPlayer`) : elle diffère, alors que les sauvegardes en sont des copies brutes | Android compile le jeu avec `-mms-bitfields`, la disposition MSVC |
| `sin`/`cos`/`atan2`… de la libm, qui diffèrent sur le dernier bit | `src/port/detmath.c` (algorithmes fdlibm, seulement `+ − × ÷` et `sqrt`, 1 ulp) inclus de force dans le jeu et dans la bibliothèque mathématique d'aurora |
| Contraction en FMA par clang | `-ffp-contract=off` sur Android |
| Ordre d'évaluation des arguments : MSVC de droite à gauche, clang de gauche à droite. 58 instructions tirent plusieurs nombres aléatoires d'un coup (`f(RandF(), RandF())`) | Sur PC, tirages de gauche à droite dans des temporaires (`tools/netplay_rngorder.py`) |
| Lectures hors limites d'un tableau, qui renvoient ce qui suit en mémoire (m506 : `MotSpeedTbl[10]`) | La valeur que lisait la GameCube (la table suivante) |

Outils de diagnostic, à mettre sur toutes les machines :
- `MP5_NET_DEEP=1` : vérification à chaque image, positions, rotations et animations de chaque modèle comprises. Le rapport nomme le hook de dessin de chaque modèle ;
- `MP5_NET_DUMPAT=<image>` : écrit MEM1 et les tas du jeu ; `tools/netplay_memdiff.py` compare deux machines, pointeurs neutralisés ;
- `MP5_NET_TRACEMDL=<modèle>` : journalise qui modifie les attributs d'un modèle (fonction appelante).

## Utilisation

Le lobby se règle dans **Paramètres > Online Play**, navigable à la manette :

- *Online Play at Start-up* : Off, *Host a Game* ou *Join a Game*.
- *Host Address* : l'adresse de l'hôte à rejoindre (`ip` ou `ip:port`).
- *Find a Game on the Local Network* : remplit l'adresse grâce à un broadcast UDP sur le port 51526.
- *Port* : le port TCP de l'hôte (51525 par défaut).
- *Input Delay* : le délai d'entrée de l'hôte, en images.
- *Network Lag* : *Rollback* (par défaut) ou *Wait for Every Player* (lockstep), au choix de l'hôte.

Au démarrage suivant, un écran de lobby s'ouvre avant le boot. L'hôte voit les joueurs arriver et appuie sur
*Start Game*. Les clients bootent en même temps que lui. *Play Offline* démarre seul.

Pour les tests, des variables d'environnement font la même chose sans l'interface :

- `MP5_NET_HOST=<port>` et `MP5_NET_CLIENTS=<n>` : l'hôte démarre dès que *n* joueurs sont là.
- `MP5_NET_JOIN=<ip[:port]>` : rejoint un hôte.
- `MP5_NET_DELAY=<images>` : délai d'entrée.
- `MP5_NET_LOCAL=<n>` : nombre de manettes locales sur cette machine (1 par défaut).

Exemple avec deux instances sur la même machine, en séparant leurs configs avec `MP5_TEST_CONFIG_DIR` et
`MP5_TEST_CACHE_DIR` :

```
MP5_TEST_CONFIG_DIR=A MP5_TEST_CACHE_DIR=A/cache MP5_NET_HOST=51525 mp5native.exe
MP5_TEST_CONFIG_DIR=B MP5_TEST_CACHE_DIR=B/cache MP5_NET_JOIN=127.0.0.1:51525 mp5native.exe
```

Banc de test d'une partie complète : `MP5_TEST_BOARD=0 MP5_TEST_BOARD_HUMANS=2 MP5_TEST_NETMASH=1` sur les deux
instances. Le joueur 1 est l'hôte et le joueur 2 le client : chacun tape A, et START sur l'écran d'instructions.
Chaque machine écrit son journal `netplay_host.log` ou `netplay_client.log` (dossier de config), avec l'image, le
CRC, les graines, l'overlay et l'attente réseau toutes les 600 images.

## Internet

- **UPnP** (implémenté) : en hébergeant, le jeu demande à la box de rediriger son port TCP vers lui (découverte
  SSDP, `AddPortMapping` sur WANIPConnection ou WANPPPConnection), lit l'adresse publique et l'affiche dans le
  salon (« Friends on the Internet join <ip>:<port> »). Le port est refermé en quittant le salon ou le jeu.
  `MP5_NET_UPNP=0` le désactive (`src/port/upnp.cpp`).
- **Connexion directe** (implémentée) : si la box refuse l'UPnP, l'hôte redirige son port TCP (51525) à la main, et
  les clients entrent son IP publique. Un VPN de jeu (Tailscale, ZeroTier…) évite toute redirection.
- **NAT traversal** (non implémenté) : il faudrait passer les entrées en UDP (prévu pour le rollback, voir
  ci-dessous), puis ajouter un petit serveur de rendez-vous. Ce serveur donne à chaque pair l'adresse publique de
  l'autre, observée façon STUN, et les deux pairs s'envoient des paquets en même temps (UDP hole punching). Ça marche
  derrière la plupart des NAT domestiques, mais pas derrière les NAT symétriques ni le CGNAT strict.
- **Relais** (non implémenté) : en dernier recours, un serveur de type TURN relaie les paquets. La topologie en
  étoile s'y prête, puisque seul l'hôte doit être joignable : un relais peut jouer le rôle de son port public.

## Tests réalisés (PC, deux instances sur la même machine)

- **Partie complète** (plateau 1, 10 tours, 10 mini-jeux, retour au titre) : 102 000 images, aucune désync, CRC
  identiques à chaque contrôle. Mario est sur l'hôte et Luigi sur le client, tous deux humains, ce qui fait
  passer les entrées dans les deux sens ; les deux autres joueurs sont des ordinateurs.
- **Parties partielles** : plus de 110 000 images sans désync, sur d'autres tirages de mini-jeux.
- **Lobby par l'interface** : un client lancé avant l'hôte se connecte tout seul, *Start Game* démarre les deux
  jeux, et la carte de l'hôte est chargée chez le client.
- **Désyncs trouvées et corrigées** :
  - le synthé MusyX sur son propre thread ;
  - la graine des mini-jeux tirée de `OSGetTick` (m537), d'où l'horloge commune `Mp5Net_OSGetTime`.

### Balayage des mini-jeux PC ↔ Thor

Chaque mini-jeu a tourné 2 min en netplay (`MP5_TEST_MG`, quatre joueurs, deux par machine) avec la vérification
`MP5_NET_DEEP` à chaque image. Le script est dans le carnet de session.

- **73 sur 76** sont synchronisés, dont m506, m512, m536, m537 et m569, corrigés en route.
- **Désync restante** :
  - m511 (image 1 324) et m564 (image 1 354) : écarts sur le dernier bit de flottants, cause pas encore trouvée ;
  - m571 (image 979) : le PC crée un système de particules d'effet de personnage que la Thor ne crée pas.
- **Plantages de portage**, sans lien avec le netplay :
  - m508 et m522 plantent sur Android : lecture hors limites d'une table d'animation, et pointeur de fonction invalide ;
  - m547 plante sur les deux plateformes : décalages GameCube écrits en dur dans `game.c`.

Corrections de portée générale apportées par ce balayage :
- la graine passée par le registre `r3` après `OSGetTick()` (m511, m512, m536, m579) ;
- la mémoire `HuMem` neuve remise à zéro sur PC (champs jamais initialisés) ;
- un `fmaf` déterministe (`ppc_rsqrte`) ;
- le scanner d'ordre des tirages, qui couvre aussi les générateurs propres à chaque mini-jeu.

Outils supplémentaires : `MP5_NET_TRACERNG=<de>-<à>` (chaque tirage avec la fonction appelante) et
`MP5_NET_TRACEMDL` (attributs et positions d'un modèle).

## Limites actuelles

- **Sauvegarde non testée en netplay.** Le banc `MP5_TEST_BOARD` désactive la sauvegarde, donc l'écriture sur la
  carte en fin de partie n'a pas été exercée.
- **Un plantage du client vu une fois**, à la sortie du mini-jeu m547 (image 66 947, avant la refonte de l'horloge).
  Il n'a pas été reproduit, et le rapport de crash élargi (`CRASH:` dans le log) l'attrapera s'il revient.

- **Lockstep** (mode *Wait for Every Player*) : si la latence aller-retour dépasse le délai, le jeu attend le réseau
  et saccade. Le rollback, par défaut, évite ce cas (voir plus bas).
- **TCP pour tout.** C'est simple et fiable, mais un paquet perdu bloque les suivants (head-of-line blocking). Sur
  Internet, ça se paie par des à-coups.
- **Pas de reconnexion ni d'arrivée en cours de partie.** Un pair perdu pendant 10 s termine la session, et chaque
  machine continue seule.
- **Même plateforme seulement.** Le build doit être identique (le hash du build est vérifié). PC↔Thor n'est pas
  garanti : MSVC x64 et clang arm64 n'ont pas les mêmes flottants (contraction FMA, libm différente). Il faudrait
  clang-cl sur Windows, `-ffp-contract=off` partout et une libm commune.
- **Audio.** Le son suit les 60 Hz du jeu et non la carte son. Au-delà de 100 ms d'avance, de l'audio est jeté, ce
  qui donne des micro-coupures possibles si l'affichage ne tient pas 60 images/s. Les changements de groupe de sons
  (`HuAudSndGrpSetSet`) attendent jusqu'à 500 ms la fin des sons, puisque le synthé n'avance pas pendant cette
  attente.
- **Vibrations.** Les vibrations d'un port distant font vibrer la manette locale de même index.
- **Android.** La découverte LAN peut ne pas recevoir les broadcasts quand le Wi-Fi les filtre (il faudrait un
  `MulticastLock`). Saisir l'IP directement fonctionne.
- **Cartes mémoire.** Seul le mode dossier GCI d'aurora est transmis, c'est le seul utilisé aujourd'hui. Les
  sauvegardes faites pendant la partie vont dans la copie `netplay/` des clients.

## Rollback

Par défaut, une session est en rollback. Le choix appartient à l'hôte : *Network Lag* dans **Paramètres > Online
Play**, ou `MP5_NET_ROLLBACK=0/1` pour les tests. `Wait for Every Player` garde le lockstep décrit plus haut.

### Principe

- **Envoi immédiat.** Chaque machine envoie ses manettes de l'image *f* pour l'image *f + délai* (`MsgInputR`) ;
  l'hôte relaie aux autres clients celles qu'il reçoit.
- **Prédiction.** Une image dont les manettes distantes ne sont pas arrivées tourne avec leur dernière valeur
  connue. Une machine peut avoir jusqu'à 8 images d'avance sur la dernière image complète (`kMaxAhead`) ; au-delà,
  elle attend comme en lockstep.
- **Correction.** Si les vraies manettes d'une image déjà jouée diffèrent de la prédiction, `rollback.cpp` restaure
  le tick qui a produit cette image et rejoue les ticks jusqu'au présent. Ces ticks rejoués ne dessinent rien
  (`AuroraGXSetDiscard` dans `extern/aurora`) et ne sortent aucun son ; le synthé avance quand même.
- **Barrières.** Le code d'un overlay chargé ne se défait pas. Avant un chargement (`omOvlLoadPending`), la
  machine attend que toutes les manettes jouées soient confirmées. L'image produite par le tick du chargement attend
  aussi les vraies manettes (`Mp5Rb_TickRestorable`).
- **Désyncs.** Le CRC d'une image n'est comparé qu'une fois confirmées toutes les manettes qui l'ont précédée.

### Ce qu'un instantané contient

L'état du jeu est restauré octet pour octet, à la même adresse :

- les variables du jeu : un en-tête forcé (`include/port/gamestate.h`) les place dans les sections `mp5gd`/`mp5gb`
  de `dol` et de chaque REL (`gamestate_sections.cpp`). Il couvre aussi MusyX (`musyx_gamestate.h`, qui envoie
  ses `malloc` dans le tas du son), le banc de test, le cache des animations de sprites, les threads OS émulés
  (`osthread_pc.c`) et la fermeture différée des DLL (`rel_loader.c`). Les variables propres au portage (objets de
  l'hôte, conteneurs C++) n'y sont pas ;
- les tas HuMem, le tas des piles des coroutines (`HuPrcStackAlloc`) et le tas du son (`msmPCSndHeapGet`), tous à
  adresse fixe ;
- l'ARAM d'Aurora ;
- l'état de session de `netplay.cpp` : image, horloge, compte des images audio.

Le reste de l'hôte est rendu compatible : les fichiers du disque (`DVDOpen`/`DVDClose` passent par
`rollback.cpp`, et une fermeture attend qu'aucun instantané plus ancien ne puisse revenir), l'interpolation
d'images (invalidée à chaque restauration) et la file GX (vidée avant de changer de mode).

### Coût

Les zones suivies sont protégées en écriture (`VirtualProtect` + gestionnaire vectoriel sous Windows,
`mprotect` + `SIGSEGV` sous Android). La première écriture dans une page en garde l'ancien contenu dans le journal
du tick ; la page redevient alors modifiable (`rollback_pages.cpp`). Les piles des coroutines ne sont pas protégées,
car l'exception se traite sur la pile fautive ; leur partie vivante est copiée à chaque tick.

- 200 à 500 pages écrites par tick ;
- environ 0,25 ms pour l'instantané d'un tick (contre 16 ms pour une copie complète de 317 Mo) ;
- environ 1 ms pour revenir 8 ticks en arrière ;
- 12 ticks gardés, avec une réserve de 32 768 pages (128 Mo).

### Outils

- `MP5_RB_SYNCTEST=<n>`, dans une session solo (`MP5_NET_HOST=… MP5_NET_CLIENTS=0 MP5_NET_LOCAL=4`) : toutes les
  *n* images, on revient en arrière, on rejoue, et on compare tout l'état octet par octet. Le rapport donne le nom
  des variables (dbghelp).
- `MP5_RB_EACH=<tick>` compare après chaque tick, processus et objet ; `MP5_RB_DIFFAT=<tick>:<point>` liste les
  variables qui diffèrent à un point donné.
- `MP5_RB_FAST` mesure les coûts sans comparer.

### Résultats (PC ↔ PC, deux instances, délai d'entrée 0)

- **Partie complète** : plateau 1, 25 min, synchronisée jusqu'à l'image 89 400, 4 142 corrections.
- **Mini-jeux** : les 76 tournent 110 s chacun, synchronisés, avec des milliers de corrections chacun. m508 plantait en
  session : sa boucle de séquence lisait au-delà de sa table, ce qui sur GameCube tombe sur des flottants qui
  l'arrêtent. Le PC lit désormais les mêmes valeurs.

### Limites

- **m505** relit l'image affichée dans sa logique (`AuroraGXCopyReadbackSync`). Les ticks rejoués n'affichant rien,
  il peut diverger après une correction.
- **Fichiers externes.** Une lecture faite par le système directement dans une page protégée (par exemple
  `ReadFile` vers un tampon du jeu) échouerait au lieu de déclencher l'exception. Aucune n'a été vue pendant les
  parties, mais les sauvegardes sur la carte en session n'ont pas été testées.
- **PC ↔ Android en rollback** pas encore testé : la protection par `SIGSEGV` n'a jamais tourné sur la Thor.

### Suite

UDP : chaque paquet porterait les N dernières images non acquittées (redondance), sans renvoi, pour éviter les
à-coups du TCP sur une connexion qui perd des paquets.
