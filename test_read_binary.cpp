#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <iostream>

using namespace std;

void read(unsigned int &size, FILE* f)
{
    fread(&size, sizeof(int), 1, f);
}

int main()
{
    unsigned int first, second, third;
    FILE* f = fopen("character.bin", "rb");
    read(first, f);
    read(second, f);
    read(third, f);
    cout << first << " " << second << " " << third << endl;
}