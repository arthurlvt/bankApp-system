# Welcome on bankApp Project

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/ton-utilisateur/bank-system)

A secure and feature-rich banking system with account management, currency conversion, and real-time exchange rate fetching.

> [!IMPORTANT]
> ***Before reading this project, you need to consult the [LICENSE](https://github.com/arthurlvt/bankApp-system/LICENSE) !!!***

---

## 🌟 Features

### Core Features
- **Account Management**: Create, access, and delete accounts with secure password protection.
- **Money Transfer**: Transfer money between accounts.
- **Currency Conversion**: Convert amounts between USD, EUR, and GBP.
- **Real-Time Exchange Rates**: Fetch and display current exchange rates using an external API.
- **Secure Password Storage**: Passwords are hashed and salted for secure storage.

### Advanced Features
- **Transaction History**: View a detailed history of all transactions.
- **Budgeting**: Set budgets and categorize expenses.
- **Savings Accounts**: Create savings accounts with interest rates.
- **Joint Accounts**: Manage accounts shared between multiple users.

---

## Security issues and ideas
- (Modifier le fichier pour pas pouvoir mettre d'autre caractères que ceux
  prévus dans les options)
- Ajouter des fonctions pour simuler les frais de change // 1% par frais pour avantager la banque et non l'utilisateur
- Revoir le système de transfert d'argent
- Stocker le dernier rapport de change pour avoir un truc cohérent

---

## 📋 🛠 Requirements & Setup

-> To start to use the bankApp-system, you first need to install some libraries and others

1. **Clone the repository:**

   ```bash
   git clone https://github.com/ton-utilisateur/bank-system.git
   cd bank-system
   ```
2. **What you need next?**
   - A C compiler (e.g., `clang` or `gcc`)
   - `libcurl` for HTTP requests
   - `make` for building the project
   - `git` for version control

3. **Install Dependencies**
   - On MacOS (if you use Homebrew)
      ```bash
      brew install curl
      ````
   - On Ubuntu/Debian
      ```bash
      sudo apt-get install libcurl4-openssl-dev
      ````
3. **Build the project / run it**
   - Run with a compiler clang / gcc
   ```bash
   gcc bankSystem.c -o bankSystem
   ./bankSystem
   ```
   ```bash
   clang bankSystem.c -o bankSystem -lcurl
   ```
   - Run with a makefile
   ```bash
   make
   ./bankSystem
   ```

4. **🔧 Configuration**
> To use the exchange rate API, replace "YOUR KEY API" in main.c with your actual API key from a service like [ExchangeRate-API](https://app.exchangerate-api.com/)

>[!IMPORTANT] 
> For enhanced security, configure your API key in a separate configuration file.

---

## Structure of the Project

```plaintext
bank-system/
├── include/
│   ├── bank.h
│   └── utils.h
├── src/
│   ├── accounts.c
│   ├── exchange_rates.c
│   ├── main.c
│   ├── transactions.c
│   └── utils.c
├── Makefile
├── README.md
└── LICENSE
```

---

## 📜 Roadmap

### Version 1.1
- [x] Make a simple system with adding, removing, transfer functions
- [x] Add transaction history feature.
- [x] Implement creating, savings and deleting accounts with interest rates.
- [x] Implement exchange rates with permanent upload
- [x] Improve error handling and make a handler

### Version 1.2
- [ ] Develop a simple graphical user interface
- [ ] Add support for more currencies
- [ ] Implement exchange rate alerts
- [ ] Replace text files with a database (SQLite, MySQL).
- [ ] Develop a REST API.
- [ ] Create a mobile application.

...more next
