from time import sleep, time_ns
import serial

# функция считывания данных из файла
def Data_Read(file, adr, len):
    file.seek(adr)
    data = bytearray(file.read(len))
    per = 0
    for i in range(len):
        per |= data[i] << (8*i)
    return per

# функция отправки картинки в UART
def FrameToUART(path, sens):
    file = open(path, "rb")

    # считывание данных о размере картинки
    head = Data_Read(file, 0x0A, 4)
    tale = Data_Read(file, 0x02, 4)
    size = tale - head

    # считывание ширины
    # (должна быть кратной 4 в большую сторону)
    width = Data_Read(file, 0x12, 2)
    if width % 4:
        width += 4 - width % 4

    # вычисление высоты
    height = size // width

    # перемещение указателя на начало данных картинки
    file.seek(head)

    # объявление массива размером с картинку
    mas = [[int(0)] * width for i in range(height)]

    # запись пикселей картинки в массив
    for i in range(height):
        for j in range(width):
            mas[height - i - 1][j] = file.read(1)

    # создание массива для отправки в UART
    Buff = bytearray(504)
    for i in range(6):
        for j in range(width):
            byte = 0x00
            for bit in range(8):
                if mas[i * 8 + bit][j] < sensitivity: byte |= 1<<bit

            Buff[i * width + j] = byte

    com_port.write(Buff)
    file.close()  


# основная программа
com_port = serial.Serial()
com_port.baudrate = 250000
com_port.port = 'COM4'
com_port.open()

# граница того, какие пиксели отправлять на дисплей
sensitivity = bytes(b'\x80')

for i in range(1,5258):

    # ожидание посылки синхронизации от ATmega8
    while com_port.inWaiting() == 0:
        pass
    com_port.readline(1)   

    path = "Frames/img"
    if      i < 10:    path += "000"
    elif    i < 100:   path += "00"
    elif    i < 1000:  path += "0"
    path += str(i) + ".bmp"

    FrameToUART(path, sensitivity)


com_port.close()