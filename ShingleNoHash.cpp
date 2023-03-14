#include<iostream>
#include<fstream>
using namespace std;

struct shingle
{
    //create shingle containing 3 words
    string s[3];
};

//doubles the size of the shingle array
shingle* dble(shingle * &sh,int size)
{
    shingle *sh1=NULL;
    sh1=new shingle[size*2];
    for(int i=0;i<size;i++)
    {
        sh1[i].s[0]=sh[i].s[0];
        sh1[i].s[1]=sh[i].s[1];
        sh1[i].s[2]=sh[i].s[2];
    }
    return sh1;
}
//create shingle and returns the number of shingle
int CreateShingle(string filename,shingle  * &sh)
{
    fstream fp;
    fp.open(filename.c_str(),ios::in);
    int i=0;
    if (sh==NULL)
    {
        sh=new shingle;
    }
    int size=1;
    while(!fp.eof())
    {
        if(size==i)
        {
            sh=dble(sh,size);
            size=size*2;
        }
        if(i==0)
        {
            //for first shingle assign the first three words respectively
            fp>>sh[i].s[0];
            fp>>sh[i].s[1];
            fp>>sh[i].s[2];
        }
        else
        {
            //after assign the first two words the last two words of the previous
            sh[i].s[0]=sh[i-1].s[1];
            sh[i].s[1]=sh[i-1].s[2];
            //assign latest word to the last word of the shingle
            fp>>sh[i].s[2];
        }
        i++;
    }
    return i;
}
//compares two shigles iteratively to find a common shingle and if found increases match by one
//May be better ways than to compare iteratively
//Case sensitive which may be removed in rust by using the .lowercase() function while comparing
float Compare(shingle *&sh1,shingle * &sh2,int l1, int l2)
{
    int match=0;
    for(int i=0;i<l1;i++)
    {
        for(int j=0;j<l2;j++)
        {
            if(sh1[i].s[0]==sh2[j].s[0] && sh1[i].s[1]==sh2[j].s[1] && sh1[i].s[2]==sh2[j].s[2])
            {
                match++;
            }
        }
    }
    cout<<match<<endl<<endl;
    cout<<l1+l2<<endl;
    //compute Jaccards coefficient JC=|sh1 n sh2|/|sh1 U sh2|
    return (float)match/((float)(l1+l2-match));
}

int main()
{
    shingle *sh1=NULL;
    string file1="text1.txt";
    int l1=CreateShingle(file1,sh1);
    string file2="text2.txt";
    shingle *sh2=NULL;
    int l2=CreateShingle(file2,sh2);
    float match=Compare(sh1,sh2,l1,l2);
    cout<<match;
    return 0;
}