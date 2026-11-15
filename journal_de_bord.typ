
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

=== ALL-D (Toujours Trahir)
Stratégie dominante de Nash (jeu statique) jouant toujours $D$ sans tenir compte de l'historique, elle exploite les profils naïfs ou désordonnés mais ne crée aucune valeur partagée, plafonnant à $bb(P)=2$ points face à elle-même ou aux stratégies punitives.

=== RANDOM (Aléatoire Équiprobable)
Ligne de base à entropie maximale introduite par Axelrod qui tire ses coups à pile ou face ($bb(P)(C) = bb(P)(D) = 0.5$), elle détruit la confiance des stratégies réactives et sert de manne financière ($T=5$) aux prédateurs opportunistes.

=== TIT-FOR-TAT (Donnant-Donnant)
Double vainqueur des tournois d'Axelrod conçu par Anatol Rapoport, elle joue $C$ au premier tour puis copie le coup adverse précédent, incarnant les quatre vertus d'Axelrod : bienveillance, riposte, indulgence et transparence.

=== TESTER (David Gladstein)
Soumise par David Gladstein au second tournoi d'Axelrod, elle sonde l'adversaire par $D$ au tour 1 : en cas de riposte, elle s'excuse ($C$) et adopte TIT-FOR-TAT (coopérant ensuite sur les 198 tours suivants face à elle), sinon elle l'exploite en alternant $D$ et $C$.

=== JOSS (Johann Joss)
Variante sournoise de TIT-FOR-TAT soumise par Johann Joss qui glisse une défection ($D$) avec une probabilité $p = 0.10$ après une coopération adverse, visant le gain de tentation $T=5$ au risque de déclencher des vendettas irréversibles.

== Les 2 Stratégies Étendues de la Littérature

=== FRIEDMAN (Rancunière)
Stratégie pivot des folk theorems théorisée par James W. Friedman (1971) qui joue $C$ tant que l'autre coopère puis bascule définitivement sur $D$ à la moindre trahison, assurant une dissuasion maximale ($600$ points) mais critique face au moindre bruit.

=== PAVLOV (Win-Stay, Lose-Shift)
Formalisée par Nowak et Sigmund (Nature, 1993), elle débute par $C$ et maintient son coup uniquement après un gain satisfaisant ($C,C$ ou $D,C$), ce qui lui permet de corriger les erreurs accidentelles tout en exploitant les coopérateurs inconditionnels.

= Étape 1 : Le Tournoi de Base

== Prédictions
Dans une population restreinte aux 5 stratégies du cours, TIT-FOR-TAT est l'unique stratégie coopératrice initiale. Sans coalition coopératrice, ALL-D exploitera massivement les défections aléatoires de RANDOM ($T=5$). Prédiction formelle : TIT-FOR-TAT ne pourra compenser son retard face à ALL-D que grâce aux points acquis contre son clone en Self-Play ($5 times 600 = 3\,000$ pts vs $2\,000$ pour ALL-D). Sans self-play, ALL-D est prédite vainqueur.

== Analyse des Résultats


= Étape 2 : Population Élargie

== Prédictions


== Analyse des Résultats


= Étape 3 : Modifications du Tournoi

== Modification nº1 : Bruit à 5%

=== Prédictions


=== Analyse des Résultats


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
