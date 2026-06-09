# AlgoTri

Visualiseur interactif d'algorithmes de tri dans le terminal.

## Stack

- C17 — GCC
- ncursesw
- Make
- Valgrind

## Prérequis

Arch Linux :

```bash
sudo pacman -S gcc make ncurses valgrind
```

Debian / Ubuntu :

```bash
sudo apt-get install -y build-essential libncurses-dev valgrind
```

## Lancer le projet

```bash
make
make run
```

## Lancer les tests

```bash
make test
```
