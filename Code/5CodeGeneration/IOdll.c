#include <stdio.h>
#include <stdbool.h>

/*extern "C" int readInt();
extern "C" float readReal();
extern "C" char readChar();
extern "C" bool readBoolean();
extern "C" int writeInt(int);
extern "C" int writeReal(float);
extern "C" int writeChar(char);
extern "C" int writeBoolean(bool);
*/
int readInt()
{
    int num;
    scanf("%d", &num);
    return num;
}
float readReal()
{
    float num;
    scanf("%f", &num);
    return num;
}
char readChar()
{
    char ch;
    ch = getchar();
    return ch;
}
bool readBoolean()
{
    int num;
    scanf("%d", &num);
    return num;
}
void writeInt(int num)
{
    printf("%d", num);
    fflush(stdout);
    return;
}
void writeReal(float num)
{
    printf("%f", num);
    fflush(stdout);
    return;
}
void writeChar(char ch)
{
    printf("%c", ch);
    fflush(stdout);
    return;
}
void writeBoolean(bool num)
{
    if (num == true)
        printf("TRUE");
    else
        printf("FALSE");
    fflush(stdout);
    return;
}
