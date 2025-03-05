#include "test_bus_lines.h"

int is_sorted(BusLine *start, BusLine *end, SortType sort_type){
    for (BusLine *i = start; i < end; i++){
            if (((sort_type == NAME) && (strcmp(i->name, (i + 1)->name) > 0))
                || ((sort_type == DURATION) && (i->duration > (i + 1)
                ->duration))
                 || ((sort_type == DISTANCE) && (i->distance > (i + 1)
                ->distance)))
            {
                return 0;
            }

        }
        return 1;
}

int is_sorted_by_distance(BusLine *start, BusLine *end){
    return is_sorted(start,end, DISTANCE);
}

int is_sorted_by_duration(BusLine *start, BusLine *end){
    return is_sorted(start,end, DURATION);
}

int is_sorted_by_name(BusLine *start, BusLine *end){
    return is_sorted(start,end, NAME);
}

int equal_buses(BusLine *bus_a, BusLine *bus_b){
    return ((!strcmp(bus_a->name,bus_b->name)) && (bus_a->distance ==
    bus_b->distance) && (bus_a->duration ==
                       bus_b->duration));
}


int is_equal(BusLine *start_sorted, BusLine *end_sorted,
			 BusLine *start_original, BusLine *end_original) {
    if ((end_sorted - start_sorted) != (end_original - start_original)) {
        return 0; // arrays have different sizes
    }

    int n = end_original - start_original; // array size
    int equal = 1;

    BusLine *copy_original = malloc((n+1) * sizeof(BusLine));
    BusLine *copy_sorted = malloc((n+1) * sizeof(BusLine));

    memcpy(copy_original, start_original, (n+1) * sizeof(BusLine));
    memcpy(copy_sorted, start_sorted, (n+1) * sizeof(BusLine));

    if (!copy_sorted || !copy_original) { // allocation failure
        free(copy_original);
        free(copy_sorted);
        return 0;
    }

    bubble_sort(copy_original, copy_original + n);
    bubble_sort(copy_sorted, copy_sorted + n);

    for (int i = 0; i < n; i++) {
        if (!equal_buses(copy_original + i, copy_sorted + i)) {
            equal = 0;
            break;
        }
    }

    free(copy_original);
    free(copy_sorted);

    return equal;
}


