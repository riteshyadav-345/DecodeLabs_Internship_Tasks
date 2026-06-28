/* ============================================================================
   PROJECT 1: RULE-BASED AI CHATBOT  —  "ARIA"
   DecodeLabs Industrial Training Kit | Batch 2026

   Architecture : INPUT -> SANITIZE -> LOOKUP (Knowledge Base) -> OUTPUT
   Loop type    : Continuous while(1) "heartbeat" loop, broken by kill command
   Lookup type  : Linear scan over a rules[] table (C has no built-in hash map,
                  so we simulate the dictionary.get(key, default) pattern from
                  the spec using a helper function get_response())
   Skills shown : Control flow, string handling, decision logic, modular design
   ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN     256      /* Max length of raw user input          */
#define MAX_RULES   30       /* Size of the knowledge base             */

/* ----------------------------------------------------------------------------
   PHASE 0: THE KNOWLEDGE BASE (Rule Table)
   ----------------------------------------------------------------------------
   Each rule maps an exact-match "intent" key to a fixed response.
   This is the C equivalent of Python's:
       responses = { "hello": "Hi there!", "bye": "Goodbye!" }
---------------------------------------------------------------------------- */
typedef struct {
    const char *key;
    const char *response;
} Rule;

static Rule knowledge_base[MAX_RULES] = {
    /* Greetings */
    {"hello",            "Hello! I'm ARIA, your rule-based AI assistant. How can I help?"},
    {"hi",               "Hey there! ARIA online and ready."},
    {"hey",              "Hey! What can I do for you?"},
    {"good morning",     "Good morning! Hope your day goes well."},
    {"good evening",     "Good evening! How can I assist you?"},

    /* Identity */
    {"who are you",      "I'm ARIA - a rule-based chatbot running on pure if-else / lookup logic."},
    {"what are you",     "I'm a deterministic AI: every input maps to an explicit, hard-coded rule."},
    {"your name",        "My name is ARIA - Autonomous Rule-based Intelligence Architecture."},
    {"are you human",    "No, I'm a machine. I run on control flow, not consciousness."},
    {"are you ai",       "Yes - specifically rule-based AI, the 'Engineer' mind, not the probabilistic 'Artist'."},

    /* AI concepts */
    {"what is ai",       "Artificial Intelligence is the simulation of human intelligence by machines."},
    {"what is ml",       "Machine Learning is a subset of AI where systems learn patterns from data."},
    {"rule based ai",    "Rule-based AI uses explicit if-else logic. It is a white-box system: zero hallucination risk."},
    {"what is deep learning", "Deep Learning uses layered neural networks to learn from large datasets."},

    /* Tech */
    {"python",           "Python is widely used for AI thanks to libraries like NumPy and TensorFlow."},
    {"c language",        "C gives you full control over memory and execution - great for understanding how logic really runs."},
    {"control flow",      "Control flow (loops, conditionals, branching) is the skeleton that gives a program intelligence."},
    {"loop",               "A continuous loop is the heartbeat of this chatbot - it keeps running until you type 'exit'."},

    /* Project / DecodeLabs */
    {"what is project 1", "Project 1 is the Rule-Based AI Chatbot - the foundation phase of the DecodeLabs AI track."},
    {"decodelabs",         "DecodeLabs is the organization powering this AI internship through hands-on projects."},
    {"ipo model",          "IPO stands for Input - Process - Output, the blueprint for transparent AI systems like this one."},
    {"guardrails",         "AI Guardrails are rule-based filters that sit above LLMs to ensure safety and compliance."},

    /* Wellbeing / small talk */
    {"how are you",        "Running at full efficiency! All logic gates nominal. How about you?"},
    {"i am fine",          "Great to hear! What can I help you with?"},
    {"thanks",              "You're welcome! Happy to help anytime."},
    {"thank you",           "You're very welcome!"},

    /* Help */
    {"help",                "Ask me about AI, ML, Python, C, or this project. Type 'exit' to quit."},
    {"commands",            "Try: hello, who are you, what is ai, help, exit."}
};

static const int NUM_RULES = 28;          /* Active number of rules above   */
static const char *FALLBACK_RESPONSE =
    "I don't have a rule for that yet. Try 'help' to see what I know.";

/* ----------------------------------------------------------------------------
   PHASE 1: INPUT & SANITIZATION
   ----------------------------------------------------------------------------
   Mirrors:  clean_input = raw_input.lower().strip()
   - Converts to lowercase
   - Trims leading/trailing whitespace
   - Strips a trailing newline left behind by fgets()
---------------------------------------------------------------------------- */
void sanitize_input(char *str)
{
    /* 1. Remove trailing newline captured by fgets() */
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }

    /* 2. Convert to lowercase in place */
    for (size_t i = 0; i < len; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }

    /* 3. Trim leading whitespace by shifting the string left */
    size_t start = 0;
    while (str[start] == ' ' || str[start] == '\t') {
        start++;
    }
    if (start > 0) {
        memmove(str, str + start, strlen(str + start) + 1);
    }

    /* 4. Trim trailing whitespace */
    len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) {
        str[len - 1] = '\0';
        len--;
    }

    /* 5. Strip trailing punctuation (?, !, ., ,) so "Who are you?"
          still matches the "who are you" key in the knowledge base. */
    while (len > 0 && ispunct((unsigned char)str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
}

/* ----------------------------------------------------------------------------
   PHASE 2: PROCESS - INTENT MATCHING (THE LOOKUP ENGINE)
   ----------------------------------------------------------------------------
   This is the C equivalent of Python's:
       reply = responses.get(user_input, "I do not understand.")

   strcmp() gives an O(n) linear scan here (C has no native hash map), but the
   *interface* mirrors the same "lookup + fallback in one atomic operation"
   pattern described in the spec.
---------------------------------------------------------------------------- */
const char *get_response(const char *clean_input)
{
    for (int i = 0; i < NUM_RULES; i++) {
        if (strcmp(clean_input, knowledge_base[i].key) == 0) {
            return knowledge_base[i].response;
        }
    }
    return FALLBACK_RESPONSE;   /* Fallback for unknowns */
}

/* ----------------------------------------------------------------------------
   EXIT STRATEGY: Clean break command check
---------------------------------------------------------------------------- */
int is_exit_command(const char *clean_input)
{
    return (strcmp(clean_input, "exit") == 0 ||
            strcmp(clean_input, "quit") == 0 ||
            strcmp(clean_input, "bye")  == 0 ||
            strcmp(clean_input, "close") == 0);
}

/* ----------------------------------------------------------------------------
   PHASE 3: OUTPUT - simple formatted print, kept separate so the
   response-generation "layer" stays decoupled from raw printf calls.
---------------------------------------------------------------------------- */
void print_bot(const char *message)
{
    printf("ARIA: %s\n", message);
}

/* ----------------------------------------------------------------------------
   MAIN: THE HEARTBEAT (continuous while loop / "the infinite loop")
---------------------------------------------------------------------------- */
int main(void)
{
    char raw_input[MAX_LEN];

    printf("============================================================\n");
    printf("   ARIA - Rule-Based AI Chatbot (C Implementation)\n");
    printf("   DecodeLabs Industrial Training Kit | Project 1\n");
    printf("============================================================\n");
    print_bot("Hello! I'm ARIA. Type 'help' for topics or 'exit' to quit.\n");

    while (1) {
        printf("You: ");

        /* INPUT: read a full line safely (bounded, avoids overflow) */
        if (fgets(raw_input, MAX_LEN, stdin) == NULL) {
            break;   /* EOF or input stream error -> exit gracefully */
        }

        /* SANITIZE */
        sanitize_input(raw_input);

        /* Skip empty input (user just pressed Enter) */
        if (strlen(raw_input) == 0) {
            print_bot("Please type something!");
            continue;
        }

        /* KILL COMMAND CHECK */
        if (is_exit_command(raw_input)) {
            print_bot("Goodbye! Keep building - every rule you define is a"
                      " step toward mastering AI. \n");
            break;   /* breaks the infinite loop -> graceful termination */
        }

        /* PROCESS + OUTPUT */
        const char *response = get_response(raw_input);
        print_bot(response);
    }

    printf("============================================================\n");
    printf("   Session terminated. ARIA offline.\n");
    printf("============================================================\n");

    return 0;
}