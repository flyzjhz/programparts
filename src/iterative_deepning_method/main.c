/*
 * Date:2015/02/03
 *
 * Q:How to search the best way form A to J?
 *
 *           A
 *          /|\
 *         / | \
 *        /  |  \
 *       B   C   D--.
 *      /|   |\   \  \
 *     / |   | \   \  \
 *    /  |   |  \   \  \
 *   E   F   G   H   I  J
 *      /|\      |\
 *     / | \     | \
 *    /  |  \    |  \
 *   K   L   M   N   O
 *
 * */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define STACK_DEAPTH 255
#include "../../lib/stack.h"

#define N ('O' - 'A' + 1)

/*
 * In the node indicated by row, if you have branches to node indicated by
 * column, the value is 1.
 * If you don't have branch to the node, the value is 0.
 * */
const uint32_t adjacent[N][N] = {
  // 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
  // A  B  C  D  E  F  G  H  I  J  K  L  M  N  O
    {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // A 0
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // B 0
    {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, // C 2
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0}, // D 3
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // E 4
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0}, // F 5
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // G 6
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // H 7
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // I 8
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // J 9
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // K 00
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // L 00
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // M 02
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, // N 03
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}  // O 04
};

const uint32_t depth[N] = {
    1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4
};

int main(int argc, char const* argv[]) {

    /* stack for nodes linked to current node */
    stack_t open;

    /* array for log */
    uint32_t log[STACK_DEAPTH];
    uint32_t log_i = 0;

    uint32_t cutoff = 0;
    uint32_t node = 0;
    uint32_t flag = 0;
    int32_t i = 0;
    uint32_t j = 0;

    /* set initial cutoff */
    cutoff = 1;

LABEL:
    /* show cutoff */
    usleep(5E5);
    printf("cutoff = %u\n", cutoff);

    /* init variables */
    init_stack(&open);
    for (i = 0; i < log_i; i++) log[i] = 0;
    log_i = 0;

    /* set root node 'A' */
    push(&open, 'A');

    for (;;) {

        /* show process */
        printf("%2u:", log_i);
        for (i = 0; i < log_i; i++) {
            printf("%c%s", (char) log[i], ((i == log_i - 1) ? "\n":"->"));
        }
        if (log_i == 0) printf("\n");

        if (node == 'J') {
            printf("route found\n");
            return 0;
        }
        usleep(5E5);

        /* get current node */
        node = get_top_of_stack(&open);
        pop(&open);

        /* search fail, try more deeper search */
        if (node == 0) {
            /* show result */
            printf("route not found\n");
            printf("\n");
            cutoff++;
            goto LABEL;
        }

        /* take log */
        log[log_i] = node;
        log_i++;

        /* check for all nodes linked to current node and add to the stack.
         * cutoff is limitting the depth for searching */
        for (i = N - 1; i >= 0; i--) {
            flag = 1;
            for (j = 0; j <= log_i; j++) {
                if (log[j] == i + 'A') flag = 0;
            }
            if (   adjacent[node - 'A'][i] == 1
                && flag == 1
                && depth[i] <= cutoff) {
                push(&open, i + 'A');
            }
        }
    }

    return 0;
}
