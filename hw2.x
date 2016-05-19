const MAXFILENAME = 16;   /* maximum length of file name is 15 */
const MAXSTRLEN = 512;    /* maximum length of input string is 511 */ 

struct stringPair
{
    string filename<MAXFILENAME>;
    string str<MAXSTRLEN>;
};

struct stringInt
{
    string filename<MAXFILENAME>;
    int len;
};

typedef string str<>;
 
program HWPROG
{   version HWVERS
    {   int deletedl(string) = 1;
        int renamedl(stringPair) = 2;
        int savedl(stringPair) = 3;
        str headdl(stringInt) = 4;
        str treedl(void) = 5;
    } = 1;
} = 0x29229945;
