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
    shingle* next;
};

void tokenize(shingle *&sh,int l1)
{
    int x=39;
    for (int i=0;i<l1;i++)
    {
        //'`' used on the assumption of it being one of the least frequent characters to appear in a text
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

int HTsize=11;
int** CreateHashTable(shingle *&sh,int size)
{
    int **HashTable=new int*[HTsize];
    //initialize the elements of the hashtable to NULL
    for(int i=0;i<HTsize;i++)
    {
        HashTable[i]=NULL;
    }
    //insert elements
    for (int i=0;i<size;i++)
    {
        int position= fmod(sh[i].token,HTsize);
        if(HashTable[position]==NULL)
        {
            HashTable[position]=reinterpret_cast<int*>(sh+i);
            sh[i].next=NULL;
        }
        else
        {
            shingle *temp=reinterpret_cast<shingle*>(HashTable[position]);
            while(temp->next!=NULL)
            {
                temp=temp->next;
            }
            temp->next=&sh[i];
            sh[i].next=NULL;
        }
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
//takes the initial hash table from the first shingle arrays and tries to insert the new shingle from another shingle array
int CollisionDetection(int** HashTable,shingle *& sh1,shingle *&sh2,int size)
{
    int match=0;
    for (int i=0;i<size;i++)
    {
        int position= fmod(sh2[i].token,HTsize);
        shingle* temp=reinterpret_cast<shingle*>(HashTable[position]);
        //loop for collisions in the chain
        while(temp!=NULL && temp->token!=sh2[i].token)
        {
            //(sh2[position].s[0]!=sh1[position].s[0] || sh2[position].s[1]!=sh1[position].s[1] || sh2[position].s[2]!=sh1[position].s[2])
            //moving through the chains
            temp=temp->next;
        }
        //if the token of the two shingles match then increments match
        if(temp!=NULL)
        {
            match++;
        }
    }
    return match;
}
//Compare using hash Table
//Best case scenario will take a o(n) where there is about 100% plagiarism.
//Worst case scenario where there is no plagiarism will take a O(n^2) as it will be similar to iteration
//For some reason the hashing part is failing when it meets a certain character' ” ' 
//turns out to be a problem while tokenizing as '”' was read as multi-character when declared as char a='”'
//char a='”' was read by the compiler as char a='Γm;
//Gave the following warning "warning: overflow in conversion from 'int' to 'char' changes value from '14844061' to ''\37777777635'' [-Woverflow]""
//Case sensitive which may be removed in rust by using the .lowercase() function while comparing
float Compare(shingle *&sh1,shingle * &sh2,int l1, int l2)
{
    int match;
    int** HT1=CreateHashTable(sh1,l1);
    match=CollisionDetection(HT1,sh1,sh2,l2);
    cout<<endl<<match<<endl;
    cout<<l1+l2<<endl;
    //compute Jaccards coefficient JC=|sh1 n sh2|/|sh1 U sh2|
    return (float)match/((float)(l1+l2-match));
}
//compares two files with the name/path provided in string file1 and file2
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