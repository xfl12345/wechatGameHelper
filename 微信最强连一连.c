#include<stdio.h>
#include<stdlib.h>
//#include<conio.h>
//#include<windows.h>
//#include"cJSON.h"
#include<time.h>
#include"cJSON.c"
#if! defined(bool)
#include<stdbool.h>
#endif
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
/*定义一个类型，名字叫“tt”，它原来叫做“struct SquareInfo” */
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
					printf("通关成功！\n");
					printf("起点的第一步为：%c\n",getNextStep(tu,tu->startPlace));
					printf("通关路径如下：\n");
					printTu(tu,3,false);
				}
				else
				{
					printf("不存在通关路径！\n");
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
			printf("欢迎回到主菜单！！\n");
			printf("1.修改矩阵信息\n");
			printf("2.开始穷举爆破本关通关路径\n");
			printf("3.展示一下当前设置好的矩阵\n");
			printf("4.存档\n");
			printf("5.读档\n");
			printf("6.删档\n");
			printf("0.退出暴力穷举系统\n");
			break;
		case 2:
			system("cls");
			printf("请选择暴力穷举模式：\n");
			printf("（以下所有模式都最终以字符数据反馈结果）\n");
			printf("1.实时数据以字符输出（较慢）\n");
			printf("2.实时数据以静默模式更新到字符数据（较快）\n");
			printf("3.实时数据更新到字符数据并输出字符数据（最慢）\n");
			printf("4.实时数据直接以数字形式输出（很快）\n");
			printf("5.不显示实时数据（最快）\n");
			printf("0.返回主菜单\n");
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
		printf("首次使用，");
		tuInit(ko,true);
		detailSet(*ko);
		printf("\n初始化");
	}
	else
	{
		printf("是否需要初始化矩阵信息？\n");
		if(justMakeSure())
		{
			tuInit(ko,true);
			detailSet(*ko);
		}
		detailSet(*ko);
		/*待添加：针对个别数据进行修改*/
	}
	ko1=*ko;
	ko1->nowPlace=ko1->startPlace;
	printf("设置完毕！\n");
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
	printf("当前图况如下：\n");
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
	printf("当前是第%d关，矩阵有%d行，%d列，\n",ko->Lv,ko->hangMax,ko->lieMax);
	printf("本关是从第%d行%d列开始的，",getXYforRead(ko,ko->startPlace,2),getXYforRead(ko,ko->startPlace,1) );
	if(ko->endPlace==-9)
		printf("但不知道终点在哪。\n");
	else
		printf("并最终走到第%d行%d列的方格。\n",getXYforRead(ko,ko->endPlace,2) , getXYforRead(ko,ko->endPlace,1) );
	printf("目前走到了第%d行%d列的方格，还有%d个方格没走。\n",getXYforRead(ko,ko->nowPlace,2),getXYforRead(ko,ko->nowPlace,1) ,ko->undo);
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
		printf("请输入：\n");
		printf("矩阵有几列：");
		ko1->lieMax=limitInputNum(1,99);
		printf("矩阵有几行：");
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
	printf("请问这是第几关：");
	ko->Lv=limitInputNum(1,9999);
	printf("请问从哪里开始？\n");
	simpleGetXY(ko,&x,&y);
	(ko->tu)[y][x]=1;
	(ko->tu2)[y][x]='S';
	ko->startPlace=ko->lieMax * y + x +1;
	printf("请问您知道最后一步是到哪里结束吗？（y/n）\n");
	if(justMakeSure())
	{
		while(1)
		{
			printf("请问到哪里结束？\n");
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
	printf("确定设置无误了吗？（y/n）\n");
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
		printf("请问有几个空洞？\n");
		n=justGetNum();
		for(i=1;i<=n;i++)
		{
			printf("请设置第%d个空洞\n",i);
			printf("请问它在哪里？\n");
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
		printf("请问要取消几个空洞？\n");
		n=justGetNum();
		for(i=1;i<=n;i++)
		{
			printf("正在取消第%d个空洞\n",i);
			printf("请问它在哪里？\n");
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
	printf("确定设置无误了吗？（y/n）\n");
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
		printf("\n超限！请输入合理的正常值！\n");
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
		printf("\n请输入一个数字！\n");
		printf("请输入：");
	}
	return result;
}
void simpleGetXY(tt *ko,int *x,int *y)
{
	printf("（从左往右数）第几列：");
	*x=limitInputNum(1,ko->lieMax) -1;
	printf("（从上往下数）第几行：");
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
	/*不知道怎么回事，经常会出现起点步数不是1的情况，先简单粗暴地解决一下*/
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
		printf("\n\n貌似您还没闯关数据呢！\n\n");
		return;
	}
	else if(ko->tu == NULL)
	{
		printf("\n\n矩阵数据非法！存档失败！\n\n");
		return;
	}
	fp=fopen(filename,"w");
	//创建一个空的文档（对象）
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
	sendProgramInfo("接下来出现的是存档文件的内容\n*不必惊慌，亦无须做任何动作");
	char *buf = cJSON_Print(jsonData);
	puts(buf);
	fwrite(buf,strlen(buf),1,fp);
	fclose(fp);
	free(buf);
	printf("\n存档完毕！\n\n");
	system("pause");
}
void readData(tt **ko)
{
	FILE *fp=fopen(filename,"r");//打开文件
	if(fp==NULL)
	{
		printf("\n读档失败！\n");
        printf("请检查一下文件%s是不是被占用了\n",filename);
        printf("或者是你还没存过档！\n\n");
		return;
	}
	if(*ko!=NULL)
	{
		if((*ko)->tu != NULL){
			int i,i2;
			for(i=0;  i < (*ko)->hangMax ; i++){
				for(i2=0;  i2 < (*ko)->lieMax ; i2++){
					if(  (  (*ko)->tu   )[i][i2] > 0  ){
						printf("已经有现有的闯关数据了哦！\n");
						printf("继续读档会导致现有闯关数据的丢失哦！\n");
						if(!jinggao())
							return;
						i=(*ko)->hangMax;
						break;
					}
				}
			}
		}
	}
    char* json_content=NULL;//文件内容
    long len;//文件长度
    fseek(fp,0,SEEK_END);//文件指针放到文件末尾
    len=ftell(fp);//读取文件指针当前位置与文件开头的偏移量
    fseek(fp,0,SEEK_SET);//文件指针放到文件开头
    json_content=(char*)malloc(len+1);//开辟临时缓存空间
    fread(json_content,1,len,fp);//读取文件
	sendProgramInfo("接下来出现的是存档文件的内容\n*不必惊慌，亦无须做任何动作");
	cJSON *jsonData = cJSON_Parse(json_content);
	if(jsonData)
	{
		printf("存档文件已加载到程序！\n");
		printf("存档文件内容如下！\n");
		char *buf=cJSON_Print(jsonData);
		puts(buf);
		//输出一下程序到底解析到了什么内容
		sendProgramInfo("接下来将执行数据校验！");
		if(buf)
			free(buf);
	}
	fclose(fp);
	if(checkJsonData(jsonData,ko))
    	printf("\n读档成功！\n\n");
	else
		printf("\n读档失败！\n\n");
    system("pause");
    return;
}
void removeData()
{
	printf("\n您即将执行删档操作！\n");
	if(!jinggao())
		return;
	if(remove(filename)==0)
	{
		printf("\n删档成功！\n\n");
	}
	else
	{
		printf("\n删档失败！\n");
		printf("错误原因如下：\n");
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
			printf("\n数据损坏或版本不匹配！");
			return false;
		}
	}
	if(nodeBuf[7]->type != cJSON_Array){
		printf("\n数据tu的类型存在异常！");
		return false;
	}
	kk.lieMax = nodeBuf[0]->valuedouble;
	kk.hangMax = nodeBuf[1]->valuedouble;
	//判断数据自身是否存在矛盾
	i3=kk.lieMax * kk.hangMax;
	for(i=2;i<6;i++){
		if(nodeBuf[i]->valuedouble > i3){
			printf("\nJSON数据自身有违背逻辑，超出自身范围！");
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
		printf("\n数据tu的大小有误，JSON数据自身矛盾！");
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
						printf("\n数据startPlace有误");
						break;
					case 'E':
						printf("\n数据endPlace有误");
						break;
					case 'N':
						printf("\n数据nowPlace有误");
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
		printf("\n数据undo错误，应为%d",count);
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
					printf("啊哦！貌似和起点重叠了呢！\n重新再输入一次吧！\n");
					break;
				case 'E':
					printf("啊哦！貌似和终点重叠了呢！\n重新再输入一次吧！\n");
					break;
				case ' ':
					printf("啊哦！貌似和空洞重叠了呢！\n重新再输入一次吧！\n");
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
		printf("只需输入字母\"y\"或\"n\"，然后回车就可以啦！\n");
	}
}
bool jinggao(void)
{
	int ttt;
	printf("\n您确定要继续执行该操作吗？！\n");
	printf("如需继续执行，请输入数字666\n");
	printf("输入其它内容将取消当前操作\n");
	printf("请输入：");
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
