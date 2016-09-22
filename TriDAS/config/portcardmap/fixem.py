import sys
for fn in sys.argv[1:]:
    lines = open(fn).readlines()
    new_f = open(fn, 'wt')
    for line in lines:
        if 'FPix' in line:
            line = line.split()
            line.insert(2, ' XXX')
            line = '\t'.join(line) + '\n'
            new_f.write(line.replace('XXX', 'A'))
            new_f.write(line.replace('XXX', 'B'))
        else:
            new_f.write(line)
