# DM1_variant_repeat
Determining variant repeat length of Myotonic Dystrophy type 1 patient from raw sequences

Notable project files:

- optimisations.txt lists all attempted changes/optimisations (description, test, reasoning...)
- pyth_orig/ contains the original python implementation from https://github.com/picrin/science_of_alignment/blob/master/variant_repeat_algorithm.ipynb
- code/ contains... the code
- sequences/ contains sample DNA sequences (see real.txt)
- test/ contains output of testing different optimisations (parameters of test, all scores obtained, and finally resulting times)
- outputs/ contains data-outputs
- visualiser.py {usage: python3 visualiser.py. then input filename with data}: displays a plot of the data with xaxis=#CCGCTG   repetitions, yaxis = score


All tests were done on a single core of a intel i7-5500U CPU @ 2.40GHz
