#include <stdio.h>

typedef int waterKing;

void Find(waterKing *id, int m, int n, waterKing* king){
  waterKing candidate[n];
  int nTimes[n],i,j,flag,flag2,flag3;

  for(i=0;i<n;i++) {nTimes[i]=0;candidate[i]=0;}

  for(i=0;i<m;i++){
    flag=1;
    for(j=0;j<n;j++){
      if(candidate[j]==id[i]){ nTimes[j]++; flag=0; break;}
    }

    if(flag){
      flag2=1;
      for(j=0;j<n;j++){
        if(nTimes[j]==0){ candidate[j]=id[i]; nTimes[j]++; flag2=0; break; }
      }
      if(flag2){
        flag3=1;
        for(j=0;j<n;j++){
          if(candidate[j]==0){ flag3=0; break; }
        }
        if(flag3){
          for(j=0;j<n;j++){
            nTimes[j]--;
            if(nTimes[j]==0) candidate[j]=0;
          }
        }
      }
    }
  }

  for(i=0;i<n;i++) {king[i]=candidate[i];}

}

int main(){
  waterKing id[]={1,2,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5}; //id should be > 0
  waterKing king[3];
  int i;

  Find(id,sizeof(id)/sizeof(waterKing),3,king);
  for(i=0;i<3;i++)
    printf("King%d = %d\n",i+1,king[i]);
  

}
