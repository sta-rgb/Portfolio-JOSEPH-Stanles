# Financial Data Analysis Portfolio Project

![Python](https://img.shields.io/badge/Python-3.10%2B-blue?logo=python)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Status](https://img.shields.io/badge/Status-In%20Progress-orange)

## Contexte métier
Ce projet simule une mission de conseil Big 4 : construire une analyse financière robuste, traçable et industrialisable pour aider à la prise de décision (performance, risque, rentabilité).

## Méthodologie
1. **Cadrage métier** : définition des KPI financiers et des hypothèses.
2. **Data engineering léger** : ingestion, nettoyage, validation des données.
3. **Exploration analytique** : EDA sur notebooks brouillons.
4. **Modélisation** : segmentation, prévision ou scoring selon le cas d'usage.
5. **Restitution** : visualisations, rapport synthétique, recommandations actionnables.

## Architecture du projet

### Commandes Bash pour générer l'arborescence
```bash
mkdir -p data/raw data/processed \
         notebooks/drafts \
         src/{data,features,models,visualization,utils} \
         reports/figures \
         deliverables

touch src/data/__init__.py src/features/__init__.py src/models/__init__.py \
      src/visualization/__init__.py src/utils/__init__.py
```

### Structure cible
```text
.
├── data/
│   ├── raw/               # Données brutes (non versionnées)
│   └── processed/         # Données transformées (non versionnées)
├── notebooks/
│   └── drafts/            # Notebooks exploratoires / brouillons
├── src/
│   ├── data/              # Ingestion, validation, nettoyage
│   ├── features/          # Feature engineering
│   ├── models/            # Entraînement / inférence
│   ├── visualization/     # Graphiques et dashboards
│   └── utils/             # Fonctions utilitaires
├── reports/
│   └── figures/           # Figures exportées
├── deliverables/          # Livrables finaux (slides, synthèses)
├── requirements.txt
└── README.md
```

## Exécution locale
```bash
python -m venv .venv
source .venv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
jupyter notebook
```

## Licence
Distribué sous licence MIT. Voir `LICENSE`.
