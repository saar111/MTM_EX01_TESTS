#ifndef TEST_UTILITIES_H_
#define TEST_UTILITIES_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * These macros are here to help you create tests more easily and keep them
 * clear
 *
 * The basic idea with unit-testing is create a test function for every real
 * function and inside the test function declare some variables and execute the
 * function under test.
 *
 * Use the ASSERT_TEST and ASSERT_TEST_WITH_FREE to verify correctness of
 * values.
 */


/**
 * Evaluates expr and continues if expr is true.
 * If expr is false, ends the test by returning false and prints a detailed
 * message about the failure.
 */
#define ASSERT_TEST(expr, goto_label)                                                         \
     do {                                                                          \
         if (!(expr)) {                                                            \
             printf("\nAssertion failed at %s:%d %s ", __FILE__, __LINE__, #expr); \
             result = false;                                                       \
             goto goto_label;                                                         \
         }                                                                         \
     } while (0)

/**
 * Macro used for running a test from the main function
 */
#define RUN_TEST(test, name)                  \
    do {                                 \
      printf("+ Running <b>%s</b> ... ", name);   \
        if (test()) {                    \
            printf("[OK]\n");            \
        } else {                         \
            printf("[Failed]\n\n <b>To see what the test does and why it failed, please check the link at the top of the page to the test file</b>");        \
        }                                \
    } while (0)


#endif /* TEST_UTILITIES_H_ */
