# quick script to re-write sequences from source file to more readily usable .txt

sequences = []

with open('sequences/X8_15C.fastq','r') as data:
    lines = data.readlines()
    assert len(lines) % 4 == 0
    for i in range(len(lines)//4):
        sequences.append( lines[i*4+1].strip() )

with open('sequences/X8_15C.txt', 'w+') as output:
    output.write(str(len(sequences)) + "\n")
    for seq in sequences:
        output.write(seq + "\n")    
