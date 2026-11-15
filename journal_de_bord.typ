
#let IMAGE_BOX_MAX_WIDTH = 120pt
#let IMAGE_BOX_MAX_HEIGHT = 50pt

#set document(author: "Adnan FAIZE", title: "Journal de Bord")
#set par(justify: true, leading: 0.52em);

#set heading(numbering: "1.1")
#show heading: it => {
  if it.numbering != none {
    numbering(it.numbering, ..counter(heading).at(it.location()))
    h(1em)
    it.body
  } else {
    it.body
  }
  v(0.5em)
}

#set page(
  numbering: "1",
  number-align: center,
  header: context {
    let page-number = counter(page).get().at(0)
    if page-number > 1 {
      align(end)[
        #text(size: 12pt, weight: "regular")[Projet nº2 : Axelrod Tournament]
      ]
    }
  },
  footer: context {
    let page-number = counter(page).get().at(0) - 1
    if page-number > 0 {
      line(length: 100%, stroke: 0.5pt)
      v(-2pt)
      text(size: 12pt, weight: "regular")[
        HAI716I
        #h(1fr)
        #page-number / #(counter(page).final().at(0) - 1)
        #h(1fr)
        2026-2027
      ]
    }
  },
)

#block[
  #box(height: IMAGE_BOX_MAX_WIDTH, width: IMAGE_BOX_MAX_WIDTH)[
    #align(start + horizon)[
      #image("res/UM.svg")
    ]
  ]
  #h(1fr)
  #box(height: IMAGE_BOX_MAX_WIDTH, width: IMAGE_BOX_MAX_WIDTH)[
    #align(center + horizon)[
      #image("res/FdS.png")
    ]
  ]
  #h(1fr)
  #box(height: IMAGE_BOX_MAX_WIDTH, width: IMAGE_BOX_MAX_WIDTH)[
    #align(end + horizon)[
      #image("res/DI.png")
    ]
  ]
]

#v(1fr)

#align(center + horizon)[
  #text(size: 14pt, tracking: 2pt)[
    #smallcaps[
      HAI716I -- Programmation Orientée Agents & Systèmes Multi-Agents
    ]
  ]
  #line(length: 100%, stroke: 0.5pt)
  #text(size: 20pt, weight: "bold")[Journal de Bord Projet nº2 : Tournoi du Dilemme du Prisonnier]
  #line(length: 100%, stroke: 0.5pt)
]

#align(center + horizon)[
  Adnan FAIZE
]

#v(1fr)

#align(center)[
  #block(width: 80%)[
    #text(size: 11pt, weight: "medium", fill: luma(60))[
      *Résumé :* Ce journal de bord décrit la conception, l'implémentation en C++20 et les tests d'un simulateur du dilemme du prisonnier selon les protocoles de Robert Axelrod (1980). Le simulateur confronte dix stratégies issues de la théorie des jeux et des systèmes multi-agents. Les expériences couvrent six régimes : un tournoi initial à cinq stratégies, une population à dix stratégies, un bruit de transmission de 5 % et un horizon probabiliste ($w=0.995$). Elles évaluent aussi la sélection par dynamique des réplicateurs sur cinquante générations et deux matrices de gains alternatives (Poule Mouillée et Chasse au Cerf). Le système exporte automatiquement les résultats en fichiers CSV, les importe dans ce document et les affiche sous forme de graphiques SVG. L'analyse détermine les conditions de domination de TIT-FOR-TAT et mesure l'effet de la composition écologique de la population.
    ]
  ]
]

#v(1fr)

#align(center + bottom)[
  Année 1 Master Informatique parcours IMAGINE
  #linebreak()
  2026 -- 2027
]

#pagebreak()

= Cadre et Protocole Expérimental

== Problématique et Objectifs
Cette étude examine comment une coopération rationnelle émerge et se stabilise entre des agents autonomes égoïstes sans autorité centrale. Elle vise à identifier les mécanismes algorithmiques (réciprocité, indulgence, réactivité) et les facteurs environnementaux (bruit, horizon temporel, dynamique populationnelle, matrices de rétribution) qui assurent une robustesse évolutive face aux prédateurs.

Sur le plan opérationnel, le projet développe un simulateur orienté agents en C++20, modulaire, déterministe et conforme aux spécifications d'Axelrod (1980). Cet outil évalue sept stratégies (les cinq du cours complétées de Friedman et Pavlov) selon un protocole round-robin, puis éprouve leur résilience sous quatre régimes perturbés : canal bruité, horizon stochastique, réplicateurs et matrices modifiées. Enfin, la démarche impose de formuler des prédictions avant chaque simulation afin de valider les écarts entre la théorie et les résultats empiriques.

== Spécification des Paramètres Expérimentaux
Le protocole applique les règles formelles du premier tournoi de Robert Axelrod (1980) :
- Nombre de tours par math ($T = 200$) : Horizon déterministe fixe. permet d'amortir la phase de test initiale et d'observer le régime stationnaire.
- Nombre de répétitions par pair ($M = 5$) : Réduit la variance statistique causée par les stratégies stochastiques (`RANDOM`, `JOSS`) et par le canal bruité.
- Matrice de Gains standard : $T=5.0, R=3.0, P=2.0, S=0.0$. Respecte strictement les deux conditions théoriques : $T > R > P > S$ ($5 > 3 > 2 > 0$) et $2R > T + S$ ($6 > 5$).
- Générateur pseudo-aléatoire (PRNG) : Algorithme SplitMix64 64-bit déterministe (graine = `42`). Assure une reproductibilité bit-à-bit stricte des tirages.
- Régime de confrontation : Exécution en round-robin incluant les confrontations clone (self-play). Le mode sans clone est disponible via l'option `--no-self-play`.

= Fiches des Stratégies

Le tournoi met en scène dix stratégies formalisées en classes C++20 dérivant de l'interface abstraite pure `Strategy`.

== Les 5 Stratégies de Base du Cours
- ALL-D (Toujours Trahir) : Stratégie dominante de Nash (jeu statique) jouant toujours $D$ sans tenir compte de l'historique, elle exploite les profils naïfs ou désordonnés mais ne crée aucune valeur partagée, plafonnant à $bb(P)=2$ points face à elle-même ou aux stratégies punitives.
- RANDOM (Aléatoire Équiprobable) : Ligne de base à entropie maximale introduite par Axelrod qui tire ses coups à pile ou face ($bb(P)(C) = bb(P)(D) = 0.5$), elle détruit la confiance des stratégies réactives et sert de manne financière ($T=5$) aux prédateurs opportunistes.
- TIT-FOR-TAT (Donnant-Donnant) : Double vainqueur des tournois d'Axelrod conçu par Anatol Rapoport, elle joue $C$ au premier tour puis copie le coup adverse précédent, incarnant les quatre vertus d'Axelrod : bienveillance, riposte, indulgence et transparence.
- TESTER (David Gladstein) : Soumise par David Gladstein au second tournoi d'Axelrod, elle sonde l'adversaire par $D$ au tour 1 : en cas de riposte, elle s'excuse ($C$) et adopte TIT-FOR-TAT (coopérant ensuite sur les 198 tours suivants face à elle), sinon elle l'exploite en alternant $D$ et $C$.
- JOSS (Johann Joss) : Variante sournoise de TIT-FOR-TAT soumise par Johann Joss qui glisse une défection ($D$) avec une probabilité $p = 0.10$ après une coopération adverse, visant le gain de tentation $T=5$ au risque de déclencher des vendettas irréversibles.

== Les 2 Stratégies Étendues de la Littérature
- FRIEDMAN (Rancunière) : Stratégie pivot des folk theorems théorisée par James W. Friedman (1971) qui joue $C$ tant que l'autre coopère puis bascule définitivement sur $D$ à la moindre trahison, assurant une dissuasion maximale ($600$ points) mais critique face au moindre bruit.
- PAVLOV (Win-Stay, Lose-Shift) : Formalisée par Nowak et Sigmund (Nature, 1993), elle débute par $C$ et maintient son coup uniquement après un gain satisfaisant ($C,C$ ou $D,C$), ce qui lui permet de corriger les erreurs accidentelles tout en exploitant les coopérateurs inconditionnels.

= Étape 1 : Le Tournoi de Base

== Prédictions
Dans une population restreinte aux 5 stratégies du cours, `TIT-FOR-TAT` est l'unique stratégie coopératrice initiale. Sans coalition coopératrice, `ALL-D` exploitera massivement les défections aléatoires de `RANDOM` ($T=5$). `TIT-FOR-TAT` ne pourra compenser son retard face à `ALL-D` que grâce aux points acquis contre son clone en *Self-Play* ($5 times 600 = 3 000$ pts vs $2 000$ pour `ALL-D`). Sans self-play, `ALL-D` est sûrement vainqueur.

== Analyse des Résultats
La victoire de `TIT-FOR-TAT` dépend directement de l'activation du mode *Self-Play*. Avec la configuration par défaut d'Axelrod, `TIT-FOR-TAT` se classe au premier rang avec $12 551.0$ points, devant `TESTER` ($12 537.0$) et `ALL-D` ($11 620.0$). En revanche, sans *Self-Play* (`--no-self-play`), elle rétrograde à la troisième place ($9 548.0$ points), devancée par `ALL-D` ($9 620.0$) et `TESTER` ($9 551.0$).

Cet écart avec le tournoi historique de 1980 s'explique d'abord par la structure écologique de la population. En 1980, la compétition réunissait quatorze stratégies, dont huit coopératrices initiales : `TIT-FOR-TAT` cumulait alors $600$ points face à chacune d'elles, formant une coalition dominante. À l'inverse, dans notre environnement restreint à cinq stratégies, `TIT-FOR-TAT` est l'unique coopératrice initiale face à des profils hostiles (`ALL-D` trahit toujours, `RANDOM` à $50\%$, `JOSS` à $10\%$ et `TESTER` dès le premier tour).

Par ailleurs, `ALL-D` tire un avantage décisif de `RANDOM` en exploitant chaque coopération ($T=5$), ce qui lui rapporte $715.0$ points par match ($3.575$ points/tour), tandis que `TIT-FOR-TAT` subit ce jeu erratique et plafonne à $499.4$ points. Le *Self-Play* joue alors un rôle compensateur essentiel : face à son propre clone, `TIT-FOR-TAT` engrange $3 000$ points ($5 times 600$) contre seulement $2 000$ pour `ALL-D` ($5 times 400$). Sans ce bonus de $+1 000$ points pour neutraliser les gains d'`ALL-D` sur `RANDOM`, la stratégie prédatrice prend la tête du classement.

Ainsi, la performance d'une stratégie n'est jamais absolue ; elle demeure étroitement conditionnée par la distribution écologique de la population adverse.

#figure(
  grid(
    columns: (1fr, 1fr),
    gutter: 8pt,
    [ #csv("data/step1_rankings.csv") ],
    [ #image("assets/step1_rankings.svg") ]
  ),
  caption: ""
)

#figure(
  [ #csv("data/step1_head_to_head.csv") ],
  caption: ""
)

= Étape 2 : Population Élargie

== Prédictions
L'adjonction de deux stratégies coopératrices supplémentaires (`FRIEDMAN` et `PAVLOV`) porte la densité d'agents bienveillants à $3/7$. `TIT-FOR-TAT` et `FRIEDMAN` doivent s'imposer en tête grâce à l'entraide mutuelle ($3.0$ pts/tour). La rentabilité relative d'`ALL-D` doit régresser, car elle sera contenue à $P=2$ (403 points par match) face à la majorité de ses adversaires.

== Analyse des Résultats
L'ajout de `FRIEDMAN` et `PAVLOV` lors de l'étape 2 augmente la densité coopérative de la population et crée une masse critique favorable aux échanges mutuels. Grâce à cette synergie, `TIT-FOR-TAT` conserve solidement le premier rang avec $18 580.0$ points, suivie de près par `FRIEDMAN` en deuxième position ($18 563.0$ points).

À l'inverse, les profils prédateurs et instables perdent d'avantage. Incapable de coopérer avec `FRIEDMAN` et `PAVLOV`, `ALL-D` est limité au gain de punition $P=2$ ($403$ points par match) et recule à la troisième place, ses profits face à `RANDOM` ne suffisant plus à compenser ce déficit. Parallèlement, `JOSS` (6e) et `RANDOM` (7e) régressent, lourdement sanctionnées par les ripostes systématiques de `TIT-FOR-TAT` et de `FRIEDMAN`.

#figure(
  grid(
    columns: (1fr, 1fr),
    gutter: 8pt,
    [ #csv("data/step2_rankings.csv") ],
    [ #image("assets/step2_rankings.svg") ]
  ),
  caption: ""
)

#figure(
  [ #csv("data/step2_head_to_head.csv") ],
  caption: ""
)


= Étape 3 : Modifications du Tournoi

== Modification nº1 : Bruit à 5%

=== Prédictions
Probabilité d'inversion accidentelle par coup : $5%$. Probabilité qu'un match de 200 tours se déroule sans erreur : $P = (1 - 0.05)^(400) approx 1.3 times 10^(-9)$. L'occurrence d'erreurs est quasi-certaine. `FRIEDMAN` (Grim Trigger) doit basculer en défection permanente dès la première erreur. `PAVLOV` doit limiter la perte de points grâce à son mécanisme d'auto-correction en 2 pas.

=== Analyse des Résultats
L'introduction du bruit entraîne l'effondrement de `FRIEDMAN`, dont le taux de coopération chute de $44.4\%$ à $7.6\%$ : la moindre inversion accidentelle ($C \to D$) la verrouille en effet dans une défection permanente. À l'inverse, cette instabilité propulse `ALL-D` à la première place ($17 547$ points), son insensibilité totale aux perturbations lui évitant de subir la pénalité de dupe ($S=0$).

#figure(
  grid(
    columns: (1fr, 1fr),
    gutter: 8pt,
    [ #csv("data/step3_noise_rankings.csv") ],
    [ #image("assets/step3_noise_rankings.svg") ]
  ),
  caption: ""
)

== Modification nº2 : Horizon Inconnu

=== Prédictions


=== Analyse des Résultats


== Modification nº3 : Dynamique Évolutionnaire

=== Prédictions


=== Analyse des Résultats


== Modification nº4 : Matrices de Gains Alternatives

=== Prédictions


=== Analyse des Résultats


= Conclusion
