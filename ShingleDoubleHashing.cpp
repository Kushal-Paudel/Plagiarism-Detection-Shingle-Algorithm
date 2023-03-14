#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

struct shingle
{
    //create shingle containing 3 words
    string s[3];
    //Token stores the hash code map of the three shingle words
    long double token=0;
};

void tokenize(shingle *&sh,int l1)
{
    int x=39;
    for (int i=0;i<l1;i++)
    {
        string word=sh[i].s[0]+sh[i].s[1]+sh[i].s[2]+'`';
        //Polynomial Accumulation
        int j=0;
        while(word[j]!='`')
        {
            sh[i].token+=(int)word[j]*pow(x,j);
            j++;
        }
        // cout<<sh[i].token<<endl;
    }
}
//Has some errors as in some elements of the hashtable are still initialized to -1 after creation
int* CreateHashTable(shingle *&sh,int size)
{
    int *HashTable=NULL;
    HashTable=new int[size];
    //initialize the elements of the hashtable to -1
    for(int i=0;i<size;i++)
    {
        HashTable[i]=-1;
    }
    //insert elements
    for (int i=0;i<size;i++)
    {
        int position= fmod(sh[i].token,size);
        //loop for collisions
        int j=1;
        while(j<size && HashTable[position]!=-1)
        {
            //double hashing
            position=fmod(fmod(sh[i].token,size)+j*fmod(sh[i].token,13),size);
            j++;
        }
        HashTable[position]=i;
    }
    return HashTable;
}



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
int CollisionDetection(int* HashTable,shingle *& sh1,shingle *&sh2,int size,int HTsize)
{
    int match=0;
    for (int i=0;i<size;i++)
    {
        int position= fmod(sh2[i].token,HTsize);
        //loop for collisions
        int j=1;
        //we can compare the tokens of the two shingles as using x=39 in Polynomial Accumulation, it's found that no more than 6 words have same token from 50,000 English words Vocabulary 
        while(j<HTsize && sh1[HashTable[position]].token!=sh2[i].token)
        {
            //(sh2[position].s[0]!=sh1[position].s[0] || sh2[position].s[1]!=sh1[position].s[1] || sh2[position].s[2]!=sh1[position].s[2])
            //double hashing
            position=fmod(fmod(sh2[i].token,HTsize)+j*fmod(sh2[i].token,13),HTsize);
            j++;
        }
        if(j<HTsize)
        {
            match++;
        }
    }
    return match;
}
//compares two shigles iteratively to find a common shingle and if found increases match by one
//May be better ways than to compare iteratively
//Case sensitive which may be removed in rust by using the .lowercase() function while comparing
float Compare(shingle *&sh1,shingle * &sh2,int l1, int l2)
{
    int match;
    int HTsize=l1;
    if(l2>l1)
    {
        HTsize=l2;
    }
    int* HT1=CreateHashTable(sh1,HTsize);
    match=CollisionDetection(HT1,sh1,sh2,l2,HTsize);
    cout<<match<<endl;
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
    // float match=Compare(sh1,sh2,l1,l2);
    // cout<<match;
    tokenize(sh1,l1);
    // cout<<endl;
    tokenize(sh2,l2);
    cout<<Compare(sh1,sh2,l1,l2);
    return 0;
}
