#include "sort_bus_lines.h"

//documentation in header file
void swap_buses(BusLine *bus_a, BusLine *bus_b){
    BusLine temp_bus = *bus_a;
    *bus_a = *bus_b;
    *bus_b =temp_bus;
}

//documentation in header file
void bubble_sort (BusLine *start, BusLine *end){
    int n = (int)(end - start); // length of the array
    for (int i=0; i< n; i++){
        for (BusLine *bus_ptr = start; bus_ptr < end - i; bus_ptr++)
        {
            if(strcmp(bus_ptr->name, (bus_ptr+1)->name) > 0){
                swap_buses(bus_ptr, bus_ptr+1);
            }
        }
    }
}
//documentation in header file
void quick_sort (BusLine *start, BusLine *end, SortType sort_type){
    if ( start < end)
    {
        BusLine *pivot = partition(start,end, sort_type);
        quick_sort(start, pivot-1, sort_type);
        quick_sort(pivot+1, end, sort_type);

    }
}
//documentation in header file
BusLine *partition (BusLine *start, BusLine *end, SortType sort_type){
    BusLine *pivot = end; //setting the pivot to the end of array
    BusLine *i = start -1;
    for (BusLine *cur_bus = start; cur_bus < end; cur_bus++){
        if (((sort_type == DISTANCE) && (cur_bus->distance < pivot->distance))
		|| ((sort_type == DURATION) && (cur_bus->duration < pivot->duration)))
        {
            i++;
            swap_buses(i, cur_bus);
        }
    }
    i++;
    swap_buses(i, end);
    return i;
}
