#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_FRAMES 10
#define MAX_PAGES 20

bool pageExists(int frames[], int n_frames, int page) {
    for (int i = 0; i < n_frames; i++)
        if (frames[i] == page)
            return true;
    return false;
}

void displayFrames(int frames[], int n_frames) {
    for (int i = 0; i < n_frames; i++) {
        if (frames[i] == -1)
            printf("[ ] ");
        else
            printf("[%d] ", frames[i]);
    }
    printf("\n");
}

void fifo(int pages[], int n_pages, int n_frames) {
    int frames[MAX_FRAMES], current = 0, hits = 0, faults = 0;
    for (int i = 0; i < n_frames; i++) frames[i] = -1;

    printf("\nFIFO Page Replacement:\n");
    for (int i = 0; i < n_pages; i++) {
        printf("Page request %d: ", pages[i]);
        if (!pageExists(frames, n_frames, pages[i])) {
            frames[current] = pages[i];
            current = (current + 1) % n_frames;
            faults++;
            printf("Miss! ");
        } else {
            hits++;
            printf("Hit! ");
        }
        displayFrames(frames, n_frames);
    }
    printf("\nTotal Hits: %d, Misses: %d\n", hits, faults);
}

void lru(int pages[], int n_pages, int n_frames) {
    int frames[MAX_FRAMES], last_used[MAX_FRAMES], hits = 0, faults = 0;
    for (int i = 0; i < n_frames; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    printf("\nLRU Page Replacement:\n");
    for (int i = 0; i < n_pages; i++) {
        printf("Page request %d: ", pages[i]);
        if (!pageExists(frames, n_frames, pages[i])) {
            int lru_idx = 0, min_time = INT_MAX;
            for (int j = 0; j < n_frames; j++) {
                if (frames[j] == -1) {
                    lru_idx = j;
                    break;
                }
                if (last_used[j] < min_time) {
                    min_time = last_used[j];
                    lru_idx = j;
                }
            }
            frames[lru_idx] = pages[i];
            last_used[lru_idx] = i;
            faults++;
            printf("Miss! ");
        } else {
            for (int j = 0; j < n_frames; j++)
                if (frames[j] == pages[i]) last_used[j] = i;
            hits++;
            printf("Hit! ");
        }
        displayFrames(frames, n_frames);
    }
    printf("\nTotal Hits: %d, Misses: %d\n", hits, faults);
}

void optimal(int pages[], int n_pages, int n_frames) {
    int frames[MAX_FRAMES], hits = 0, faults = 0;
    for (int i = 0; i < n_frames; i++) frames[i] = -1;

    printf("\nOptimal Page Replacement:\n");
    for (int i = 0; i < n_pages; i++) {
        printf("Page request %d: ", pages[i]);
        if (!pageExists(frames, n_frames, pages[i])) {
            int replace_idx = -1, farthest = i + 1, future[MAX_FRAMES];
            for (int j = 0; j < n_frames; j++) future[j] = INT_MAX;

            for (int j = 0; j < n_frames; j++) {
                if (frames[j] == -1) {
                    replace_idx = j;
                    break;
                }
                for (int k = i + 1; k < n_pages; k++)
                    if (frames[j] == pages[k]) {
                        future[j] = k;
                        break;
                    }
            }

            if (replace_idx == -1) {
                int max_future = -1;
                for (int j = 0; j < n_frames; j++)
                    if (future[j] > max_future) {
                        max_future = future[j];
                        replace_idx = j;
                    }
            }
            frames[replace_idx] = pages[i];
            faults++;
            printf("Miss! ");
        } else {
            hits++;
            printf("Hit! ");
        }
        displayFrames(frames, n_frames);
    }
    printf("\nTotal Hits: %d, Misses: %d\n", hits, faults);
}

int main() {
    int pages[MAX_PAGES], n_pages, n_frames, choice;
    
    printf("Enter the number of page requests: ");
    scanf("%d", &n_pages);
    printf("Enter the page requests: ");
    for (int i = 0; i < n_pages; i++) scanf("%d", &pages[i]);
    
    printf("Enter the number of frames: ");
    scanf("%d", &n_frames);

    printf("Select algorithm: 1 - FIFO, 2 - LRU, 3 - Optimal, 4 - All: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: fifo(pages, n_pages, n_frames); break;
        case 2: lru(pages, n_pages, n_frames); break;
        case 3: optimal(pages, n_pages, n_frames); break;
        case 4:
            fifo(pages, n_pages, n_frames);
            lru(pages, n_pages, n_frames);
            optimal(pages, n_pages, n_frames);
            break;
        default: printf("Invalid choice.\n");
    }
    return 0;
}

/*
Input Given: 

Enter the number of page requests: 20
Enter the page requests: 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1         
Enter the number of frames: 3

20
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
3 
*/


/*
LRU 

Page Requests
Page 1:
Miss! Replace frame 0.
Frames: [1, -1, -1]
Last Used: [0, -1, -1]
Page 2:
Miss! Replace frame 1.
Frames: [1, 2, -1]
Last Used: [0, 1, -1]
Page 3:
Miss! Replace frame 2.
Frames: [1, 2, 3]
Last Used: [0, 1, 2]
Page 4:
Miss! Replace frame 0 (LRU: Page 1).
Frames: [4, 2, 3]
Last Used: [3, 1, 2]
Page 1:
Miss! Replace frame 1 (LRU: Page 2).
Frames: [4, 1, 3]
Last Used: [3, 4, 2]
Page 2:
Miss! Replace frame 2 (LRU: Page 3).
Frames: [4, 1, 2]
Last Used: [3, 4, 5]
Page 5:
Miss! Replace frame 0 (LRU: Page 4).
Frames: [5, 1, 2]
Last Used: [6, 4, 5]
Page 1:
Hit! Update last used.
Frames: [5, 1, 2]
Last Used: [6, 7, 5]
Page 2:
Hit! Update last used.
Frames: [5, 1, 2]
Last Used: [6, 7, 8]
Page 3:
Miss! Replace frame 0 (LRU: Page 5).
Frames: [3, 1, 2]
Last Used: [9, 7, 8]
Page 4:
Miss! Replace frame 1 (LRU: Page 1).
Frames: [3, 4, 2]
Last Used: [9, 10, 8]
Page 5:
Miss! Replace frame 2 (LRU: Page 2).
Frames: [3, 4, 5]
Last Used: [9, 10, 11]
Final State
Frames: [3, 4, 5]
Last Used: [9, 10, 11]
Total Hits: 2, Misses: 10
*/

/*
OPTIMAL


*/
