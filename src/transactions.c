#include "../include/bank.h"
#include "../include/json_utils.h"
#include <stdio.h>

void transferMoney(Account accounts[], int count, int currentIndex) {
    BankFeesTransfer fees;
    loadBankFees("../data/fees_data.json", &fees);

    printf("\n=== Transfer Money with Fees ===\n");

    // Afficher les comptes de destination disponibles
    printf("Select the destination account:\n");
    for (int i = 0; i < count; i++) {
        if (i != currentIndex) {
            printf("%d. %s\n", i + 1, accounts[i].name);
        }
    }
    printf("> ");

    int destChoice;
    scanf("%d", &destChoice);

    // Valider le choix de destination
    if (destChoice < 1 || destChoice > count || destChoice == currentIndex + 1) {
        printf("Invalid choice.\n");
        return;
    }

    int destIndex = destChoice - 1;

    printf("Enter the amount to transfer: ");
    int amount;
    scanf("%d", &amount);

    // Valider le montant
    if (amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }

    if (accounts[currentIndex].balance < amount) {
        printf("Insufficient funds.\n");
        return;
    }

    int instantTransferOption;
    printf("Do you want to use instant transfer? (1 for Yes, 0 for No): ");
    scanf("%d", &instantTransferOption);

    double totalFees = 0.0;
    if (instantTransferOption) {
        totalFees += fees.instant_transfer_fee;
    } else {
        totalFees += amount * (fees.standard_transfer_fee / 100);
    }

    // Appliquer la marge sur le taux de change si les devises sont différentes
    double convertedAmount = amount;
    if (accounts[currentIndex].currency != accounts[destIndex].currency) {
        double exchangeRate = 1.0; // Remplacez par le taux de change réel
        if (accounts[currentIndex].currency == USD && accounts[destIndex].currency == EUR) {
            exchangeRate = 0.85; // Exemple de taux de change USD vers EUR
        } else if (accounts[currentIndex].currency == USD && accounts[destIndex].currency == GBP) {
            exchangeRate = 0.75; // Exemple de taux de change USD vers GBP
        } else if (accounts[currentIndex].currency == EUR && accounts[destIndex].currency == USD) {
            exchangeRate = 1.18; // Exemple de taux de change EUR vers USD
        } else if (accounts[currentIndex].currency == EUR && accounts[destIndex].currency == GBP) {
            exchangeRate = 0.88; // Exemple de taux de change EUR vers GBP
        } else if (accounts[currentIndex].currency == GBP && accounts[destIndex].currency == USD) {
            exchangeRate = 1.33; // Exemple de taux de change GBP vers USD
        } else if (accounts[currentIndex].currency == GBP && accounts[destIndex].currency == EUR) {
            exchangeRate = 1.14; // Exemple de taux de change GBP vers EUR
        }

        // Appliquer la marge sur le taux de change
        if (accounts[currentIndex].currency < accounts[destIndex].currency) {
            exchangeRate *= (1 - fees.exchange_rate_margin / 100);
        } else {
            exchangeRate *= (1 + fees.exchange_rate_margin / 100);
        }

        convertedAmount = amount * exchangeRate;
    }

    // Vérifier si le compte source a suffisamment de fonds pour couvrir le montant du transfert plus les frais
    if (accounts[currentIndex].balance < amount + totalFees) {
        printf("Insufficient funds including fees.\n");
        return;
    }

    // Afficher un résumé du transfert
    printf("\nRésumé du Transfert :\n");
    printf("- Montant à transférer : %d\n", amount);
    printf("- Type de virement : %s\n", instantTransferOption ? "Instantané" : "Classique");
    printf("- Frais de transfert : %.2f\n", totalFees);
    if (accounts[currentIndex].currency != accounts[destIndex].currency) {
        printf("- Taux de change appliqué : %.4f\n", convertedAmount / amount);
    }
    printf("- Montant reçu : %.2f\n", convertedAmount);
    printf("- Total déduit de votre compte : %.2f\n", amount + totalFees);

    printf("Do you want to proceed with this transfer? (1 for Yes, 0 for No): ");
    int proceed;
    scanf("%d", &proceed);

    if (proceed == 1) {
        accounts[currentIndex].balance -= (int)(amount + totalFees);
        accounts[destIndex].balance += (int)convertedAmount;

        printf("✅ Transfer successful!\n");
        printf("New balance for %s: %d\n", accounts[currentIndex].name, accounts[currentIndex].balance);
        printf("New balance for %s: %d\n", accounts[destIndex].name, accounts[destIndex].balance);
    } else {
        printf("Transfer cancelled.\n");
    }
}
