
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to check password strength
int checkPassword(const char password[]) {

    int length = strlen(password);

    int IsUpper = 0;
    int IsLower = 0;
    int IsDigit = 0;
    int IsSymbol = 0;
    int IsLength = 0;

    int score = 0;

    // Check password length
    if (length >= 8) {
        IsLength = 1;
    }

    // Check each character in the password
    for (int i = 0; i < length; i++) {

        unsigned char ch = (unsigned char)password[i];

        if (isupper(ch)) {
            IsUpper = 1;
        }

        if (islower(ch)) {
            IsLower = 1;
        }

        if (isdigit(ch)) {
            IsDigit = 1;
        }

        if (ispunct(ch)) {
            IsSymbol = 1;
        }
    }

    // Calculate the total score
    score = IsLength + IsUpper + IsLower
            + IsDigit + IsSymbol;

    // Display password information
    printf("\n========== Password Report ==========\n");

    printf("Password Length: %d Is:\n", length);

    printf("Minimum Length (8): %s\n",
           IsLength ? "Passed" : "Not Passed");

    printf("Uppercase Letters: %s\n",
           IsUpper ? "Found" : "Missing");

    printf("Lowercase Letters: %s\n",
           IsLower ? "Found" : "Missing");

    printf("Numbers: %s\n",
           IsDigit ? "Found" : "Missing");

    printf("Special Symbols: %s\n",
           IsSymbol ? "Found" : "Missing");

    printf("-------------------------------------\n");

    printf("Password Score: %d/5\n", score);

    // Determine password strength
    if (score == 5) {
        printf("Password Strength: STRONG\n");
    }
    else if (score >= 3) {
        printf("Password Strength: ACCEPTABLE\n");
    }
    else {
        printf("Password Strength: WEAK\n");
    }

    // Display suggestions for missing requirements
    if (score < 5) {

        printf("\nSuggestions to improve your password:\n");

        if (!IsLength) {
            printf("- Use at least 8 characters.\n");
        }

        if (!IsUpper) {
            printf("- Add an uppercase letter (A-Z).\n");
        }

        if (!IsLower) {
            printf("- Add a lowercase letter (a-z).\n");
        }

        if (!IsDigit) {
            printf("- Add at least one number (0-9).\n");
        }

        if (!IsSymbol) {
            printf("- Add a special symbol (@, #, !, etc.).\n");
        }
    }
    else {
        printf("\nAll security requirements have been met.\n");
    }

    printf("=====================================\n");

    return score;
}

int main(void) {

    char password[100];

    printf("       Password Strength Checker     \n");

    printf("Enter your password: ");

    // Read password safely
    if (scanf("%99s", password) != 1) {
        printf("Error: Unable to read password.\n");
        return 1;
    }

    // Check password and display the report
    checkPassword(password);

    return 0;
}
