#include "utils.h"
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t g_alt_screen_active = 0;
static volatile sig_atomic_t g_continue_pending = 0;
static volatile sig_atomic_t g_reinstall_suspend_pending = 0;
static volatile sig_atomic_t g_resume_to_menu_pending = 0;
static volatile sig_atomic_t g_action_guard_active = 0;
static sigjmp_buf g_action_abort_env;

static void suspend_handler(int sig);

static void install_suspend_handler(void) {
  struct sigaction sa_suspend;
  memset(&sa_suspend, 0, sizeof(sa_suspend));
  sa_suspend.sa_handler = suspend_handler;
  sa_suspend.sa_flags = SA_RESTART;
  sigemptyset(&sa_suspend.sa_mask);
  sigaction(SIGTSTP, &sa_suspend, NULL);
}

static void exit_alternate_screen_signal_safe(void) {
  if (g_alt_screen_active) {
    const char escape_seq[] = "\033[?1049l";
    ssize_t bytes_written = write(STDOUT_FILENO, escape_seq, sizeof(escape_seq) - 1);
    (void)bytes_written;
    g_alt_screen_active = 0;
  }
}

static void signal_handler(int sig) {
  if (sig == SIGCONT) {
    g_continue_pending = 1;
    g_reinstall_suspend_pending = 1;
    g_resume_to_menu_pending = 1;
    return;
  }

  exit_alternate_screen_signal_safe();
  _exit(128 + sig);
}

static void suspend_handler(int sig) {
  (void)sig;
  exit_alternate_screen_signal_safe();
  g_resume_to_menu_pending = 1;

  signal(SIGTSTP, SIG_DFL);
  kill(getpid(), SIGTSTP);
}

void setup_terminal_cleanup(void) {
  atexit(exit_alternate_screen);

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = signal_handler;
  sa.sa_flags = SA_RESTART;
  sigemptyset(&sa.sa_mask);

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGHUP, &sa, NULL);
  sigaction(SIGQUIT, &sa, NULL);
  sigaction(SIGCONT, &sa, NULL);
  install_suspend_handler();

  signal(SIGPIPE, SIG_IGN);
}

void sync_terminal_events(void) {
  if (g_reinstall_suspend_pending) {
    g_reinstall_suspend_pending = 0;
    install_suspend_handler();
  }

  if (g_continue_pending) {
    g_continue_pending = 0;
    enter_alternate_screen();
    clear_screen();
  }
}

int consume_resume_to_menu_request(void) {
  if (g_resume_to_menu_pending) {
    g_resume_to_menu_pending = 0;
    return 1;
  }
  return 0;
}

int run_menu_action_with_resume_guard(void (*action)(void)) {
  g_action_guard_active = 1;

  if (sigsetjmp(g_action_abort_env, 1) != 0) {
    g_action_guard_active = 0;
    return 0;
  }

  action();
  g_action_guard_active = 0;
  return 1;
}

static void abort_action_if_resuming(void) {
  if (g_resume_to_menu_pending && g_action_guard_active) {
    siglongjmp(g_action_abort_env, 1);
  }
}

int guarded_scanf(const char *format, ...) {
  sync_terminal_events();
  abort_action_if_resuming();

  va_list args;
  va_start(args, format);
  int result = vscanf(format, args);
  va_end(args);

  sync_terminal_events();
  abort_action_if_resuming();
  return result;
}

void enter_alternate_screen(void) {
  if (!g_alt_screen_active) {
    printf("\033[?1049h");
    fflush(stdout);
    g_alt_screen_active = 1;
  }
}

void exit_alternate_screen(void) {
  if (g_alt_screen_active) {
    printf("\033[?1049l");
    fflush(stdout);
    g_alt_screen_active = 0;
  }
}

void clear_screen(void) {
  printf("\033[2J\033[H");
  fflush(stdout);
}

void print_separator(char ch, int length) {
  for (int i = 0; i < length; i++) {
    putchar(ch);
  }
  putchar('\n');
}

void clear_input_buffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void wait_for_enter(void) {
  printf("\nPress Enter to continue...");
  fflush(stdout);
  clear_input_buffer();
}

int get_int_input(const char *prompt) {
  int value;
  int result;

  while (1) {
    printf("%s", prompt);
    fflush(stdout);

    result = guarded_scanf("%d", &value);

    clear_input_buffer();

    if (result == 1) {
      return value;
    }

    printf("Invalid input! Please enter a valid integer.\n");
  }
}

int get_int_input_range(const char *prompt, int min, int max) {
  int value;
  int has_min = !isinf((double)min);
  int has_max = !isinf((double)max);

  while (1) {
    sync_terminal_events();
    abort_action_if_resuming();

    value = get_int_input(prompt);

    int valid = 1;
    if (has_min && value < min) {
      valid = 0;
    }
    if (has_max && value > max) {
      valid = 0;
    }

    if (valid) {
      return value;
    }

    if (has_min && has_max) {
      printf("Value must be between %d and %d. Please try again.\n", min, max);
    } else if (has_min) {
      printf("Value must be at least %d. Please try again.\n", min);
    } else if (has_max) {
      printf("Value must be at most %d. Please try again.\n", max);
    }
  }
}

double get_double_input(const char *prompt) {
  double value;
  int result;

  while (1) {
    printf("%s", prompt);
    fflush(stdout);

    result = guarded_scanf("%lf", &value);

    clear_input_buffer();

    if (result == 1) {
      return value;
    }

    printf("Invalid input! Please enter a valid number.\\n");
  }
}

double get_double_input_range(const char *prompt, double min, double max) {
  double value;
  int has_min = !isinf(min);
  int has_max = !isinf(max);

  while (1) {
    sync_terminal_events();
    abort_action_if_resuming();

    value = get_double_input(prompt);

    int valid = 1;
    if (has_min && value < min) {
      valid = 0;
    }
    if (has_max && value > max) {
      valid = 0;
    }

    if (valid) {
      return value;
    }

    if (has_min && has_max) {
      printf("Value must be between %.2f and %.2f. Please try again.\\n", min, max);
    } else if (has_min) {
      printf("Value must be at least %.2f. Please try again.\\n", min);
    } else if (has_max) {
      printf("Value must be at most %.2f. Please try again.\\n", max);
    }
  }
}

int get_yes_no_input(const char *prompt, int default_yes) {
  char input[10];

  while (1) {
    sync_terminal_events();
    abort_action_if_resuming();

    if (default_yes) {
      printf("%s (Yes/no): ", prompt);
    } else {
      printf("%s (yes/No): ", prompt);
    }
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) {
      continue;
    }

    input[strcspn(input, "\n")] = 0;

    if (input[0] == '\0') {
      return default_yes ? 1 : 0;
    }

    char first = input[0];
    if (first == 'y' || first == 'Y') {
      return 1;
    } else if (first == 'n' || first == 'N') {
      return 0;
    }

    printf("Invalid input! Please enter 'y' or 'n'.\n");
  }
}

void print_vec1(const int *arr, size_t size, const char *title) {
  if (title) {
    printf("\n%s\n", title);
    print_separator('-', 60);
  }

  printf("[ ");
  for (size_t i = 0; i < size; i++) {
    printf("%d", arr[i]);
    if (i < size - 1) {
      printf(", ");
    }
  }
  printf(" ]\n");
}

void print_vec2(const int *arr, size_t rows, size_t cols, const char *title) {
  if (title) {
    printf("\n%s\n", title);
    print_separator('-', 60);
  }

  int max_width = 0;
  for (size_t i = 0; i < rows * cols; i++) {
    int width = snprintf(NULL, 0, "%d", arr[i]);
    if (width > max_width) {
      max_width = width;
    }
  }
  max_width += 2;

  for (size_t i = 0; i < rows; i++) {
    printf("| ");
    for (size_t j = 0; j < cols; j++) {
      printf("%*d ", max_width, arr[i * cols + j]);
    }
    printf("|\n");
  }
}

void input_vec1(int *arr, size_t size) {
  printf("\nEnter %zu elements:\n", size);
  for (size_t i = 0; i < size; i++) {
    char prompt[50];
    snprintf(prompt, sizeof(prompt), "Element [%zu]: ", i);
    arr[i] = get_int_input(prompt);
  }
}

void input_vec2(int *arr, size_t rows, size_t cols) {
  printf("\nEnter elements for %zu x %zu matrix:\n", rows, cols);
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      char prompt[50];
      snprintf(prompt, sizeof(prompt), "Element [%zu][%zu]: ", i, j);
      arr[i * cols + j] = get_int_input(prompt);
    }
  }
}

void generate_random_vec1(int *arr, size_t size, int min, int max) {
  static int seeded = 0;
  if (!seeded) {
    srand((unsigned int)time(NULL));
    seeded = 1;
  }

  int range = max - min + 1;
  for (size_t i = 0; i < size; i++) {
    arr[i] = min + (rand() % range);
  }
}

void generate_random_vec2(int *arr, size_t rows, size_t cols, int min, int max) {
  generate_random_vec1(arr, rows * cols, min, max);
}

void copy_vec1(int *dest, const int *src, size_t size) { memcpy(dest, src, size * sizeof(int)); }

void copy_vec2(int *dest, const int *src, size_t rows, size_t cols) { memcpy(dest, src, rows * cols * sizeof(int)); }

int *duplicate_vec1(const int *src, size_t size) {
  int *copy = (int *)malloc(size * sizeof(int));
  if (copy) {
    memcpy(copy, src, size * sizeof(int));
  }
  return copy;
}

int *duplicate_vec2(const int *src, size_t rows, size_t cols) { return duplicate_vec1(src, rows * cols); }
