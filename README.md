# DM1_variant_repeat
Determining variant repeat length of Myotonic Dystrophy type 1 patient from raw sequences

Notable project files:

- optimisations.txt lists all attempted changes/optimisations (description, test, reasoning...)
- pyth_orig/ contains the original python implementation from https://github.com/picrin/science_of_alignment/blob/master/variant_repeat_algorithm.ipynb
- code/ contains... the code. compilation: ./compile.sh
- sequences/ contains sample DNA sequences (real.txt is parsed from github, garbagefree.txt after passing through cut_garbage)
- test/ contains output of testing different optimisations
- outputs/ contains data-outputs
- visualisers (python3 {visualizer}.py to get usage)

All tests were done on a single core of a intel i7-5500U CPU @ 2.40GHz
