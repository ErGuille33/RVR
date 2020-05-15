#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

using namespace std;
class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        int32_t total = 80 * sizeof(char) + 2 * sizeof(int16_t);

        alloc_data(total);

        char * tmp = _data;

        memcpy(tmp, name , 80);
        tmp += 80 * sizeof(char);

        memcpy(tmp, &x, sizeof(int16_t));
        tmp += sizeof(int16_t);

        memcpy(tmp, &y, sizeof(int16_t));

    }



    int from_bin(char * data)
    {
        char * tmp = data;

        memcpy(name, tmp, 80);
        tmp += 80 * sizeof(char);

        memcpy(&x, tmp, sizeof(int16_t));
        tmp += sizeof(int16_t);

        memcpy(&y, tmp, sizeof(int16_t));
        tmp += sizeof(int16_t);

        return 0;
    }


    static const size_t MAX_NAME = 80;
    char name[MAX_NAME];

    int16_t x;
    int16_t y;
private:

};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);


    one_w.to_bin();

    fstream fs;


    fs.open("Player_ONE", fstream::out);

    fs.write(one_w.data(), one_w.size());

    fs.close();

    char data[one_w.size()];

    fs.open("Player_ONE", fstream::in);

    fs.read(data, one_w.size());

    fs.close();
    one_r.from_bin(data);

    cout << "Name: "<< one_r.name << endl;
    cout << "X: "<< one_r.x << endl;
    cout << "Y: "<< one_r.y << endl;

    return 0;
}

/* ¿Qué hace el comando od: El comando OD permite visualizar un archivo en binario cambiandolo de formato. 
   ¿Qué relación hay entre la salida del comando y la serialización? Lo que sale del comando es la representación visual
   de los datos previamente serializados en binario. */