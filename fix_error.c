#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

uint8_t *packets[5];    // Array for storing up to five data packets.
int packet_sizes[5];    // Array for storing the size of each packets
int set_packets = 0;    // Variable starts on zero, to keep track of the number of packets in the array

// Function to swap two packets, with double pointers (from INF-1100 lecture 09.26.23)
void swap_packet(uint8_t **packet_to_swap, uint8_t **packet_to_compare) {
    uint8_t *temporary = *packet_to_swap; //Temporary store the value/adress of the first packet(packet to swap)
    *packet_to_swap = *packet_to_compare; //Pointing packet_to_swap on packet_to_compare to swap value
    *packet_to_compare = temporary; //Pointing packet_to_compare on temporary, so the swap is complete
}
// Function to swap two integers, that behave on the same way as above
void swap_int(int *packet_to_swap, int *packet_to_compare) {
    int temporary = *packet_to_swap;
    *packet_to_swap = *packet_to_compare;
    *packet_to_compare = temporary;
}

// Implementation of the bubbleSort function (sorting algorithm)
void bubbleSort(uint8_t *packets[], int packet_sizes[], int sequence_counter) {
    int i, j;
    bool swapped;
    for (i = 0; i < sequence_counter - 1; i++) {
    swapped = false;
        for (j = 0; j < sequence_counter - i - 1; j++) {
            if (get_bits(packets[j], 14, 18) > get_bits(packets[j + 1], 14, 18)) { // Compare,
                swap_packet(&packets[j], &packets[j + 1]);                         // if criteria are met, swap data
                swap_int(&packet_sizes[j], &packet_sizes[j + 1]);                  // and packet size.
                swapped = true;
            }
        }

        // Stop/break the loop if no swapping har been performed
        if (!swapped)
            break;
    }
}
/* Copied main-function from introduce_error,
    with some minor changes to meet the task description */
int main() {
    uint8_t *data = malloc(sizeof(uint8_t) * 100000); // Allocate memory for temp. input budder
    for (int i = 0; i < 5; i++) {   // Intitialize the data packet and size arrays
        packets[i] = NULL;
    }
    while (1) { // Changed from feof(stdin) to an unlimited loop
        int data_read = fread(data, 1, 6, stdin);   //Read 6 bytes from stdin
        if (data_read <= 0) {   // Check if EOF (End of file) had been reached
            break;  // Breaking loop at EOF
        }
        int data_length = get_bits(data, 16, 32) + 1; // Calculate the length of data packet, based on specified bit position
        data_read += fread(data + 6, 1, data_length, stdin); // Read the remaining of the packet

        // Store data i packet array
        packets[set_packets] = malloc(data_read);   //Allocating memory for new packet
        memcpy(packets[set_packets], data, data_read); //copy memory (copy the data on the new packet)
        packet_sizes[set_packets] = data_read;          //Saves the size on the packet
        set_packets += 1;                               //Adding one
        
        if (set_packets == 5) {                 // If, and only if it reaches 5 
        bubbleSort(packets, packet_sizes, 5);   // perform bubblesort.
            for (int i = 0; i < 5; i++) {
                //  Write to standar output (stdout)
                int bytes_written = write(fileno(stdout), packets[i], packet_sizes[i]);
                fflush(stdout);     //flush/empty the stdout
                free(packets[i]);   //empty the memory on the particular packet
                packets[i] = NULL;  //set pointer to zero, (pause pointing)
            }
            set_packets = 0;    // Reset count of packets
        }
    }
}

