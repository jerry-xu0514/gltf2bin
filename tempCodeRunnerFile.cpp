
void read(int &size, FILE* f)
{
    fread(&size, sizeof(int), 1, f);
}

int main()
{
    int first, second, third;
    FILE* f = fopen("character.bin", "rb");
    read(first, f);
    read(second, f);
    read(third, f);
    cout << first << " " << second << " " << third << endl;
}