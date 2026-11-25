// DONT CHANGE ANYTHING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_TASKS 50
#define TASK_LENGTH 100
#define INPUT_LENGTH 200
#define KEYWORD_LENGTH 25

/* finding array size for keywords list*/
#define COUNT(list) (sizeof(list) / sizeof(list[0]))

/* ANSI colours. If they look strange in your terminal,
   change each of these to "" (empty quotes). */
#define COLOR_RESET "\033[0m"
#define COLOR_BOT   "\033[36m"
#define COLOR_TITLE "\033[33m"
#define COLOR_ERROR "\033[31m"

/* global variable for task functions */
char tasks[MAX_TASKS][TASK_LENGTH];
int taskCount = 0;

/* protos for funcs, dont mess */
void showBanner(void);
void botSay(char message[]);
void readLine(char buffer[], int size);
int readInt(char prompt[]);
double readNumber(char prompt[]);
void toLowerCase(char text[]);
int isBlank(char text[]);
int containsKeyword(char text[], char keyword[]);
int matchAny(char text[], char list[][KEYWORD_LENGTH], int count);
int wantsExit(char text[]);
int wantsHelp(char text[]);
int wantsCalculator(char text[]);
int wantsCooking(char text[]);
int wantsGame(char text[]);
int wantsTodo(char text[]);
void chatResponse(char text[]);
void showHelp(void);
void calculator(void);
int findRecipe(char text[]);
void showRecipeList(void);
void showRecipe(int number);
void cookingAssistant(char text[]);
void guessingGame(void);
void todoAssistant(void);
void addTask(void);
void viewTasks(void);
void deleteTask(void);

/* main function, base for the chatbot,  */
int main(void)
{
    char input[INPUT_LENGTH];

    srand((unsigned int)time(NULL));   /* seed random numbers once */
    showBanner();

    while (1)
    {
        printf("You: ");
        readLine(input, INPUT_LENGTH);
        toLowerCase(input);

        if (isBlank(input))
        {
            botSay("Forgot to say something?");
        }
        else if (wantsExit(input))
        {
            botSay(":( See you soon!");
            break;
        }
        else if (wantsCalculator(input))
        {
            calculator();
        }
        else if (wantsCooking(input))
        {
            cookingAssistant(input);
        }
        else if (wantsGame(input))
        {
            guessingGame();
        }
        else if (wantsTodo(input))
        {
            todoAssistant();
        }
        else if (wantsHelp(input))
        {
            showHelp();
        }
        else
        {
            chatResponse(input);
        }
    }

    return 0;
}

/* Entry banner REMOVE IF GUI */
void showBanner(void)
{
    printf(COLOR_TITLE);
    printf("==================================================\n");
    printf("                    B Y T E B O T\n");
    printf("          Your friendly chatbot written in C\n");
    printf("==================================================\n");
    printf(COLOR_RESET);
    botSay("Hello! I'm ByteBot. I was made for a final semester project! I can chat, calculate, share recipes,\n"
           "play a guessing game and manage a to-do list.\n"
           "Type 'help' to see examples, or 'exit' to leave.");
}

void botSay(char message[])
{
    printf("\n" COLOR_BOT "ByteBot: " COLOR_RESET "%s\n\n", message);
}

/* Input functions below: */

/* reads line by line, remove newline,  */
void readLine(char buffer[], int size)
{
    int length;
    int ch;

    if (fgets(buffer, size, stdin) == NULL)
    {
        /* if user ends the input manually, this stops the loop*/
        printf("\n\nInput ended. Goodbye!\n");
        exit(0);
    }

    length = (int)strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n')
    {
        buffer[length - 1] = '\0';
    }
    else
    {
        /* Line was too long: throw away the rest of it */
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
        }
    }
}

/* keep looping until user enters a number */
int readInt(char prompt[])
{
    char line[INPUT_LENGTH];
    int value;
    char extra;

    while (1)
    {
        printf("%s", prompt);
        readLine(line, INPUT_LENGTH);

        /* will return 1 if a nubmer was read and NOTHING else follows */
        if (sscanf(line, "%d %c", &value, &extra) == 1)
        {
            return value;
        }
        printf(COLOR_ERROR "Invalid input. Please enter a whole number.\n" COLOR_RESET);
    }
}

/* loop until reads a number(decimals allowed) */
double readNumber(char prompt[])
{
    char line[INPUT_LENGTH];
    double value;
    char extra;

    while (1)
    {
        printf("%s", prompt);
        readLine(line, INPUT_LENGTH);

        if (sscanf(line, "%lf %c", &value, &extra) == 1)
        {
            return value;
        }
        printf(COLOR_ERROR "Invalid input. Please enter a number.\n" COLOR_RESET);
    }
}

/* Text and keyword funcs
   this is to convert to lower */
void toLowerCase(char text[])
{
    int i;
    for (i = 0; text[i] != '\0'; i++)
    {
        text[i] = (char)tolower((unsigned char)text[i]);
    }
}

int isBlank(char text[])
{
    int i;
    for (i = 0; text[i] != '\0'; i++)
    {
        if (!isspace((unsigned char)text[i]))
        {
            return 0;
        }
    }
    return 1;
}

/* checks for keywords as a WHOLE word */
int containsKeyword(char text[], char keyword[])
{
    int keywordLength = (int)strlen(keyword);
    char *found = text;
    int start;
    int end;
    int okBefore;
    int okAfter;

    while ((found = strstr(found, keyword)) != NULL)
    {
        start = (int)(found - text);
        end = start + keywordLength;

        okBefore = (start == 0) || !isalnum((unsigned char)text[start - 1]);
        okAfter = (text[end] == '\0') || !isalnum((unsigned char)text[end]);

        if (okBefore && okAfter)
        {
            return 1;
        }
        found++;   /* move on and look for the next match */
    }
    return 0;
}

/* Returns 1 if ANY keyword from the list is found in text */
int matchAny(char text[], char list[][KEYWORD_LENGTH], int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        if (containsKeyword(text, list[i]))
        {
            return 1;
        }
    }
    return 0;
}

/* it compares with keyword array to check if any words matchs, call the MatchAny func, and returns it */
int wantsExit(char text[])
{
    char words[][KEYWORD_LENGTH] = {"bye", "goodbye", "quit", "exit",
                                    "see you", "see ya", "good night"};
    return matchAny(text, words, (int)COUNT(words));
}

int wantsHelp(char text[])
{
    char words[][KEYWORD_LENGTH] = {"help", "commands", "what can you do",
                                    "features", "instructions", "how to use"};
    return matchAny(text, words, (int)COUNT(words));
}

int wantsCalculator(char text[])
{
    char words[][KEYWORD_LENGTH] = {"calculator", "calculate", "calculation",
                                    "calc", "math", "maths", "arithmetic"};
    return matchAny(text, words, (int)COUNT(words));
}

int wantsCooking(char text[])
{
    char words[][KEYWORD_LENGTH] = {"cook", "cooking", "recipe", "recipes",
                                    "hungry", "kitchen"};

    if (findRecipe(text) != 0)
    {
        return 1;
    }
    return matchAny(text, words, (int)COUNT(words));
}

int wantsGame(char text[])
{
    char words[][KEYWORD_LENGTH] = {"game", "games", "guessing game",
                                    "guess the number", "number game",
                                    "play a game", "guess game"};
    return matchAny(text, words, (int)COUNT(words));
}

int wantsTodo(char text[])
{
    char words[][KEYWORD_LENGTH] = {"todo", "to-do", "to do list", "task",
                                    "tasks", "checklist"};
    return matchAny(text, words, (int)COUNT(words));
}

/* conversations i'm sorry for the jokes */
void chatResponse(char text[])
{
    char thanks[][KEYWORD_LENGTH] = {"thank you", "thanks", "thankyou", "thx",
                                     "appreciate it"};
    char sorry[][KEYWORD_LENGTH] = {"sorry", "apologize", "apologies",
                                    "my bad", "forgive me"};
    char works[][KEYWORD_LENGTH] = {"how do you work", "how does it work",
                                    "how you work", "how are you made",
                                    "how were you made", "how do you think"};
    char identity[][KEYWORD_LENGTH] = {"your name", "who are you",
                                       "what is your name", "what's your name",
                                       "introduce yourself",
                                       "tell me about yourself",
                                       "who made you", "who created you"};
    char howAreYou[][KEYWORD_LENGTH] = {"how are you", "how r u", "how do you do",
                                        "how is your day", "how's your day",
                                        "how was your day", "how is it going",
                                        "how's it going", "what's up", "whats up",
                                        "how have you been"};
    char joke[][KEYWORD_LENGTH] = {"joke", "jokes", "make me laugh",
                                   "cheer me up", "entertain me",
                                   "something funny"};
    char compliment[][KEYWORD_LENGTH] = {"you are smart", "you're smart",
                                         "you are great", "you're great",
                                         "you are awesome", "you're awesome",
                                         "you are amazing", "you're amazing",
                                         "you are cool", "you're cool",
                                         "you are funny", "you're funny",
                                         "you are helpful", "you're helpful",
                                         "you are the best", "good bot",
                                         "nice bot", "great job", "good job",
                                         "well done", "love you", "i like you"};
    char sad[][KEYWORD_LENGTH] = {"sad", "unhappy", "not happy", "depressed",
                                  "lonely", "feeling down", "feel down",
                                  "upset", "crying", "heartbroken", "miserable",
                                  "feel bad"};
    char angry[][KEYWORD_LENGTH] = {"angry", "furious", "annoyed", "irritated",
                                    "frustrated", "mad"};
    char stressed[][KEYWORD_LENGTH] = {"stress", "stressed", "stressful",
                                       "anxious", "anxiety", "overwhelmed",
                                       "worried", "nervous", "pressure"};
    char tired[][KEYWORD_LENGTH] = {"tired", "sleepy", "exhausted", "fatigued",
                                    "drained", "worn out", "no energy"};
    char happy[][KEYWORD_LENGTH] = {"happy", "glad", "excited", "joyful",
                                    "delighted", "cheerful", "feel great",
                                    "feeling great", "feel good", "feeling good",
                                    "feel awesome"};
    char greetings[][KEYWORD_LENGTH] = {"hello", "hi", "hey", "hiya", "howdy",
                                        "greetings", "good morning",
                                        "good afternoon", "good evening"};

    char jokes[5][100] = {
        "Why do programmers prefer dark mode? Because light attracts bugs!",
        "There are 10 kinds of people: those who understand binary and those who don't.",
        "A SQL query walks into a bar, walks up to two tables and asks: Can I join you?",
        "Why was the computer cold? It left its Windows open!",
        "I would tell you a UDP joke, but you might not get it."
    };

    if (matchAny(text, thanks, (int)COUNT(thanks)))
    {
        botSay("You're welcome! Happy to help.");
    }
    else if (matchAny(text, sorry, (int)COUNT(sorry)))
    {
        botSay("No problem at all! There's nothing to apologise for.");
    }
    else if (matchAny(text, works, (int)COUNT(works)))
    {
        botSay("I scan your message for keywords, like 'recipe' or 'calculator'.\n"
               "         When I find one, I run the matching part of my C program.\n"
               "         I have no internet and no AI - just strings, loops and ifs!");
    }
    else if (matchAny(text, identity, (int)COUNT(identity)))
    {
        botSay("I'm ByteBot, a small chatbot written in C for a Programming\n"
               "         Fundamentals project. I run entirely on this computer.");
    }
    else if (matchAny(text, howAreYou, (int)COUNT(howAreYou)))
    {
        botSay("I'm doing great, thanks for asking! Every line of my code is\n"
               "         running smoothly. How are you?");
    }
    else if (matchAny(text, joke, (int)COUNT(joke)))
    {
        botSay(jokes[rand() % 5]);
    }
    else if (matchAny(text, compliment, (int)COUNT(compliment)))
    {
        botSay("Aw, thank you! That makes my circuits glow.");
    }
    else if (matchAny(text, sad, (int)COUNT(sad)))
    {
        botSay("I'm sorry you're feeling this way. Your feelings are valid.\n"
               "         Talking to a friend or family member can really help.\n"
               "         If you like, I can tell you a joke to lighten the mood.");
    }
    else if (matchAny(text, angry, (int)COUNT(angry)))
    {
        botSay("That sounds frustrating. Try taking a few slow, deep breaths.\n"
               "         A short walk or a glass of water can help you cool down.");
    }
    else if (matchAny(text, stressed, (int)COUNT(stressed)))
    {
        botSay("Stress is tough. Try breaking your work into small steps.\n"
               "         I can help - just open my to-do list by typing 'todo'!");
    }
    else if (matchAny(text, tired, (int)COUNT(tired)))
    {
        botSay("You sound tired. Maybe take a short break, drink some water\n"
               "         and get some rest. You deserve it!");
    }
    else if (matchAny(text, happy, (int)COUNT(happy)))
    {
        botSay("That's wonderful to hear! Your happiness is contagious.");
    }
    else if (matchAny(text, greetings, (int)COUNT(greetings)))
    {
        switch (rand() % 3)
        {
            case 0:
                botSay("Hello! I'm ByteBot. How can I help you today?");
                break;
            case 1:
                botSay("Hi there! Type 'help' to see what I can do.");
                break;
            default:
                botSay("Hey! Nice to see you. What would you like to do?");
                break;
        }
    }
    else
    {
        botSay("Hmm, I'm not sure I understand that.\n"
               "         Try 'help' to see everything I can do.");
    }
}

void showHelp(void)
{
    printf(COLOR_TITLE "\n------------------- ByteBot Help -------------------\n" COLOR_RESET);
    printf(" CHAT         Try: \"hello\", \"how are you\", \"what is your name\",\n");
    printf("              \"I feel sad\", \"tell me a joke\", \"thanks\"\n\n");
    printf(" CALCULATOR   Try: \"calculator\", \"I want to calculate\"\n");
    printf("              Add, subtract, multiply or divide two numbers.\n\n");
    printf(" COOKING      Try: \"how do I cook pasta\", \"chicken curry recipe\"\n");
    printf("              Recipes: pasta, chicken curry, omelette,\n");
    printf("              fried rice, sandwich.\n\n");
    printf(" GAME         Try: \"play a game\", \"guessing game\"\n");
    printf("              Guess my number between 1 and 100.\n\n");
    printf(" TO-DO LIST   Try: \"todo\", \"open my task list\"\n");
    printf("              Add, view and delete tasks (up to %d).\n\n", MAX_TASKS);
    printf(" EXIT         Try: \"bye\", \"quit\", \"exit\"\n");
    printf(COLOR_TITLE "----------------------------------------------------\n\n" COLOR_RESET);
}

/* calc function calc is short for calculator */
void calculator(void)
{
    int choice;
    double firstNumber;
    double secondNumber;
    double result = 0;
    char symbol = '?';

    printf(COLOR_TITLE "\n--------------- Calculator ---------------\n" COLOR_RESET);
    printf("  1. Addition        (+)\n");
    printf("  2. Subtraction     (-)\n");
    printf("  3. Multiplication  (*)\n");
    printf("  4. Division        (/)\n");
    printf("  5. Back to chat\n");
    printf("------------------------------------------\n");

    /* loops until menu choice is valid */
    while (1)
    {
        choice = readInt("Choose an option (1-5): ");
        if (choice >= 1 && choice <= 5)
        {
            break;
        }
        printf(COLOR_ERROR "Please choose a number from 1 to 5.\n" COLOR_RESET);
    }

    if (choice == 5)
    {
        botSay("Okay, calculator closed. Back to chat!");
        return;
    }

    firstNumber = readNumber("Enter the first number: ");
    secondNumber = readNumber("Enter the second number: ");

    switch (choice)
    {
        case 1:
            result = firstNumber + secondNumber;
            symbol = '+';
            break;
        case 2:
            result = firstNumber - secondNumber;
            symbol = '-';
            break;
        case 3:
            result = firstNumber * secondNumber;
            symbol = '*';
            break;
        case 4:
            if (secondNumber == 0)
            {
                printf(COLOR_ERROR "\nError: you cannot divide by zero!\n" COLOR_RESET);
                botSay("Division cancelled. Back to chat!");
                return;
            }
            result = firstNumber / secondNumber;
            symbol = '/';
            break;
    }

    printf("\n  %g %c %g = %.2f\n", firstNumber, symbol, secondNumber, result);
    botSay("Calculation complete. What would you like to do next?");
}

/* Baking functions cake included for you Moheb */

/* keyowrds for cooking, returns 1-5 for a recognized recipe, 0 for none */
int findRecipe(char text[])
{
    char pasta[][KEYWORD_LENGTH] = {"pasta", "spaghetti", "macaroni"};
    char curry[][KEYWORD_LENGTH] = {"curry", "chicken curry"};
    char omelette[][KEYWORD_LENGTH] = {"omelette", "omelet", "omlette"};
    char friedRice[][KEYWORD_LENGTH] = {"fried rice"};
    char sandwich[][KEYWORD_LENGTH] = {"sandwich", "sandwiches"};

    if (matchAny(text, curry, (int)COUNT(curry)))
        return 2;
    if (matchAny(text, friedRice, (int)COUNT(friedRice)))
        return 4;
    if (matchAny(text, omelette, (int)COUNT(omelette)))
        return 3;
    if (matchAny(text, pasta, (int)COUNT(pasta)))
        return 1;
    if (matchAny(text, sandwich, (int)COUNT(sandwich)))
        return 5;

    return 0;
}

void showRecipeList(void)
{
    printf("  Available recipes:\n");
    printf("    1. Pasta\n");
    printf("    2. Chicken curry\n");
    printf("    3. Omelette\n");
    printf("    4. Fried rice\n");
    printf("    5. Cake\n\n");
}

void showRecipe(int number)
{
    printf(COLOR_TITLE "\n=============== RECIPE ===============\n" COLOR_RESET);

    switch (number)
    {
        case 1:
            printf(" PASTA (serves 2)\n\n Ingredients:\n");
            printf("   - 200 g pasta\n   - 1 litre water\n   - 1 tsp salt\n");
            printf("   - 1 tbsp olive oil\n   - 2 cloves garlic, chopped\n");
            printf("   - 200 g tomato sauce\n   - 1/2 tsp dried oregano\n");
            printf("   - Black pepper to taste\n   - 30 g grated cheese (optional)\n");
            printf("\n Steps:\n");
            printf("   1. Boil the water in a large pot and add the salt.\n");
            printf("   2. Add the pasta and cook for 8-10 minutes, stirring now and then.\n");
            printf("   3. Drain the pasta and set it aside.\n");
            printf("   4. Heat the olive oil in a pan and fry the garlic for 30 seconds.\n");
            printf("   5. Add the tomato sauce, oregano and pepper. Simmer for 5 minutes.\n");
            printf("   6. Mix the pasta into the sauce and top with cheese.\n");
            break;

        case 2:
            printf(" CHICKEN CURRY (serves 4)\n\n Ingredients:\n");
            printf("   - 500 g chicken pieces\n   - 2 onions, chopped\n");
            printf("   - 2 tomatoes, chopped\n   - 1 tbsp ginger-garlic paste\n");
            printf("   - 3 tbsp cooking oil\n   - 1 tsp salt\n");
            printf("   - 1 tsp cumin powder\n   - 1 tsp coriander powder\n");
            printf("   - 1 tsp red chilli powder\n   - 1/2 tsp turmeric\n");
            printf("   - 1 tsp garam masala\n   - 1 cup water\n");
            printf("   - Fresh coriander to garnish\n");
            printf("\n Steps:\n");
            printf("   1. Heat the oil in a pot and fry the onions until golden (about 8 minutes).\n");
            printf("   2. Add the ginger-garlic paste and fry for 1 minute.\n");
            printf("   3. Add the tomatoes, salt, cumin, coriander, chilli and turmeric.\n");
            printf("      Cook until the oil separates (8-10 minutes).\n");
            printf("   4. Add the chicken and fry for 5-7 minutes.\n");
            printf("   5. Add the water, cover and simmer for 25-30 minutes, until the\n");
            printf("      chicken is fully cooked with no pink inside.\n");
            printf("   6. Stir in the garam masala and garnish with fresh coriander.\n");
            break;

        case 3:
            printf(" OMELETTE (serves 1)\n\n Ingredients:\n");
            printf("   - 2 eggs\n   - 2 tbsp milk\n   - 1/4 tsp salt\n");
            printf("   - A pinch of black pepper\n   - 1 tbsp butter or oil\n");
            printf("   - 2 tbsp chopped onion\n   - 1 tbsp chopped tomato\n");
            printf("   - 1 green chilli, chopped (optional)\n");
            printf("\n Steps:\n");
            printf("   1. Crack the eggs into a bowl and add the milk, salt and pepper.\n");
            printf("   2. Whisk well for about 30 seconds.\n");
            printf("   3. Heat the butter in a non-stick pan on medium heat.\n");
            printf("   4. Add the onion, tomato and chilli and fry for 1 minute.\n");
            printf("   5. Pour in the egg mixture and spread it evenly.\n");
            printf("   6. Cook for 2-3 minutes until the edges set, then fold in half.\n");
            printf("   7. Cook 1 more minute and serve hot.\n");
            break;

        case 4:
            printf(" FRIED RICE (serves 2)\n\n Ingredients:\n");
            printf("   - 2 cups cooked rice (cold, from the fridge)\n   - 2 eggs\n");
            printf("   - 1 cup mixed vegetables (carrot, peas, beans)\n");
            printf("   - 2 spring onions, sliced\n   - 2 cloves garlic, chopped\n");
            printf("   - 2 tbsp cooking oil\n   - 2 tbsp soy sauce\n");
            printf("   - 1/2 tsp salt\n   - 1/4 tsp black pepper\n");
            printf("\n Steps:\n");
            printf("   1. Heat 1 tbsp oil in a wok and scramble the eggs. Set them aside.\n");
            printf("   2. Add the remaining oil and fry the garlic for 30 seconds.\n");
            printf("   3. Add the vegetables and stir-fry for 3-4 minutes.\n");
            printf("   4. Add the rice, soy sauce, salt and pepper. Stir-fry for 3 minutes.\n");
            printf("   5. Mix in the scrambled eggs and spring onions.\n");
            printf("   6. Cook 1 more minute and serve hot.\n");
            break;

        case 5:
            printf(" CAKE (serves 5)\n\n Ingredients:\n");
            printf("   - 2 ½ cups all-purpose flour\n   - 2 cups granulated sugar\n");
            printf("   - 2 ½ tsp baking powder\n   - 4 large eggs, at room temperature\n");
            printf("   - ½ tsp salt\n   - 1 tbsp vanilla extract\n");
            printf("   - 1 cup unsalted butter, softened\n   - 1 cup whole milk, at room temperature\n");
            printf("\n Steps:\n");
            printf("   1. Preheat your oven to 350°F (175°C). Grease and flour two 9-inch round cake pans or line them with parchment paper.\n");
            printf("   2. Whisk together the flour, baking powder, and salt in a bowl.Beat the softened butter and granulated sugar together until light and fluffy (about 2 to 3 minutes).\n");
            printf("   3. Beat in the eggs one at a time, then stir in the vanilla extract. Add the dry ingredients in three parts, alternating with the milk. Start and end with the flour mixture, mixing only until just combined.\n");
            printf("   4. Divide the batter evenly between the pans. Bake for 25 to 30 minutes until a toothpick inserted into the center comes out clean.\n");
            printf("   5. Let the cakes cool in the pans for 10 minutes, then transfer them to a wire rack to cool completely before frosting or serving.\n");
            break;
    }

    printf(COLOR_TITLE "======================================\n\n" COLOR_RESET);
    botSay("Enjoy your meal! Ask me for another recipe any time.");
}

void cookingAssistant(char text[])
{
    char answer[INPUT_LENGTH];
    char backWords[][KEYWORD_LENGTH] = {"back", "cancel", "menu", "exit",
                                        "quit", "stop"};
    int recipe = findRecipe(text);
    int firstTry = 1;

    while (recipe == 0)
    {
        if (firstTry)
        {
            botSay("I'd love to help you cook! Which recipe would you like?");
        }
        else
        {
            botSay("Sorry, I don't have that recipe yet. Please try one of these.");
        }
        showRecipeList();

        printf("Type a recipe name (or 'back' to return to chat): ");
        readLine(answer, INPUT_LENGTH);
        toLowerCase(answer);

        if (matchAny(answer, backWords, (int)COUNT(backWords)))
        {
            botSay("Okay, back to chatting!");
            return;
        }

        recipe = findRecipe(answer);
        firstTry = 0;
    }

    showRecipe(recipe);
}

/* Binary search game */
void guessingGame(void)
{
    int secretNumber = rand() % 100 + 1;   /* 1 to 100 */
    int guess;
    int attempts = 0;

    botSay("Game time! I'm thinking of a number between 1 and 100.\n"
           "         Enter 0 at any time to give up.");

    while (1)
    {
        guess = readInt("Your guess: ");

        if (guess == 0)
        {
            printf("\nThe number was %d. Better luck next time!\n", secretNumber);
            botSay("Back to chat!");
            return;
        }

        if (guess < 1 || guess > 100)
        {
            printf(COLOR_ERROR "Please guess a number between 1 and 100.\n" COLOR_RESET);
            continue;   /* does not count as an attempt since error */
        }

        attempts++;

        if (guess < secretNumber)
        {
            printf("Too low! Try a higher number.\n");
        }
        else if (guess > secretNumber)
        {
            printf("Too high! Try a lower number.\n");
        }
        else
        {
            printf("\nCongratulations! You guessed %d in %d attempt(s)!\n",
                   secretNumber, attempts);
            botSay("Great game! Back to chat.");
            return;
        }
    }
}

/* To-do list functions. theres 3 of them "main", add and del.  */
void todoAssistant(void)
{
    int choice;

    while (1)
    {
        printf(COLOR_TITLE "\n--------------- To-Do List ---------------\n" COLOR_RESET);
        printf("  1. Add a task\n");
        printf("  2. View all tasks\n");
        printf("  3. Delete a task\n");
        printf("  4. Back to chat\n");
        printf("------------------------------------------\n");

        choice = readInt("Choose an option (1-4): ");

        switch (choice)
        {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                deleteTask();
                break;
            case 4:
                botSay("Your tasks are saved while I'm running. Back to chat!");
                return;
            default:
                printf(COLOR_ERROR "Please choose a number from 1 to 4.\n" COLOR_RESET);
                break;
        }
    }
}

void addTask(void)
{
    char newTask[TASK_LENGTH];

    if (taskCount >= MAX_TASKS)
    {
        printf(COLOR_ERROR "\nYour list is full (%d tasks). Delete one first.\n" COLOR_RESET,
               MAX_TASKS);
        return;
    }

    printf("Enter your task (max %d characters): ", TASK_LENGTH - 1);
    readLine(newTask, TASK_LENGTH);

    if (isBlank(newTask))
    {
        printf(COLOR_ERROR "A task cannot be empty.\n" COLOR_RESET);
        return;
    }

    strcpy(tasks[taskCount], newTask);
    taskCount++;
    printf("\nTask added! You now have %d task(s).\n", taskCount);
}

void viewTasks(void)
{
    int i;

    if (taskCount == 0)
    {
        printf("\n  Your to-do list is empty. Add a task to get started!\n");
        return;
    }

    printf("\n  Your tasks (%d of %d used):\n", taskCount, MAX_TASKS);
    for (i = 0; i < taskCount; i++)
    {
        printf("  %2d. %s\n", i + 1, tasks[i]);
    }
}

void deleteTask(void)
{
    int number;
    int i;

    if (taskCount == 0)
    {
        printf("\n  Your to-do list is empty. Nothing to delete :).\n");
        return;
    }

    viewTasks();
    number = readInt("\nPlease enter the number of the task to delete: ");

    if (number < 1 || number > taskCount)
    {
        printf(COLOR_ERROR "Invalid task number. Choose between 1 and %d.\n" COLOR_RESET,
               taskCount);
        return;
    }

    /* shifts each task once to the left */
    for (i = number - 1; i < taskCount - 1; i++)
    {
        strcpy(tasks[i], tasks[i + 1]);
    }

    taskCount--;
    tasks[taskCount][0] = '\0';   /* clears last slot which is now unused? thora sa buggy code */
    printf("\nTask deleted successfully. You now have %d task(s).\n", taskCount);
}