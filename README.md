# DM1_variant_repeat
Determining variant repeat length of Myotonic Dystrophy type 1 patient from raw sequences

Notable project files:

- optimisations.txt lists all attempted changes/optimisations (description, test, reasoning...)
- pyth_orig/ contains the original python implementation from https://github.com/picrin/science_of_alignment/blob/master/variant_repeat_algorithm.ipynb
- scorecode/ contains the align scoring code. compilation: ./compile.sh
- scorevisualizers contains visualizers for outputs of align scoring code (distribution of results per parameter, param vs. param etc.) 
    ^ use python3 somevisualizer.py to get usage, -h for list of options
- sequences/ contains sample DNA sequences (real.txt is parsed from github, garbagefree.txt after trimming. raw/ contain the original raw data)
    ^ due to privacy, they are not part of the repo. Raw data is on S3 in dm1data bucket.
- test/ contains output of testing different optimisations
- outputs/ contains data-outputs
- windowcode, windowvisualizers is a similar pair for an (un)supervised sliding window approach
- HMM contains code for the Hidden Markov Model approach
- Tablet contains files used for visualization of our data in Tablet (early work in progress)


All tests were done on a single core of a intel i7-5500U CPU @ 2.40GHz
