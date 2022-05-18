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
    
    sym = open("mdl.sym", "w")
    cmd = open("mdl.cmd", "w")

    sym.write(str(symbols))
    

    for command in commands:
        cmd.write(str(command))
        cmd.write("\n")

    sym.close()
    cmd.close()


if __name__ == "__main__":
    run(sys.argv[1])
