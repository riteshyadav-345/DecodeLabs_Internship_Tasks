#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 150

void toLowerCase(char text[]) {
    int i;

    for (i = 0; text[i] != '\0'; i++) {
        text[i] = (char)tolower((unsigned char)text[i]);
    }
}

void removeNewLine(char text[]) {
    int length = strlen(text);

    if (length > 0 && text[length - 1] == '\n') {
        text[length - 1] = '\0';
    }
}

void trimSpaces(char text[]) {
    int start = 0, end, i;

    while (text[start] == ' ') {
        start++;
    }

    end = strlen(text) - 1;
    while (end >= start && text[end] == ' ') {
        end--;
    }

    for (i = start; i <= end; i++) {
        text[i - start] = text[i];
    }
    text[i - start] = '\0';
}

int contains(char text[], char word[]) {
    return strstr(text, word) != NULL;
}

int main() {
    char input[MAX_INPUT];

    printf("====================================\n");
    printf("        ARIA - AI Chatbot\n");
    printf("====================================\n");
    printf("You can wake me by saying Hii ,ask about me, my name, help, C language, AI, study tips, time, or jokes.\n");
    printf("Type 'bye', 'exit', or 'quit' to end the chat.\n\n");

    while (1) {
        printf("You: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nBot: Input error. Chat ended.\n");
            break;
        }

        removeNewLine(input);
        toLowerCase(input);
        trimSpaces(input);

        if (strlen(input) == 0) {
            printf("Bot: Please type something so I can reply.\n");
        } else if (strcmp(input, "hi") == 0 || strcmp(input, "hii") == 0 ||
                   strcmp(input, "hiii") == 0 || strcmp(input, "hello") == 0 ||
                   strcmp(input, "helloo") == 0 || strcmp(input, "hey") == 0 ||
                   strcmp(input, "heyy") == 0 || strcmp(input, "namaste") == 0 ||
                   strcmp(input, "namaskar") == 0 || strcmp(input, "good morning") == 0 ||
                   strcmp(input, "good afternoon") == 0 || strcmp(input, "good evening") == 0) {
            printf("Bot: Hello! Nice to meet you. How can I help you?\n");
        } else if (strcmp(input, "good night") == 0) {
            printf("Bot: Good night! Sleep well and take care.\n");
        } else if (contains(input, "how are you")) {
            printf("Bot: I am doing great! I am ready to answer your questions.\n");
        } else if (contains(input, "kaise ho") || contains(input, "kese ho")) {
            printf("Bot: Main badhiya hoon! Tum batao, kaise ho?\n");
        } else if (contains(input, "i am fine") || contains(input, "i am good") ||
                   contains(input, "main thik") || contains(input, "mai thik")) {
            printf("Bot: Nice! Keep smiling and keep learning.\n");
        } else if (contains(input, "your name") || contains(input, "who are you")) {
            printf("Bot: My name is CodeBot. I am a simple rule-based chatbot.\n");
        } else if (contains(input, "my name is")) {
            printf("Bot: Nice name! It is good to meet you.\n");
        } else if (contains(input, "what can you do") || strcmp(input, "help") == 0) {
            printf("Bot: I can reply to greetings, answer basic questions, give study tips, tell jokes, and exit when you say bye.\n");
        } else if (contains(input, "c language") || contains(input, "programming")) {
            printf("Bot: C is a powerful programming language used for system software, games, and embedded systems.\n");
        } else if (contains(input, "ai") || contains(input, "artificial intelligence")) {
            printf("Bot: AI means making computers perform tasks that usually need human intelligence.\n");
        } else if (contains(input, "study tip") || contains(input, "study tips") ||
                   contains(input, "how to study")) {
            printf("Bot: Study in small focused sessions, revise daily, and practice by writing code yourself.\n");
        } else if (contains(input, "exam")) {
            printf("Bot: For exams, revise important topics first and solve previous questions.\n");
        } else if (contains(input, "college")) {
            printf("Bot: College life is a great time to build skills, projects, and confidence.\n");
        } else if (contains(input, "time")) {
            printf("Bot: I cannot check the real clock, but your computer can show it using the time command.\n");
        } else if (contains(input, "date")) {
            printf("Bot: I cannot check the real date, but your computer can show it using the date command.\n");
        } else if (contains(input, "joke")) {
            printf("Bot: Why do programmers prefer dark mode? Because light attracts bugs!\n");
        } else if (contains(input, "motivate") || contains(input, "motivation")) {
            printf("Bot: Keep going. Small daily progress becomes big success over time.\n");
        } else if (contains(input, "sad") || contains(input, "not happy")) {
            printf("Bot: I am sorry you feel that way. Take a short break and talk to someone you trust.\n");
        } else if (contains(input, "happy")) {
            printf("Bot: That is great! I hope your day keeps getting better.\n");
        } else if (contains(input, "thank")) {
            printf("Bot: You are welcome!\n");
        } else if (contains(input, "sorry")) {
            printf("Bot: No problem at all.\n");
        } else if (strcmp(input, "bye") == 0 || strcmp(input, "exit") == 0 ||
                   strcmp(input, "quit") == 0) {
            printf("Bot: Goodbye! Have a nice day.\n");
            break;
        } else {
            printf("Bot: Sorry, I do not understand that yet. Type 'help' to see what I can answer.\n");
        }
    }

    return 0;
}
