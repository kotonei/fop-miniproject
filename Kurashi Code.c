#include <stdio.h>   // Library for input/output functions (printf, fgets, sprintf)
#include <stdlib.h>  // Library for general utilities (system, rand, srand, exit)
#include <string.h>  // Library for string manipulation (strcmp, strlen)
#include <time.h>    // Library for time functions, used to seed randomness

// The main entry point of the program
int main(void) {
    // Seed the random number generator using the current time.
    // If we don't do this, rand() produces the same sequence of numbers every time.
    srand((unsigned) time(NULL));

    // The Database
    // These arrays store the visual characters and their answers.
    // Index 0 in hiragana matches Index 0 in romaji.

    const char *hiragana[] = {
        // Basic Characters (Indices 0-45)
        "あ","い","う","え","お", "か","き","く","け","こ",
        "さ","し","す","せ","そ", "た","ち","つ","て","と",
        "な","に","ぬ","ね","の", "は","ひ","ふ","へ","ほ",
        "ま","み","む","め","も", "や","ゆ","よ",
        "ら","り","る","れ","ろ", "わ","を","ん",
        // Dakuten (Indices 46-65), Sounds with (")
        "が","ぎ","ぐ","げ","ご", "ざ","じ","ず","ぜ","ぞ",
        "だ","ぢ","づ","で","ど", "ば","び","ぶ","べ","ぼ",
        // Handakuten (Indices 66-70), Sounds with (°)
        "ぱ","ぴ","ぷ","ぺ","ぽ"
    };

    const char *katakana[] = {
        // Basic Characters (Indices 0-45)
        "ア","イ","ウ","エ","オ", "カ","キ","ク","ケ","コ",
        "サ","シ","ス","セ","ソ", "タ","チ","ツ","テ","ト",
        "ナ","ニ","ヌ","ネ","ノ", "ハ","ヒ","フ","ヘ","ホ",
        "マ","ミ","ム","メ","モ", "ヤ","ユ","ヨ",
        "ラ","リ","ル","レ","ロ", "ワ","ヲ","ン",
        // Dakuten (Indices 46-65), Sounds with (")
        "ガ","ギ","グ","ゲ","ゴ", "ザ","ジ","ズ","ゼ","ゾ",
        "ダ","ヂ","ヅ","デ","ド", "バ","ビ","ブ","ベ","ボ",
        // Handakuten (Indices 66-70), Sounds with (°)
        "パ","ピ","プ","ペ","ポ"
    };

    // The Answer Key. This is shared because "ka" is the answer for both "か" and "カ".
    const char *romaji[] = {
        // Basic Characters (Indices 0-45)
        "a","i","u","e","o", "ka","ki","ku","ke","ko",
        "sa","shi","su","se","so", "ta","chi","tsu","te","to",
        "na","ni","nu","ne","no", "ha","hi","fu","he","ho",
        "ma","mi","mu","me","mo", "ya","yu","yo",
        "ra","ri","ru","re","ro", "wa","wo","n",
        // Dakuten (Indices 46-65), Sounds with (")
        "ga","gi","gu","ge","go", "za","ji","zu","ze","zo",
        "da","ji","zu","de","do", "ba","bi","bu","be","bo",
        // Handakuten (Indices 66-70), Sounds with (°)
        "pa","pi","pu","pe","po"
    };

    // Variables and Menu
    int total_items = 46;       // Default limit: 46 means we only ask Basic questions.
    int is_katakana = 0;        // Flag: 0 means show Hiragana, 1 means show Katakana.
    char input_buffer[256];     // Temporary storage for the user's menu choice.
    char answer[256];           // Storage for the user's answer during the quiz.
    char cmd[256];              // Storage to build the Linux terminal command.

    // Prepare the title command using the 'toilet' program (ASCII art generator).
    sprintf(cmd, "toilet -f mono12 'Kurashi'");
    
    // Execute the command in the terminal to show the title.
    system("clear");
    system(cmd);

    // Print instructions and menu options to the user.
    printf("This is a quiz game that tests you on your knowledge of the kanas in the Japanese language.\n");
    printf("1. Hiragana (Basic (excluding the dakuten and handakuten))\n");
    printf("2. Hiragana (Full)\n");
    printf("3. Katakana (Basic (excluding the dakuten and handakuten))\n");
    printf("4. Katakana (Full)\n");
    printf("Select a game mode (1-4): ");

    // Get the user's menu selection.
    // fgets reads until the user hits Enter. If it returns NULL (error), we skip the block.
    if (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        
        char choice = input_buffer[0]; // Take the very first character they typed.
        
        // Logic to configure the game based on the choice.
        if (choice == '1') {
            is_katakana = 0;   // Set to Hiragana
            total_items = 46;  // Set limit to Basic only
            printf("\nStarting Hiragana (Basic)...\n");
        } 
        else if (choice == '2') {
            is_katakana = 0;   // Set to Hiragana
            total_items = 71;  // Set limit to Full (Basic + Dakuten + Handakuten)
            printf("\nStarting Hiragana (Full)...\n");
        }
        else if (choice == '3') {
            is_katakana = 1;   // Set to Katakana
            total_items = 46;  // Set limit to Basic only
            printf("\nStarting Katakana (Basic)...\n");
        }
        else if (choice == '4') {
            is_katakana = 1;   // Set to Katakana
            total_items = 71;  // Set limit to Full (Basic + Dakuten + Handakuten)
            printf("\nStarting Katakana (Full)...\n");
        }
        else {
            // If the user types an invalid option like "9", default to Mode 1.
            printf("Invalid choice. Defaulting to Hiragana Basic.\n");
        }
    }

    printf("Type 'exit' or Ctrl+D to quit.\n\n");

    // Game initialization
    // Create an array to hold the order of questions (indices 0 to 70).
    int order[71];
    
    // Fill the array with numbers in sequence: 0, 1, 2, 3... up to total_items.
    for (int i = 0; i < total_items; i++) {
        order[i] = i;
    }
    
    int asked = 0;   // Counter for total questions attempted.
    int correct = 0; // Counter for total correct answers.

    // Game loop
    // while(1) creates a loop that never stops on its own.
    // We rely on "goto end_game" to break out of it.
    while (1) {
        
        // Shuffling using Fisher-Yates algorithm
        // This randomizes the 'order' array so questions don't appear alphabetically.
        for (int i = total_items - 1; i > 0; i--) {
            int j = rand() % (i + 1); // Pick a random slot 'j'
            int tmp = order[i];       // Swap slot 'i' with slot 'j'
            order[i] = order[j];
            order[j] = tmp;
        }

        // Question loop
        // Iterate through the newly shuffled list of questions.
        for (int index = 0; index < total_items; index++) {
            int i = order[index]; // Get the actual character ID (e.g., 5 might be 'ka')

            // Display logic
            // Check our flag to see which character set to display.
            if (is_katakana == 1) {
                // If Katakana mode, prepare command to show Katakana character.
                sprintf(cmd, "toilet -f mono12 '%s'", katakana[i]);
            } else {
                // Otherwise, prepare command to show Hiragana character.
                sprintf(cmd, "toilet -f mono12 '%s'", hiragana[i]);
            }
            
            // Execute the toilet command to draw the ASCII art.
            system(cmd);

            printf("Romanization: ");
            
            // Input handling
            // Wait for user input. 
            // If fgets returns NULL (meaning they pressed Ctrl+D), exit game.
            if (!fgets(answer, sizeof(answer), stdin)) {
                goto end_game;
            }

            // Calculate length of the input string.
            int len = strlen(answer);
            
            // Check if the last character is a newline ('\n').
            // fgets includes the Enter key press, so we usually need to remove it.
            if (answer[len - 1] == '\n') {
                answer[len - 1] = '\0'; // Replace newline with Null Terminator (end of string).
            }

            // Exit check
            // Compare the cleaned input to the word "exit".
            if (strcmp(answer, "exit") == 0) {
                goto end_game; // Jump to the end label.
            }

            asked++; // Increment the number of questions asked.

            // Check answer
            // Compare user input with the correct Romaji from our database.
            if (strcmp(answer, romaji[i]) == 0) {
                correct++;          // Increment score.
                printf("Correct!\n");
            } else {
                // Show the correct answer if they got it wrong.
                printf("Wrong! It was '%s'.\n", romaji[i]);
            }

            // Display score
            // Calculate percentage: (correct / asked) * 100.
            // %.1f%% formats it to 1 decimal place (e.g., 50.5%).
            printf("Score: %d / %d (%.1f%%)\n\n", correct, asked, correct * 100.0 / asked);
        }
    }

// End Game
// This is a label. The 'goto' command jumps exactly to this spot.
end_game:
    double finalPercentage = 0.0;
    
    // Final calculation
    // Check if 'asked' is greater than 0 to prevent division by zero.
    if (asked > 0) {
        finalPercentage = (correct * 100.0) / asked;
    }

    // Print the final result. %.0f rounds to the nearest whole number.
    printf("\nFinal score: %d / %d (%.0f%%)\n", correct, asked, finalPercentage);
    
    // Return 0 tells the operating system "The program finished successfully."
    return 0;
}