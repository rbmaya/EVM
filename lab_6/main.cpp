#include <libusb-1.0/libusb.h>
#include <stdio.h>

using namespace std;

void printdev(libusb_device *dev) {
    libusb_device_descriptor desc; // дескриптор устройства
    int r = libusb_get_device_descriptor(dev, &desc); //получение дискриптора устройства,
    //первый аргумент - libusb_device *dev, второй аргумент - struct libusb_device_descriptor *desc

    if (r < 0) {
        fprintf(stderr, "Ошибка: дескриптор устройства не получен, код: %d.\n", r);
        return;
    }

    printf("\tКласс устройства: %.2X\n"
           "\tИдентифиактор производителя: %.4d\n"
           "\tИдентификатор изделия: %.4d\n",
           (int) desc.bDeviceClass,
           desc.idVendor,
           desc.idProduct
    );

    libusb_device_handle *dev_handle;//Ещё один дескриптор устройства, используемый для операций ввода/вывода и тд
    r = libusb_open(dev, &dev_handle);//Открыть устройство

    if (r < 0) {
        fprintf(stderr, "Ошибка: не удалось открыть устройство, код: %d.\n", r);
        return;
    }

    unsigned char data[150];
    libusb_get_string_descriptor_ascii(dev_handle, desc.iManufacturer, data, 150); //перевести идентификатор в формат СИ-строки
    printf("\tПроизводитель: %s\n", data);
    libusb_get_string_descriptor_ascii(dev_handle, desc.iProduct, data, 150);
    printf("\tИзделие: %s\n", data);
    libusb_get_string_descriptor_ascii(dev_handle, desc.iSerialNumber, data, 150);
    printf("\tСерийный номер устройства: %s\n", data);

}

int main() {
    libusb_device **devs; // указатель на указатель на устройство,
    // используется для получения списка устройств
    libusb_context *ctx = NULL; // контекст сессии libusb

    // инициализировать библиотеку libusb, открыть сессию работы с libusb
    int r = libusb_init(&ctx);

    if (r < 0) {
        fprintf(stderr,
                "Ошибка: инициализация не выполнена, код: %d.\n", r);
        return 1;
    }

    // задать уровень подробности отладочных сообщений
    libusb_set_debug(ctx, 3);

    // получить список всех найденных USB- устройств
    size_t cnt = libusb_get_device_list(ctx, &devs);

    if (cnt < 0) {
        fprintf(stderr,
                "Ошибка: список USB устройств не получен, код: %d\n", r);
        return 1;
    }


    printf("Найдено устройств: %lu\n", cnt);
    for (size_t i = 0; i < cnt; i++) { // цикл перебора всех устройств
        printf("Устройство № %.2d:\n", (int)i + 1);
        printdev(devs[i]); // печать параметров устройства
    }


    printf("=============================="
           "=============================\n");


    libusb_free_device_list(devs, 1);// освободить память, выделенную функцией получения списка устройств

    libusb_exit(ctx); // завершить работу с библиотекой libusb,
    // закрыть сессию работы с libusb

    return 0;
}
