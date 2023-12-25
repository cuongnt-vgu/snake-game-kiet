#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linked_list.h"
#include "mbstrings.h"
#include "common.h"

/** Updates the game by a single step, and modifies the game information
 * accordingly. Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: width of the board.
 *  - height: height of the board.
 *  - snake_p: pointer to your snake struct (not used until part 2!)
 *  - input: the next input.
 *  - growing: 0 if the snake does not grow on eating, 1 if it does.
 */
void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing) {
    // `update` should update the board, your snake's data, and global
    // variables representing game information to reflect new state. If in the
    // updated position, the snake runs into a wall or itself, it will not move
    // and global variable g_game_over will be 1. Otherwise, it will be moved
    // to the new position. If the snake eats food, the game score (`g_score`)
    // increases by 1. This function assumes that the board is surrounded by
    // walls, so it does not handle the case where a snake runs off the board.

    
    // Retrieve the current position of the snake's head
    int new_x = snake_p->head->x;
    int new_y = snake_p->head->y;

    // Update the position based on the input
    switch (input) {
        case INPUT_UP:    new_y--; break;
        case INPUT_DOWN:  new_y++; break;
        case INPUT_LEFT:  new_x--; break;
        case INPUT_RIGHT: new_x++; break;
        default:          /* No change in direction */ break;
    }

    // Check for collisions (with the wall or snake itself)
    if (new_x < 0 || new_y < 0 || 
        cells[new_y * width + new_x] == FLAG_SNAKE) {
        g_game_over = 1;
        return;
    }

    // Move the snake
    // Create a new node for the new head position
    node_t *new_head = malloc(sizeof(node_t));
    if (new_head == NULL) {
        // Handle memory allocation failure
        g_game_over = 1;
        return;
    }
    new_head->x = new_x;
    new_head->y = new_y;

    // Add the new head to the snake
    insert_first(&(snake_p->head), new_head, 1);

    // Check if the snake has eaten food
    int ate_food = (cells[new_y * width + new_x] == FLAG_FOOD);

    // If the snake hasn't eaten food and isn't growing, remove the tail
    if (!ate_food && !growing) {
        free(remove_last(&(snake_p->head)));
    }

    // Update the score and place new food if the snake has eaten
    if (ate_food) {
        g_score++;
        place_food(cells, width, height);
    }

    // Update the cells on the board with the new position of the snake
    update(cells, width, height, snake_p, input, growing);
}

/** Sets a random space on the given board to food.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: the width of the board
 *  - height: the height of the board
 */
void place_food(int* cells, size_t width, size_t height) {
    /* DO NOT MODIFY THIS FUNCTION */
    unsigned food_index = generate_index(width * height);
    if (*(cells + food_index) == FLAG_PLAIN_CELL) {
        *(cells + food_index) = FLAG_FOOD;
    } else {
        place_food(cells, width, height);
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Prompts the user for their name and saves it in the given buffer.
 * Arguments:
 *  - `write_into`: a pointer to the buffer to be written into.
 */
void read_name(char* write_into) {
    printf("Enter your name: ");
    fgets(write_into, 100, stdin);
    // Remove the newline character if present
    if (write_into[strlen(write_into) - 1] == '\n') {
        write_into[strlen(write_into) - 1] = '\0';
    }
}


/** Cleans up on game over — should free any allocated memory so that the
 * LeakSanitizer doesn't complain.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - snake_p: a pointer to your snake struct. (not needed until part 2)
 */
void teardown(int* cells, snake_t* snake_p) {
    free(cells); // Assuming cells were dynamically allocated
    while (snake_p->head != NULL) {
        node_t* next = snake_p->head->next;
        free(snake_p->head);
        snake_p->head = next;
    }
}
