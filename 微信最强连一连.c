#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
//#include<conio.h>
//#include<windows.h>
//#include"cJSON.h"
#include"cJSON.c"
struct SquareInfo{
	int lieMax;
	int hangMax;
	int startPlace;
	int endPlace;
	int nowPlace;
	int undo;
	int Lv;
	int **tu;
	char **tu2;
};
typedef struct SquareInfo  tt;
/*����һ�����ͣ����ֽС�tt������ԭ��������struct SquareInfo�� */
struct fangXiang{
	char shang;
	char xia;
	char zuo;
	char you;
}fxCN;
const char filename[]="wechatGameLYL.json";
clock_t start,nowTime,finish,tmpTime;
int menu(int choice,int min,int max);
bool DFS(tt *ko,int step,int mode);

void tuModify(tt **ko,int is_firstrun);
void printTu(tt *ko,int mode,bool cleanScreen);
void printTuInfo(tt *ko);

void deleteTu(tt *ko);
void tuInit(tt **ko,bool need_ask);
void detailSet(tt *ko);
void obstacleSet(tt *ko,bool mode);
int startGame(tt *ko);

int limitInputNum(int min,int max);
int justGetNum(void);
void simpleGetXY(tt *ko,int *x,int *y);
char getPlaceName(tt *ko,int x,int y);
int getXYforRead(tt *ko,int where,int which);
char getNextStep(tt *ko,int where);

void saveData(tt *ko);
void readData(tt **ko);
void removeData();
bool checkJsonData(cJSON *json,tt **ko);

bool is_replace(tt *ko,int x,int y,bool warming);
bool justMakeSure(void);
bool jinggao(void);
void sendProgramInfo(char *info);
int main(void)
{
	int m_choice,e_choice,ci=0;
	tt *tu=NULL;

	fxCN.shang='A';
	fxCN.xia='V';
	fxCN.zuo='<';
	fxCN.you='>';
	while(1)
	{
		m_choice=menu(1,1,5);
		if(m_choice==0)
			break;
		switch(m_choice)
		{
			case 1:
				tuModify(&tu,ci);
				break;
			case 2:
				if(tu == NULL)
					tuModify(&tu,ci);
				m_choice=menu(2,1,4);
				if(m_choice==0)
					break;
				start=nowTime=tmpTime=clock();
				if(DFS(tu,1,m_choice))
				{
					system("cls");
					printf("ͨ�سɹ���\n");
					printf("���ĵ�һ��Ϊ��%c\n",getNextStep(tu,tu->startPlace));
					printf("ͨ��·�����£�\n");
					printTu(tu,3,false);
				}
				else
				{
					printf("������ͨ��·����\n");
				}
				system("pause");
				break;
			case 3:
				if(tu == NULL)
					tuModify(&tu,ci);
				printTuInfo(tu);
				break;
			case 4:
				saveData(tu);
				break;
			case 5:
				readData(&tu);
				break;
			case 6:
				removeData();
				break;
			default:
				break;
		}
	}
	return 0;
}
int menu(int choice,int min,int max)
{
	int result=-1;
	switch(choice)
	{
		case 1:
			system("cls");
			printf("��ӭ�ص����˵�����\n");
			printf("1.�޸ľ�����Ϣ\n");
			printf("2.��ʼ��ٱ��Ʊ���ͨ��·��\n");
			printf("3.չʾһ�µ�ǰ���úõľ���\n");
			printf("4.�浵\n");
			printf("5.����\n");
			printf("6.ɾ��\n");
			printf("0.�˳��������ϵͳ\n");
			break;
		case 2:
			system("cls");
			printf("��ѡ�������ģʽ��\n");
			printf("����������ģʽ���������ַ����ݷ��������\n");
			printf("1.ʵʱ�������ַ������������\n");
			printf("2.ʵʱ�����Ծ�Ĭģʽ���µ��ַ����ݣ��Ͽ죩\n");
			printf("3.ʵʱ���ݸ��µ��ַ����ݲ�����ַ����ݣ�������\n");
			printf("4.ʵʱ����ֱ����������ʽ������ܿ죩\n");
			printf("5.����ʾʵʱ���ݣ���죩\n");
			printf("0.�������˵�\n");
			break;
		default:break;
	}
	result=justGetNum();
	return result;
}
bool DFS(tt *ko,int step,int mode)
{
	int **kk=ko->tu;
	int x=getXYforRead(ko,ko->nowPlace,1) -1;
	int y=getXYforRead(ko,ko->nowPlace,2) -1;
	if(y-1>=0)
		if(kk[y-1][x]==0)
		{//Go up
			kk[y-1][x]= step+1;
			ko->nowPlace=ko->nowPlace - ko->lieMax;
			ko->undo--;
			printTu(ko,mode,true);
			if(DFS(ko,step+1,mode))
				return true;
			kk[y-1][x]= 0;
			ko->nowPlace=ko->nowPlace + ko->lieMax;
			ko->undo++;
			printTu(ko,mode,true);
		}
	if(y+1 < ko->hangMax)
		if(kk[y+1][x]==0 )
		{//Go down
			kk[y+1][x]= step+1;
			ko->nowPlace=ko->nowPlace + ko->lieMax;
			ko->undo--;
			printTu(ko,mode,true);
			if(DFS(ko,step+1,mode))
				return true;
			kk[y+1][x]= 0;
			ko->nowPlace=ko->nowPlace - ko->lieMax;
			ko->undo++;
			printTu(ko,mode,true);
		}
	if(x-1>=0)
		if(kk[y][x-1]==0 )
		{//Go left
			kk[y][x-1]= step+1;
			ko->nowPlace--;
			ko->undo--;
			printTu(ko,mode,true);
			if(DFS(ko,step+1,mode))
				return true;
			kk[y][x-1]= 0;
			ko->nowPlace++;
			ko->undo++;
			printTu(ko,mode,true);
		}
	if(x+1 < ko->lieMax)
		if(kk[y][x+1]==0 )
		{//Go right
			kk[y][x+1]= step+1;
			ko->nowPlace++;
			ko->undo--;
			printTu(ko,mode,true);
			if(DFS(ko,step+1,mode))
				return true;
			kk[y][x+1]= 0;
			ko->nowPlace--;
			ko->undo++;
			printTu(ko,mode,true);
		}
	if(ko->undo == 0)
		if(ko->nowPlace == ko->endPlace || ko->endPlace == -9 )
		{
			if(ko->endPlace==-9)
				ko->endPlace = ko->nowPlace;
			return true;
		}
	return false;
}

void tuModify(tt **ko,int is_firstrun)
{
	int lie,hang,i,i2;
	tt *ko1 = NULL;
	if(is_firstrun==0&& *ko==NULL)
	{
		printf("�״�ʹ�ã�");
		tuInit(ko,true);
		detailSet(*ko);
		printf("\n��ʼ��");
	}
	else
	{
		printf("�Ƿ���Ҫ��ʼ��������Ϣ��\n");
		if(justMakeSure())
		{
			tuInit(ko,true);
			detailSet(*ko);
		}
		detailSet(*ko);
		/*����ӣ���Ը������ݽ����޸�*/
	}
	ko1=*ko;
	ko1->nowPlace=ko1->startPlace;
	printf("������ϣ�\n");
	system("pause");
	return;
}
void printTu(tt *ko,int mode,bool cleanScreen)
{
	if(mode == 5)
		return;
	if(mode == 6)
	{
		double t;
		nowTime=clock();
		t=(double)(nowTime-tmpTime)/CLOCKS_PER_SEC;
		if(t>10)
		{
			mode=3;
			tmpTime=clock();
		}
		else
			return;
	}
	if(cleanScreen)
		system("cls");
	int x,y,i,**kk=ko->tu;
	char **kk2=ko->tu2;
	char tmpChar;
	printf("��ǰͼ�����£�\n");
	for(y=0;y < ko->hangMax ; y++)
	{
		for( x=0;x < ko->lieMax ; x++)
		{
			switch(mode)
			{
				case 1:
					switch( kk[y][x] )
					{
						case 0:printf("%c",getPlaceName(ko,x,y));break;
						case -4:printf(" ");break;
						default:
							tmpChar=getPlaceName(ko,x,y);
							if(tmpChar != 'U'){
								printf("%c",tmpChar);
								break;
							}
							if(y-1>=0)
								if(kk[y-1][x]-kk[y][x]==1)
									printf("%c",fxCN.shang);
							if(y+1 < ko->hangMax)
								if(kk[y+1][x]-kk[y][x]==1)
									printf("%c",fxCN.xia);
							if(x-1>=0)
								if(kk[y][x-1]-kk[y][x]==1)
									printf("%c",fxCN.zuo);
							if(x+1 < ko->lieMax)
								if(kk[y][x+1]-kk[y][x]==1)
									printf("%c",fxCN.you);		
							break;
					}
					break;
				case 2:
					switch( kk[y][x] )
					{
						case 0:
							kk2[y][x]=getPlaceName(ko,x,y);
							break;
						case -4:break;
						default:
							tmpChar=getPlaceName(ko,x,y);
							if(tmpChar != 'U'){
								kk2[y][x]=tmpChar;
								break;
							}
							if(y-1>=0)
								if(kk[y-1][x]-kk[y][x]==1)
									kk2[y][x]=fxCN.shang;
							if(y+1 < ko->hangMax)
								if(kk[y+1][x]-kk[y][x]==1)
									kk2[y][x]=fxCN.xia;
							if(x-1>=0)
								if(kk[y][x-1]-kk[y][x]==1)
									kk2[y][x]=fxCN.zuo;
							if(x+1 < ko->lieMax)
								if(kk[y][x+1]-kk[y][x]==1)
									kk2[y][x]=fxCN.you;
							break;
					}
					break;
				case 3:
					switch( kk[y][x] )
					{
						case 0:
							kk2[y][x]=getPlaceName(ko,x,y);
							printf("%c",kk2[y][x]);
							break;
						case -4:printf(" ");break;
						default:
							tmpChar=getPlaceName(ko,x,y);
							if(tmpChar != 'U'){
								kk2[y][x]=tmpChar;
								printf("%c",kk2[y][x]);
								break;
							}
							if(y-1>=0)
								if(kk[y-1][x]-kk[y][x]==1)
									kk2[y][x]=fxCN.shang;
							if(y+1 < ko->hangMax)
								if(kk[y+1][x]-kk[y][x]==1)
									kk2[y][x]=fxCN.xia;
							if(x-1>=0)
								if(kk[y][x-1]-kk[y][x]==1)
									kk2[y][x]=fxCN.zuo;
							if(x+1 < ko->lieMax)
								if(kk[y][x+1]-kk[y][x]==1)
									kk2[y][x]=fxCN.you;
							printf("%c",kk2[y][x]);
							break;
					}
					break;
				case 4:printf("%4d",kk[y][x]);break;
				case 7:printf("%c",kk2[y][x]);break;
				default:break;
			}
		}
		printf("\n");
	}
	return;
}
void printTuInfo(tt *ko)
{
	system("cls");
	printf("��ǰ�ǵ�%d�أ�������%d�У�%d�У�\n",ko->Lv,ko->hangMax,ko->lieMax);
	printf("�����Ǵӵ�%d��%d�п�ʼ�ģ�",getXYforRead(ko,ko->startPlace,2),getXYforRead(ko,ko->startPlace,1) );
	if(ko->endPlace==-9)
		printf("����֪���յ����ġ�\n");
	else
		printf("�������ߵ���%d��%d�еķ���\n",getXYforRead(ko,ko->endPlace,2) , getXYforRead(ko,ko->endPlace,1) );
	printf("Ŀǰ�ߵ��˵�%d��%d�еķ��񣬻���%d������û�ߡ�\n",getXYforRead(ko,ko->nowPlace,2),getXYforRead(ko,ko->nowPlace,1) ,ko->undo);
	printTu(ko,3,false);
	system("pause");
	return;
}

void deleteTu(tt *ko)
{
	int i;
	if(ko == NULL)
		return;
	if(ko->tu == NULL)
		return;
	for(i=0;i < ko->hangMax;i++)
	{
		free((ko->tu)[i]);
		free((ko->tu2)[i]);
	}
	free(ko->tu);
	free(ko->tu2);
	ko->tu=NULL;
	ko->tu2=NULL;
	return;
}
void tuInit(tt **ko,bool need_ask)
{
	int i,i2;
	tt *ko1=NULL;
	deleteTu(*ko);
	if(*ko == NULL)
		*ko=(tt *)malloc(sizeof(tt));
	ko1=*ko;
	if(need_ask)
	{
		printf("�����룺\n");
		printf("�����м��У�");
		ko1->lieMax=limitInputNum(1,99);
		printf("�����м��У�");
		ko1->hangMax=limitInputNum(1,99);
	}
	ko1->Lv=ko1->startPlace=ko1->endPlace=-1;
	ko1->undo=0;
	ko1->tu=(int **)malloc(sizeof(int *)*(ko1->hangMax));
	ko1->tu2=(char **)malloc(sizeof(char *)*(ko1->hangMax));
	for(i=0;i < ko1->hangMax;i++)
	{
		(ko1->tu)[i]=(int *)malloc(sizeof(int)*(ko1->lieMax));
		(ko1->tu2)[i]=(char *)malloc(sizeof(char)*(ko1->lieMax));
		for(i2=0;i2 < ko1->lieMax;i2++)
		{
			(ko1->tu)[i][i2]=0;
			(ko1->tu2)[i][i2]='O';
		}
	}
	return;
}
void detailSet(tt *ko)
{
	int x,y;
	int ci;
	char tmpchar;
	printf("�������ǵڼ��أ�");
	ko->Lv=limitInputNum(1,9999);
	printf("���ʴ����￪ʼ��\n");
	simpleGetXY(ko,&x,&y);
	(ko->tu)[y][x]=1;
	(ko->tu2)[y][x]='S';
	ko->startPlace=ko->lieMax * y + x +1;
	printf("������֪�����һ���ǵ���������𣿣�y/n��\n");
	if(justMakeSure())
	{
		while(1)
		{
			printf("���ʵ����������\n");
			simpleGetXY(ko,&x,&y);
			if(!is_replace(ko,x,y,true))
				break;
		}
		//(ko->tu)[y][x]=-3;
		(ko->tu2)[y][x]='E';
		ko->endPlace=ko->lieMax * y + x +1;
	}
	else
	{
		ko->endPlace=-9;
	}
	printf("ȷ�������������𣿣�y/n��\n");
	if(!justMakeSure())
	{
		detailSet(ko);
		return;
	}
	ko->undo=ko->hangMax * ko->lieMax;
	if(ko->startPlace>=0)
		ko->undo--;
	obstacleSet(ko,true);
	return;
}
void obstacleSet(tt *ko,bool mode)
{
	int x,y;
	int tmp2,m,n,i;
	if(mode)
	{
		printf("�����м����ն���\n");
		n=justGetNum();
		for(i=1;i<=n;i++)
		{
			printf("�����õ�%d���ն�\n",i);
			printf("�����������\n");
			simpleGetXY(ko,&x,&y);
			if(is_replace(ko,x,y,true))
			{
				i--;
				continue;
			}
			ko->tu[y][x]=-4;
			ko->tu2[y][x]=' ';
			ko->undo--;
		}
	}
	else
	{
		printf("����Ҫȡ�������ն���\n");
		n=justGetNum();
		for(i=1;i<=n;i++)
		{
			printf("����ȡ����%d���ն�\n",i);
			printf("�����������\n");
			simpleGetXY(ko,&x,&y);
			if(is_replace(ko,x,y,true))
			{
				i--;
				continue;
			}
			ko->tu[y][x]=0;
			ko->tu2[y][x]='O';
			ko->undo++;
		}
	}
	printf("ȷ�������������𣿣�y/n��\n");
	if(!justMakeSure())
		obstacleSet(ko,!mode);
	return;
}

int limitInputNum(int min,int max)
{
	int result;
	while(1)
	{
		result=justGetNum();
		if(result>=min && result<=max)
			break;
		printf("\n���ޣ���������������ֵ��\n");
	}
	return result;
}
int justGetNum(void)
{
	char b[50];
	int i,result;
	while(1)
	{
		scanf("%s",b);
		for(i=0,result=0;b[i]!='\0';i++)
		{
			if( !(b[i]>='0'&&b[i]<='9'))
				break;
			result=b[i]-'0'+result*10;
		}
		if(b[i]=='\0' && i!=0)
			break;
		printf("\n������һ�����֣�\n");
		printf("�����룺");
	}
	return result;
}
void simpleGetXY(tt *ko,int *x,int *y)
{
	printf("���������������ڼ��У�");
	*x=limitInputNum(1,ko->lieMax) -1;
	printf("���������������ڼ��У�");
	*y=limitInputNum(1,ko->hangMax) -1;
	return;
}
char getPlaceName(tt *ko, int x,int y)
{
	char result='U';
	int n= y*(ko->lieMax) + x + 1;
	if(n == ko->startPlace )
		result='S';
	else if(n == ko->endPlace)
		result='E';
	else if(n == ko->nowPlace)
		result='N';
	else if( (ko->tu)[y][x] == -4)
		result=' ';
	else if( (ko->tu)[y][x] == 0 )
		result='O';
	return result;
}
int getXYforRead(tt *ko,int where,int which)
{/*which=1,return X;which=2,return Y;*/
	if(which == 1)
	{
		if(where % ko->lieMax == 0)
			return ko->lieMax;
		else
			return where % ko->lieMax;
	}
	if(where / ko->lieMax == 0)
		return 1;
	if(where % ko->lieMax == 0)
		return where / ko->lieMax;
	else
		return where / ko->lieMax +1;
}
char getNextStep(tt *ko,int where)
{
	int **kk=ko->tu;
	int x=getXYforRead(ko,where,1) -1;
	int y=getXYforRead(ko,where,2) -1;
	if(getPlaceName(ko,x,y)=='S')
		kk[y][x]=1;
	/*��֪����ô���£������������㲽������1��������ȼ򵥴ֱ��ؽ��һ��*/
	if(y-1>=0)
		if(kk[y-1][x]-kk[y][x]==1)
			return fxCN.shang;
	if(y+1 < ko->hangMax)
		if(kk[y+1][x]-kk[y][x]==1)
			return fxCN.xia;
	if(x-1>=0)
		if(kk[y][x-1]-kk[y][x]==1)
			return fxCN.zuo;
	if(x+1 < ko->lieMax)
		if(kk[y][x+1]-kk[y][x]==1)
			return fxCN.you;
	return NULL;
}

void saveData(tt *ko)
{
	FILE *fp=NULL;
	if(ko==NULL)
	{
		printf("\n\nò������û���������أ�\n\n");
		return;
	}
	else if(ko->tu == NULL)
	{
		printf("\n\n�������ݷǷ����浵ʧ�ܣ�\n\n");
		return;
	}
	fp=fopen(filename,"w");
	//����һ���յ��ĵ�������
    cJSON *jsonData = cJSON_CreateObject();
	cJSON_AddItemToObject(jsonData,"lieMax",cJSON_CreateNumber(ko->lieMax));
	cJSON_AddItemToObject(jsonData,"hangMax",cJSON_CreateNumber(ko->hangMax));
	cJSON_AddItemToObject(jsonData,"startPlace",cJSON_CreateNumber(ko->startPlace));
	cJSON_AddItemToObject(jsonData,"endPlace",cJSON_CreateNumber(ko->endPlace));
	cJSON_AddItemToObject(jsonData,"nowPlace",cJSON_CreateNumber(ko->nowPlace));
	cJSON_AddItemToObject(jsonData,"undo",cJSON_CreateNumber(ko->undo));
	cJSON_AddItemToObject(jsonData,"Lv",cJSON_CreateNumber(ko->Lv));
	cJSON *partOfTu = NULL;
	cJSON *tu = cJSON_CreateArray();
	int i,i2;
	if(ko->tu)
	{
		for(i=0;  i < ko->hangMax ; i++)
		{
			partOfTu=cJSON_CreateIntArray((ko->tu)[i],ko->lieMax);
			cJSON_AddItemToArray(tu , partOfTu);
		}
	}
	cJSON_AddItemToObject(jsonData,"tu",tu);
	sendProgramInfo("���������ֵ��Ǵ浵�ļ�������\n*���ؾ��ţ����������κζ���");
	char *buf = cJSON_Print(jsonData);
	puts(buf);
	fwrite(buf,strlen(buf),1,fp);
	fclose(fp);
	free(buf);
	printf("\n�浵��ϣ�\n\n");
	system("pause");
}
void readData(tt **ko)
{
	FILE *fp=fopen(filename,"r");//���ļ�
	if(fp==NULL)
	{
		printf("\n����ʧ�ܣ�\n");
        printf("����һ���ļ�%s�ǲ��Ǳ�ռ����\n",filename);
        printf("�������㻹û�������\n\n");
		return;
	}
	if(*ko!=NULL)
	{
		if((*ko)->tu != NULL){
			int i,i2;
			for(i=0;  i < (*ko)->hangMax ; i++){
				for(i2=0;  i2 < (*ko)->lieMax ; i2++){
					if(  (  (*ko)->tu   )[i][i2] > 0  ){
						printf("�Ѿ������еĴ���������Ŷ��\n");
						printf("���������ᵼ�����д������ݵĶ�ʧŶ��\n");
						if(!jinggao())
							return;
						i=(*ko)->hangMax;
						break;
					}
				}
			}
		}
	}
    char* json_content=NULL;//�ļ�����
    long len;//�ļ�����
    fseek(fp,0,SEEK_END);//�ļ�ָ��ŵ��ļ�ĩβ
    len=ftell(fp);//��ȡ�ļ�ָ�뵱ǰλ�����ļ���ͷ��ƫ����
    fseek(fp,0,SEEK_SET);//�ļ�ָ��ŵ��ļ���ͷ
    json_content=(char*)malloc(len+1);//������ʱ����ռ�
    fread(json_content,1,len,fp);//��ȡ�ļ�
	sendProgramInfo("���������ֵ��Ǵ浵�ļ�������\n*���ؾ��ţ����������κζ���");
	cJSON *jsonData = cJSON_Parse(json_content);
	if(jsonData)
	{
		printf("�浵�ļ��Ѽ��ص�����\n");
		printf("�浵�ļ��������£�\n");
		char *buf=cJSON_Print(jsonData);
		puts(buf);
		//���һ�³��򵽵׽�������ʲô����
		sendProgramInfo("��������ִ������У�飡");
		if(buf)
			free(buf);
	}
	fclose(fp);
	if(checkJsonData(jsonData,ko))
    	printf("\n�����ɹ���\n\n");
	else
		printf("\n����ʧ�ܣ�\n\n");
    system("pause");
    return;
}
void removeData()
{
	printf("\n������ִ��ɾ��������\n");
	if(!jinggao())
		return;
	if(remove(filename)==0)
	{
		printf("\nɾ���ɹ���\n\n");
	}
	else
	{
		printf("\nɾ��ʧ�ܣ�\n");
		printf("����ԭ�����£�\n");
		perror("remove");
	}
	printf("\n\n");
	system("pause");
}
bool checkJsonData(cJSON *json,tt **ko)
{
	tt kk,*kk2;
	kk2=&kk;
	kk.tu=NULL;
	kk.tu2=NULL;
	cJSON *nodeBuf[9];
	int *tuInOne=NULL,i,i2,i3,count;
	nodeBuf[0]=cJSON_GetObjectItem(json,"lieMax");
	nodeBuf[1]=cJSON_GetObjectItem(json,"hangMax");
	nodeBuf[2]=cJSON_GetObjectItem(json,"startPlace");
	nodeBuf[3]=cJSON_GetObjectItem(json,"endPlace");
	nodeBuf[4]=cJSON_GetObjectItem(json,"nowPlace");
	nodeBuf[5]=cJSON_GetObjectItem(json,"undo");
	nodeBuf[6]=cJSON_GetObjectItem(json,"Lv");
	nodeBuf[7]=cJSON_GetObjectItem(json,"tu");
	for(i=0;i<8;i++){
		if(nodeBuf[i]==NULL){
			printf("\n�����𻵻�汾��ƥ�䣡");
			return false;
		}
	}
	if(nodeBuf[7]->type != cJSON_Array){
		printf("\n����tu�����ʹ����쳣��");
		return false;
	}
	kk.lieMax = nodeBuf[0]->valuedouble;
	kk.hangMax = nodeBuf[1]->valuedouble;
	//�ж����������Ƿ����ì��
	i3=kk.lieMax * kk.hangMax;
	for(i=2;i<6;i++){
		if(nodeBuf[i]->valuedouble > i3){
			printf("\nJSON����������Υ���߼�����������Χ��");
			return false;
		}
	}
	tuInit( &kk2 , false);
	kk.startPlace = nodeBuf[2]->valuedouble;
	kk.endPlace = nodeBuf[3]->valuedouble;
	kk.nowPlace = nodeBuf[4]->valuedouble;
	kk.undo = nodeBuf[5]->valuedouble;
	kk.Lv = nodeBuf[6]->valuedouble;
	cJSON *tuY = cJSON_GetArrayItem(nodeBuf[7],0);
	if(cJSON_GetArraySize(tuY) != kk.hangMax){
		printf("\n����tu�Ĵ�С����JSON��������ì�ܣ�");
		return false;
	}
	bool flag=false;
	cJSON *tuX = NULL;
	count=0;
	//i*kk.lieMax + i2
	for(i=0;  i < kk.hangMax ; i++ , tuY = tuY->next)
	{
		for(i2=0;  i2 < kk.lieMax ; i2++ , tuX = tuX->next)
		{
			tuX = cJSON_GetArrayItem(tuY,i2);
			i3 = i * kk.lieMax + i2 +1;
			if(tuX->valuedouble == -4)
			{
				flag=true;
				switch(getPlaceName(&kk,i2,i))
				{
					case 'S':
						printf("\n����startPlace����");
						break;
					case 'E':
						printf("\n����endPlace����");
						break;
					case 'N':
						printf("\n����nowPlace����");
						break;
					default:
						flag=false;
						break;
				}
				if(flag)
					return false;
			}
			kk.tu[i][i2]=tuX->valuedouble;
			if(getPlaceName(&kk,i2,i)=='S')
				kk.tu[i][i2]=++(tuX->valuedouble);
			if(tuX->valuedouble == 0)
				count++;
		}
	}
	if(count != kk.undo)
	{
		printf("\n����undo����ӦΪ%d",count);
		return false;
	}
	if(ko)
	{
		deleteTu(*ko);
		free(*ko);
		*ko=(tt *)malloc(sizeof(tt));
		**ko=kk;
	}
	return true;
}

bool is_replace(tt *ko,int x,int y,bool warming)
{
	char tmpChar=getPlaceName(ko,x,y);
	if(tmpChar != 'U' && tmpChar != 'O')
	{
		if(warming)
		{
			switch(tmpChar)
			{
				case 'S':
					printf("��Ŷ��ò�ƺ�����ص����أ�\n����������һ�ΰɣ�\n");
					break;
				case 'E':
					printf("��Ŷ��ò�ƺ��յ��ص����أ�\n����������һ�ΰɣ�\n");
					break;
				case ' ':
					printf("��Ŷ��ò�ƺͿն��ص����أ�\n����������һ�ΰɣ�\n");
					break;
			}
		}
		return true;
	}
	return false;
}
bool justMakeSure(void)
{
	char tmpchar;
	while(1)
	{
		scanf("%c",&tmpchar);
		if(tmpchar=='y')
			return true;
		if(tmpchar=='n')
			return false;
		printf("ֻ��������ĸ\"y\"��\"n\"��Ȼ��س��Ϳ�������\n");
	}
}
bool jinggao(void)
{
	int ttt;
	printf("\n��ȷ��Ҫ����ִ�иò����𣿣�\n");
	printf("�������ִ�У�����������666\n");
	printf("�����������ݽ�ȡ����ǰ����\n");
	printf("�����룺");
	scanf("%d",&ttt);
	if(ttt==666)
		return true;
	return false;
}
void sendProgramInfo(char *info)
{
	int i;
	printf("\n");
	for(i=0;i<50;i++)
		printf("*");
	if(info)
	{
		printf("\n*");
		puts(info);
	}
	else
	{
		return;
	}	
	for(i=0;i<50;i++)
		printf("*");
	printf("\n");
	return;
}
