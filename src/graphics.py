import matplotlib.pyplot as plt

def get_lines_from_file(filename_omp, filename_la):
    file_omp = open(filename_omp, 'r')
    file_la = open(filename_la, 'r')

    open_mp = [int(x) for x in file_omp.readline().split('ns') if x.isdigit()]
    linux = [int(x) for x in file_la.readline().split('ns') if x.isdigit()]
    return [open_mp, linux]

def creat_graphic(open_mp, linux):
    x = list(range(5,5+len(open_mp)))
    plt.xlabel("Размер матриц")
    plt.ylabel("Время на перемножение в мс")
    plt.plot(x, open_mp, label="OpenMP")
    plt.plot(x, linux, label="Linux API")
    plt.legend()
    plt.savefig("resources/pp_grahic.png")
    plt.show()


def main():
    filename_la = "resources/linuxapi.txt"
    filename_omp = "resources/openmp.txt"
    open_mp, linux = get_lines_from_file(filename_omp, filename_la)
    if (len(open_mp) != len(linux)):
        print("Different sizes of data!")
        return
    creat_graphic(open_mp, linux)


if __name__ == "__main__":
    main()