#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROGRAMS 100
#define MAX_PARTITIONS 10

typedef struct {
    char name[50];
    int size;
    int del; //if 0 not deleted if -1 == deleted
} Program;

typedef struct {
    int partition_num;
    int size;
    int partition_status;
    
} PDT;

typedef struct {
    int partition_id;
    int size;
    int is_allocated;
    char allocated_program[50];
} Partition;

void create_main_memory(int num_partitions, int partition_sizes[], Partition memory[], PDT PDT[]) {
    int start_address = 0;
    for (int i = 0; i < num_partitions; i++) {
        memory[i].partition_id = i;
        memory[i].is_allocated = 0;
        strcpy(memory[i].allocated_program, "");

        PDT[i].partition_num = i + 1;
        PDT[i].size = partition_sizes[i];
        PDT[i].partition_status = 0;

        start_address += partition_sizes[i];
    }
}

int find_free_partition_first_fit(int requested_size, PDT PDT[], int num_partitions) {
    for (int i = 0; i < num_partitions; i++) {
        if (!PDT[i].partition_status && PDT[i].size >= requested_size) {
            return i;
        }
    }
    return -1;
}

int find_free_partition_best_fit(int requested_size, PDT PDT[], int num_partitions) {
    int a = -1;
    int min_remaining_size = 100;

    for (int i = 0; i < num_partitions; i++) {
        if (!PDT[i].partition_status && PDT[i].size >= requested_size) {
            int remaining_size = PDT[i].size - requested_size;
            if (remaining_size < min_remaining_size) {
                min_remaining_size = remaining_size;
                a = i;
            }
        }
    }
    return a;
}

int allocate_memory(Program program, Partition memory[], PDT PDT[], int num_partitions, int x) {
    int free_partition_index = -1;

    if (x == 1) { // First Fit
        free_partition_index = find_free_partition_first_fit(program.size, PDT, num_partitions);
    } else if (x == 2) { // Best Fit
        free_partition_index = find_free_partition_best_fit(program.size, PDT, num_partitions);
    }

    if (free_partition_index != -1 && program.del != -1 ) {
        
        memory[free_partition_index].is_allocated = 1;
        strcpy(memory[free_partition_index].allocated_program, program.name);
        PDT[free_partition_index].partition_status = 1; 
        printf("Allocated %s to Partition %d\n", program.name, PDT[free_partition_index].partition_num);
        return -1;
    }
    else if (program.del == -1 )
    printf("The program %s is deleted \n", program.name);
    else{
        printf("Error: Not enough memory to allocate %s\n", program.name);
    }
}

void free_partition(Partition memory[], PDT PDT[], int num_partitions) {
    int partition_num;
    printf("Enter the partition number you want to free: ");
    scanf("%d", &partition_num);

    if (partition_num < 1 || partition_num > num_partitions) {
        printf("Invalid partition number. Please try again.\n");
        return;
    }

    int partition_index = partition_num - 1;

    if (memory[partition_index].is_allocated) {
        memory[partition_index].is_allocated = 0;
        strcpy(memory[partition_index].allocated_program, "");
        PDT[partition_index].partition_status = 0;
        printf("Freed Partition %d.\n", PDT[partition_index].partition_num);
    } else {
        printf("Partition %d is already free.\n", partition_num);
    }
}

void display_memory_status(Partition memory[], PDT PDT[], int num_partitions) {
    printf("Partition Description Table (PDT):\n");
    printf("+--------------+------+-----------+-----------+\n");
    printf("| Partition N  | Size | Status    | Allocated |\n");
    printf("+--------------+------+-----------+-----------+\n");

    for (int i = 0; i < num_partitions; i++) {
        printf("| %-12d | %-4d | %-9s | %-9s |\n",
               PDT[i].partition_num, PDT[i].size,
               PDT[i].partition_status == 0 ? "Free" : "Allocated",
               memory[i].is_allocated == 1 ? memory[i].allocated_program : "None");
    }

    printf("+--------------+------+-----------+-----------+\n");
}

void display_menu() {
    printf("\n Menu of Operations:\n");
    printf("1. Allocate memory for a program (First Fit)\n");
    printf("2. Allocate memory for a program (Best Fit)\n");
    printf("3. Free a partition\n");
    printf("4. Display memory status\n");
    printf("5. Exit\n");
}

int main() {
    Program programs[MAX_PROGRAMS];
    Partition memory[MAX_PARTITIONS];
    PDT PDT[MAX_PARTITIONS];
    int num_programs, num_partitions;
    int partition_sizes[MAX_PARTITIONS];
    int x;

    printf("Enter the number of programs: ");
    scanf("%d", &num_programs);

    for (int i = 0; i < num_programs; i++) {
        printf("Enter program name and size for program %d: ", i + 1);
        scanf("%s %d", programs[i].name, &programs[i].size);
        programs[i].del=0;
    }

    printf("Enter the number of memory partitions: ");
    scanf("%d", &num_partitions);
    printf("Enter the size of each memory partition: ");
    for (int i = 0; i < num_partitions; i++) {
        scanf("%d", &partition_sizes[i]);
    }

    create_main_memory(num_partitions, partition_sizes, memory, PDT);
    int z=0;
    int choice;
    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1 || choice == 2) {
            x = choice;
            printf("\n-------------------------------\n\n");
            for (int i = 0; i < num_programs; i++) {
                if(programs[i].del!=-1)
                z=allocate_memory(programs[i], memory, PDT, num_partitions, x);
               if(z==-1)
                programs[i].del=-1;
            }
            printf("\n-------------------------------");
        } else if (choice == 3) {
            printf("\n-------------------------------\n");
            free_partition(memory, PDT, num_partitions);
            printf("\n-------------------------------\n");
        } else if (choice == 4) {
            printf("\n-------------------------------\n");
            display_memory_status(memory, PDT, num_partitions);
            printf("\n-------------------------------\n");
        } else if (choice == 5) {
            exit(0);
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
