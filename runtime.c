#include <stddef.h>

#if defined(_MSC_VER)
#define EHIR_PARSE_THREAD_LOCAL __declspec(thread)
#else
#define EHIR_PARSE_THREAD_LOCAL _Thread_local
#endif
static EHIR_PARSE_THREAD_LOCAL size_t ehir_bundle_parse_count = 0;
void ehir_note_bundle_parse(void) { ++ehir_bundle_parse_count; }
size_t ehir_debug_bundle_parse_count(void) { return ehir_bundle_parse_count; }
void ehir_debug_reset_bundle_parse_count(void) { ehir_bundle_parse_count = 0; }
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* The include path is supplied by Encore. The relative fallback also
 * supports the .14 bootstrap and SDK sibling-directory development. */
#if defined(__has_include)
# if __has_include(<runtime_abi.h>)
#  include <runtime_abi.h>
# else
#  include "../core/runtime_abi.h"
# endif
#else
# include "../core/runtime_abi.h"
#endif
typedef encore_str ehir_string;
/* Pure formatting must work while frontend output builders are suppressed. */
ehir_string ehir_core_join_strings_parts(const ehir_string *parts, size_t count,
                                         ehir_string separator) {
    if (count == 0) return encore_empty_str();
    size_t sep = encore_str_size(separator);
    size_t total = 0;
    for (size_t i = 0; i < count; ++i) {
        size_t len = encore_str_size(parts[i]);
        if (len > SIZE_MAX - total) return encore_empty_str();
        total += len;
        if (i != 0) {
            if (sep > SIZE_MAX - total) return encore_empty_str();
            total += sep;
        }
    }
    if (total == SIZE_MAX) return encore_empty_str();
    char *buffer = malloc(total + 1);
    if (buffer == NULL) return encore_empty_str();
    size_t position = 0;
    for (size_t i = 0; i < count; ++i) {
        if (i != 0 && sep != 0) {
            memcpy(buffer + position, encore_str_data(separator), sep);
            position += sep;
        }
        size_t len = encore_str_size(parts[i]);
        if (len != 0) memcpy(buffer + position, encore_str_data(parts[i]), len);
        position += len;
    }
    buffer[total] = '\0';
    return encore_from_owned_buffer(buffer, total);
}
