#define ID      1   //identifikátor
#define INTEGER 2
#define DOUBLE  3
#define STRING  4
#define ASSING  5   //=
#define EQ      6   //==
#define NEQ     7   //<>
#define LESS    8   //<
#define GREAT   9   //>
#define LESSEQ  10  //<=
#define GREATEQ 11  //>=
#define ADD     12  //+
#define SUB     13  //-
#define MUL     14  //*
#define DIV     15  //"/"
#define INTDIV  16  //"\"
#define LINEC   17  //radkovy komentar
#define BLOCKC  18  //blokovy komentar
#define KEYW    19  //klicove slovo



typedef struct token {
    int type;           //typ lexemu
    char *value;        //hodnota lexemu
    int length;         // delka lexemu
} t_token;


int get_next_token(FILE *, t_token *);
void char_append(char, t_token *);
void token_init(t_token *);
void token_destr(t_token *);