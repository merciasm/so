typedef struct Clothing
{
   int code;
   char kind[20];
   float price;
   char size[4];
} clothing;


clothing *clothing_new(int id, char *kind, float price, char *size) {
    clothing *new_clothing = malloc(sizeof(clothing));
    new_clothing->code = id;
    strcpy(new_clothing->kind, kind);
    new_clothing->price = price;
    strcpy(new_clothing->size, size);
    return new_clothing;
}
