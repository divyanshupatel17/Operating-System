#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_FRAMES 10
#define MAX_PAGES 21
#define EMPTY -1

typedef struct {
    int hits;
    int faults;
    float hit_rate;
    float fault_rate;
} PageReplacementResult;

bool pageExists(int frames[], int frameCount, int page) {
    for (int i = 0; i < frameCount; i++)
        if (frames[i] == page)
            return true;
    return false;
}

PageReplacementResult fifo(int referenceString[], int pageCount, int frameCount) {
    int frames[MAX_FRAMES] = {EMPTY}, currentFrame = 0, hits = 0, faults = 0;
    for (int i = 0; i < pageCount; i++) {
        if (!pageExists(frames, frameCount, referenceString[i])) {
            frames[currentFrame] = referenceString[i];
            currentFrame = (currentFrame + 1) % frameCount;
            faults++;
        } else {
            hits++;
        }
    }
    return (PageReplacementResult){hits, faults, (float)hits / pageCount, (float)faults / pageCount};
}

PageReplacementResult lru(int referenceString[], int pageCount, int frameCount) {
    int frames[MAX_FRAMES], lastUsed[MAX_FRAMES], hits = 0, faults = 0;
    for (int i = 0; i < frameCount; i++) {
        frames[i] = EMPTY;
        lastUsed[i] = EMPTY;
    }
    for (int i = 0; i < pageCount; i++) {
        if (!pageExists(frames, frameCount, referenceString[i])) {
            int lruIndex = 0, minTime = INT_MAX;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == EMPTY) {
                    lruIndex = j;
                    break;
                }
                if (lastUsed[j] < minTime) {
                    minTime = lastUsed[j];
                    lruIndex = j;
                }
            }
            frames[lruIndex] = referenceString[i];
            lastUsed[lruIndex] = i;
            faults++;
        } else {
            for (int j = 0; j < frameCount; j++)
                if (frames[j] == referenceString[i]) lastUsed[j] = i;
            hits++;
        }
    }
    return (PageReplacementResult){hits, faults, (float)hits / pageCount, (float)faults / pageCount};
}

PageReplacementResult optimal(int referenceString[], int pageCount, int frameCount) {
    int frames[MAX_FRAMES], hits = 0, faults = 0;
    for (int i = 0; i < frameCount; i++) frames[i] = EMPTY;
    for (int i = 0; i < pageCount; i++) {
        if (!pageExists(frames, frameCount, referenceString[i])) {
            int replaceIndex = -1, farthestIndex = i + 1, futureIndex[MAX_FRAMES];
            for (int j = 0; j < frameCount; j++) futureIndex[j] = INT_MAX;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == EMPTY) {
                    replaceIndex = j;
                    break;
                }
                for (int k = i + 1; k < pageCount; k++)
                    if (frames[j] == referenceString[k]) {
                        futureIndex[j] = k;
                        break;
                    }
            }
            if (replaceIndex == -1) {
                int maxFuture = -1;
                for (int j = 0; j < frameCount; j++)
                    if (futureIndex[j] > maxFuture) {
                        maxFuture = futureIndex[j];
                        replaceIndex = j;
                    }
            }
            frames[replaceIndex] = referenceString[i];
            faults++;
        } else {
            hits++;
        }
    }
    return (PageReplacementResult){hits, faults, (float)hits / pageCount, (float)faults / pageCount};
}

PageReplacementResult secondChance(int referenceString[], int pageCount, int frameCount) {
    int frames[MAX_FRAMES], referenceBits[MAX_FRAMES], currentFrame = 0, hits = 0, faults = 0;
    for (int i = 0; i < frameCount; i++) {
        frames[i] = EMPTY;
        referenceBits[i] = 0;
    }
    for (int i = 0; i < pageCount; i++) {
        if (!pageExists(frames, frameCount, referenceString[i])) {
            while (referenceBits[currentFrame]) {
                referenceBits[currentFrame] = 0;
                currentFrame = (currentFrame + 1) % frameCount;
            }
            frames[currentFrame] = referenceString[i];
            referenceBits[currentFrame] = 1;
            currentFrame = (currentFrame + 1) % frameCount;
            faults++;
        } else {
            for (int j = 0; j < frameCount; j++)
                if (frames[j] == referenceString[i]) {
                    referenceBits[j] = 1;
                    break;
                }
            hits++;
        }
    }
    return (PageReplacementResult){hits, faults, (float)hits / pageCount, (float)faults / pageCount};
}

PageReplacementResult lfu(int referenceString[], int pageCount, int frameCount) {
    int frames[MAX_FRAMES] = {EMPTY}, frequency[MAX_FRAMES] = {0}, hits = 0, faults = 0;
    for (int i = 0; i < pageCount; i++) {
        if (!pageExists(frames, frameCount, referenceString[i])) {
            int lfuIndex = 0, minFreq = INT_MAX;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == EMPTY) {
                    lfuIndex = j;
                    break;
                }
                if (frequency[j] < minFreq) {
                    minFreq = frequency[j];
                    lfuIndex = j;
                }
            }
            frames[lfuIndex] = referenceString[i];
            frequency[lfuIndex] = 1;
            faults++;
        } else {
            for (int j = 0; j < frameCount; j++)
                if (frames[j] == referenceString[i]) frequency[j]++;
            hits++;
        }
    }
    return (PageReplacementResult){hits, faults, (float)hits / pageCount, (float)faults / pageCount};
}

PageReplacementResult mfu(int referenceString[], int pageCount, int frameCount) {
    int frames[MAX_FRAMES] = {EMPTY}, frequency[MAX_FRAMES] = {0}, hits = 0, faults = 0;
    for (int i = 0; i < pageCount; i++) {
        if (!pageExists(frames, frameCount, referenceString[i])) {
            int mfuIndex = 0, maxFreq = -1;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == EMPTY) {
                    mfuIndex = j;
                    break;
                }
                if (frequency[j] > maxFreq) {
                    maxFreq = frequency[j];
                    mfuIndex = j;
                }
            }
            frames[mfuIndex] = referenceString[i];
            frequency[mfuIndex] = 1;
            faults++;
        } else {
            for (int j = 0; j < frameCount; j++)
                if (frames[j] == referenceString[i]) frequency[j]++;
            hits++;
        }
    }
    return (PageReplacementResult){hits, faults, (float)hits / pageCount, (float)faults / pageCount};
}

void displayComparison(PageReplacementResult fifoResult, PageReplacementResult lruResult, PageReplacementResult optimalResult, PageReplacementResult secondChanceResult, PageReplacementResult lfuResult, PageReplacementResult mfuResult) {
    printf("\nAlgorithm Comparison:\n");
    printf("-----------------------------------------------------------\n");
    printf("| Algorithm      | Hits | Faults | Hit Rate | Fault Rate |\n");
    printf("-----------------------------------------------------------\n");
    printf("| FIFO           | %4d | %6d |  %.2f   |    %.2f    |\n", fifoResult.hits, fifoResult.faults, fifoResult.hit_rate, fifoResult.fault_rate);
    printf("| LRU            | %4d | %6d |  %.2f   |    %.2f    |\n", lruResult.hits, lruResult.faults, lruResult.hit_rate, lruResult.fault_rate);
    printf("| Optimal        | %4d | %6d |  %.2f   |    %.2f    |\n", optimalResult.hits, optimalResult.faults, optimalResult.hit_rate, optimalResult.fault_rate);
    printf("| Second Chance  | %4d | %6d |  %.2f   |    %.2f    |\n", secondChanceResult.hits, secondChanceResult.faults, secondChanceResult.hit_rate, secondChanceResult.fault_rate);
    printf("| LFU            | %4d | %6d |  %.2f   |    %.2f    |\n", lfuResult.hits, lfuResult.faults, lfuResult.hit_rate, lfuResult.fault_rate);
    printf("| MFU            | %4d | %6d |  %.2f   |    %.2f    |\n", mfuResult.hits, mfuResult.faults, mfuResult.hit_rate, mfuResult.fault_rate);
    printf("-----------------------------------------------------------\n");
}
int main() {
    int referenceString[] = {5, 0, 1, 0, 2, 3, 0, 2, 4, 3, 3, 2, 0, 2, 1, 2, 7, 0, 1, 1, 0};
    int pageCount = sizeof(referenceString) / sizeof(referenceString[0]);
    int frameCount;

    printf("Enter the number of frames: ");
    scanf("%d", &frameCount);

    PageReplacementResult fifoResult = fifo(referenceString, pageCount, frameCount);
    PageReplacementResult lruResult = lru(referenceString, pageCount, frameCount);
    PageReplacementResult optimalResult = optimal(referenceString, pageCount, frameCount);
    PageReplacementResult secondChanceResult = secondChance(referenceString, pageCount, frameCount);
    PageReplacementResult lfuResult = lfu(referenceString, pageCount, frameCount);
    PageReplacementResult mfuResult = mfu(referenceString, pageCount, frameCount);

    displayComparison(fifoResult, lruResult, optimalResult, secondChanceResult, lfuResult, mfuResult);

    return 0;
}
