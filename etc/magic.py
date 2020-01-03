import random

def magic(p):
    l = '='*25
    g = [ [] for i in range(love) ]
    for i in range(love*7):
        g[i%love].append(p[i])
    print('\n', '1'.center(24, ' '), '2'.center(25), '3'.center(25))
    print(l, l, l)
    for i in range(7):
        print('|', g[0][i].ljust(21), '|', '|', g[1][i].ljust(21), '|',
              '|', g[2][i].ljust(21), '|' )
    print(l, l, l, '\n')
    return g

love = 3
PLL = [ "Java", "C", "C++", "C#", "Python", "Visual Basic .NET", "PHP",
        "JavaScript", "Delphi/Object Pascal", "Swift", "Perl", "Ruby",
        "Assembly Language", "R", "Visual Basic", "Objective-C", "Go",
        "MATLAB", "PL/SQL", "Scratch", "SAS" ]

random.shuffle(PLL)
g = [ [] for i in range(love) ]

for i in range(love):
    g = magic(PLL)
    pick = input("{:d}. 당신이 좋아하는 언어는 몇 번에 있습니까? ".format(i+1))
    g[1], g[int(pick)-1] = g[int(pick)-1], g[1]
    PLL = g[0] + g[1] + g[2]
LOVE = g[love-2][love]
print('You Love ' + LOVE + '.')


