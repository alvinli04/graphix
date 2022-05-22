import mdl
import sys

def run(filename):
    """
    This function runs an mdl script
    """
    p = mdl.parseFile(filename)

    if p:
        (commands, symbols) = p
    else:
        print("Parsing failed.")
        return

    sym = open("src/compiler/mdl.sym", "w")
    cmd = open("src/compiler/mdl.cmd", "w")

    for k in symbols:
        sym.write(symbols[k][0] + '\n')
        sym.write(k + '\n')
        if symbols[k][0] == 'constants':
            d = symbols[k][1]
            # sym.write(str(d))
            for i in d['red']:
                sym.write(str(i) + ' ')
            for i in d['green']:
                sym.write(str(i) + ' ')
            for i in d['blue']:
                sym.write(str(i) + ' ')

        sym.write('\n')

    for command in commands:
        # cmd.write(str(command) + '\n')
        for k in command:
            if k == 'args' and command[k] is not None:
                for n in command[k]:
                    cmd.write(str(n) + ' ')
            else:
                cmd.write(str(command[k]) + ' ')
        cmd.write('\n')

    sym.close()
    cmd.close()


if __name__ == "__main__":
    run(sys.argv[1])
