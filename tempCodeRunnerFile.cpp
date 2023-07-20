#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <iostream>

#include "character.h"
#include "array.h"

using namespace std;

void read(unsigned int &size, FILE* f)
{
    fread(&size, sizeof(int), 1, f);
}

int main()
{
    character character_data;
    character_load(character_data, "./character.bin");
    cout << character_data.triangles.size << endl;
    for(int i = 1000; i < 1100; i++) cout << character_data.triangles(i) << endl;
}