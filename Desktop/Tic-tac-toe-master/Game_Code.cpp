#include <iostream>
#include <bitset>
#include <algorithm>
#include <set>
#include <vector>
const std::string red("\033[0;31m");
const std::string green("\033[1;32m");
const std::string cyan("\033[0;36m");
const std::string magenta("\033[0;35m");
const std::string white("\033[0;37m");
const std::string reset("\033[0m");
const std::string bold("\033[1m");
const std::string on_blue("\033[44m");
const std::string blink ("\033[5m");
using namespace std;


/*
  Change of GAME RULE to standard Tic-Tac-Toe
  Our game follows standard rules of Tic-Tac-Toe at the start.
  The objective is to get three in a row. You play on a three by three game board.
  The first player is known as X and the second is O. Players alternate placing Xs and Os   on the game board until either oppent has 
  three in a row or all nine squares are filled.
  
  If all 9 squares are filled and there are no 3s in a row, it leads to a DRAW else the winner is whoever gets the 3 in a row.
  
  Being a 0-sum game, the outcome will always be a draw if both players play optimally. So we have introduced a tie-breaker that can lead to 
  the first player's victory if he/she plays optimally.

  Winner is decided using a tie-breaker.The rules are:
  1.Each player plays alternatively.
  2.At each turn, a player can choose a sub-matrix/sub-rectangle consisting of CROSSES(X) only and turn it to NOUGHTS(O).
  3.The person who cannot make any move loses.
  4.The game starts at the board obtained from the draw game.
  5.You cannot select a sub-matrix that has a NOUGHTS(O) in it.
  6.If you want to play manually, enter indices of the submatrix with CROSSES ONLY that you want to choose.
  
  Proof:

  In a normal game of tic-tac-toe, if the first player always plays optimally, he will either draw or win depending upon whether the 
  second player plays optimally or not respectively.
  
  So first we prove that a draw state can lead to a win.

  For that, we defined states on the board. A board is denoted by a 9-bit binary number where a CROSS denotes a 1 and NOUGHT denotes 0.
  Since it is a DRAW board, all bits will be either a X or a 0 and no empty cells will be left.

  A state is a winning state if there exists atleast 1 losing state that you can go to from your current state.
  A state is a losing state if all states that you can go to from your current state are winning states.
  
  For a board with all NOUGHTS(O), the state is 000000000 or 0 and is a losing state as we cannot select any valid sub-matrix.
  For a board with all CROSSES(X), the state is 111111111 or 511 . Whether this is winning or losing can be calculated recursively.

  There are total 3 DRAW patterns possible and they can reflected and rotated to give all draw states.
  
  oxo   oxx  oxx
  xxo   xoo  xoo
  xox   oxx  xox
  (1)   (2)  (3)
  
  We computed the above states and found out that (1) is a losing state and the rest are winning.
  Since it will be NOUGHT's(O) turn first after the DRAW as they play alternatively, if the DRAW state is (1) then O will lose and X will win as 
  it was a losing state for O.

  So X can win before DRAW if O plays unoptimally or reaches a DRAW and wins it if the DRAW state is (1).
Xa  
  Since X goes first, X can always play his first move at the center of the board. If he draws after that, he will always reach (1) as a DRAW state is 
  either (1), (2) or (3) and if there is X at the center it cannot be (2) or (3).
  Hence, if its a draw with X at the center, it will always lead to (1).
  
  We now just need to prove that X can Win or lead to a draw(1) after playing X at the center first.

  This can be proven analytically.

  The same move sequence can be rotated and reflected for all cases.
  I will be showing for only one.
  
  Case A: O plays at edge center

  ___       _o_       xo_       xo_            xo_
  _x_  __\  _x_  __\  _x_ __\   _x_       __\  _x_
  ___    /  ___    /  ___   /   __o         /  x_o
    
                    X plays     O makes        X creates a fork that guarantees a win for X 
                    next to O   forced move


  Case B: O plays at Corner 
  o__       o__       o_x       o_x            o_x          o_x             o_x
  _x_  __\  _x_  __\  _x_ __\   _x_       __\  xx_    ___\  xxo      ____\  xxo
  ___    /  ___    /  ___   /   o__         /  o__       /  o__          /  o__
    
                    X plays     O makes      X makes        O makes         x has 2 turns and o only has 1 
                    a corner   forced move   forced move    forced move     and o cannot create a row with just 1 more o 
                    next to O                                               so this leads to X's win or a draw(1)

  Hence, it is proven that if X plays at the center and plays optimally, he can always WIN or lead to a DRAW(1). And if its a draw, 
  (1) is a losing state and it is NOUGHT's(O) so X wins from the draw if he plays optimally.
*/

int grundy[1<<9]={-1};

int mex(set<int> &s){                 //finds minimum whole number that is not present in the set
    int i=0;
    
    while(s.find(i)!=s.end()) i++;
    return i;
}
 
int check(int state,int i,int j,int k, int l){      //if the submatrix has a NOUGHT returns -1
    bitset<9> b(state);                              //else returns the next state that would be reached if all the CROSSES are reduced to NOUGHTS
    for(int x=i;x<=j;x++){                            //bitset is used to store the binary representation of the number
        for(int y=k;y<=l;y++){
            int pos=3*y+x;
            if(b.test(pos)==0) return -1;
            b.reset(pos);
        }
    }
    int kk=b.to_ulong();
    return kk;
}
 
int getGrundy(int state){                             //if grundy[state]==0, it is a losing state else it is a winning state
    if(grundy[state]!=-1) return grundy[state];
    set<int> s;
    int i,j,k,l,tmp;
    for(i=0;i<3;i++){
        for(j=i;j<3;j++){
            for(k=0;k<3;k++){
                for(l=k;l<3;l++){                       //iterating through all possible submatrices
                    tmp=check(state,i,j,k,l);           // check() is used to check if the sub-matrix consists of all CROSSES
                    if(tmp!=-1){
                        grundy[tmp]=getGrundy(tmp);      //recursively calulates if the sub-state that we reached is winning or losing
                        s.insert(grundy[tmp]);
                    }
                }
            }
        }
    }                                                   //s stores the the grundy[state] of all states that we can go to
    return mex(s);
}



struct Move
{
    int row, col;
}; 
 
char player = 'X', opponent = 'O';
 
bool isMovesLeft(char board[3][3])
{
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (board[i][j]=='_')
                return true;
    return false;
}
bool drawML(char board[3][3])
{
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (board[i][j]=='X')
                return true;
    return false;
}
 
int evaluate(char b[3][3])
{
    for (int row = 0; row<3; row++)
    {
        if (b[row][0]==b[row][1] &&
            b[row][1]==b[row][2])
        {
            if (b[row][0]==player)
                return +10;
            else if (b[row][0]==opponent)
                return -10;
        }
    }
    for (int col = 0; col<3; col++)
    {
        if (b[0][col]==b[1][col] &&
            b[1][col]==b[2][col])
        {
            if (b[0][col]==player)
                return +10;
 
            else if (b[0][col]==opponent)
                return -10;
        }
    }
 
    if (b[0][0]==b[1][1] && b[1][1]==b[2][2])
    {
        if (b[0][0]==player)
            return +10;
        else if (b[0][0]==opponent)
            return -10;
    }
 
    if (b[0][2]==b[1][1] && b[1][1]==b[2][0])
    {
        if (b[0][2]==player)
            return +10;
        else if (b[0][2]==opponent)
            return -10;
    }
 
    return 0;
}
 
int minimax(char board[3][3], int depth, bool isMax)
{ 
    int score = evaluate(board);
    if (score == 10)
        return score;
    if (score == -10)
        return score;
    if (isMovesLeft(board)==false)
        return 0;
    if (isMax)
    {
        int best = -1000;
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                if (board[i][j]=='_')
                {
                    board[i][j] = player;
 
                    best = max( best,
                        minimax(board, depth+1, !isMax) );
                    board[i][j] = '_';
                }
            }
        }
        return best;
    }
    else
    {
        int best = 1000;
 
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                if (board[i][j]=='_')
                {
                    board[i][j] = opponent;
                    best = min(best,
                           minimax(board, depth+1, !isMax));
                    board[i][j] = '_';
                }
            }
        }
        return best;
    }
}
   
Move findBestMove(char board[3][3])
{
    int bestVal = -1000;
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;
     

    for (int i = 0; i<3; i++)
    {
        for (int j = 0; j<3; j++)
        {

            if (board[i][j]=='_')
            {
               
                board[i][j] = player;
                

                int moveVal = minimax(board, 0, false); 
                if (moveVal > bestVal)
                {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
                if(evaluate(board)==10){
                 
                  moveVal=99999;
                } 

                if (moveVal > bestVal)
                {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
                board[i][j]=opponent;
                if(evaluate(board)==-10){
                  moveVal=1000;
                } 

                board[i][j] = '_';
                if (moveVal > bestVal)
                {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }


            }
        }
    }
    return bestMove;
}
void pind(){
  cout<<white<<bold<<"Indexing of board cells are:\n"<<reset;
  for(int i=1;i<=9;i++){
    cout<<white<<"|"<<i;
    if(i%3==0) cout<<"|\n";
  }
  cout<<reset<<endl;
}


void print(char board[3][3]){
  
  cout<<cyan<<bold<<"Current Board is this:\n"<<reset;
  for(int i=0;i<3;i++){
          for(int j=0;j<3;j++) {
            cout<<cyan<<"|";
            cout <<bold;
            if (board[i][j]=='_') cout <<cyan<<board[i][j];
            else if (board[i][j]=='X')cout <<red<<board[i][j];
            else cout <<green<<board[i][j];
            cout <<reset;
          }
          cout<<cyan<<"|\n";
  }
  cout <<reset;
}

int main()
{
  	cout <<white;
    cout <<endl;
    cout<<"\t--------------------------------------------------------------------------------------------------------------------------\n";
    cout <<bold<<"\t\t\t\t\t\t\t\tTIC-TAC-TOE\n"<<reset;
    cout <<white;
    cout<<"\t--------------------------------------------------------------------------------------------------------------------------\n";
    cout <<reset;
    cout <<endl;
    
    char board[3][3] =
    {
        { '_', '_', '_' },
        { '_', '_', '_' },
        { '_', '_', '_' }
    };
    print(board);
    int f=1;
    memset(grundy,-1,sizeof(grundy));
    grundy[0]=0;
    while(isMovesLeft(board)){
        pind();
        cout<<white<<"--------------------------------------\nSelect the index you want to play your turn or press 0 to play optimal move\n";
        if (player=='X')
          cout <<red;
        else
          cout <<green;
        cout<<blink<<"################# "<<player<<"'s turn #################\n"<<reset;
        int ch;
        cin>>ch;
        ch--;
        if((ch!=-1&&board[ch/3][ch%3]!='_')||ch<-1||ch>=9){
          cout <<magenta;
          cout<<"################################\n#       ENTER VALID MOVE       #\n################################\n";
          cout <<reset;
          print(board);
          continue;
        }
        if(ch==-1){
          Move bestMove;
          bestMove = findBestMove(board);
          board[bestMove.row][bestMove.col]=player;
          int chk;
          chk=evaluate(board)==10;
          board[bestMove.row][bestMove.col]=opponent;
          chk|=evaluate(board)==-10;

          board[bestMove.row][bestMove.col]='_';

          if(!chk&&f&&board[1][1]=='_') {f=0;bestMove.row=bestMove.col=1;}
          board[bestMove.row][bestMove.col]=player;
        }
        else{
          board[ch/3][ch%3]=player;
        }

        if(evaluate(board)==10) {print(board);
          if (player=='X')
            cout <<red;
          else
            cout <<green;
          cout<<blink<<"##################################\n#       "<<player<<" HAS WON THE GAME       #\n##################################\n"<<reset;return 0;}

         char c=player;player=opponent;opponent=c;
        print(board);
    }
    cout <<white<<on_blue;
    cout <<endl;
    cout<<"\t--------------------------------------------------------------------------------------------------------------------------\n";
    cout <<bold<<"\t\t\t\t\t\t\t\tINSTRUCTIONS\n"<<reset;
    cout <<white<<on_blue;
    cout<<"\t--------------------------------------------------------------------------------------------------------------------------\n";
    cout<<"\tIts a DRAW. \n\tWinner is decided using a tie-breaker.The rules are:\n";
    cout<<"\t1.Each player plays alternatively\n";
    cout<<"\t2.At each turn, a player can choose a sub-matrix/sub-rectangle consisting of CROSSES(X) only and turn it to NOUGHTS(O)\n";
    cout<<"\t3.The person who cannot make any move loses.\n";
    cout<<"\t4.The game starts at the board obtained from the draw game.\n";
    cout<<"\t5.You cannot select a sub-matrix that has a NOUGHT(O) in it.\n";
    cout<<"\t6.If you want to play manually, enter indices of the submatrix with CROSSES ONLY that you want to choose\n";
    cout<<"\t--------------------------------------------------------------------------------------------------------------------------\n";
    cout <<reset;
    cout <<endl;
    print(board);
    int mask=0,pptmp=1;
    int pp[3][3];
    for(int i=0;i<3;i++){
      for(int j=0;j<3;j++){
        if(board[i][j]=='X') mask+=pptmp;
        pp[i][j]=pptmp;
        pptmp<<=1; 
      }
    }
    while(drawML(board)){
        pind();
        cout <<white;
        cout<<"--------------------------------------\nEnter 1 to play manually or 0 to play optimal move\n";
        cout <<reset;
        if (player=='X')
          cout <<red;
        else
          cout <<green;
        cout <<blink;
        cout<<"################# "<<player<<"'s turn #################\n";
        cout <<reset;
        int ch;
        cin>>ch;
        if(ch==1){
          cout <<white<<bold;
          cout<<"Enter the no. cells in the submatrix that you want to choose\n";
          int num;
          cin>>num;
          cout<<"Enter the indices of the cells of the sub-matrix that you want to select\n";
          cout<<reset;
          int arr[num];
          for(int numitr=0;numitr<num;numitr++) cin>>arr[numitr],arr[numitr]--;
          if(num>2||num<1){
            cout <<magenta;
            cout<<"######################################\n#       NOT A VALID SUBMATRIX        #\n######################################\n";
            cout<<reset;
            print(board);
            continue;
          }
          if(num==1&&board[arr[0]/3][arr[0]%3]=='O'){
            cout <<magenta;
            cout<<"######################################\n#       NOT A VALID SUBMATRIX        #\n######################################\n";
            cout<<reset;
            print(board);
            continue;
          }

          if(num==2&&arr[0]==arr[1]){
            cout <<magenta;
            cout<<"##################################################################\n#       DO I CONSIDER THIS AS ONE CELL?? PLEASE ENTER AGAIN      #\n##################################################################\n";
            cout <<reset;
            print(board);
            continue;
          }
          if(num==2&&(board[arr[0]/3][arr[0]%3]=='O'||board[arr[1]/3][arr[1]%3]=='O')){
            cout <<magenta;
            cout<<"######################################\n#       NOT A VALID SUBMATRIX        #\n######################################\n";
            cout <<reset;
            print(board);
            continue;
          }
          if(num==2&&!(abs(arr[0]-arr[1])==3||abs(arr[0]-arr[1])==1)){
            cout <<magenta;
            cout<<"######################################\n#       NOT A VALID SUBMATRIX        #\n######################################\n";
            cout <<reset;
            print(board);
            continue;
          }
          for(int numitr=0;numitr<num;numitr++) board[arr[numitr]/3][arr[numitr]%3]='O',mask-=pp[arr[numitr]/3][arr[numitr]%3];

        }
        else if(ch==0){
          int i,j,k,l,tmp,fff=1,latest;
          for(i=0;i<3;i++){
              for(j=i;j<3;j++){
                  for(k=0;k<3;k++){
                      for(l=k;l<3;l++){
                          tmp=check(mask,i,j,k,l);
                          if(tmp!=-1){
                              latest=tmp;
                              grundy[tmp]=getGrundy(tmp);
                              if(grundy[tmp]==0) mask=tmp,fff=0;
                          }
                      }
                  }
              }
          } 
          if(!fff){
            for(int i=0;i<3;i++){
              for(int j=0;j<3;j++){
                if((mask>>(3*i+j))&1) board[i][j]='X';
                else board[i][j]='O';

              }
            }
          }
          else{
            for(int i=0;i<3;i++){
              for(int j=0;j<3;j++){
                if((latest>>(3*i+j))&1) board[i][j]='X';
                else board[i][j]='O';

              }
            }
            mask=latest;
          }
        }
        else{
          cout <<magenta;
          cout<<"#################################\n#       ENTER 1 or 0   \t\t#\n#################################\n";
          cout <<reset;
          print(board);
          continue;
        } 
        if(mask==0) 
        {
          if (player=='X')
            cout <<red;
          else
            cout <<green;
        cout<<blink<<"##################################\n#       "<<player<<" HAS WON THE GAME       #\n##################################\n"<<reset;}
      
        char c=player;player=opponent;opponent=c;
        print(board);
    }
    return 0;
}