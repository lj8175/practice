#include <iostream>

using namespace std;

class SpiralMatrix{

public:
  SpiralMatrix(int m, int n){
    arr = new int*[m];
    for(int i=0;i<m;i++){
      arr[i] = new int[n];
    }

    lines = m;
    rows = n;

    for(int i=0; i<lines; i++)
      for(int j=0; j<rows; j++)
        arr[i][j]=0;
  }

  void clockwiseInside(){
    counterclockwise();
    for(int i=0; i<lines; i++)
      for(int j=0; j<rows; j++)
        arr[i][j] = lines*rows - arr[i][j];
  }

  void clockwise(){
    int flines = 0;
    int frows = 0;
    int i = 0;
    int j = 0;
    int sign = 0;
    for(int c=0; c<lines*rows;){
      for(j=0+frows; j<rows-frows; j++)
        arr[i+sign][j] = c++;
      for(i=1+flines; i<lines-flines; i++)
        arr[i][j-1] = c++;
      for(j=j-2; j>=0+frows; j--)
        arr[i-1][j] = c++;
      for(i=i-2; i>=1+flines; i--)
        arr[i][j+1] = c++;
      flines++;
      frows++;
      sign = 1;
    }
  }

  void counterclockwiseInside(){
    clockwise();
    for(int i=0; i<lines; i++)
      for(int j=0; j<rows; j++)
        arr[i][j] = lines*rows - arr[i][j];
  }

  void counterclockwise(){
    int flines = 0;
    int frows  = 0;
    int i      = 0;
    int j      = 0;
    int sign   = 0;
    for(int c=0; c<lines*rows;){
      for(i=0+flines; i<lines-flines; i++)
        arr[i][j+sign] = c++;
      for(j=1+frows; j<rows-frows; j++)
        arr[i-1][j] = c++;
      for(i=i-2; i>=0+flines; i--)
        arr[i][j-1] = c++;
      for(j=j-2; j>=1+frows; j--)
        arr[i+1][j] = c++;
      frows++;
      flines++;
      sign=1;
    }

  }

  void print(){
    for(int i=0; i<lines; i++){
      for(int j=0; j<rows; j++){
        cout<< arr[i][j] << "\t";
      }
      cout << endl;
    }
  }

  ~SpiralMatrix(){
    for(int i=0;i<lines;i++)
      delete [] arr[i];
    delete [] arr;
  }

private:
  int **arr;
  int lines;
  int rows;

};


int main(){
  SpiralMatrix sm(56,22);
  //sm.clockwise();
  sm.clockwiseInside();
  sm.print();
}
