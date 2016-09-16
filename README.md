# Manatee
Manatee checks whether the expected values (means) of two samples are significantly different.

In order to decide whether the null hypothesis that the two samples came from
the same unknown distribution should be accepted or rejected,
Manatee uses the combined data sets to bootstrap (i.e. select random) samples of the same sizes
as the original samples, and measures the difference between their means.

It outputs the ratio of these experiments where the measured difference
is larger than the difference between the means of the original samples.
If this ratio is less than 5%, then it is unlikely that the original samples
came from the same distribution.

As Manatee actually selects random samples to measure the distribution of this difference,
it does not rely on any assumption about the unknown distribution.

## Compilation
Use `gcc -O3 -lm -o manatee manatee.c` to compile the source.

## Usage
`manatee` reads data from STDIN, one number per line:
- size of set 1 (integer)
- size of set 2 (integer)
- number of experiments (integer)
- data points of set 1 (float)
- ...
- data points of set 2 (float)
- ...

## Author

Copyright (c) 2016 Elod Csirmaz