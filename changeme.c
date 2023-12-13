#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_FRAMES 8
#define MAX_LENGTH 20

bool inputExceeded = false;
int referenceString[MAX_LENGTH];
int numFrames = 0;
int pageFaultsOPT = 0;
int pageFaults[MAX_LENGTH] = {0};
int victimPages[MAX_LENGTH];
int simulationTable[MAX_FRAMES][MAX_LENGTH]; // To hold the state of each frame at each step

void inputN() {
    printf("Enter the number of physical frames (N): ");
    scanf("%d", &numFrames);
    if (numFrames < 2 || numFrames > 8) {
        printf("N must be between 2 and 8.\n");
    } else {
        printf("N is set to %d\n", numFrames);
    }
}

void inputReferenceString() {
    int temp, i = 0;
    char endChar;
    bool inputValid = true;

    do {
        i = 0;
        inputValid = true;
        printf("Enter the reference string (space-separated integers, max length 20): ");

        while (scanf("%d%c", &temp, &endChar) == 2 && i < MAX_LENGTH) {
            if (temp < 0 || temp > 9) {
                printf("Page number must be between 0 and 9. Please try again.\n");
                // Clear the input buffer
                while (getchar() != '\n');
                inputValid = false;
                break;
            }
            referenceString[i++] = temp;
            if (endChar == '\n') {
                break;
            }
        }

        if (!inputValid || (endChar != '\n' && i >= MAX_LENGTH)) {
            printf("Invalid input. Please try again.\n");
            // Clear the input buffer
            while (getchar() != '\n');
        } else {
            // Fill the rest of the array with -1
            while (i < MAX_LENGTH) {
                referenceString[i++] = -1;
            }
            break;
        }
    } while (true);
}

int getNextOccurrenceIndex(int page, int startIndex) {
    for (int i = startIndex + 1; i < MAX_LENGTH; i++) {
        if (referenceString[i] == page) {
            return i;
        }
    }
    // Page won't be referenced again
    return INT_MAX;
}

void displaySimulationTable(int currentStep, int totalLength) {
    // Line of dashes above the Reference String
    for (int i = 0; i < totalLength * 5 + 17; i++) {
        printf("-");
    }
    printf("\n");

    // Print the reference string header
    printf("Reference String ");
    for (int i = 0; i < totalLength; i++) {
        printf("| %2d ", referenceString[i]);
    }
    printf("|\n");

    // Line of dashes below the Reference String
    for (int i = 0; i < totalLength * 5 + 17; i++) {
        printf("-");
    }
    printf("\n");

    // Print the physical frames state
    for (int i = 0; i < numFrames; i++) {
        printf("Physical frame %d ", i);
        for (int j = 0; j < currentStep; j++) {
            if (simulationTable[i][j] == -1) {
                printf("|    ");
            } else {
                printf("| %2d ", simulationTable[i][j]);
            }
        }
        printf("|\n");
    }

    // Line of dashes above the Page Faults
    for (int i = 0; i < totalLength * 5 + 17; i++) {
        printf("-");
    }
    printf("\n");

    // Print the page faults
    printf("Page Faults      ");
    for (int i = 0; i < currentStep; i++) {
        printf("| %2c ", pageFaults[i] ? 'F' : ' ');
    }
    printf("|\n");

    // Print the victim pages
    printf("Victim Pages     ");
    for (int i = 0; i < currentStep; i++) {
        if (victimPages[i] != -1 && pageFaults[i]) {
            printf("| %2d ", victimPages[i]);
        } else {
            printf("|    ");
        }
    }
    printf("|\n");

    // Line of dashes below the Victim Pages
    for (int i = 0; i < totalLength * 5 + 17; i++) {
        printf("-");
    }
    printf("\n");

    printf("\nPress Enter to continue...\n");
    getchar();
}

void simulateOPT() {
    int frames[MAX_FRAMES];
    int referenceLength = 0;
    // Initialize all frames to -1 to indicate they are empty
    for (int i = 0; i < MAX_FRAMES; i++) {
        frames[i] = -1;
    }
    // Initialize victim pages array
    for (int i = 0; i < MAX_LENGTH; i++) {
        victimPages[i] = -1;
        if (referenceString[i] != -1) {
            referenceLength++;
        }
    }

        for (int currentStep = 0; currentStep < referenceLength; currentStep++) {
        int page = referenceString[currentStep];
        bool found = false;
        // Reset victim for each step
        int victim = -1;

        // Check if the page is already in one of the frames
        for (int j = 0; j < numFrames; j++) {
            if (frames[j] == page) {
                found = true;
                break;
            }
        }

        // If the page was not found, have a page fault
        if (!found) {
            // First try to find an empty frame
            for (int j = 0; j < numFrames; j++) {
                if (frames[j] == -1) {
                    frames[j] = page;
                    found = true;
                    // Mark a page fault
                    pageFaults[currentStep] = 1;
                    pageFaultsOPT++;
                    break;
                }
            }

            // If no empty frame was found, find the victim frame to replace
            if (!found) {
                int farthest = -1;
                int farthestIndex = -1;
                for (int j = 0; j < numFrames; j++) {
                    int nextPageIndex = getNextOccurrenceIndex(frames[j], currentStep);
                    if (nextPageIndex > farthest) {
                        farthest = nextPageIndex;
                        farthestIndex = j;
                    }
                    // If the page will not be used again, it's the best victim
                    if (nextPageIndex == INT_MAX) {
                        farthestIndex = j;
                        break;
                    }
                }
                victim = frames[farthestIndex]; // Save the page number of the victim
                frames[farthestIndex] = page; // Replace with the new page
                pageFaults[currentStep] = 1; // Mark a page fault
                victimPages[currentStep] = victim; // Record the victim page
                pageFaultsOPT++;
            }
        }

        // Update the simulation table for the current step
        for (int k = 0; k < numFrames; k++) {
            simulationTable[k][currentStep] = frames[k];
        }

        // Record the page fault and victim page if a replacement occurred
        victimPages[currentStep] = (!found && victim != -1) ? victim : -1;

        // Display the current state of the simulation
        displaySimulationTable(currentStep + 1, referenceLength);
    }
}

void simulateNEW() {
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES];
    int referenceLength = 0;

    // Initialize all frames to -1 and last used times to -1
    for (int i = 0; i < MAX_FRAMES; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }

    // Initialize victim pages array
    for (int i = 0; i < MAX_LENGTH; i++) {
        victimPages[i] = -1;
        if (referenceString[i] != -1) {
            referenceLength++;
        }
    }

    for (int currentStep = 0; currentStep < referenceLength; currentStep++) {
        int page = referenceString[currentStep];
        bool found = false;
        int victimIndex = -1;

        // Check if the page is already in one of the frames
        for (int j = 0; j < numFrames; j++) {
            if (frames[j] == page) {
                found = true;
                // Update last used time
                lastUsed[j] = currentStep;
                break;
            }
        }

        if (!found) {
            int first = INT_MAX, second = INT_MAX;
            int firstIndex = -1, secondIndex = -1;

            // Find the least and second least recently used pages
            for (int j = 0; j < numFrames; j++) {
                if (frames[j] == -1) {
                    victimIndex = j;
                    break;
                } else if (lastUsed[j] < first) {
                    second = first;
                    secondIndex = firstIndex;
                    first = lastUsed[j];
                    firstIndex = j;
                } else if (lastUsed[j] < second) {
                    second = lastUsed[j];
                    secondIndex = j;
                }
            }

            // If no empty frame was found, second least recently used page is the victim
            if (victimIndex == -1) {
                victimIndex = secondIndex;
                // Record the victim page
                victimPages[currentStep] = frames[victimIndex];
            }

            frames[victimIndex] = page; // Replace the victim page with the new page
            lastUsed[victimIndex] = currentStep; // Update last used time
            pageFaults[currentStep] = 1; // Mark a page fault
            pageFaultsOPT++;
        }

        // Update the simulation table for the current step
        for (int k = 0; k < numFrames; k++) {
            simulationTable[k][currentStep] = frames[k];
        }

        displaySimulationTable(currentStep + 1, referenceLength);
    }
}

int main() {
    int choice, scanResult;

    while (true) {
        printf(" %.*s\n", 32, "------------------------------------------------------------");
        printf("| Demand Paging Simulator Menu   |\n");
        printf("| 1 - Input N                    |\n");
        printf("| 2 - Input the reference string |\n");
        printf("| 3 - Simulate the OPT algorithm |\n");
        printf("| 4 - Simulate the NEW algorithm |\n");
        printf("| 0 - Exit                       |\n");
        printf(" %.*s\n", 32, "------------------------------------------------------------");
        printf("Select option: ");

        // Use scanf to read the input and store the result
        scanResult = scanf("%d", &choice);

        // Flush the newline character from the input buffer
        while (getchar() != '\n');

        // Check if the scanf was successful and the choice is within the valid range
        if (scanResult != 1 || choice < 0 || choice > 4) {
            printf("Invalid input. Please enter a number between 0 and 4.\n");
            continue;
        }

        switch (choice) {
            case 0:
                printf("Exiting the program.\n");
                return 0;
            case 1:
                inputN();
                break;
            case 2:
                inputReferenceString();
                if (inputExceeded) {
                    inputExceeded = false; // Reset the flag
                    continue; // Re-prompt the menu
                }
                break;
            case 3:
                if (numFrames > 0 && referenceString[0] != -1) {
                    simulateOPT();
                } else {
                    printf("Please input N and the reference string first.\n");
                }
                break;
            case 4:
                if (numFrames > 0 && referenceString[0] != -1) {
                    simulateNEW();
                } else {
                    printf("Please input N and the reference string first.\n");
                }
                break;
            default:
                // This is technically unreachable now due to the earlier check
                printf("Invalid option. Please select a valid option.\n");
                break;
        }
    }
    return 0;
}
