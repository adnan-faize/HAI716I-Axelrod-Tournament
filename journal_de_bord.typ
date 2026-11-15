
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
  #block(width: 75%)[
    #text(size: 11pt, weight: "medium", fill: luma(60))[
      *Résumé :* Ce journal de bord documente la conception, l'implémentation en C++20 et l'expérimentation d'un simulateur de tournois pour le dilemme du prisonnier, suivant les protocoles fondateurs de Robert Axelrod (1980). Le système confronte dix stratégies issues de la théorie des jeux et des systèmes multi-agents sous divers régimes environnementaux : tournoi initial de base à cinq stratégies, population enrichie à dix stratégies, présence de bruit de transmission (5%), horizon temporel probabiliste ($w=0.995$), sélection naturelle par dynamique des réplicateurs sur cinquante générations, et matrices de gains alternatives (Poule Mouillée et Chasse au Cerf). Les résultats sont automatiquement consolidés sous forme de jeux de données CSV importés dans ce document et visualisés par graphiques vectoriels SVG. Une analyse rigoureuse répond à la question centrale du sujet relative à la domination conditionnelle de TIT-FOR-TAT et à l'impact critique de la composition écologique de la population.
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

== Spécification des Paramètres Expérimentaux

#pagebreak()

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

#pagebreak()

= Étape 1 : Le Tournoi de Base

= Étape 2 : Population Élargie

= Étape 3 : Modifications du Tournoi
