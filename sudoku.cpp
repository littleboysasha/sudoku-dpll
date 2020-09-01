#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#define ROW 9
#define COL 9
#define CORRECT 0
#define WRONG -1 
#define MAXNUM 10   /*数独个数*/ 
#define SEED 20200610 /*随机数生成种子*/ 


int isValidSudoku(int board[9][9]) 
{
    int isGood = 1;
    for (int i = 0; i < 9; i++)
    {
        if (!isGood) break;
        for (int j = 0; j < 9; j++)
        {
            if (!isGood) break;   
            if (board[i][j] == 0) continue;    
            for (int row = i + 1; row < 9; row++)
            {
                if (board[row][j] != 0 && board[row][j] == board[i][j]) isGood = 0;
            }    
            for (int col = j + 1; col < 9; col++)
            {
            	if (board[i][col] != 0 && board[i][col] == board[i][j]) isGood = 0;
            }    
            int r = (i / 3) * 3, c = (j / 3) * 3;          
            for (int row = r; row < r + 3; row++)
            {
                for (int col = c; col < c + 3; col++)
                {
                    if (row != i && col != j && board[row][col] != 0 && board[row][col] == board[i][j]) isGood = 0;
                }
            }
        }
    }    
    return isGood;
}


unsigned int seed = SEED;
int Digit(char a[][COL], int i, int j){	/*递归填充数独元素*/
	if(i < ROW && j < COL){	
		int x,y,k;
		char check[COL+1]={CORRECT};/*用于排除已经使用过的的数字*/
		for(x = 0 ; x < i ; x++)
			check[a[x][j]] = WRONG;
		for(x = 0 ; x < j ; x++)
			check[a[i][x]] = WRONG;
		for(x = i/3*3 ; x <= i; x++){
			if(x == i)
				for(y = j/3*3 ; y < j; y++)
					check[a[x][y]] = WRONG;
			else
				for(y = j/3*3 ; y < j/3*3 + 3; y++)
					check[a[x][y]] = WRONG;		
		}
		int flag = 0;
		for(k = 1; k <= COL && flag == 0 ; k++){/*从check数组中查找安全的数字*/ 
			if(check[k] == CORRECT){
				flag = 1;
				a[i][j] = k;
				if(j == COL-1 && i != ROW-1){
					if(Digit(a,i+1,0) == CORRECT) return CORRECT;
					else flag = 0;
				} 	
				else if(j != COL-1){
					if(Digit(a,i,j+1) == CORRECT) return CORRECT;
					else flag = 0;	
				}  
			}	 	
		}
		if( flag == 0 ){
			a[i][j] = 0;
			return WRONG;
		}	 	
	}
	return CORRECT;			
}
void randomFirstRow(char a0[], int n){/*随机生成第一行*/
	int i,j;
	srand(seed++);
	for( i = 0 ; i < n ; i++){
		a0[i] = rand()%9 + 1;
		j = 0 ; 
		while(j < i){
			if(a0[i] == a0[j]){
				a0[i] = rand()%9 + 1;
				j = 0;	
			}
			else j++;
		}				
	}
}
void createSudoku(char a[][COL]){	/*生成数独*/
	randomFirstRow(a[0],COL);
	Digit(a,1,0);	
}
void createStartinggrid(const char a[][COL], char b[][COL], int numDigits){/*随机生成初盘*/
	int i,j,k;
	srand(seed ++);
	for( i = 0; i < ROW ; i ++)
		for( j = 0; j < COL ; j++)
			b[i][j] = a[i][j];
	
	char c[numDigits][2];
	int m,flag = 0;
	
	for( i = 0; i < numDigits ; i++){
		j = rand()%9;
		k = rand()%9;
		
		flag = 0;
		for(m = 0; m < i ; m++)
			if( j == c[m][0] && k == c[m][1])
				flag = 1;		
		
		if(flag == 0){
			b[j][k] = 0;
			c[i][0] = j;
			c[i][1] = k;
		}
		else
			i--;	
	}
}
int checkSudoku(const char a[][COL]){/*检查函数*/
	int i,j,x,y;
	for( i = 0; i < ROW ; i++){
		for(j = 0 ; j < COL ; j++){		
			for(x = 0 ; x < COL ; x++)
				if(a[i][x] == a[i][j] && x != j ) 
					return WRONG;
			
			for(x = 0 ; x < ROW ; x++)
				if(a[x][j] == a[i][j] && x != i ) 
					return WRONG;
		
			for(x = i/3*3 ; x < i/3*3+3 ; x ++)
				for(y = j/3*3 ; y < j/3*3+3 ; y++)
			 		if(a[x][y] == a[i][j] && (x != i && y != j)) 
					 	return WRONG;	 	
		}	
	}
	return CORRECT;
}

void printToFile(const char a[][COL], const char filename[]){/*打印数独数组到文件*/
	FILE * fp;
	int i,j;
	fp = fopen(filename,"w");
	for( i = 0 ; i < ROW ; i++){
		for( j = 0 ; j < COL ; j++)
			fprintf(fp , "%d ", a[i][j]);
		fprintf(fp , "\n");
	}
	fclose(fp);	
}
void createSudokuToFile(void){/*生成数独初盘到文件中*/
	char sudoku[ROW][COL]={0};
	char starting_grid[ROW][COL]={0};
	char filename[50];
	int i;
//	mkdir("sudoku");
	for( i = 0; i < MAXNUM ; i++){
		createSudoku(sudoku);
		createStartinggrid(sudoku,starting_grid,i%81+1);
		
		sprintf(filename,"%04d.txt",i);
		printToFile(starting_grid,filename);
	}
}


using namespace std;
enum STATUS{sat,  unsat,  ok,  finish};
struct Formula {
  vector<int> lit;
  vector<int> freqs; 
  vector<int> adder;
  vector<int> pos_or_neg;
  vector<vector<int> > clauses;
  Formula() = default;
};
struct Process {
  Formula f;
  int dpll_time;             
  int simplify1(Formula &);
  int DPLL(Formula);    
  int simplify2(Formula &, int); 
  int pure_literal(Formula &); 
  void print_res(Formula &, int); 
  void readfile();
  void inputdatas(); 
  void proc(int &);
  void sudo_file(Formula &, int);
  void sudo_readfile();
  int sudo_DPLL(Formula);
  void sudo_proc();
  int fetch_one(Formula);
  Process() = default;
};

int Process::pure_literal(Formula &f)
{
	vector<int> tmp1;
	for(int i = 0; i < f.pos_or_neg.size(); i++)
	{
		if(f.pos_or_neg[i] == f.adder[2 * i])
			tmp1.push_back(2 * i);
		else if(f.pos_or_neg[i] == f.adder[2 * i + 1])
			tmp1.push_back(2 * i + 1);	
	}
	int flag = true;
	for(int i = 0; i < f.clauses.size(); i++)
	{
		flag = false;
		for(int j = 0; j < f.clauses[i].size(); j++)
		{
			for(int k = 0; k < tmp1.size(); k++)
			{
				if(tmp1[k] == f.clauses[i][j])
				{
					f.clauses.erase(f.clauses.begin() + i);
					i--;
			        if (f.clauses.size() == 0)
			        {
          				return STATUS::sat;
       				}
       				flag = true;
       				break;
				}
			}
		if(flag)
			break;
		}
	}
	return STATUS::ok;
}


int Process::fetch_one(Formula f)
{
	
	int max = f.freqs[0];
	int m;
	for(int i = 0 ; i < f.freqs.size(); i++)
	{
		if ( f.freqs[i] >= max )
		{
			max  = f.freqs[i];
			m = i;	
		}
	}
	return m; 
}

struct Sudoku {
  	Process p;
	int sudoku[9][9];
	int result[9][9]; 
	int lit1[9][9][9];
	int num;
	vector<vector<int> > su_clauses;
	bool jud_cft(int ,int ,int ,int);
	void init();
	void save_sudo(int i);
	bool proc1();
	void print_sudo();
	void use_fast();
	Sudoku() = default;
};

void Sudoku::use_fast()
{
	system("./minisat sudoku.cnf result.txt");
}

bool Sudoku::proc1()
{
	ifstream ifs1("result.txt");
	string res;
	ifs1 >> res;
	if(res == "unsat" || res == "UNSAT")
	{
		cout << "there is no solution"<<endl;
	}
	else if(res == "sat" || res == "SAT")
	{
		cout << "there is a solution"<<endl;
	    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                int v; ifs1 >> v;
                if (v > 0) result[i][j] = k + 1;
            }
        }
    }	
	}
	ifs1.close();	
}

void Sudoku::print_sudo()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) 
            printf("%d ", result[i][j]);
        puts("");
    }	
}


void Sudoku::save_sudo(int i)
{
//	int n;
//	cout << "you want to input sudoku or read sudoku from file?  1 or 2"<<endl;
//	cin >> n;
	if(i == 1)
	{
		cout << "input the sudoku by urself plz :)"<<endl;
		for (int i = 0; i < 9; i++) {
        	for (int j = 0; j < 9; j++) {
            	cin >> sudoku[i][j];
            	if (sudoku[i][j] > 0)
                	su_clauses.push_back({lit1[i][j][sudoku[i][j]-1]});
       		    }
    	}	
    	int res = isValidSudoku(sudoku);
    	if(res == 0) {
    		cout << "Invalid sudoku" << endl;
    		exit(0); 
		} else if(res == 1){
			cout << "It's a valid sudoku" << endl;
		}
	}
	else if(i == 2)
	{
		cout << "input the sudoku filename:"<<endl;
		string filename;
		cin >> filename;
		ifstream ifs(filename);
		cout << "here is the sudoku:" << endl;
    	for (int i = 0; i < 9; i++) {
        	for (int j = 0; j < 9; j++) {
            	ifs >> sudoku[i][j];
            	cout << sudoku[i][j] <<" ";
            	if (sudoku[i][j] > 0)
                	su_clauses.push_back({lit1[i][j][sudoku[i][j]-1]});
       	    }
       		cout << endl;
    	}
		ifs.close();	
	}
   FILE *fp = fopen("sudoku.cnf","w");
   fprintf(fp ,"p cnf %d %d\n", num, (int) su_clauses.size());
     for (auto& clause : su_clauses) {
        for (int m : clause) 
            fprintf(fp ,"%d ", m);
        fprintf(fp ,"0\n");
    }   	
	fclose(fp);
}

bool Sudoku::jud_cft(int i,int j,int k,int l)
{
    if (i == k or j == l) return true;
    if (i / 3 == k / 3 && j / 3 == l / 3) return true;
    return false;	
}

void Sudoku::init()
{
	num = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++) 
                lit1[i][j][k] = ++num;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            vector<int> clause;
            for (int k = 0; k < 9; k++) 
                clause.push_back(lit1[i][j][k]);
            su_clauses.push_back(clause);
        }
    }
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                for (int l = 0; l < 9; l++) {
                    if (i == k && j == l) continue;
                    if (jud_cft(i, j, k, l)) 
                        for (int a = 0; a < 9; a++) 
                            su_clauses.push_back({-lit1[i][j][a], -lit1[k][l][a]});
                }
            }
        }
    }	
}


void Process::sudo_readfile()
{
  ifstream infile("sudoku.cnf");
  char c;  
  string str;
  int lit_num, clause_num, tmp;      
  while (true) 
  {
    infile >> c;
    if (c == 'c') 
    {
      getline(infile, str);
    } 
	else         
    {
      infile >> str;
      break;
    }
  }
  infile >> lit_num;
  infile >> clause_num;
  f.lit.resize(lit_num, -1);
  f.clauses.resize(clause_num);
  f.freqs.resize(lit_num, 0);
  f.pos_or_neg.resize(lit_num, 0);
  for (int i = 0; i < clause_num; i++) {
    while (true)
    {
      infile >> tmp;
      if (tmp > 0)
      {
        f.clauses[i].push_back(2 * (tmp - 1));
        f.freqs[tmp - 1]++;
        f.pos_or_neg[tmp - 1]++;
      }
	  else if (tmp < 0) 
      {
        f.clauses[i].push_back(2 * ((-1) * tmp - 1) + 1); 
        f.freqs[-1 - tmp]++;
        f.pos_or_neg[-1 - tmp]--;
      } 
	  else 
	  {
       break;
      }
    }
  }	
  infile.close();	
}


void Process::sudo_file(Formula &f, int result)
{
  ofstream ofs("result.txt", ios::trunc|ios::out|ios::in);
  if (result == STATUS::sat)
  {
    ofs << "sat ";
    for (int i = 0; i < f.lit.size(); i++)
    {
      if (i != 0)
	  {
        ofs << " ";
      }
      if (f.lit[i] != -1) 
	  {
      	if(f.lit[i] % 2 == 0)
      		ofs<<(i + 1);
      	else 
		  	ofs<<-(i + 1);	
      }
	  else 
      {
        ofs << (i + 1);
      }
    }
  } 
  else
  {
    ofs << "unsat";
  }	
  ofs.close();
}

void Process::sudo_proc()
{
  int result = sudo_DPLL(f);
  if (result == STATUS::ok)
  {
    sudo_file(f, STATUS::unsat); 
  }	
}

void Process::readfile()
{
  string filename;
  cout << "please input the filename:"<<endl;
  cin >> filename;
  ifstream infile(filename);
  char c;  
  string str;
  int lit_num, clause_num, tmp;      
  while (true) 
  {
    infile >> c;
    if (c == 'c') 
    {
      getline(infile, str);
    } 
	else         
    {
      infile >> str;
      break;
    }
  }
  infile >> lit_num;
  infile >> clause_num;
  f.lit.resize(lit_num, -1);
  f.clauses.resize(clause_num);
  f.freqs.resize(lit_num, 0);
  f.pos_or_neg.resize(lit_num, 0);
  for (int i = 0; i < clause_num; i++) {
    while (true)
    {
      infile >> tmp;
      if (tmp > 0)
      {
        f.clauses[i].push_back(2 * (tmp - 1));
        f.freqs[tmp - 1]++;
        f.pos_or_neg[tmp - 1]++;
      }
	  else if (tmp < 0) 
      {
        f.clauses[i].push_back(2 * ((-1) * tmp - 1) + 1); 
        f.freqs[-1 - tmp]++;
        f.pos_or_neg[-1 - tmp]--;
      } 
	  else 
	  {
       break;
      }
    }
  }	
  infile.close();
}

void Process::inputdatas() {
  char c;  
  string str;
  int lit_num, clause_num, tmp;      
  while (true)
  {
  cin >> c;
  if (c == 'c') 
  {
    getline(cin, str);
  }
  else         
  {
    cin >> str;
    break;
  }
  }
  cin >> lit_num;
  cin >> clause_num;
  f.lit.resize(lit_num, -1);
  f.clauses.resize(clause_num);
  f.freqs.resize(lit_num, 0);
  f.pos_or_neg.resize(lit_num, 0);
  for (int i = 0; i < clause_num; i++) {
  while (true)
  {
    cin >> tmp;
    if (tmp > 0)
    {
      f.clauses[i].push_back(2 * (tmp - 1));
      f.freqs[tmp - 1]++;
      f.pos_or_neg[tmp - 1]++;
    }
    else if (tmp < 0) 
    {
      f.clauses[i].push_back(2 * ((-1) * tmp - 1) + 1); 
      f.freqs[-1 - tmp]++;
      f.pos_or_neg[-1 - tmp]--;
    } 
	else
    {
      break;
    }
    }
  }
}


int Process::simplify1(Formula &f) {
  bool flag = false;
  if (f.clauses.size() == 0)
  {
    return STATUS::sat; 
  }
  do {
    flag = false;
    for (int i = 0; i < f.clauses.size(); i++) {
      if (f.clauses[i].size() == 1) 
      {
        flag = true;
        f.lit[f.clauses[i][0] / 2] = f.clauses[i][0] % 2; 
        f.freqs[f.clauses[i][0] / 2] = -1; 
        int result = simplify2(f, f.clauses[i][0] / 2);
        if (result == STATUS::sat || result == STATUS::unsat)
	    {
          return result;
        }
        break; 
      } 
	  else if(f.clauses[i].size() == 0) 
      {
        return STATUS::unsat;
      }
    }
  } while (flag);
  return STATUS::ok;
}

int Process::simplify2(Formula &f, int literal_to_apply) {
  int value_to_apply = f.lit[literal_to_apply];
  for (int i = 0; i < f.clauses.size(); i++)
  {
    for (int j = 0; j < f.clauses[i].size(); j++) 
	{
      if ((2 * literal_to_apply + value_to_apply) == f.clauses[i][j]) 
	  {
        f.clauses.erase(f.clauses.begin() + i); 
        i--;                
        if (f.clauses.size() == 0)
        {
          return STATUS::sat;
        }
        break;
      } 
	  else if (f.clauses[i][j] / 2 == literal_to_apply)
      {
        f.clauses[i].erase(f.clauses[i].begin() + j); 
        j--;  
        if (f.clauses[i].size() == 0)
        {
          return STATUS::unsat;
        }
        break; 
      }
    }
  }
  return STATUS::ok;
}

int Process::DPLL(Formula f) {
  int res1 = pure_literal(f);
  if(res1 == STATUS::sat)
  {
  	print_res(f, res1);
  	return STATUS::finish;
  }
  int result = simplify1(f);
  if (result == STATUS::sat)
  {
    print_res(f, result);
    return STATUS::finish;
  }
  else if (result == STATUS::unsat)
  {
    return STATUS::ok;
  }
  int i = fetch_one(f);
  //int i = distance(f.freqs.begin(), max_element(f.freqs.begin(), f.freqs.end()));
  for (int j = 0; j < 2; j++) 
  {
    Formula for1 = f; 
    if (for1.pos_or_neg[i] > 0)
    {
      for1.lit[i] = j; 
    } 
	else              
    {
      for1.lit[i] = (j + 1) % 2;
    }
    for1.freqs[i] = -1; 
    int res1 = simplify2(for1, i); 
    if (res1 == STATUS::sat) 
    {
      print_res(for1, res1);
      return STATUS::finish;
    } 
	else if (res1 == STATUS::unsat)
    {
      continue;
    }
    int res2 = DPLL(for1);
    if (res2 == STATUS::finish)
    {
      return res2;
    }
  }
  return STATUS::ok;
}

int Process::sudo_DPLL(Formula f){
  int result = simplify1(f);
  if (result == STATUS::sat)
  {
    sudo_file(f, result);
    return STATUS::finish;
  }
  else if (result == STATUS::unsat)
  {
    return STATUS::ok;
  }
  int i = distance(f.freqs.begin(), max_element(f.freqs.begin(), f.freqs.end()));
  for (int j = 0; j < 2; j++) 
  {
    Formula for1 = f; 
    if (for1.pos_or_neg[i] > 0)
    {
      for1.lit[i] = j; 
    } 
	else              
    {
      for1.lit[i] = (j + 1) % 2;
    }
    for1.freqs[i] = -1; 
    int res1 = simplify2(for1, i); 
    if (res1 == STATUS::sat) 
    {
      sudo_file(for1, res1);
      return STATUS::finish;
    } 
	else if (res1 == STATUS::unsat)
    {
      continue;
    }
    int res2 = sudo_DPLL(for1);
    if (res2 == STATUS::finish)
    {
      return res2;
    }
  }
  return STATUS::ok;	
}

void Process::print_res(Formula &f, int result) {
  if (result == STATUS::sat)
  {
    cout << "s 1" << endl;
    cout<<"v ";
    for (int i = 0; i < f.lit.size(); i++)
    {
      if (i != 0)
	  {
        cout << " ";
      }
      if (f.lit[i] != -1) 
	  {
      	if(f.lit[i] % 2 == 0)
      		cout<<(i + 1);
      	else 
		  	cout<<-(i + 1);	
      }
	  else 
      {
        cout << (i + 1);
      }
    }
  } 
  else
  {
    cout << "unsat result : 0";
  }
}

void Process::proc(int &t) {
  clock_t start_time=clock();
  int result = DPLL(f);
  clock_t end_time=clock();
  t = static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000;
  if (result == STATUS::ok)
  {
    print_res(f, STATUS::unsat); 
  }
}

int main() 
{
  cout << "Finished by steveclapx on 2020-06-10" << endl;
  int i1;
  cout<<"you want to have a automatically generated sudoku(1) or input the sudoku by urself(2) ? choose  1 or 2  :)"<<endl;
  cin >> i1;	   
  if(i1 == 1)
  {
  	createSudokuToFile();
    cout << "just generated 10 random valid unsolved sudoku, please choose one of them, from 0000.txt to 0009.txt :)" << endl;
    int time;
  	Sudoku s;
  	s.init();
  	s.save_sudo(2);
    clock_t start_time=clock();	
  	s.p.sudo_readfile();
  	s.p.sudo_proc();
  	s.proc1();
  	s.print_sudo();
  	clock_t end_time=clock();
  	time = static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000;
  	cout << "time used : " << time << "ms" << endl;
  }
  else if(i1 == 2)
  {
    int time;
  	Sudoku s;
  	s.init();
  	s.save_sudo(1);
    clock_t start_time=clock();	
  	s.p.sudo_readfile();
  	s.p.sudo_proc();
  	s.proc1();
  	s.print_sudo();
  	clock_t end_time=clock();
  	time = static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000;
  	cout << "time used : " << time << "ms" << endl;
  }
  else
  	cout<<"you can only input 1 or 2"<<endl;	
  cout << endl;	
  cout<<"按任意键退出...";
  cin.clear();
  cin.sync();
  cin.get();
  return 0;
}
