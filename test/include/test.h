#ifndef DIFFUSION_TEST_H
#define DIFFUSION_TEST_H

#include <stdio.h>
#include <string.h>

#define STRIGNIFY(x) STRIGNIFY2(x)
#define STRIGNIFY2(x) #x
#define LINE_STRING STRIGNIFY(__LINE__)

extern int tests_ran;
extern int tests_succeeded;
extern int tests_failed;

#define TEST_DATA_IMPL       \
    int tests_ran = 0;       \
    int tests_succeeded = 0; \
    int tests_failed = 0

#define TEST_CASE(test_name) static const char* test_name(void)

#define TEST_ASSERT(message, test) \
    do {                           \
        if (!(test))               \
            return message;        \
    } while (0)

#define ASSERT_TRUE(actual)                                                                      \
    TEST_ASSERT(                                                                                 \
      "Error in file " __FILE__ " at line " LINE_STRING ": expected \"" #actual "\" to be true", \
      (actual) == 1)

#define ASSERT_EQUALS(expected, actual)                                         \
    TEST_ASSERT(                                                                \
      "Error in file " __FILE__ " at line " LINE_STRING ": expected \"" #actual \
      "\" to equal \"" #expected "\"",                                          \
      expected == actual)

#define ASSERT_STRING_EQUALS(expected, actual)                                  \
    TEST_ASSERT(                                                                \
      "Error in file " __FILE__ " at line " LINE_STRING ": expected \"" #actual \
      "\" to equal \"" #expected "\"",                                          \
      strcmp(expected, actual) == 0)

#define ASSERT_NULL(actual)                                                                      \
    TEST_ASSERT(                                                                                 \
      "Error in file " __FILE__ " at line " LINE_STRING ": expected \"" #actual "\" to be NULL", \
      actual == NULL)

#define ASSERT_NOT_NULL(actual)                                                 \
    TEST_ASSERT(                                                                \
      "Error in file " __FILE__ " at line " LINE_STRING ": expected \"" #actual \
      "\" to not be "                                                           \
      "NULL",                                                                   \
      actual != NULL)

#define TEST_SUCCESS return NULL

#define TEST_RUN(test)                        \
    do {                                      \
        printf("Executing %s... ", #test);    \
        const char* message = test();         \
        ++tests_ran;                          \
        if (message) {                        \
            printf("Failed!: %s\n", message); \
            ++tests_failed;                   \
        } else {                              \
            printf("Success!\n");             \
            ++tests_succeeded;                \
        }                                     \
    } while (0)

#define TEST_RUN_ALL static void all_tests(void)

#define TEST_MAIN_IMPL                                                           \
    int main(int argc, char** argv) {                                            \
        /* unused */                                                             \
        (void)argc;                                                              \
        (void)argv;                                                              \
                                                                                 \
        all_tests();                                                             \
                                                                                 \
        if (tests_failed != 0) {                                                 \
            printf("%d test(s) failed. See above for details.\n", tests_failed); \
        } else {                                                                 \
            printf("ALL TESTS PASSED\n");                                        \
        }                                                                        \
                                                                                 \
        printf(                                                                  \
          "Test(s) ran: %d succeeded, %d failure. Total: %d\n",                  \
          tests_succeeded,                                                       \
          tests_failed,                                                          \
          tests_ran);                                                            \
                                                                                 \
        return tests_failed != 0;                                                \
    }

#endif // DIFFUSION_TEST_H
