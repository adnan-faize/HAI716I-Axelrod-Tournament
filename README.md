# HAI716I - Projet 2 : Axelrod tournoi

**Auteur :** Adnan Faize  
**Langage :** C++20

## Compilation & Prérequis

Le projet requiert un compilateur supportant **C++20** (`g++ 11+` ou `clang++ 14+`). Il n'a aucune dépendance externe.

```bash
cmake -B build
cmake --build build
```

L'exécutable se situe alors dans `build/tournoi`.

---

## Commandes pour Relancer Chaque Tournoi

### Fiches des stratégies

Affiche les règles, origines historiques et comportements attendus des dix stratégies :

```bash
./tournoi strats
```

### Étape 1 : Tournoi de base (5 stratégies du cours)

Lance le tournoi à 5 stratégies sur 5 matchs de 200 tours :

```bash
./tournoi etape-1
```

Pour tester l'impact sans affrontement contre soi-même :

```bash
./tournoi etape-1 --no-self-play
```

### Étape 2 : Tournoi avec population élargie (7 stratégies)

Lance le tournoi complet avec les 7 stratégies (les 5 de base + Pavlov & Friedman) :

```bash
./tournoi etape-2
```

### Étape 3.1 : Bruit 

Introduit 5% de probabilité d'inversion des coups ($C \leftrightarrow D$) :

```bash
./tournoi etape-3-bruit
```

Il est possible de spécifier un autre taux de bruit (ex. 10%) :
```bash
./tournoi etape-3-bruit -b 0.10
```

### Étape 3.2 : Horizon inconnu (fin probabiliste)

Simule des parties à durée aléatoire avec probabilité de poursuite $w = 0.995$ ($\mathbb{E}[T] = 200$ tours) :

```bash
./tournoi etape-3-horizon
```

### Étape 3.3 : Sélection évolutionnaire 

Fait évoluer la population sur 50 générations selon le fitness relatif :

```bash
./tournoi etape-3-selection
```

### Étape 3.4 : Autres matrices de gains (Poule Mouillée & Chasse au Cerf)

Confronte les 7 stratégies sur le jeu de Poule Mouillée et de Chasse au Cerf :

```bash
./tournoi etape-3-gains
```

### Exécution globale de toutes les étapes

Pour exécuter l'ensemble des scénarios :

```bash
./tournoi all
```

---

## Options et Paramètres CLI

Vous pouvez moduler les paramètres globaux via les options en ligne de commande :

- `-r, --rounds <N>` : Nombre de tours par match (défaut : `200`).
- `-m, --matches <N>` : Nombre de répétitions par paire (défaut : `5`).
- `-s, --seed <N>` : Graine pseudo-aléatoire du PRNG (défaut : `42`).
- `-b, --bruit <F>` : Taux de bruit stochastique entre 0.0 et 1.0 (défaut : `0.05`).
- `-p, --prob <F>` : Probabilité de continuer pour l'horizon inconnu (défaut : `0.995`).
- `-c, --csv` : Active l'export des fichiers `.csv`.
- `--no-self-play` : Désactive les affrontements d'une stratégie contre son propre clone.
- `-h, --help` : Affiche l'aide détaillée en ligne de commande.

---

## Tests Unitaires

Une suite de tests unitaires intégrée valide la conformité du PRNG et les comportements décisionnels clés (pardon de Gladstein Tester, rancune de Friedman, règle de Pavlov, tolérance de TFTT) :

```bash
./tournoi test
```

Résultat : 8 / 8 tests validés avec succès. (Hopefully)
