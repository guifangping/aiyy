#include<iostream>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string>    
#include<fstream>   
#include<vector> 
#include<map>
#include<set>
#include<cctype>//tolower()函数和ispunct函数  
#include<algorithm> //remove_if函数 
#define PATH1  "./finaltest1/"
#define PATH2  "./finaltest2/"
using namespace std;
char filename[256][256];
int len = 0,s=1;
string a;

int write_string_to_file_append(const std::string & file_string, const std::string str )
{
	std::ofstream	OsWrite(file_string);
	OsWrite<<str;
	OsWrite<<std::endl;
	OsWrite.close();
   return 0;
}


bool cmp(const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
}

void fileFilter(const char * fileName, const char * format)
{
    char tempName[10];
    memset(tempName,0,10);
    memcpy(tempName,fileName+strlen(fileName)-5,5);
    //printf("find file name :%s\n",tempName);
    if(0==(strcmp(tempName,format)))
    {
        printf("find file name:%s\n",fileName);
        FILE *fp;
        int num=0,ch,title;
	char a[50];
	//cout<<s<<endl;
	if(s==1)
		sprintf(a,PATH1"%s",fileName);
	else if(s==2)
		sprintf(a,PATH2"%s",fileName);
        fp=fopen(a,"r");
        if(fp==NULL)
        {
        	printf("没有检测到文件\n");
        	return;
    	}

        //获取title字段
        char line[1000];
        while(!feof(fp))
        {
            fgets(line,1000,fp);
            //printf("%s\n",line);
            for(int i=0;i<1000;i++)
            {
                if(line[i]=='<'&&line[i+1]=='t'&&line[i+2]=='i'&&line[i+3]=='t'&&line[i+4]=='l'&&line[i+5]=='e'&&line[i+6]=='>')
                {
                    //printf("title字段内容:");
		    cout<<"title字段内容:";
                    for(int n=i+7;n<1000;n++)
                    {
                        printf("%c",line[n]);
                        if(line[n+1]=='<'&&line[n+2]=='/'&&line[n+3]=='t'&&line[n+4]=='i'&&line[n+5]=='t'&&line[n+6]=='l'&&line[n+7]=='e'&&line[n+8]=='>')
                        	break;
                    }
                    cout<<endl;
                }
            }
        }
	fclose(fp);
	//词频统计
	//map的定义  
	map<string, size_t> word_count;
	ifstream in(a);//定义一个输入流  
	string word,word1;
	char b=' ';
	set<string> exclude = {"a","b","d","e","f","i","p","em","com","http","zh","www","px","cn","https","if","hk","og","stylesheet","id","nbsp","mosoteach","div","css","gcn","lanmoyunbanke"};
	while (in >> word)
	{
		string::iterator it1;
		//去除空格
		word.erase(remove_if(word.begin(), word.end(), static_cast<int(*)(int)>(&isspace)), word.end());
		for (it1 = word.begin(); it1 != word.end(); ++it1)
		{
					
			if(!isalpha(*it1))
				*it1=b;
			//转为小写
			else	*it1 = tolower(*it1);
		}
		write_string_to_file_append(std::string ("1.txt"),std::string (word));
		ifstream in("1.txt");
		while(in >> word1)
		{
	   	//若在排除词库中，则不统计
	   	if (exclude.find(word1) != exclude.end())
			continue;
	  	 ++word_count[word1];
		}
		
	}
	//输出统计结果
		cout<<"词频统计:"<<endl;
		vector<pair<string, size_t>> vec(word_count.begin(), word_count.end());
   		//对线性的vector进行排序
    		sort(vec.begin(), vec.end(), cmp);
    		for (int i = 0; i < 10; ++i)
		{
			cout << vec[i].first << "  ";
			cout << vec[i].second<<endl;
		}
	cout<<endl;
    }
}
int trave_dir(char* path, int depth)
{
    DIR *d;
    struct dirent *file;
    struct stat sb;
    if(!(d = opendir(path)))
    {
        printf("error opendir %s!!!\n",path);
        return -1;
    }
    while((file = readdir(d)) != NULL)
    {
        if(strncmp(file->d_name, ".", 1) == 0)
            continue;
        strcpy(filename[len++], file->d_name);
        if(stat(file->d_name, &sb) >= 0 && S_ISDIR(sb.st_mode) && depth <= 2)
        {
            trave_dir(file->d_name, depth + 1);
        }
    }
    closedir(d);
    return 0;
}
int main(int argv,char *argc[])
{
    int depth = 1;
    //cout<<argc[1];
    if(argc[1][1]!='c')
	{
		cout<<"error"<<endl;
		return 0;
	}
    ifstream in(argc[2]);
    char path[64];
    string str;
    if(in.is_open())
	{
		while(!in.eof())
		{
			getline(in,str);
			strcpy(path,str.c_str());
    			int i;
			cout<<path<<endl;
			if(strlen(path)==0)
				break;
    			trave_dir(path, depth);
			//memset(path,'\0',sizeof(path));	
			a=str;	
   			for(i = 0; i < len; i++)
    			{
				fileFilter(filename[i],".html");
       				 //printf("%s\n", filename[i]);
    			}
			s++;
			len=0;
		}
	}
    return 0;
}

