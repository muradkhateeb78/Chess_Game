#include <iostream>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <fstream>
#include <string.h>
int endingrow=88,endingcol=98;
using namespace std;
int psr,psc,per,pec;
int ppsr,ppsc,pper,ppec;
char pfirst;
char plast;
char first;
char last;
int K1=0,K2=0,R11=0,R12=0,R21=0,R22=0;
HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
void gotoRowCol(int rpos, int cpos)
{
    int xpos=cpos, ypos = rpos;
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = xpos;
    scrn.Y = ypos;
    SetConsoleCursorPosition(hOuput,scrn);
}
void sleep(int m);
void inputnames(char name1[],char name2[]);
void initboard(char b[][8]);
void printboard(char [][8],int,int,char);
void takeinput(char b[][8],int & sr,int & sc, int & er,int & ec,int & pr,int & pc,int & turn,char name1[],char name[2]);
bool checklegality(char b[][8],int sr,int sc,int er,int ec,int turn);
void moveplayer(char b2[][8],int,int,int,int,int,int,int);
void turnchange(int & turn);
bool check(char b[][8],int turn);
bool selfcheck(char b[][8],int srow,int scol,int erow,int ecol,int turn);
void pawnpromotion(char b[][8],int turn);
bool staleorcheckmate(char b[][8],int turn,bool mate,char whichmate);
bool am_i_in_check(char b[][8],int turn);
void castlemove(char b[][8],int sr,int sc,int er,int ec,int rdim, int cdim,int turn);
bool castling(char b[][8],int sr,int sc,int er,int ec,int turn);
void savegame(char b[][8],int turn);
void namebox(char name1[],char name2[],int turn);
void menubox(char name1[],char name2[],int turn);
void letsplay();
void initiatgame(char b[][8],int rdim,int cdim,char symbol,char name1[],char name2[],int & turn);
int main()
{
    SetConsoleTextAttribute(h,10);
    bool legality,isincheck;
    bool stalemate=false;
    bool checkmate=true;
    SetConsoleTextAttribute(h,10);
    int rdim,cdim,pr=0,pc=0;
    char name1[20],name2[20];
    rdim=cdim=10;
    char b[8][8];
    char sy=-78;
    int sr=0,er=0,sc=0,ec=0;
    int turn=1;
    initiatgame(b,rdim,cdim,sy,name1,name2,turn);
    do
    {
        menubox(name1,name2,turn);
        namebox(name1,name2,turn);
        if(staleorcheckmate(b,turn,stalemate,'s'))
        {
            gotoRowCol(30,100);
            cout<<"Stalemate!!! The game has been drawn.";
            getch();
            return 0;
        }
        if(staleorcheckmate(b,turn,checkmate,'c'))
        {
            gotoRowCol(30,100);
            if(turn==1)
                cout<<"Checkmate! "<<name2<<" have won the Game";
            else
                cout<<"Checkmate! "<<name2<<" have won the Game";
            getch();
            return 0;
        }
        do
        {
            takeinput(b,sr,sc,er,ec,pr,pc,turn,name1,name2);
            legality=checklegality(b,sr,sc,er,ec,turn);
            isincheck=selfcheck(b,sr,sc,er,ec,turn);
            if (!legality || isincheck)
            {
                gotoRowCol(30,95);
                SetConsoleTextAttribute(h,12);
                cout<<"This is an illegal move...";
                sleep(4000);
                gotoRowCol(30,95);
                cout<<"                              ";
                SetConsoleTextAttribute(h,10);

            }
        }
        while(!legality || isincheck);
        if((turn==1 && ( sr==0 && sc==4 && er==0 && ec==0))|| (sr==0 && sc==4 && er==0 && ec==7) && b[0][4]=='K')
        {
            castlemove(b,sr,sc,er,ec,rdim,cdim,turn);
        }
        else if((turn==2 && (sr==7 && sc==4 && er==7 && ec==0)) || (sr==7 && sc==4 && er==7 && ec==7) && b[7][4]=='k')
        {
            castlemove(b,sr,sc,er,ec,rdim,cdim,turn);
        }
        else
            moveplayer(b,sr,sc,er,ec,rdim,cdim,turn);

        if(turn==1 && check(b,turn) && !(staleorcheckmate(b,2,checkmate,'c')))
        {
            gotoRowCol(40,100);
            SetConsoleTextAttribute(h,12);
            cout<<name2<<" is in Check!!!";
            SetConsoleTextAttribute(h,10);
        }
        else if(turn==2 && check(b,turn) && !(staleorcheckmate(b,1,checkmate,'c')))
        {
            gotoRowCol(45,100);
            SetConsoleTextAttribute(h,12);
            cout<<name1<<" is in Check!!!";
            SetConsoleTextAttribute(h,10);

        }
        if(turn==1 && !am_i_in_check(b,turn))
        {
            gotoRowCol(45,100);
            cout<<"                               ";
        }
        else if(turn==2 && !am_i_in_check(b,turn))
        {
            gotoRowCol(40,100);
            cout<<"                               ";
        }
        pawnpromotion(b,turn);
        turnchange(turn);
        savegame(b,turn);
    }
    while(true);
    getch();
    return 0;
}
void clearfile()
{
    std::ofstream printout("save_game.txt",ios_base::trunc);
}
void sleep(int m)
{
    for(int i=0; i<(m*99999); i++)
    {


    }
}
int absolute(int x)
{
    if(x<0)
    {
        x=x*-1;
    }
    return x;
}
bool ismypiece(char b[][8],int row,int col,int turn)
{
    if(turn==1)
    {
        return(b[row][col]>= 'A' && b[row][col]<='Z');
    }
    else
    {
        return(b[row][col]>= 'a' && b[row][col]<='z');
    }
}
bool ismyopponentpiece(char b[][8],int row,int col,int turn)
{
    if(turn==1)
    {
        return(b[row][col]>= 'a' && b[row][col]<='z');
    }
    else
    {
        return(b[row][col]>= 'A' && b[row][col]<='Z');
    }
}
bool selfcheck(char b[][8],int srow,int scol,int erow,int ecol,int turn)
{
    char x,y;
    bool isincheck;
    x=b[srow][scol];
    y=b[erow][ecol];
    b[erow][ecol]=x;
    b[srow][scol]=' ';
    turnchange(turn);
    isincheck=check(b,turn);
    b[srow][scol]=x;
    b[erow][ecol]=y;
    turnchange(turn);
    return isincheck;
}
bool ishorizental(int sr,int sc,int er,int ec)
{
    return(sr==er);
}
bool isvertical(int sr,int sc,int er,int ec)
{
    return(sc==ec);
}
bool isdiagonal(int sr,int sc,int er,int ec)
{
    return(absolute(er-sr)==absolute(ec-sc));
}
bool isverticalpathclear(char b[][8],int sr,int sc,int er,int ec)
{
    int temp=0;
    if(sr>er)                 //direction ko reverse karnay k lia. is ka origonal values pay koi farq nahi paray ga.
    {
        temp=sr;
        sr=er;
        er=temp;
    }
    for(int ri=sr+1; ri<=er-1; ri++)
    {
        if(b[ri][sc]!=' ')
        {
            return false;
        }
    }
    return true;
}
bool ishorizentalpathclear(char b[][8],int sr,int sc,int er,int ec)
{
    int temp=0;
    if(sc>ec)                             //direction ko reverse karnay k lia. is ka origonal values pay koi farq nahi paray ga.
    {
        temp=sc;
        sc=ec;
        ec=temp;
    }
    for(int ri=sc+1; ri<=ec-1; ri++)
    {
        if(b[sr][ri]!=' ')
        {
            return false;
        }
    }
    return true;
}
bool isdiagonalpathclear(char b[][8],int sr,int sc,int er,int ec)
{
    int time=absolute(sr-er)-1;
    int temp;
    int diagonal;
    if(sr>er && sc > ec)
    {
        temp=sr;
        sr=er;                                             //for reversing row;
        er=temp;

        temp=sc;
        sc=ec;                                             //for reversing colms;
        ec=temp;
    }
    else if(sr>er && sc<ec)
    {
        temp=sr;
        sr=er;                                             //for reversing row;
        er=temp;

        temp=sc;
        sc=ec;                                             //for reversing colms;
        ec=temp;
    }
    if(er-sr>0 && ec-sc>0)
    {
        diagonal=1;                                        //kon sa diagonal hay yah pata karnay k lia.
    }
    else if(er-sr>0 && ec-sc<0)
    {
        diagonal=2;
    }
    if(diagonal==1)
    {
        for(int i=1; i<=time; i++)
        {
            if(b[sr+i][sc+i]!=' ')
            {
                return false;
            }
        }
    }
    else if(diagonal==2)
    {
        for(int i=1; i<=time; i++)
        {
            if(b[sr+i][sc-i]!=' ')
            {
                return false;
            }
        }
    }
    return true;

}
void turnchange(int & turn)
{
    switch(turn)
    {
    case 1:
    {
        turn =2;
        break;
    }
    case 2:
    {
        turn =1;
        break;
    }
    }
}
bool am_i_in_check(char b[][8],int turn)
{
    bool isincheck;
    turnchange(turn);
    isincheck=check(b,turn);
    turnchange(turn);
    return isincheck;
}
void initboard(char b[][8])
{
    b[0][0]=b[0][7]='R';
    b[0][1]=b[0][6]='N';
    b[0][2]=b[0][5]='B';
    b[0][3]='Q';
    b[0][4]='K';
    b[7][0]=b[7][7]='r';
    b[7][1]=b[7][6]='n';
    b[7][2]=b[7][5]='b';
    b[7][3]='q';
    b[7][4]='k';
    for(int ri=1; ri<=1; ri++)
    {
        for(int ci=0; ci<8; ci++)
        {
            b[ri][ci]='P';
        }
    }
    for(int ri=6; ri<=6; ri++)
    {
        for(int ci=0; ci<8; ci++)
        {
            b[ri][ci]='p';
        }
    }

    for(int ri=2; ri<=5; ri++)
    {
        for(int ci=0; ci<8; ci++)
        {
            b[ri][ci]=' ';
        }
    }

}
void removepreviousposition(int sr,int sc,int r,int c,char symbol,int rdim,int cdim)
{
    int color;
    if(symbol==' ')
    {
        if(endingrow==r && endingcol==c)
        {
            color=390;
        }
        else
        {
            color=15;
        }
        SetConsoleTextAttribute(h,color);
        gotoRowCol(sr+(rdim/2-1),sc+(cdim/2));

        cout<<symbol;
    }
    else
    {
        if(endingrow==r && endingcol==c)
        {
            color=390;
        }
        else
        {
            color=240;
        }
        SetConsoleTextAttribute(h,color);
        gotoRowCol(sr+(rdim/2-1),sc+(cdim/2));

        cout<<' ';                                 // space actually prints a pixel to remove prev pos
    }


}
void printinbox(int sr,int sc,int r,int c,char symbol,int rdim,int cdim,char sign)
{
    int color1;
    int color2;
    if(r==endingrow && c==endingcol)
    {
        color1=396;
        color2=393;
    }
    else
    {
        color1=508;
        color2=505;
    }
    if(((symbol >='a' && symbol<='z') || symbol==' ') && (((r%2==0) && (c%2==0)) || ((r%2==1) && (c%2==1))))
    {
        SetConsoleTextAttribute(h,color1);
        gotoRowCol(sr+(rdim/2-1),sc+(cdim/2));
            if(symbol=='g')
                cout<<'p';
            else
                cout<<symbol;

    }
    else if((((symbol >='a') && (symbol<='z')) || symbol==' ') && (((r%2==0) && (c%2==1)) || ((r%2==1) && (c%2==0))) && ((color1==508)))
    {
        SetConsoleTextAttribute(h,12);
        gotoRowCol(sr+(rdim/2-1),sc+(cdim/2));
                    if(symbol=='g')
                cout<<'p';
            else
                cout<<symbol;

    }
    else if((((symbol >='a') && (symbol<='z')) || symbol==' ') && (((r%2==0) && (c%2==1)) || ((r%2==1) && (c%2==0))) && (color1==396))
    {
        SetConsoleTextAttribute(h,color1);
        gotoRowCol(sr+(rdim/2-1),sc+(cdim/2));
        {

                if(symbol=='g')
                    cout<<'p';
                else
                    cout<<symbol;
            }


    }
    else if((((symbol >='A') && (symbol<='Z')) || symbol==' ') && (((r%2==0) && (c%2==0)) || ((r%2==1) && (c%2==1))))
    {
        SetConsoleTextAttribute(h,color2);
        gotoRowCol(sr+(rdim/2-1),sc+(cdim/2));

            if(symbol=='G')
                cout<<'P';
            else
                cout<<symbol;

    }
    else if((((symbol >='A') && (symbol<='Z')) || symbol==' ') && (((r%2==0) && (c%2==1)) || ((r%2==1) && (c%2==0))) && (color2==505))
    {
        SetConsoleTextAttribute(h,9);
        gotoRowCol(sr+(rdim/2-1),sc+(cdim/2));

            if(symbol=='G')
                cout<<'P';
            else
                cout<<symbol;
    }
    else if((((symbol >='A') && (symbol<='Z')) || symbol==' ') && (((r%2==0) && (c%2==1)) || ((r%2==1) && (c%2==0))) && (color2==393))
    {
        SetConsoleTextAttribute(h,color2);
        gotoRowCol(sr+(rdim/2-1),sc+(cdim/2));

            if(symbol=='G')
                cout<<'P';
            else
                cout<<symbol;

    }

}
bool bishoplegality(char b[][8],int sr,int sc,int er,int ec)
{
    if(isdiagonal(sr,sc,er,ec) && isdiagonalpathclear(b,sr,sc,er,ec))
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool rooklegality(char b[][8],int sr,int sc,int er,int ec)
{
    if((isvertical(sr,sc,er,ec))&& (isverticalpathclear(b,sr,sc,er,ec)))
    {
        return true;
    }
    else if ((ishorizental(sr,sc,er,ec)) && (ishorizentalpathclear(b,sr,sc,er,ec)))
    {
        return true;
    }
    else
    {
        return false;
    }

}
bool queenlegality(char b[][8],int sr,int sc,int er,int ec)
{
    if(bishoplegality(b,sr,sc,er,ec))
    {
        return true;
    }
    else if(rooklegality(b,sr,sc,er,ec))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool knightlegality(int sr,int sc,int er,int ec)
{

    return ((absolute(er-sr)==2 && absolute(ec-sc)==1) || (absolute(er-sr)==1 && absolute(ec-sc)==2));
}
bool kingslegality(char b[][8],int  sr,int  sc,int er,int  ec,int turn)
{
    if((turn==1 && (((sr==0 && sc==4 && er==0 && ec==0) && b[sr][sc]=='K' && b[er][ec]=='R') || ((sr==0 && sc==4 && er==0 && ec==7) && b[sr][sc]=='K' && b[er][ec]=='R'))) ||
            (turn==2 && (((sr==7 && sc==4 && er==7 && ec==0) && b[sr][sc]=='k' && b[er][ec]=='r') || ((sr==7 && sc==4 && er==7 && ec==7) && b[sr][sc]=='k' && b[er][ec]=='r'))))

    {
        return castling(b,sr,sc,er,ec,turn);
    }
    else if((absolute(er-sr)==1) && (isvertical(sr,sc,er,ec)))                               //yahan pay path clear karwanay ki zarorat nahi hay. kiu k aik hi box move karna hay and wo take input main check ho jata hay.
    {
        return true;
    }
    else if((absolute(ec-sc)==1) && (ishorizental(sr,sc,er,ec)))                        //yahan pay path clear karwanay ki zarorat nahi hay. kiu k aik hi box move karna hay and wo take input main check ho jata hay.
    {
        return true;
    }
    else if((absolute(er-sr)==1) && (isdiagonal(sr,sc,er,ec)))                            //yahan pay path clear karwanay ki zarorat nahi hay. kiu k aik hi box move karna hay and wo take input main check ho jata hay.
    {
        return true;
    }
    else
    {
        return false;
    }

}
bool pawnslegality(char b[][8],int sr,int sc,int er,int ec,int turn)
{
    bool diagonal=isdiagonal(sr,sc,er,ec);
    bool opponent=ismyopponentpiece(b,er,ec,turn);
    if(!isvertical(sr,sc,er,ec) && (!diagonal))
    {
        return false;
    }
    if ((turn==1 && b[sr][sc]!='G' && b[sr][sc]!='g') || (turn==2 && b[sr][sc]=='g'))
    {
        if (er-sr<0)
        {
            return false;
        }
        if (sr==1 && (b[sr][sc]=='P' || b[sr][sc]=='g')  && (er-sr==2 || er-sr==1) && (!diagonal) && (!opponent) && (isverticalpathclear(b,sr,sc,er,ec)))          //!diagonal was added because without this it would have taken diagonals as well.
        {
            return true;
        }
        else if(sr!=1 && (b[sr][sc]=='P' || b[sr][sc]=='g') && er-sr==1 && (!diagonal) && (!opponent))
        {
            return true;
        }
        else if((b[sr][sc]=='P' || b[sr][sc]=='g') && (diagonal && (er-sr==1)) && (ismyopponentpiece(b,er,ec,turn)))
        {
            return true;
        }
        return false;
    }
    else if ((turn==2 && b[sr][sc]!='G' && b[sr][sc]!='g') || (turn==1 && b[sr][sc]=='G'))
    {
        if (er-sr>0)
        {
            return false;
        }
        if (sr==6 && (b[sr][sc]=='p' || b[sr][sc]=='G') && (er-sr==-2 || er-sr==-1) && (!diagonal) && (!opponent) && (isverticalpathclear(b,sr,sc,er,ec)))      //!diagonal was added because without this it would have taken diagonals as well.
        {
            return true;
        }
        else if((sr!=6) && (b[sr][sc]=='p' || b[sr][sc]=='G') && (er-sr==-1) && (!diagonal) && (!opponent))
        {
            return true;
        }
        else if((b[sr][sc]=='p' || b[sr][sc]=='G') && (diagonal && (er-sr==-1)) && (ismyopponentpiece(b,er,ec,turn)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}
bool checklegality(char b[][8],int sr,int sc,int er,int ec,int turn)
{
    switch(b[sr][sc])
    {
    case 'R':
    case 'r':
    {
        return rooklegality(b,sr,sc,er,ec);
        break;
    }
    case 'B':
    case 'b':
    {
        return bishoplegality(b,sr,sc,er,ec);
        break;
    }
    case 'Q':
    case 'q':
    {
        return queenlegality(b,sr,sc,er,ec);
        break;
    }
    case 'N':
    case 'n':
    {
        return knightlegality(sr,sc,er,ec);
        break;
    }
    case 'K':
    case 'k':
    {
        return kingslegality(b,sr,sc,er,ec,turn);
        break;
    }
    case 'P':
    case 'p':
    {
        return pawnslegality(b,sr,sc,er,ec,turn);
        break;
    }
    }
    return true;
}
bool load_game(char b[][8],int rdim,int cdim,char sy,int & turn,int champ)
{
    char a;
    ifstream printin;
    printin.open("save_game.txt",ios::app);
    while(1)
    {
        printin>>turn;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {

                if(printin.eof() && champ==2)
                {
                    printin.close();
                    printboard(b,rdim,cdim,sy);
                    return true;
                }
                else if(printin.eof() && champ==1)
                {
                    printin.close();
                   // printboard(b,rdim,cdim,sy);
                    return true;
                }
                printin>>a;
                if(a=='-')
                {
                    a=' ';
                }
                b[i][j]=a;
            }
        }
    }
    printin.close();
    printboard(b,rdim,cdim,sy);
}
void printbox(int sr,int sc,int rdim,int cdim,char sy,int color)
{
    SetConsoleTextAttribute(h,13);
    if(sy==' ')                                                         // is ko mujay full box is lia banana para kiu k main nay take input k block main full box banana tha.
    {
        for(int ri=sr; ri<(sr+rdim); ri++)
        {
            for(int ci=sc; ci<(sc+cdim); ci++)
            {
                {
                    gotoRowCol(ri,ci);
                    cout<<' ';                                          //space ki waja say wo pink colour ka backgroun color print karta hay.
                }

            }
        }
    }
    else if(sy==-78)
    {
        SetConsoleTextAttribute(h,color);
        gotoRowCol(sr,sc);
        for(int ri=sr; ri<(sr+rdim); ri++)
        {
            for(int ci=sc; ci<(sc+cdim); ci++)
            {
                {
                    gotoRowCol(ri,ci);
                    cout<<' ';                                          //space ki waja say wo pink colour ka backgroun color print karta hay.
                }

            }
        }
    }
}
bool islegal(int row,int col)
{
    return((row>=0 && row<8) && (col>=0 && col<8));
}
bool promotedpawnlegality(char x,int turn)
{
    if (turn==1)
    {
        if(x=='R' || x=='B' || x=='N'|| x=='Q' || x=='P')
        {
            return true;
        }
    }
    else if(turn==2)
    {
        if(x=='r' || x=='b' || x=='n'|| x=='q' || x=='p')
        {
            return true;
        }
    }
    else
        return false;
}
void sidelines()
{
    int row=4,col=5;
    gotoRowCol(row,col);
    for (int j=2; j<5; j++)
    {
        for(int i=0; i<=82; i++)

        {
            gotoRowCol(j,i+5);
            SetConsoleTextAttribute(h,320);
            cout<<' ';
        }

    }
    col=col+80;
    for (int j=col; j<col+3; j++)
    {
        for(int i=1; i<=82; i++)
        {
            gotoRowCol(row+i,j);
            cout<<' ';
        }
    }
    row=row+80;
    col=col-80;
    for(int j=row+1; j<row+3; j++)
    {
        for(int i=0; i<=81; i++)
        {
            gotoRowCol(j,i+5);
            cout<<' ';
        }
    }
    row=4;
    col=4;
    for (int j=col-2; j<col+1; j++)
    {
        for(int i=0; i<85; i++)
        {
            gotoRowCol(i+2,j);
            cout<<' ';
        }
    }

}
void printarray(char b[][8],int rdim,int cdim)
{
    int r=0,c=0;
    for(int ri=5; ri<85; ri+=rdim)
    {
        r++;
        c=0;
        for(int ci=5; ci<85; ci+=cdim)
        {
            c++;
            printinbox(ri,ci,r-1,c-1,b[r-1][c-1],rdim,cdim,' '); //yeh jo main printinbox k function main spaces dall raha houn. wo actually sirf move able box k lia hayn.
        }
    }

}

bool undo(char b[][8], int & turn,char name1[],char name2[])
{
    int a,c,d,e;
    turnchange(turn);
    b[psr][psc]=first;
    b[per][pec]=last;
    printarray(b,10,10);
    SetConsoleTextAttribute(h,14);
    a=psr;
    c=psc;
    d=per;
    e=pec;
    ppsr=per;
    ppsc=pec;
    pper=psr;
    ppec=psc;
    plast=b[per][pec];
    pfirst=b[psr][psc];
    if(turn==1)
    {
        gotoRowCol(9,135);
        SetConsoleTextAttribute(h,10);
        cout<<">>";
        gotoRowCol(14,135);
        cout<<"  ";
    }
    else
    {
        gotoRowCol(14,135);
        SetConsoleTextAttribute(h,10);
        cout<<">>";
        gotoRowCol(9,135);
        cout<<"  ";
    }
}
bool redo(char b[][8], int & turn,char name1[],char name2[])
{
    load_game(b,10,10,char(-79),turn,1);
    printarray(b,10,10);
    if(turn==1)
    {
        gotoRowCol(9,135);
        SetConsoleTextAttribute(h,10);
        cout<<">>";
        gotoRowCol(14,135);
        cout<<"  ";
    }
    else
    {
        gotoRowCol(14,135);
        SetConsoleTextAttribute(h,10);
        cout<<">>";
        gotoRowCol(9,135);
        cout<<"  ";
    }
}

void savegame(char b[][8],int turn)
{
    char a;
    ofstream printout;
    printout.open("save_game.txt",ios::app);
    printout<<turn<<"\n";
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            a=b[i][j];
            if(a==' ')
            {
                a='-';
            }
            printout<<a;
        }
        printout<<"\n";
    }

    printout.close();

}
bool replay(char b[][8],int rdim,int cdim,char sy,int & turn,char name1[],char name2[])
{
    int count=1;
    int key=3;
    char a;
    ifstream printin;
    printin.open("save_game.txt",ios::app);
    while(1)
    {

        key=3;
        gotoRowCol(40,100);
        SetConsoleTextAttribute(h,10);
        cout<<"Replay...";
        printin>>turn;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {

                if(printin.eof())
                {
                    printin.close();
                    gotoRowCol(40,100);
                    cout<<"                  ";
                    return true;
                }
                printin>>a;
                if(a=='-')
                {
                    a=' ';
                }
                b[i][j]=a;
            }
        }
        namebox(name1,name2,turn);
        if(count==1)
        {
            printboard(b,rdim,cdim,sy);
        }
        else
        {
        printarray(b,rdim,cdim);
        sleep(90000);
        }

        count++;
    }
    printin.close();
}
void printboard(char b[][8],int rdim,int cdim,char sy)
{
    int r=0,c=0;
    for(int ri=5; ri<85; ri+=rdim)
    {
        r++;
        c=0;
        for(int ci=5; ci<85; ci+=cdim)
        {
            c++;
            if ((r%2==1) && (c%2==1))
            {
                printbox(ri,ci,rdim,cdim,sy,240);           //396 colour hay because i had printed spaces to show pink colours and in takeinput function. i had used different colour scheme.
            }
            if((r%2==0) && (c%2==0))
            {
                printbox(ri,ci,rdim,cdim,sy,240);
            }
            if ((r%2==1) && (c%2==0))
            {
                printbox(ri,ci,rdim,cdim,' ',240);
            }
            if((r%2==0) && (c%2==1))
            {
                printbox(ri,ci,rdim,cdim,' ',240);
            }


        }
    }
    sidelines();
    printarray(b,rdim,cdim);
}
void strings(int row,int col,string x,int color)
{
    gotoRowCol(row,col);
    SetConsoleTextAttribute(h,color);
    cout<<x;
}
void yellowline1(int row,int col,int x,int color,char sy)
{
    gotoRowCol(row,col);
    SetConsoleTextAttribute(h,color);
    for(int i=1; i<x; i++)
    {
        gotoRowCol(row+i,col);
        cout<<sy;
    }

}
void yellowline(int row,int col,int x,int color,char sy)
{
    gotoRowCol(row,col);
    SetConsoleTextAttribute(h,color);
    for(int i=1; i<x; i++)
    {
        cout<<sy;
    }

}
void menubox(char name1[],char name2[],int turn)
{
    yellowline(68,125,40,11,char(219));
    yellowline(67,124,42,12,'%');
    yellowline(84,125,40,11,char(219));
    yellowline(85,124,42,12,'%');
    yellowline1(68,163,16,11,char(219));
    yellowline1(67,164,18,12,'%');
    yellowline1(68,125,16,11,char(219));
    yellowline1(67,124,18,12,'%');
    gotoRowCol(68,138);
    SetConsoleTextAttribute(h,10);
    cout<<"CONTROL PANEL";
    strings(72,130,"NEW GAME",12);
    strings(72,150,"N",11);
    strings(75,130,"SAVE GAME",12);
    strings(75,150,"Q",11);
    strings(78,130,"Movement",12);
    strings(78,144,"UP,DOWN,LEFT,RIGHT",11);
    strings(81,130,"UNDO",12);
    strings(81,150,"U",11);



}
void namebox(char name1[],char name2[],int turn)
{
    gotoRowCol(9,90);
    SetConsoleTextAttribute(h,9);
    cout<<name1;
    gotoRowCol(80,90);
    SetConsoleTextAttribute(h,12);
    cout<<name2;
    yellowline(3,125,40,11,char(219));
    yellowline(2,124,42,12,'%');
    yellowline(19,125,40,11,char(219));
    yellowline(20,124,42,12,'%');
    yellowline1(3,163,16,11,char(219));
    yellowline1(2,164,18,12,'%');
    yellowline1(3,125,16,11,char(219));
    yellowline1(2,124,18,12,'%');
    gotoRowCol(3,143);
    SetConsoleTextAttribute(h,10);
    cout<<"TURN";
    if (turn==1)
    {
        gotoRowCol(9,135);
        SetConsoleTextAttribute(h,10);
        cout<<">>";
        gotoRowCol(14,135);
        cout<<"  ";
    }
    else if (turn==2)
    {
        gotoRowCol(14,135);
        SetConsoleTextAttribute(h,10);
        cout<<">>";
        gotoRowCol(9,135);
        cout<<"  ";

    }
    gotoRowCol(9,140);
    SetConsoleTextAttribute(h,9);
    cout<<name1;
    gotoRowCol(14,140);
    SetConsoleTextAttribute(h,12);
    cout<<name2;

}
void inputnames(char name1[],char name2[])
{
    yellowline(35,44,60,14,char(219));
    yellowline(34,43,62,12,'*');
    yellowline(55,44,60,14,char(219));
    yellowline(56,43,62,12,'*');
    strings(42,60,"Enter Player 1's name:",9);
    SetConsoleTextAttribute(h,10);
    cin>>name1;
    strings(47,60,"Enter Player 2's name:",9);
    SetConsoleTextAttribute(h,10);
    cin>>name2;
}
void initiatgame(char b[][8],int rdim,int cdim,char symbol,char name1[],char name2[],int & turn)
{
    yellowline(35,44,60,14,char(219));
    yellowline(34,43,62,12,'*');
    yellowline(55,44,60,14,char(219));
    yellowline(56,43,62,12,'*');
    strings(37,45,"Please Enter:",10);
    strings(43,70,"'N'",9);
    strings(43,75,"For",12);
    strings(43,80,"NEW GAME",11);
    strings(47,70,"'L'",9);
    strings(47,75,"For",12);
    strings(47,80,"LOAD GAME",11);
    strings(50,70,"'R'",9);
    strings(50,75,"For",12);
    strings(50,80,"Replay       ",11);
    char a='p';
    while(a!='n' && a!='N' && a!='l' && a!='L' && a!='r' && a!='R')
    {
        cin>>a;
        if(a!='n' && a!='N'&& a!='l' && a!='L' && a!='r' && a!='R')
        {
            gotoRowCol(10,100);
            SetConsoleTextAttribute(h,12);
            cout<<"Wrong input....";
            sleep(4000);
            gotoRowCol(10,100);
            cout<<"                         ";
            gotoRowCol(50,96);
            SetConsoleTextAttribute(h,11);

        }
    }
    if(a=='n' || a=='N')
    {
        clearfile();
        system("CLS");
        inputnames(name1,name2);
        system("CLS");
        initboard(b);
        printboard(b,rdim,cdim,symbol);
    }
    else if(a=='r' || a=='R')
    {
        system("CLS");
        inputnames(name1,name2);
        system("CLS");
        namebox(name1,name2,turn);
        menubox(name1,name2,turn);
        replay(b,rdim,cdim,symbol,turn,name1,name2);
    }
    else
    {
        system("CLS");
        inputnames(name1,name2);
        system("CLS");
        load_game(b,rdim,cdim,symbol,turn,2);
    }

}
bool castling(char b[][8],int sr,int sc,int er,int ec,int turn)
{
    if(turn==1)
    {
        if((sr==0 && sc==4 && er==0 && ec==0))
        {
            if(b[0][1]==' ' && b[0][2]==' ' && b[0][3]==' ')
            {
                if(K1==0 && R11==0)
                {
                    if(!am_i_in_check(b,turn) && (!selfcheck(b,0,4,0,3,turn)) && (!selfcheck(b,0,4,0,2,turn)))
                    {
                        return true;
                    }

                }
            }

        }
        else if((sr==0 && sc==4 && er==0 && ec==7))
        {
            if(b[0][5]==' ' && b[0][6]==' ')
            {
                if(K1==0 && R12==0)
                {
                    if(!am_i_in_check(b,turn) && (!selfcheck(b,0,4,0,5,turn)) && (!selfcheck(b,0,4,0,6,turn)))
                    {
                        return true;
                    }

                }
            }

        }
        gotoRowCol(5,100);
        SetConsoleTextAttribute(h,12);
        cout<<"Illegal Castle move";
        SetConsoleTextAttribute(h,10);
        sleep(4000);
        gotoRowCol(5,100);
        cout<<"                     ";
        return false;
    }
    else if(turn==2)
    {
        if((sr==7 && sc==4 && er==7 && ec==0))
        {
            if(b[7][1]==' ' && b[7][2]==' ' && b[7][3]==' ')
            {
                if(K2==0 && R21==0)
                {
                    if(!am_i_in_check(b,turn) && (!selfcheck(b,7,4,7,3,turn))&& (!selfcheck(b,7,4,7,2,turn)))
                    {
                        return true;
                    }

                }
            }

        }
        else if((sr==7 && sc==4 && er==7 && ec==7))
        {
            if(b[7][5]==' ' && b[7][6]==' ')
            {
                if(K2==0 && R22==0)
                {
                    if(!am_i_in_check(b,turn) && (!selfcheck(b,7,4,7,5,turn)) && (!selfcheck(b,7,4,7,6,turn)))
                    {
                        return true;
                    }

                }
            }

        }
        gotoRowCol(5,100);
        SetConsoleTextAttribute(h,12);
        cout<<"Illegal Castle move...";
        SetConsoleTextAttribute(h,10);
        sleep(4000);
        gotoRowCol(5,100);
        cout<<"                         ";
        return false;
    }
}
void takeinput(char b[][8],int & sr,int & sc, int & er,int & ec,int & pr,int & pc,int & turn,char name1[],char name2[])
{
    SetConsoleTextAttribute(h,10);
    int row,col;
    row=pr;
    col=pc;
    int key=0;
    char symbol;
    gotoRowCol(pr,pc);
    int rdim=10,cdim=10;
    bool issecond=false;
    bool ismypiece1=false;
    bool ismypiece2=true;
    while(!ismypiece1 || ismypiece2)
    {
        if(kbhit())
        {
            key=getch();
            if(key==224)
            {
                key=getch();
                switch(key)
                {
                case 72:
                {
                    row--;
                    if(row==-1)
                    {
                        row=7;
                    }
                    break;

                }
                case 75:
                {
                    col--;
                    if(col==-1)
                    {
                        col=7;
                    }
                    break;

                }
                case 77:
                {
                    col++;
                    if(col==8)
                    {
                        col=0;
                    }
                    break;

                }
                case 80:
                {
                    row++;
                    if(row==8)
                    {
                        row=0;
                    }
                    break;

                }
                }


            }
            else
            {
                switch(key)
                {
                case 87:
                case 119:
                {
                    row--;
                    if(row==-1)
                    {
                        row=7;
                    }
                    break;

                }
                case 65:
                case 97:
                {
                    col--;
                    if(col==-1)
                    {
                        col=7;
                    }
                    break;

                }
                case 68:
                case 100:
                {
                    col++;
                    if(col==8)
                    {
                        col=0;
                    }
                    break;

                }
                case 83:
                case 115:
                {
                    row++;
                    if(row==8)
                    {
                        row=0;
                    }
                    break;

                }
                }
            }
            if(key==13)
            {
                if(!issecond)
                {
                    sr=row;
                    sc=col;
                    ismypiece1=ismypiece(b,sr,sc,turn);
                    if (!ismypiece1)
                    {
                        gotoRowCol(30,95);
                        SetConsoleTextAttribute(h,10);
                        cout<<"Wrong input.....Enter again";
                        sleep(3000);
                        gotoRowCol(30,95);
                        cout<<"                             ";

                        gotoRowCol(row,col);
                        continue;

                    }
                    issecond=true;
                }
                else
                {
                    er=row;
                    ec=col;
                    if((turn==1 && (((sr==0 && sc==4 && er==0 && ec==0) && b[sr][sc]=='K' && b[er][ec]=='R') || ((sr==0 && sc==4 && er==0 && ec==7) && b[sr][sc]=='K' && b[er][ec]=='R'))) ||
                            (turn==2 && (((sr==7 && sc==4 && er==7 && ec==0) && b[sr][sc]=='k' && b[er][ec]=='r') || ((sr==7 && sc==4 && er==7 && ec==7) && b[sr][sc]=='k' && b[er][ec]=='r'))))
                    {
                        ismypiece2=false;
                    }
                    else
                        ismypiece2=ismypiece(b,er,ec,turn);
                    if (ismypiece2)
                    {
                        gotoRowCol(30,95);
                        SetConsoleTextAttribute(h,10);
                        cout<<"Wrong input.....Enter again";
                        sleep(3000);
                        gotoRowCol(30,95);
                        cout<<"                               ";
                        issecond=false;
                        gotoRowCol(row,col);
                        continue;


                    }
                }
            }
            else if(key==85 || key==117)
            {
                undo(b,turn,name1,name2);
                 printbox(row*rdim+5,col*cdim+5,rdim,cdim,char(-78),396);
                 printinbox(row*rdim+5,col*cdim+5,row,col,b[row][col],rdim,cdim,' ');
            }
            else if(key==89 || key==121)
            {
                 redo(b,turn,name1,name2);
            }
            else if(key==67 || key==99)
            {
                    load_game(b,10,10,char(-78),turn,2);
            }
            else if(key==78 || key==110)
            {
                system("cls");
                main();
            }

            if(row==pr && col==pc)
            {
                continue;
            }
            if((pr%2==0 && pc%2==0) || (pr%2==1 && pc%2==1))
            {
                symbol=-78;
            }
            else
            {
                symbol=' ';
            }
            endingrow=row;
            endingcol=col;

            printbox(row*rdim+5,col*cdim+5,rdim,cdim,char(-78),397);
            printinbox(row*rdim+5,col*cdim+5,row,col,b[row][col],rdim,cdim,symbol);
            printbox(pr*rdim+5,pc*cdim+5,rdim,cdim,symbol,240);
            printinbox(pr*rdim+5,pc*cdim+5,pr,pc,b[pr][pc],rdim,cdim,' ');
            gotoRowCol(row*rdim+14,col*cdim+15);


            pr=row;
            pc=col;
        }


    }
    endingrow=er;
    endingcol=ec;
}
void castlemove(char b[][8],int sr,int sc,int er,int ec,int rdim, int cdim,int turn)
{
    gotoRowCol(5,90);
    SetConsoleTextAttribute(h,10);
    cout<<"Castle move";
    sleep(4000);
    gotoRowCol(5,90);
    cout<<"             ";
    if(turn==1)
    {
        if(sr==0 && sc==4 && er==0 && ec==0)
        {
            moveplayer(b,sr,sc,sr,sc-2,rdim,cdim,turn);
            moveplayer(b,er,ec,er,ec+3,rdim,cdim,turn);
        }
        else if(sr==0 && sc==4 && er==0 && ec==7)
        {
            moveplayer(b,sr,sc,sr,sc+2,rdim,cdim,turn);
            moveplayer(b,er,ec,er,ec-2,rdim,cdim,turn);
        }
    }
    else if(turn==2)
    {
        if(sr==7 && sc==4 && er==7 && ec==0)
        {
            moveplayer(b,sr,sc,sr,sc-2,rdim,cdim,turn);
            moveplayer(b,er,ec,er,ec+3,rdim,cdim,turn);
        }
        else if(sr==7 && sc==4 && er==7 && ec==7)
        {
            moveplayer(b,sr,sc,sr,sc+2,rdim,cdim,turn);
            moveplayer(b,er,ec,er,ec-2,rdim,cdim,turn);
        }
    }


}
void moveplayer(char b[][8],int sr,int sc, int er, int ec,int rdim,int cdim,int turn)
{
    psr=sr;
    psc=sc;
    per=er;
    pec=ec;
    first=b[sr][sc];
    last=b[er][ec];
    char symbol;
    gotoRowCol(er,ec);
    if(b[sr][sc]=='K')
    {
        K1++;
    }
    else if(b[sr][sc]=='k')
    {
        K2++;
    }
    else if(sr==0 && sc==0 && b[sr][sc]=='R')
    {
        R11++;
    }
    else if(sr==0 && sc==7 && b[sr][sc]=='R')
    {
        R12++;
    }
    else if(sr==7 && sc==0 && b[sr][sc]=='r')
    {
        R21++;
    }
    else if(sr==7 && sc==7 && b[sr][sc]=='r')
    {
        R22++;
    }
    printinbox((er*rdim)+5,(ec*cdim)+5,er,ec,b[sr][sc],rdim,cdim,' ');
    b[er][ec]=b[sr][sc];
    if((sr%2==0 && sc%2==0) || (sr%2==1 && sc%2==1))
    {
        symbol=-78;
    }
    else
    {
        symbol=' ';
    }
    removepreviousposition(( sr*rdim)+5,(sc*cdim)+5,sr,sc,symbol,rdim,cdim);
    SetConsoleTextAttribute(h,10);
    b[sr][sc]=' ';
}
bool staleorcheckmate(char b[][8],int turn,bool mate,char whichmate)
{
    int sr,sc;
    bool isincheck;
    if(turn==1)
    {
        isincheck=am_i_in_check(b,turn);
        if(isincheck==mate)                            //matlab k check main hay.
        {
            for(int i=0; i<8; i++)
            {
                for(int j=0; j<8; j++)
                {
                    if(b[i][j]>='A' && b[i][j]<='Z' && b[i][j]!='N')
                    {
                        sr=i;
                        sc=j;
                        for(int x=(sr-1); x<(sr+2); x++)
                        {
                            for(int y=(sc-1); y<(sc+2); y++)
                            {
                                if(x<0 || x>7 || y<0 || y>7)
                                {
                                    continue;
                                }
                                if(x==sr && y==sc)
                                {
                                    continue;
                                }
                                else if(!selfcheck(b,sr,sc,x,y,turn) && !ismypiece(b,x,y,turn) && checklegality(b,sr,sc,x,y,turn))
                                {

                                    if(whichmate=='s')
                                    {
                                        return false;
                                    }
                                    else if(whichmate=='c')
                                    {
                                        for(int ri=(sr-7); ri<(sr+8); ri++)
                                        {
                                            for(int ci=(sc-7); ci<(sc+8); ci++)
                                            {
                                                if(ri<0 || ri>7 || ci<0 || ci>7)
                                                {
                                                    continue;
                                                }
                                                else if(!selfcheck(b,sr,sc,ri,ci,turn) && !ismypiece(b,ri,ci,turn) && checklegality(b,sr,sc,ri,ci,turn))
                                                {
                                                    return false;
                                                }
                                            }
                                        }
                                    }

                                }
                            }
                        }
                    }
                    else if(b[i][j]=='N')
                    {
                        sr=i;
                        sc=j;
                        for(int x=(sr-2); x<(sr+3); x++)
                        {
                            for(int y=(sc-2); y<(sc+3); y++)
                            {
                                if((abs(x-sr)==2 && abs(y-sc)==1) || (abs(x-sr)==1 && abs(y-sc)==2))
                                {
                                    if(!selfcheck(b,sr,sc,x,y,turn) && ! ismypiece(b,x,y,turn))
                                    {
                                        return false;
                                    }
                                }
                            }
                        }


                    }
                }
            }
            return true;
        }
        return false;

    }
    else if(turn==2)
    {
        isincheck=am_i_in_check(b,turn);
        if(isincheck==mate)                            //matlab k check main hay.
        {
            for(int i=0; i<8; i++)
            {
                for(int j=0; j<8; j++)
                {
                    if(b[i][j]>='a' && b[i][j]<='z' && b[i][j]!='n')
                    {
                        sr=i;
                        sc=j;
                        for(int x=(sr-1); x<(sr+2); x++)
                        {
                            for(int y=(sc-1); y<(sc+2); y++)
                            {
                                if(x<0 || x>7 || y<0 || y>8)
                                {
                                    continue;
                                }
                                if(x==sr && y==sc)
                                {
                                    continue;
                                }
                                else if(!selfcheck(b,sr,sc,x,y,turn) && !ismypiece(b,x,y,turn) && checklegality(b,sr,sc,x,y,turn))
                                {
                                    if(whichmate=='s')
                                    {
                                        return false;
                                    }
                                    else if(whichmate=='c')
                                    {
                                        for(int ri=(sr-7); ri<(sr+8); ri++)
                                        {
                                            for(int ci=(sc-7); ci<(sc+8); ci++)
                                            {
                                                if(ri<0 || ri>7 || ci<0 || ci>7)
                                                {
                                                    continue;
                                                }
                                                else if(!selfcheck(b,sr,sc,ri,ci,turn) && !ismypiece(b,ri,ci,turn) && checklegality(b,sr,sc,ri,ci,turn))
                                                {
                                                    return false;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if(b[i][j]=='n')
                    {
                        sr=i;
                        sc=j;
                        for(int x=(sr-2); x<(sr+3); x++)
                        {
                            for(int y=(sc-2); y<(sc+3); y++)
                            {
                                if((abs(x-sr)==2 && abs(y-sc)==1) || (abs(x-sr)==1 && abs(y-sc)==2))
                                {
                                    if(!selfcheck(b,sr,sc,x,y,turn) && ! ismypiece(b,x,y,turn))
                                    {
                                        return false;
                                    }
                                }
                            }
                        }


                    }
                }
            }
            return true;
        }
        return false;
    }
}
bool check(char b[][8],int turn)
{
    SetConsoleTextAttribute(h,10);
    int er,ec;
    if(turn==1)
    {
        for (int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(b[i][j]=='k')
                {
                    er=i;
                    ec=j;
                    break;
                }
            }
        }
        for(int i=0; i<8; i++)
        {
            for (int j=0; j<8; j++)
            {
                if(b[i][j]>='A' && b[i][j]<='Z')
                {
                    if(checklegality(b,i,j,er,ec,turn))
                    {
                        return true;
                    }
                }
            }
        }
    }
    else if(turn==2)
    {
        for (int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(b[i][j]=='K')
                {
                    er=i;
                    ec=j;
                    break;
                }
            }
        }
        for(int i=0; i<8; i++)
        {
            for (int j=0; j<8; j++)
            {
                if(b[i][j]>='a'  && b[i][j]<='z')
                {
                    if(checklegality(b,i,j,er,ec,turn))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
void pawnpromotion(char b[][8],int turn)
{
    char x;
    if(turn==1)
    {
        for(int i=0; i<8; i++)
        {
            if(b[7][i]=='P')
            {
                do
                {
                    gotoRowCol(50,105);
                    cout<<"Enter to which you want to promot:";
                    cin>>x;
                }
                while(!promotedpawnlegality(x,turn));
                gotoRowCol(50,105);
                cout<<"                                                       ";
                if(x=='P')
                {
                    b[7][i]='G';
                }
                else
                    b[7][i]=x;
                printinbox(75,i*10+5,7,i,x,10,10,' ');

            }
        }
    }
    else if(turn==2)
    {
        for(int i=0; i<8; i++)
        {
            if(b[0][i]=='p')
            {
                do
                {
                    gotoRowCol(50,105);
                    cout<<"Enter to which you want to promot:";
                    cin>>x;
                }
                while(!promotedpawnlegality(x,turn));
                gotoRowCol(50,105);
                cout<<"                                   ";
                if (x=='p')
                {
                    b[0][i]='g';
                }
                else
                    b[0][i]=x;
                printinbox(5,i*10+5,0,i,x,10,10,' ');

            }
        }
    }
}
