Average
=======

Perform various averaging functions on arrays

Current function support:

    Rolling average
    Mean
    Mode
    Minimum
    Maximum
    Standard Deviation

All the functions are fully overloaded to support the following data types:

    char
    unsigned char
    int
    unsigned int
    long
    unsigned long
    float

With the exception of stddev() they all return the same data type as the array. An array of int values returns a single int. stddev() always returns a float.

All the functions except rollingAverage() take two arguments. The first is the array to work on. The second is the number of entries in the array. rollingAverage() takes a third argument - the new entry to add to the array.

Rolling average
---------------

Format: average = rollingAverage(history_array, slice_count, value);

Adds value to the array history_array shifting all the values down one place. The mean average is then returned.

Mean
----

Format: average = mean(array, slice_count);

Calculates the mean average of the values in array. slice_count is the number of entries in the array.

Mode
----

Format: average = mode(array,slice_count);

Finds the most common number in the array.

Maximum
-------

Format: max = maximum(array,slice_count);

Finds the largest value in the array.

Minimum
-------

Format: min = minimum(array,slice_count);

Finds the smallest value in the array.

Standard Deviation
------------------

Format: deviation = stddev(array,slice_count);

The standard deviation is the square root of the mean of the sum of the squares of the difference between each data point and the array mean average.

This is the only function which does not return the same data type as the array. The standard deviation is always returned as a float.
