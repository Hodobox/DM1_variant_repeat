import sys
sys.setrecursionlimit(2048)
deletion_score = -1
gap_score = -1
mismatch_score = -1
match_score = +1

lensum = 0

def align(template, sequence, cache = None):
    if cache is None:
        cache = dict()
    if (template, sequence) in cache:
        return cache[(template, sequence)]
    if not template:
        return len(sequence) * deletion_score, ""
    if not sequence:
        return len(template) * gap_score, template
    
    template_head, template_tail = template[0], template[1:]
    sequence_head, sequence_tail = sequence[0], sequence[1:]

    global lensum
    lensum += len(template) + len(sequence)
    
    if template_head == sequence_head:
        match_mismatch_score = match_score
    else:
        match_mismatch_score = mismatch_score
    
    s, seq = align(template_tail, sequence_tail, cache)
    match_result = s + match_mismatch_score, template_head + seq
    s, seq = align(template, sequence_tail, cache)
    delete_result = s + deletion_score, seq
    s, seq = align(template_tail, sequence, cache)
    gap_result = s + gap_score, template_head + seq
    
    results = [match_result, delete_result, gap_result]
    results.sort()
    result = results[-1]
    cache[(template, sequence)] = result
    return result

def produce_sequences(pattern, max_length, outer_index, inner_index, partial_result):
    if len(partial_result) >= max_length:
        return [partial_result]

    current_pattern = pattern[outer_index]
    results = []
    next_pattern_available = (outer_index != len(pattern) - 1) and (inner_index == 0)
        
    next_inner_index = (inner_index + 1) % len(current_pattern)
    current_character = pattern[outer_index][inner_index]
    next_partial = partial_result + current_character
    
    if next_pattern_available:
        jump_right_now = produce_sequences(pattern, max_length, outer_index + 1, 0, partial_result)
        for p in jump_right_now:
            results.append(p)
    
    same_pattern_result = produce_sequences(pattern, max_length, outer_index, next_inner_index, next_partial)
    for p in same_pattern_result:
        results.append(p)
    
    return results

def produce_sequences_outer(pattern, max_length):
    return list(set(produce_sequences(pattern, max_length, 0, 0, "")))



def produce_sequence_and_align(pattern, template):
    sequences = produce_sequences_outer(pattern, len(template))
    results = sorted([align(seq, template) for seq in sequences])
    return results[-1]

def collapse_sequence(pattern, sequence):
    answer = []
    repetitions = 0
    for elem in pattern:
        while True:
            tried = elem * repetitions
            if sequence[:len(tried)] != tried:
                answer.append(repetitions - 1)
                sequence = sequence[len(tried) - len(elem):]
                repetitions = 0
                break
            repetitions += 1
    return list(zip(answer, pattern))

def best_templates_from_raw_reads(pattern, sequences):
    avg_sequence_length = round(sum(len(sequence) for sequence in sequences)/len(sequences))
    candidate_patterns = produce_sequences_outer(pattern, avg_sequence_length)
    scores = {sequence: 0 for sequence in candidate_patterns}
    for pattern in candidate_patterns:
        for sequence in sequences:
            scores[pattern] += align(pattern, sequence)[0]
    
    sorted_sequences = sorted([(value, sequence) for sequence, value in scores.items()], reverse=True)
    return sorted_sequences

#sequence = 'GCTGTTGCTGCTGCTTGCTGCTGCTTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCATGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCTGCTGCTGCTGCTGCTGCTGCCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGATGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCCGCTGCCGCTGCGCTGCCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCGCTGCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCCGCTGCGCTGCGCTGCGCTGCCGCTGCGCTGCCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCGCTGCCGCTGCCGCTGCGCTGCGCTGCCGCTGCGCTGCCGCTGCCGCTGCGCTGCCGCTGCCGCAGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCCGCTGCTGCTGCT'

sequence = input()

import time
start = time.time()
candidate_templates = produce_sequences_outer(["CTG", "CCGCTG", "CTG"], len(sequence))
stop = time.time()

print("took {}s".format(stop - start))

print(len(candidate_templates))
candidate_templates = list(sorted(candidate_templates))
#print(*candidate_templates)

#exit()

start = time.time()
print(align(candidate_templates[0], sequence))
print(lensum)
stop = time.time()

single_alignment = stop - start

print("took {}s".format(single_alignment))

