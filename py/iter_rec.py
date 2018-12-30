import numpy as np
import csv
import matplotlib.pyplot as plt

def setup1():
    R1 = np.array([[4, 0]])
    R2 = np.array([[0, 4]])
    R3 = np.array([[4, 4]])
    S = np.array([[0, 0]])
    tgt = np.array([[1, 1.6]])

    e = np.linalg.norm(R1-S)
    h = np.linalg.norm(R2-S)
    g = np.linalg.norm(R3-S)
    t1 = np.linalg.norm(R1-tgt) + np.linalg.norm(S-tgt) - e
    t2 = np.linalg.norm(R2-tgt) + np.linalg.norm(S-tgt) - h
    t3 = np.linalg.norm(R3-tgt) + np.linalg.norm(S-tgt) - g

    def f(x, y):
        T = np.array([[x, y]])
        a = np.linalg.norm(S-T)
        b = np.linalg.norm(R1-T)
        c = np.linalg.norm(R2-T)
        d = np.linalg.norm(R3-T)
        
        return (a + b - t1 - e)**2 + (a + c - t2 - h)**2 + (a + d - t3 - g)**2

    x = np.arange(-5, 5, 0.1)
    y = np.arange(-5, 5, 0.1)

    p_X, p_Y = np.meshgrid(x, y)

    p_Z = [[0]*100 for _ in range(100)]

    for i in range(100):
        for j in range(100):
            p_Z[i][j] = f(x[i], y[j])

    plt.contour(p_X, p_Y, p_Z)
    plt.plot(1, 1.6, 'ro', label="real target")
    plt.plot(0.8, 2.1, 'bo', label="Newton Method Searched Point")
    plt.legend()
    plt.show()
    return

def setup2():
    radius, deg = 5.0, 60.0
    R1 = np.array([[radius*np.cos(0), radius*np.sin(0)]])
    R2 = np.array([[radius*np.cos(np.radians(deg)),
                    radius*np.sin(np.radians(deg))]])
    R3 = np.array([[radius*np.cos(np.radians(2*deg)),
                    radius*np.sin(np.radians(2*deg))]])
    R4 = np.array([[radius*np.cos(np.radians(3*deg)),
                    radius*np.sin(np.radians(3*deg))]])
    tgt = np.array([[-1.0, -1.0]])

    r21 = np.linalg.norm(R2-tgt) - np.linalg.norm(R1-tgt)
    r31 = np.linalg.norm(R3-tgt) - np.linalg.norm(R1-tgt)
    r41 = np.linalg.norm(R4-tgt) - np.linalg.norm(R1-tgt)

    def obj_func(x, y):
        T = np.array([[x, y]])
        dist21 = np.linalg.norm(R2-T) - np.linalg.norm(R1-T)
        dist31 = np.linalg.norm(R3-T) - np.linalg.norm(R1-T)
        dist41 = np.linalg.norm(R4-T) - np.linalg.norm(R1-T)
        return (dist21 - r21)**2 + (dist31-r31)**2 + (dist41-r41)**2
    x = np.arange(-10, 10, 0.1)
    y = np.arange(-10, 10, 0.1)

    p_X, p_Y = np.meshgrid(x, y)
    p_Z = [[0]*200 for _ in range(200)]

    for i in range(200):
        for j in range(200):
            p_Z[i][j] = obj_func(x[i], y[j])

    f = open("iterRecord.csv", 'r')
    dataIter = csv.reader(f)
    iterX, iterY = [], []
    for cell in dataIter:
        iterX.append(round(float(cell[0]), 4))
        iterY.append(round(float(cell[1]), 4))
    f.close()
    # print p_Z
    cs = plt.contour(p_X, p_Y, p_Z)
    plt.clabel(cs, inline=1, fontsize=10)
    plt.plot(iterX, iterY, 'b-')
    plt.scatter(R1[0][0], R1[0][1], marker='o', c='r')
    plt.scatter(R2[0][0], R2[0][1], marker='o', c='r')
    plt.scatter(R3[0][0], R3[0][1], marker='o', c='r')
    sensor = plt.scatter(R4[0][0], R4[0][1], marker='o', c='r')
    target = plt.scatter(tgt[0][0], tgt[0][1], marker='^', c='r')
    plt.legend((sensor, target), ('Receivers', 'Emitter'))
    plt.grid(True)
    plt.show()
    # print p_Z[116][111]
    return

# setup1()
setup2()


        
