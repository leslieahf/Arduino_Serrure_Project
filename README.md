# 🔐 Projet : Serrure Électronique avec Code — Arduino MEGA

Projet réalisé par **OCLOO Leslie**, **ANTOINE David**, et **MARQUES LEBEN Michèle**.

---

## 📌 Présentation du Projet

Ce projet consiste à concevoir et programmer une **serrure électronique** capable de déverrouiller une porte grâce à la saisie d’un **code secret** via un **clavier matriciel 4x4**.  
Lorsqu’un utilisateur entre le bon code, un **servo-moteur** (ou relais) active l’ouverture de la serrure.

Ce système vise à reproduire un mécanisme de verrouillage sécurisé simple, basé sur l'Arduino MEGA et l'utilisation d’entrées numériques.

---

## 🔧 Composants Utilisés

- **Arduino UNO**  
- **Clavier matriciel 4x4**  
- **Servo-moteur** *ou* **relais** pour la commande de la serrure  
- Câbles de connexion (jumpers)  
- Optionnel : Écran LCD pour le retour visuel (non obligatoire)

---

## 🧠 Compétences Mobilisées

### ✔️ Entrées numériques  
Lecture du clavier matriciel 4x4 via les broches numériques de l'Arduino.

### ✔️ Gestion de code secret  
Stockage, comparaison et validation du code entré par l’utilisateur.

### ✔️ Contrôle moteur  
Activation du servo-moteur pour simuler l'ouverture et la fermeture de la serrure.

---

## ⚙️ Fonctionnement du Système

1. L’utilisateur saisit un code via le clavier 4x4.  
2. L’Arduino compare la séquence entrée au **code prédéfini**.  
3. Si le code est correct :  
   - Le servo tourne pour **déverrouiller** la serrure.  
   - Un message peut être affiché (ex : *Accès autorisé*).  
4. Si le code est incorrect :  
   - La serrure reste verrouillée.  
   - Un message peut signaler l’erreur (ex : *Code incorrect*).  


