# Aquarium

## Présentation du sujet

Ce projet est la réalisation du projet de réseau de 2e année d'informatique de l'ENSEIRB-Matmeca proposé par Toufik Ahmed pour la promotion 2024. Il consistait en l'implémentation d'un aquarium multi-vues avec gestion centralisée sur un serveur par connexion TCP-IP.

## Arborescence

```
├── display
│   ├── build
│   ├── src
│   ├── res
│   ├── affichage.cfg
│   ├── displayer.log
│   ├── displayer.sh
│   └── Makefile
├── server
│   ├── src
│   ├── build
|   ├── controller.cfg
│   ├── aquarium.txt
│   ├── server.log
│   └── Makefile
└── README.md
```

# Installation

## Serveur

Pour lancer le serveur, il suffit de le compiler avec `make` puis d'exécuter `./server`. Le port du serveur est à définit dans [`controller.cfg`](./server/controller.cfg).

## Client

Pour lancer un client, il suffit de compiler avec `make` puis lancer le client avec le script `./displayer.sh` :

```
./displayer.sh <HOST> <PORT>
```
Les paramètres sont optionnels, si non précisés alors la configuration utilisée sera celle dans [`affichage.cfg`](./display/affichage.cfg).

Une fois le client lancé (et idéalement connecté au serveur) vous pouvez utiliser des commandes pour interagir avec le programme client.\
Des exemples de commandes pour le client sont disponibles dans [`commands.txt`](./display/commands.txt).\
Si vous êtes perdu, commencez par utiliser `help`.

# Auteurs

Ce projet a été réalisé par l'équipe 2 de RE202 du groupe de TD 4 de la promotion 2024 de la filière informatique de l'ENSEIRB-Matmeca, composé de 5 élèves.
