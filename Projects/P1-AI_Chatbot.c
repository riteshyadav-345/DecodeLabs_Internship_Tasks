#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 256

typedef struct {
    const char *intent;
    const char *response;
} KnowledgeEntry;

static const KnowledgeEntry KNOWLEDGE_BASE[] = {
    {"hello", "Hello! I am ARIA. How can I assist you today?"},
    {"hi", "Hey there! ARIA is online and ready. What is on your mind?"},
    {"hey", "Hey! What can I help you with?"},
    {"good morning", "Good morning! What do you need help with?"},
    {"good evening", "Good evening! How can I assist you tonight?"},
    {"who are you", "I am ARIA, a deterministic rule-based AI chatbot."},
    {"what are you", "I am a rule-based chatbot where each known input maps to a response."},
    {"your name", "My name is ARIA: Autonomous Rule-based Intelligence Architecture."},
    {"are you human", "No. I am a program that uses explicit rules and text matching."},
    {"are you ai", "I am a rule-based AI that uses explicit logic instead of a language model."},
    {"what is ai", "Artificial Intelligence is the simulation of human intelligence by machines."},
    {"what is ml", "Machine Learning is a subset of AI that learns patterns from data."},
    {"rule based ai", "Rule-based AI uses predefined rules to make transparent decisions."},
    {"what is deep learning", "Deep Learning uses multilayer neural networks to learn complex patterns."},
    {"difference between ai and ml", "AI is the broad field; ML is an approach that learns from data."},
    {"python", "Python is widely used for AI because of libraries such as NumPy and TensorFlow."},
    {"what is a dictionary", "A dictionary stores key-value pairs for efficient lookup."},
    {"hash map", "A hash map stores key-value pairs and usually provides constant-time lookup."},
    {"control flow", "Control flow includes loops, conditions, and branches that direct execution."},
    {"loop", "A loop repeats instructions. This chatbot uses one to keep the conversation active."},
    {"what is project 1", "Project 1 is the DecodeLabs Rule-Based AI Chatbot."},
    {"decodelabs", "DecodeLabs focuses on hands-on, project-based learning."},
    {"ipo model", "IPO stands for Input, Process, and Output."},
    {"guardrails", "AI guardrails are rules that filter, block, or modify unsafe output."},
    {"hybrid architecture", "A hybrid architecture combines rule-based logic with flexible AI models."},
    {"how are you", "I am running normally. How are you?"},
    {"i am fine", "Great to hear! What can I help you with?"},
    {"i am good", "Awesome! Let me know what you would like to discuss."},
    {"thanks", "You are welcome!"},
    {"thank you", "You are very welcome!"},
    {"help", "Ask me about AI, ML, Python, control flow, DecodeLabs, or Project 1."},
    {"commands", "Ask a question, or type exit, quit, close, or end to stop."}
};

static const size_t KNOWLEDGE_BASE_SIZE =
    sizeof(KNOWLEDGE_BASE) / sizeof(KNOWLEDGE_BASE[0]);

static void sanitize_input(const char *input, char *output, size_t output_size)
{
    size_t read_index = 0;
    size_t write_index = 0;
    int previous_was_space = 1;

    while (input[read_index] != '\0' && write_index + 1 < output_size) {
        unsigned char character = (unsigned char)input[read_index++];

        if (isalnum(character) || character == '_') {
            output[write_index++] = (char)tolower(character);
            previous_was_space = 0;
        } else if (isspace(character) && !previous_was_space) {
            output[write_index++] = ' ';
            previous_was_space = 1;
        }
    }

    if (write_index > 0 && output[write_index - 1] == ' ') {
        write_index--;
    }
    output[write_index] = '\0';
}

static int is_exit_command(const char *input)
{
    static const char *commands[] = {"exit", "quit", "close", "end"};
    size_t index;

    for (index = 0; index < sizeof(commands) / sizeof(commands[0]); index++) {
        if (strcmp(input, commands[index]) == 0) {
            return 1;
        }
    }
    return 0;
}

static const char *get_response(const char *input)
{
    size_t index;

    for (index = 0; index < KNOWLEDGE_BASE_SIZE; index++) {
        if (strcmp(input, KNOWLEDGE_BASE[index].intent) == 0) {
            return KNOWLEDGE_BASE[index].response;
        }
    }

    return "I do not have a rule for that yet. Try asking about AI, Python, or DecodeLabs.";
}

int main(void)
{
    char input[INPUT_SIZE];
    char clean_input[INPUT_SIZE];

    printf("ARIA ONLINE ChatBot\n");
    printf("Ask about AI, Python, or this project. Type 'exit' to quit.\n\n");

    while (1) {
        printf("You: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            putchar('\n');
            break;
        }

        sanitize_input(input, clean_input, sizeof(clean_input));

        if (clean_input[0] == '\0') {
            printf("ARIA: Please enter a message.\n\n");
            continue;
        }

        if (is_exit_command(clean_input)) {
            printf("ARIA: Session terminated. Goodbye!\n");
            break;
        }

        printf("ARIA: %s\n\n", get_response(clean_input));
    }

    return 0;
}
