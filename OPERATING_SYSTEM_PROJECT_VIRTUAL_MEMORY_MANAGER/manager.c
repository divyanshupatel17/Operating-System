#include<stdio.h>
#include<stdlib.h>
#define PAGE_NUM 256
#define PAGE_SIZE 256
#define FRAME_NUM 256       // change frame num
#define FRAME_SIZE 256
#define TLB_ENTRY_NUM 16
#define PAGE_TABLE_SIZE 256

FILE* addresses;
FILE* backing_store;
FILE* result_file;

typedef struct PAGE_TABLE_ITEM{
    int valid;
    int frame_id;
} page_table_item;

typedef struct TLB_ITEM{
    int used_time;
    int frame_id;
    int page_id;
} tlb_item;

typedef struct MEMORY_ITEM{
    int used_time;
    char data[FRAME_SIZE];
} memory_item;

page_table_item page_table[PAGE_TABLE_SIZE];
tlb_item TLB[TLB_ENTRY_NUM];
memory_item memory[FRAME_NUM];

/* initialize page table */
void page_table_init(){
    for(int i=0; i<PAGE_TABLE_SIZE; i++){
        page_table[i].valid = 0; // set to not valid
        page_table[i].frame_id = -1; // set to None
    }
}

/* initialize TLB */
void tlb_init(){
    for(int i=0; i<TLB_ENTRY_NUM; i++){
        TLB[i].used_time = -1; 
        TLB[i].frame_id = -1;
        TLB[i].page_id = -1;
    }
}

/* initialize memory*/
void memory_init(){
    for(int i=0; i<FRAME_NUM; i++){
        memory[i].used_time = -1;
    }
}

/* initialize page table, TLB, memory */
void initialize(){
    page_table_init();
    tlb_init();
    memory_init();
    printf("Initialize Finish.\n");
}

/* get the page num, given address*/
int get_page(int address){
    return address >> 8;
}

/* get the offset, given address */
int get_offset(int address){
    return address & 0xFF;
}

/* LRU replacement for TLB */
void TLB_LRU_Replacement(int page_id, int frame_id, int time){
    int min_time = time;
    int min_idx = 0;
    for(int i = 0; i < TLB_ENTRY_NUM; i++){
        if(TLB[i].used_time < min_time){
            min_time = TLB[i].used_time;
            min_idx = i;
        }
    }
    TLB[min_idx].frame_id = frame_id;
    TLB[min_idx].page_id = page_id;
    TLB[min_idx].used_time = time;
}

/* LRU Replacement for Memory
   Updating new data
   returning the frame id */
int memory_LRU_Replacement(int page_id, int time){
    int min_time = time;
    int min_idx = 0;
    for(int i = 0; i < FRAME_NUM; i++){
        if(memory[i].used_time < min_time){
            min_time = memory[i].used_time;
            min_idx = i;
        }
    }
    memory[min_idx].used_time = time;

    // Find the old page id, and set invalid
    for(int i = 0; i < PAGE_TABLE_SIZE; i++){
        if(page_table[i].frame_id == min_idx){
            page_table[i].valid = -1;
        }
    }

    // Load data from backing store
    fseek(backing_store, page_id * PAGE_SIZE, SEEK_SET);
    fread(memory[min_idx].data, sizeof(char), FRAME_SIZE, backing_store);

    return min_idx;
}

int main(int argc, char*argv[]){
    addresses = fopen(argv[1], "r");
    backing_store = fopen("BACKING_STORE.bin", "rb");
    result_file = fopen("result.txt", "w");

    // Initialize
    initialize();

    int address;
    int page_id;
    int frame_id;
    int offset;

    // Counters
    int count = 0;
    int tlb_hit = 0;
    int page_fault = 0;
    int tlb_miss = 0;
    int page_hit = 0;
    int time = 0;

    fscanf(addresses, "%u", &address);
    while(!feof(addresses)){
        count++;
        time++;

        // Decode address
        page_id = get_page(address);
        offset = get_offset(address);

        // Search in TLB
        int tlb_find = 0;
        for(int i = 0; i < TLB_ENTRY_NUM; i++){
            if(page_id == TLB[i].page_id){
                tlb_hit++;
                tlb_find = 1;
                frame_id = TLB[i].frame_id;
                memory[frame_id].used_time = time;
                TLB[i].used_time = time;
                break;
            }
        }

        // Not found in TLB, search in page table
        int page_find = 0;
        if(!tlb_find){
            tlb_miss++;
            if(page_table[page_id].valid == 1){
                page_find = 1;
                page_hit++;
                frame_id = page_table[page_id].frame_id;
                memory[frame_id].used_time = time;
                TLB_LRU_Replacement(page_id, frame_id, time); // Update TLB
            }
            else {
                page_fault++;
                frame_id = memory_LRU_Replacement(page_id, time);

                // Update page table
                page_table[page_id].frame_id = frame_id;
                page_table[page_id].valid = 1;

                // Update TLB
                TLB_LRU_Replacement(page_id, frame_id, time);
            }
        }

        // Calculate physical address and get data
        int physical_address = frame_id * FRAME_SIZE + offset;
        int data = memory[frame_id].data[offset];

        // Output result
        fprintf(result_file, "Virtual address: %d Physical address: %d Value: %d\n", address, physical_address, data);
        fscanf(addresses, "%u", &address);  
    }

    // Finish   
    fclose(addresses);
    fclose(backing_store);
    fclose(result_file);

    // Statistics
    printf("Execution Finish.\n");
    printf("-----------------------------\n");

    double page_fault_rate = page_fault / (double)count;
    double page_hit_rate = page_hit / (double)count;
    double tlb_hit_rate = tlb_hit / (double)count;
    double tlb_miss_rate = tlb_miss / (double)count;

    printf("Frame Num: %d\n", FRAME_NUM);
    printf("TLB Hits: %d\n", tlb_hit);
    printf("TLB Misses: %d\n", tlb_miss);
    printf("TLB Hit Rate: %f\n", tlb_hit_rate);
    printf("TLB Miss Rate: %f\n", tlb_miss_rate);
    printf("Page Hits: %d\n", page_hit);
    printf("Page Faults: %d\n", page_fault);
    printf("Page Hit Rate: %f\n", page_hit_rate);
    printf("Page Fault Rate: %f\n", page_fault_rate);
    printf("Allocation Technique: Demand Paging\n");
    printf("Replacement Technique: LRU (Least Recently Used)\n");

    return 0;
}

/*
┌─[user@parrot]─[/media/sf_LINUX_SHARED_FOLDER/OSproj_v3]
└──╼ $make clean
rm manager
┌─[user@parrot]─[/media/sf_LINUX_SHARED_FOLDER/OSproj_v3]
└──╼ $make
gcc manager.c -o manager
┌─[user@parrot]─[/media/sf_LINUX_SHARED_FOLDER/OSproj_v3]
└──╼ $./manager addresses.txt
Initialize Finish.
Execution Finish.
-----------------------------
Frame Num: 256
TLB Hits: 55
TLB Misses: 945
TLB Hit Rate: 0.055000
TLB Miss Rate: 0.945000
Page Hits: 701
Page Faults: 244
Page Hit Rate: 0.701000
Page Fault Rate: 0.244000
Allocation Technique: Demand Paging
Replacement Technique: LRU (Least Recently Used)
┌─[user@parrot]─[/media/sf_LINUX_SHARED_FOLDER/OSproj_v3]
└──╼ $cmp correct.txt result.txt
┌─[user@parrot]─[/media/sf_LINUX_SHARED_FOLDER/OSproj_v3]
└──╼ $sh compare.sh
The files match

*/