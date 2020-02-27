def readf(file):
    seqs = [ line.strip() for line in file.readlines()]
    assert len(seqs) % 4 == 0
    seqs = seqs[1::4]
    return seqs

with open('sequences/raw/DMGV14C/X4_14C_original.fastq','r') as DMGV:
    with open('sequences/raw/X4_14C/X4_14C.fastq','r') as X4_14C:

        DMGVData = readf(DMGV)
        X4_14CData = readf(X4_14C)

        DinX = 0
        XinD = 0

        for seq in DMGVData:
            if seq in X4_14CData:
                DinX += 1

        for seq in X4_14CData:
            if seq in DMGVData:
                XinD += 1

        print('DMGV:',DinX,'/',len(DMGVData))
        print('X4_14C:',XinD,'/',len(X4_14CData))