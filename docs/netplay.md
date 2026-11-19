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

## Utilisation

Le lobby se règle dans **Paramètres > Online Play**, navigable à la manette :

- *Online Play at Start-up* : Off, *Host a Game* ou *Join a Game*.
- *Host Address* : l'adresse de l'hôte à rejoindre (`ip` ou `ip:port`).
- *Find a Game on the Local Network* : remplit l'adresse grâce à un broadcast UDP sur le port 51526.
- *Port* : le port TCP de l'hôte (51525 par défaut).
- *Input Delay* : le délai d'entrée de l'hôte, en images.

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

- **Connexion directe** (implémentée) : l'hôte redirige son port TCP (51525) sur sa box, et les clients entrent son
  IP publique.
- **NAT traversal** (non implémenté) : il faudrait passer les entrées en UDP (prévu pour le rollback, voir
  ci-dessous), puis ajouter un petit serveur de rendez-vous. Ce serveur donne à chaque pair l'adresse publique de
  l'autre, observée façon STUN, et les deux pairs s'envoient des paquets en même temps (UDP hole punching). Ça marche
  derrière la plupart des NAT domestiques, mais pas derrière les NAT symétriques ni le CGNAT strict.
- **Relais** (non implémenté) : en dernier recours, un serveur de type TURN relaie les paquets. La topologie en
  étoile s'y prête, puisque seul l'hôte doit être joignable : un relais peut jouer le rôle de son port public.

## Limites actuelles

- **Lockstep à délai, pas encore de rollback.** Si la latence aller-retour dépasse le délai, le jeu attend le
  réseau et saccade. Le rollback est le jalon suivant, voir plus bas.
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

## Jalon suivant : rollback

Le lockstep reste la base et sert de repli. Le rollback s'ajoute par-dessus :

1. **Instantanés.** L'état du jeu natif comprend :
   - MEM1, un bloc contigu à adresse fixe, où vivent les tas du jeu ;
   - les `.data/.bss` de `dol` et des DLL d'overlay chargées ;
   - les piles des coroutines libco (`process.c`, `osthread_pc.c`), à allouer dans un pool suivi au lieu de
     `malloc` ;
   - les globales de MusyX, qui font partie de `dol` ;
   - l'état DVD et les globales du port qui influent sur la logique.

   On garde un anneau de 8 à 10 instantanés, en ne copiant que les zones utilisées des tas.
2. **Prédiction.** Une entrée distante manquante est prédite (on répète la dernière). Quand la vraie entrée arrive
   et diffère, on restaure l'image fautive et on re-simule jusqu'à l'image courante, sans rendu ni sortie audio.
3. **Barrières.** On ne revient jamais en arrière à travers un chargement ou déchargement de DLL d'overlay, une
   lecture DVD ou un accès carte. À ces points, on attend les entrées confirmées (lockstep), le temps d'un fondu.
4. **UDP.** Chaque paquet porte les N dernières images non acquittées (redondance), sans renvoi.
