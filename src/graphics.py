import matplotlib.pyplot as plt

def get_lines_from_file(openMp, linuxAPI):
    openMpFile = open(openMp, 'r')
    linuxAPIFile = open(linuxAPI, 'r')
    open_mp = [float(int(x)/1000000) for x in openMpFile.readline().split('ns') if x.isdigit()]
    linux = [float(int(x)/1000000) for x in linuxAPIFile.readline().split('ns') if x.isdigit()]
    return [open_mp, linux]

def create_graphic_mp(open_mp):
    x = list(range(5,(len(open_mp)+1)*5,5))
    plt.subplot(2,2,1)

    plt.xlabel("Размер матрицы")
    plt.ylabel("Время на перемножение в мс")

    plt.plot(x, open_mp, label='OpenMP')
    plt.grid(True)
    plt.legend(loc='upper left')

    # plt.savefig("resources/mp_grahic.png")

def create_graphic_linux(linux):
    x = list(range(5,(len(linux)+1)*5,5))
    plt.subplot(2,2,2)

    plt.xlabel("Размер матрицы")
    plt.ylabel("Время на перемножение в мс")

    plt.plot(x, linux, label='Linux API')
    plt.grid(True)
    plt.legend(loc='upper left')

    # plt.savefig("resources/linux_grahic.png")

def creat_graphic_overall(open_mp, linux):
    x = list(range(5,(len(open_mp)+1)*5,5))
    plt.subplot(2,2,3)

    plt.xlabel("Размер матрицы")
    plt.ylabel("Время на перемножение в мс")

    plt.plot(x, open_mp, label='OpenMP')
    plt.plot(x, linux, label='Linux API')
    plt.grid(True)
    plt.legend(loc='upper left')

    # plt.savefig("resources/overall_grahic.png")

def print_table(open_mp, linux):
    print("N*N\topenMP\t\tlinuxAPI")
    for i in range(len(open_mp)):
        if ((i+1)*5 in (1,2,5,10,20,50,100,150,250,350,450,500)):
            print(f"{(i+1)*5}x{(i+1)*5}\t{open_mp[i]}ms\t{linux[i]}ms")

def main():
    openMp = "resources/openmp.txt"
    linuxAPI = "resources/linuxapi.txt"
    open_mp, linux = get_lines_from_file(openMp, linuxAPI)
    print_table(open_mp, linux)
    if (len(open_mp) != len(linux)):
        print("Different sizes of data!")
        return
    plt.figure(figsize=(14,8))
    create_graphic_mp(open_mp)
    create_graphic_linux(linux)
    creat_graphic_overall(open_mp, linux)
    plt.savefig("resources/graphics.png")
    
    

if __name__ == "__main__":
    main()