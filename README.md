# DM1_variant_repeat
Determining variant repeat length of Myotonic Dystrophy type 1 patient from raw sequences

Notable project files:

- optimisations.txt lists all attempted changes/optimisations (description, test, reasoning...) [OUTDATED ON THIS BRANCH]
- pyth_orig/ contains the original python implementation from https://github.com/picrin/science_of_alignment/blob/master/variant_repeat_algorithm.ipynb
- code/ contains... the code. compilation: g++ -o bin -fopenmp main.cpp (requires openmp library installed)
- sequences/ contains sample DNA sequences (real.txt is parsed from github, garbagefree.txt after passing through remove_garbage)
- test/ contains output of testing different optimisations (parameters of test, all scores obtained, and finally resulting times) [OUTDATED ON THIS BRANCH]
- outputs/ contains data-outputs
- visualisers {usage: python3 visualiser.py [filename] [number of parameter (0-indexed)] [alpha value of points]

All tests were done on a single core of a intel i7-5500U CPU @ 2.40GHz
