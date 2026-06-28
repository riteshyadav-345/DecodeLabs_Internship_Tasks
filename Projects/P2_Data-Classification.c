#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_SAMPLES 24
#define TRAIN_SIZE 18
#define TEST_SIZE (TOTAL_SAMPLES - TRAIN_SIZE)
#define FEATURE_COUNT 4
#define CLASS_COUNT 3
#define K_NEIGHBORS 3

typedef struct {
    double features[FEATURE_COUNT];
    int label;
} FlowerSample;

typedef struct {
    double distance;
    int label;
} Neighbor;

const char *class_name(int label) {
    switch (label) {
        case 0:
            return "Setosa";
        case 1:
            return "Versicolor";
        case 2:
            return "Virginica";
        default:
            return "Unknown";
    }
}

double squared_distance(const FlowerSample *a, const FlowerSample *b) {
    double total = 0.0;

    for (int i = 0; i < FEATURE_COUNT; i++) {
        double diff = a->features[i] - b->features[i];
        total += diff * diff;
    }

    return total;
}

void swap_samples(FlowerSample *a, FlowerSample *b) {
    FlowerSample temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle_dataset(FlowerSample dataset[], int size) {
    srand(42);

    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap_samples(&dataset[i], &dataset[j]);
    }
}

void split_dataset(const FlowerSample dataset[], FlowerSample train[], FlowerSample test[]) {
    for (int i = 0; i < TRAIN_SIZE; i++) {
        train[i] = dataset[i];
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        test[i] = dataset[TRAIN_SIZE + i];
    }
}

void print_dataset_summary(const FlowerSample dataset[], int size) {
    int class_counts[CLASS_COUNT] = {0};
    double min_values[FEATURE_COUNT];
    double max_values[FEATURE_COUNT];

    for (int i = 0; i < FEATURE_COUNT; i++) {
        min_values[i] = dataset[0].features[i];
        max_values[i] = dataset[0].features[i];
    }

    for (int i = 0; i < size; i++) {
        class_counts[dataset[i].label]++;

        for (int j = 0; j < FEATURE_COUNT; j++) {
            if (dataset[i].features[j] < min_values[j]) {
                min_values[j] = dataset[i].features[j];
            }
            if (dataset[i].features[j] > max_values[j]) {
                max_values[j] = dataset[i].features[j];
            }
        }
    }

    printf("Dataset loaded: %d Iris flower samples\n", size);
    printf("Features: sepal length, sepal width, petal length, petal width\n\n");

    printf("Class distribution:\n");
    for (int i = 0; i < CLASS_COUNT; i++) {
        printf("  %-10s : %d samples\n", class_name(i), class_counts[i]);
    }

    printf("\nFeature ranges:\n");
    printf("  Sepal length: %.1f to %.1f\n", min_values[0], max_values[0]);
    printf("  Sepal width : %.1f to %.1f\n", min_values[1], max_values[1]);
    printf("  Petal length: %.1f to %.1f\n", min_values[2], max_values[2]);
    printf("  Petal width : %.1f to %.1f\n\n", min_values[3], max_values[3]);
}

void insert_neighbor(Neighbor neighbors[], double distance, int label) {
    int position = K_NEIGHBORS;

    for (int i = 0; i < K_NEIGHBORS; i++) {
        if (distance < neighbors[i].distance) {
            position = i;
            break;
        }
    }

    if (position == K_NEIGHBORS) {
        return;
    }

    for (int i = K_NEIGHBORS - 1; i > position; i--) {
        neighbors[i] = neighbors[i - 1];
    }

    neighbors[position].distance = distance;
    neighbors[position].label = label;
}

int predict_knn(const FlowerSample train[], int train_size, const FlowerSample *sample) {
    Neighbor neighbors[K_NEIGHBORS];
    int votes[CLASS_COUNT] = {0};

    for (int i = 0; i < K_NEIGHBORS; i++) {
        neighbors[i].distance = 1e9;
        neighbors[i].label = -1;
    }

    for (int i = 0; i < train_size; i++) {
        double distance = squared_distance(&train[i], sample);
        insert_neighbor(neighbors, distance, train[i].label);
    }

    for (int i = 0; i < K_NEIGHBORS; i++) {
        if (neighbors[i].label >= 0) {
            votes[neighbors[i].label]++;
        }
    }

    int best_label = 0;
    for (int i = 1; i < CLASS_COUNT; i++) {
        if (votes[i] > votes[best_label]) {
            best_label = i;
        }
    }

    return best_label;
}

double evaluate_model(const FlowerSample train[], const FlowerSample test[]) {
    int correct = 0;

    printf("Testing predictions:\n");
    printf("No.  Actual       Predicted    Result\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < TEST_SIZE; i++) {
        int predicted = predict_knn(train, TRAIN_SIZE, &test[i]);
        int is_correct = predicted == test[i].label;

        if (is_correct) {
            correct++;
        }

        printf("%-4d %-12s %-12s %s\n",
               i + 1,
               class_name(test[i].label),
               class_name(predicted),
               is_correct ? "Correct" : "Wrong");
    }

    return (correct * 100.0) / TEST_SIZE;
}

void predict_user_sample(const FlowerSample train[]) {
    FlowerSample sample = {{0.0, 0.0, 0.0, 0.0}, -1};
    char choice;

    printf("\nTry your own flower measurement? (y/n): ");
    if (scanf(" %c", &choice) != 1 || (choice != 'y' && choice != 'Y')) {
        return;
    }

    printf("Enter sepal length, sepal width, petal length, petal width: ");
    if (scanf("%lf %lf %lf %lf",
              &sample.features[0],
              &sample.features[1],
              &sample.features[2],
              &sample.features[3]) != FEATURE_COUNT) {
        printf("Invalid input. Please enter four numeric values.\n");
        return;
    }

    int prediction = predict_knn(train, TRAIN_SIZE, &sample);
    printf("Predicted class: %s\n", class_name(prediction));
}

int main(void) {
    FlowerSample dataset[TOTAL_SAMPLES] = {
        {{5.1, 3.5, 1.4, 0.2}, 0}, {{4.9, 3.0, 1.4, 0.2}, 0},
        {{4.7, 3.2, 1.3, 0.2}, 0}, {{4.6, 3.1, 1.5, 0.2}, 0},
        {{5.0, 3.6, 1.4, 0.2}, 0}, {{5.4, 3.9, 1.7, 0.4}, 0},
        {{4.6, 3.4, 1.4, 0.3}, 0}, {{5.0, 3.4, 1.5, 0.2}, 0},

        {{7.0, 3.2, 4.7, 1.4}, 1}, {{6.4, 3.2, 4.5, 1.5}, 1},
        {{6.9, 3.1, 4.9, 1.5}, 1}, {{5.5, 2.3, 4.0, 1.3}, 1},
        {{6.5, 2.8, 4.6, 1.5}, 1}, {{5.7, 2.8, 4.5, 1.3}, 1},
        {{6.3, 3.3, 4.7, 1.6}, 1}, {{4.9, 2.4, 3.3, 1.0}, 1},

        {{6.3, 3.3, 6.0, 2.5}, 2}, {{5.8, 2.7, 5.1, 1.9}, 2},
        {{7.1, 3.0, 5.9, 2.1}, 2}, {{6.3, 2.9, 5.6, 1.8}, 2},
        {{6.5, 3.0, 5.8, 2.2}, 2}, {{7.6, 3.0, 6.6, 2.1}, 2},
        {{4.9, 2.5, 4.5, 1.7}, 2}, {{7.3, 2.9, 6.3, 1.8}, 2}
    };

    FlowerSample train[TRAIN_SIZE];
    FlowerSample test[TEST_SIZE];

    print_dataset_summary(dataset, TOTAL_SAMPLES);

    shuffle_dataset(dataset, TOTAL_SAMPLES);
    split_dataset(dataset, train, test);

    printf("Data split complete:\n");
    printf("  Training set: %d samples\n", TRAIN_SIZE);
    printf("  Testing set : %d samples\n\n", TEST_SIZE);

    printf("Model: K-Nearest Neighbors classification, k = %d\n\n", K_NEIGHBORS);

    double accuracy = evaluate_model(train, test);
    printf("\nModel accuracy: %.2f%%\n", accuracy);

    predict_user_sample(train);

    return 0;
}
