#include "search.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"

static int contains_case_insensitive(const char *haystack, const char *needle) {
  if (!haystack || !needle || needle[0] == '\0') {
    return 0;
  }

  size_t needle_len = strlen(needle);
  for (const char *cursor = haystack; *cursor != '\0'; cursor++) {
    size_t matched = 0;
    while (matched < needle_len) {
      unsigned char hay_char = (unsigned char)cursor[matched];
      unsigned char needle_char = (unsigned char)needle[matched];
      if (hay_char == '\0' || tolower(hay_char) != tolower(needle_char)) {
        break;
      }
      matched++;
    }

    if (matched == needle_len) {
      return 1;
    }
  }

  return 0;
}

static const char *field_name(ProductSortField field) {
  switch (field) {
  case PRODUCT_SORT_BY_NAME:
    return "Name";
  case PRODUCT_SORT_BY_COUNTRY:
    return "Country";
  case PRODUCT_SORT_BY_MANUFACTURER:
    return "Manufacturer";
  case PRODUCT_SORT_BY_ITEM:
    return "Item";
  case PRODUCT_SORT_BY_PRICE:
    return "Price";
  default:
    return "Unknown";
  }
}

int product_search_by_field(const Product *items, size_t count, ProductSortField field, const char *query) {
  if (!items || !query || query[0] == '\0') {
    return 0;
  }

  printf("\nSearch results for %s matching \"%s\":\n", field_name(field), query);
  print_separator('-', 90);

  char *endptr = NULL;
  double price_query = 0.0;
  if (field == PRODUCT_SORT_BY_PRICE) {
    price_query = strtod(query, &endptr);
    if (endptr == query) {
      printf("No products found.\n");
      return 0;
    }
  }

  int matches = 0;
  for (size_t i = 0; i < count; i++) {
    const Product *product = &items[i];
    int match = 0;

    switch (field) {
    case PRODUCT_SORT_BY_NAME:
      match = contains_case_insensitive(product->name, query);
      break;
    case PRODUCT_SORT_BY_COUNTRY:
      match = contains_case_insensitive(product->country, query);
      break;
    case PRODUCT_SORT_BY_MANUFACTURER:
      match = contains_case_insensitive(product->manufacturer, query);
      break;
    case PRODUCT_SORT_BY_ITEM:
      match = contains_case_insensitive(product->item, query);
      break;
    case PRODUCT_SORT_BY_PRICE:
      match = (product->price == price_query);
      break;
    default:
      break;
    }

    if (match) {
      product_display(product);
      matches++;
    }
  }

  if (matches == 0) {
    printf("No products found.\n");
  } else {
    printf("\nFound %d matching products.\n", matches);
  }

  return matches;
}
