import matplotlib.pyplot as plt

def get_lines_from_file(filename):
    file = open(filename, 'r')
    open_mp = [float(int(x)/1000000) for x in file.readline().split('ns') if x.isdigit()]
    linux = [float(int(x)/1000000) for x in file.readline().split('ns') if x.isdigit()]
    return [open_mp, linux]

def creat_graphic(open_mp, linux):
    x = list(range(5,5+len(open_mp)))

    plt.xlabel("Размер матриц")
    plt.ylabel("Время на перемножение в мс")

    plt.plot(x, open_mp, label='OpenMP')
    plt.plot(x, linux, label='Linux API')
    plt.legend(loc='upper left')

    plt.savefig("resources/pp_grahic.png")
    plt.show()


def main():
    filename = "src/data.txt"
    open_mp, linux = get_lines_from_file(filename)
    if (len(open_mp) != len(linux)):
        print("Different sizes of data!")
        return
    creat_graphic(open_mp, linux)
    
    

if __name__ == "__main__":
    main()