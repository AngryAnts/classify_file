// test.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <process.h> 
#include <direct.h>
#include<stdlib.h>

#define _MAXPATH 256
#define MAX_CMD_LEN 256
#define MAX_FILE_NAME_LEN	(256)

#define VERTICAL_SUB_FOLDER_NAME	"VerticalPicture"
#define HORIZONTAL_SUB_FOLDER_NAME	"HorizontalPicture"
#define MODIFY_VERTICAL_SUB_FOLDER_NAME	"VerticalPicture_Modify"
#define MODIFY_HORIZONTAL_SUB_FOLDER_NAME	"HorizontalPicture_Modify"
#define SMALL_FOLDER_NAME	"SmallPic"
#define COVER_FOLDER_NAME	"Cover"
#define LETTER_FOLDER_NAME	"Letter"
#define PAGE7_FOLDER_NAME	"zPage7"

#define JPG_SEG_FLAG	(0xFF)

typedef enum {
	IMG_VERTICAL = 0,
	IMG_HORIZONTAL = 1,
	IMG_SIZE_TYPE_INVLID,
	IMG_SIZE_TYPE_NUM
}IMG_SIZE_TYPE_en;

typedef enum {
	SPLIT,
	MERGE,
	GET_SMALL,
	GET_COVER,
	GET_PAGE7,
	SPLIT_MOV
}ACT_TYPE_en;

typedef enum {
	JPG_SEG_TYPE_SOI	= 0xDB,
	JPG_SEG_TYPE_EOI	= 0xD9,
	JPG_SEG_TYPE_SOF0	= 0xC0,
	JPG_SEG_TYPE_SOF1	= 0xC1,
	JPG_SEG_TYPE_DHT	= 0xC4,
	JPG_SEG_TYPE_SOS	= 0xDA,
	JPG_SEG_TYPE_DQT	= 0xDB,
	JPG_SEG_TYPE_DRI	= 0xDD,
	JPG_SEG_TYPE_APP0	= 0xE0,
	JPG_SEG_TYPE_COM	= 0xFE
}JPG_SEG_TYPE_en;
char acSubDirName[_MAXPATH] = "";//路径
int VerticalImgNum = 1;
int HorizontalImgNum = 2;
int iSamllPicNum = 0;

void act(ACT_TYPE_en enAct);
void SubFileProc(char * pcFileName, ACT_TYPE_en enAct);
void SubDirProc(char * pcSubDirName, ACT_TYPE_en enAct);
void split(char * pcFileName);
void merge(char * pcFileName);
void getSmall(char * pcFileName);
void getCover(char * pcFileName);
void getPage7(char * pcFileName);
void split_move(char * pcFileName);
IMG_SIZE_TYPE_en GetImgSizeType(char * pcFileName);

int _tmain(int argc, _TCHAR* argv[])
{
	int status;

//	status = _mkdir(VERTICAL_SUB_FOLDER_NAME);		/*在当前文件夹下创建VerticalPicture*/ 
//	status = _mkdir(HORIZONTAL_SUB_FOLDER_NAME);	/*在当前文件夹下创建HorizontalPicture*/ 
//	status = _mkdir(MODIFY_VERTICAL_SUB_FOLDER_NAME);		/*在当前文件夹下创建VerticalPicture*/ 
//	status = _mkdir(MODIFY_HORIZONTAL_SUB_FOLDER_NAME);	/*在当前文件夹下创建HorizontalPicture*/ 
//	status = _mkdir(SMALL_FOLDER_NAME);				/*在当前文件夹下创建SamllPic*/ 
	status = _mkdir(COVER_FOLDER_NAME);				/*在当前文件夹下创建Cover*/ 
//	status = _mkdir(LETTER_FOLDER_NAME);			/*在当前文件夹下创建Letter*/ 
//	status = _mkdir(PAGE7_FOLDER_NAME);				/*在当前文件夹下创建Page7*/ 

//  act(GET_SMALL);
	act(GET_COVER);
//	act(SPLIT);

//	act(MERGE);
//  act(GET_PAGE7);
//	act(SPLIT_MOV);

	system("pause");

	return 0;
}

void act(ACT_TYPE_en enAct)
{
    struct _finddata_t file; //定义结构体变量
    long handle;

	handle=_findfirst("*",&file);//查找所有文件
    if(handle == -1)/*如果handle为－1, 表示当前目录为空, 则结束查找而返回如果handle为－1, 表示当前目录为空, 则结束查找而返回 */
	{
		return ;
	}

	do
	{
		if(file.attrib &_A_SUBDIR ) //是目录
		{
			SubDirProc(file.name, enAct);
		}
		else    //// 如果第一个实体不是目录,显示该文件
		{
			SubFileProc(file.name, enAct);
		}
	} while(!(_findnext(handle,&file)));
 
    _findclose(handle);

	return ;
}

void SubFileProc(char * pcFileName, ACT_TYPE_en enAct)
{
	if (0 == strlen(acSubDirName))
	{
		return ;
	}

	switch (enAct)
	{
	case SPLIT:
		split(pcFileName);
		break;
	case MERGE:
		merge(pcFileName);
		break;
	case GET_SMALL:
		getSmall(pcFileName);
		break;
	case GET_COVER:
		getCover(pcFileName);
		break;
	case GET_PAGE7:
		getPage7(pcFileName);
		break;
	case SPLIT_MOV:
		split_move(pcFileName);
		break;
	}
	return ;
}

void SubDirProc(char * pcSubDirName, ACT_TYPE_en enAct)
{
	if ((0 == strcmp(SMALL_FOLDER_NAME, pcSubDirName))
		|| (0 == strcmp(COVER_FOLDER_NAME, pcSubDirName))
		|| (0 == strcmp(LETTER_FOLDER_NAME, pcSubDirName))
		|| (0 == strcmp(PAGE7_FOLDER_NAME, pcSubDirName))
		|| ('.' == pcSubDirName[0])
		|| (strlen(acSubDirName))

		|| ((MERGE != enAct)
		&& ((0 == strcmp(VERTICAL_SUB_FOLDER_NAME, pcSubDirName))
		|| (0 == strcmp(HORIZONTAL_SUB_FOLDER_NAME, pcSubDirName)))))

	{
		return ;
	}

	VerticalImgNum = 1;
	HorizontalImgNum = 2;
	iSamllPicNum = 0;

	_chdir(pcSubDirName); //进入该目录
	strncpy_s(acSubDirName, (_MAXPATH -1), pcSubDirName, (_MAXPATH - 1));
	act(enAct);//继续遍历
	_chdir("..");/*查查找完毕之后, 返回上一级目录找完毕之后, 返回　　　　　　　　　　　　　　　　　上一级目录*/ 
	memset(acSubDirName, 0x00, sizeof(acSubDirName));

	return ;
}

void split(char * pcFileName)
{
	char acNewFileName[MAX_FILE_NAME_LEN] = {0};
	char cmd[MAX_CMD_LEN];
	IMG_SIZE_TYPE_en enImgSizeType;
	int i = 0;

	enImgSizeType = GetImgSizeType(pcFileName);
	if (IMG_SIZE_TYPE_INVLID == enImgSizeType)
	{
		printf("Get image size error: %s/%s\n", acSubDirName, pcFileName);
		return ;
	}

	i = atoi(pcFileName);
	if (0 != i)
	{
		sprintf_s(acNewFileName, (MAX_FILE_NAME_LEN - 1),"%s#page%d.jpg", acSubDirName, i);
	}
	else if (NULL != strstr(pcFileName, "新娘"))
	{
		sprintf_s(acNewFileName, (MAX_FILE_NAME_LEN - 1),"%s#hpage1.jpg", acSubDirName);
	}
	else if ((NULL != strstr(pcFileName, "新郎")) || (NULL != strstr(pcFileName, "新浪")))
	{
		sprintf_s(acNewFileName, (MAX_FILE_NAME_LEN - 1),"%s#hpage2.jpg", acSubDirName);
	}
	else if (NULL != strstr(pcFileName, "合"))
	{
		sprintf_s(acNewFileName, (MAX_FILE_NAME_LEN - 1),"%s#hpage3.jpg", acSubDirName);
	}

	if (0 == strlen(acNewFileName))
	{
		printf("File Error: %s/%s\n", acSubDirName, pcFileName);
		return ;
	}

	if (IMG_HORIZONTAL == enImgSizeType)
	{
		sprintf_s(cmd,(MAX_CMD_LEN - 1), "move %s ../%s/%s",pcFileName, HORIZONTAL_SUB_FOLDER_NAME, acNewFileName);
	}
	else
	{
		sprintf_s(cmd,(MAX_CMD_LEN - 1), "move %s ../%s/%s",pcFileName, VERTICAL_SUB_FOLDER_NAME, acNewFileName);
	}

	system(cmd);
	puts(cmd); //输出目录路径

	return ;
}

void split_move(char * pcFileName)
{
	char acNewFileName[MAX_FILE_NAME_LEN];
	char cmd[MAX_CMD_LEN];
	IMG_SIZE_TYPE_en enImgSizeType;

	enImgSizeType = GetImgSizeType(pcFileName);
	if (IMG_SIZE_TYPE_INVLID == enImgSizeType)
	{
		printf("Get image size error: %s/%s\n", acSubDirName, pcFileName);
		return ;
	}

	sprintf_s(acNewFileName, (MAX_FILE_NAME_LEN - 1),"%s#%s", acSubDirName, pcFileName);
	if (IMG_HORIZONTAL == enImgSizeType)
	{
		sprintf_s(cmd,(MAX_CMD_LEN - 1), "move %s ../%s/%s",pcFileName, HORIZONTAL_SUB_FOLDER_NAME, acNewFileName);
	}
	else
	{
		sprintf_s(cmd,(MAX_CMD_LEN - 1), "move %s ../%s/%s",pcFileName, VERTICAL_SUB_FOLDER_NAME, acNewFileName);
	}

	system(cmd);
	puts(cmd); //输出目录路径

	return ;
}

IMG_SIZE_TYPE_en GetImgSizeType(char * pcFileName)
{
	errno_t err;
	FILE * fpImgFile;
	int iReadBuf;
	int iHeight;
	int iWidth;
	bool bSegFlg = false;
	bool bSOF0Flg = false;

	err  = fopen_s(&fpImgFile, pcFileName, "rb");
	if (0 != err)
	{
		return IMG_SIZE_TYPE_INVLID;
	}
	
	iReadBuf = fgetc(fpImgFile);
	//while (EOF != iReadBuf)
	while (!feof(fpImgFile))
	{
		if ((JPG_SEG_FLAG != iReadBuf)
			&& (JPG_SEG_TYPE_SOF0 != iReadBuf))
		{
			bSegFlg = false;
			bSOF0Flg = false;
			iReadBuf = fgetc(fpImgFile);
			continue;
		}

		if (JPG_SEG_FLAG == iReadBuf)
		{
			bSegFlg = true;
		}
		else
		{
			bSOF0Flg = true;
		}

		if (bSOF0Flg && bSegFlg)
		{
			iReadBuf = fgetc(fpImgFile);
			iReadBuf = fgetc(fpImgFile);	/*段长度*/
			iReadBuf = fgetc(fpImgFile);	/*样本精度*/
			iReadBuf = fgetc(fpImgFile);	/*图片高度*/
			iHeight = iReadBuf;
			iReadBuf = fgetc(fpImgFile);	/*图片高度*/
			iHeight = iHeight * 256 + iReadBuf;

			iReadBuf = fgetc(fpImgFile);	/*图片宽度*/
			iWidth = iReadBuf;
			iReadBuf = fgetc(fpImgFile);	/*图片宽度*/
			iWidth = iWidth * 256 + iReadBuf;

			fclose(fpImgFile);
			return (iHeight > iWidth) ? IMG_VERTICAL : IMG_HORIZONTAL;
		}
		iReadBuf = fgetc(fpImgFile);
	}

	fclose(fpImgFile);
	return IMG_SIZE_TYPE_INVLID;
}

void getSmall(char * pcFileName)
{
	char acNewFileName[MAX_FILE_NAME_LEN] = {0};
	char cmd[MAX_CMD_LEN] = {0};

	if (NULL != strstr(pcFileName, "头像"))
	{
		sprintf_s(acNewFileName, (MAX_FILE_NAME_LEN - 1),"%s#smallpic.jpg", acSubDirName);
		sprintf_s(cmd,(MAX_CMD_LEN - 1), "move %s ..\\%s\\%s", pcFileName, SMALL_FOLDER_NAME, acNewFileName);
		system(cmd);
		puts(cmd); //输出目录路径

		iSamllPicNum++;
	}

	return ;
}


void getCover(char * pcFileName)
{
	char acNewFileName[MAX_FILE_NAME_LEN] = {0};
	char cmd[MAX_CMD_LEN] = {0};

	if (0 == strcmp(pcFileName, "page1.jpg"))
	{
		sprintf_s(acNewFileName, (MAX_FILE_NAME_LEN - 1),"%s#fengmian.jpg", acSubDirName, pcFileName);
		sprintf_s(cmd,(MAX_CMD_LEN - 1), "copy %s ..\\%s\\%s", pcFileName, COVER_FOLDER_NAME, acNewFileName);
		system(cmd);
		puts(cmd); //输出目录路径

//		sprintf_s(acNewFileName, (MAX_FILE_NAME_LEN - 1),"%s#letter.jpg", acSubDirName, pcFileName);
//		sprintf_s(cmd,(MAX_CMD_LEN - 1), "copy %s ..\\%s\\%s", pcFileName, LETTER_FOLDER_NAME, acNewFileName);
//		system(cmd);
//		puts(cmd); //输出目录路径
	}

	return ;
}


void getPage7(char * pcFileName)
{
	char acNewFileName[MAX_FILE_NAME_LEN] = {0};
	char cmd[MAX_CMD_LEN] = {0};

	if (NULL != strstr(pcFileName, "page7"))
	{
//		sprintf_s(cmd,(MAX_CMD_LEN - 1), "copy %s orig_%s", pcFileName, pcFileName);
//		system(cmd);
//		puts(cmd); //输出目录路径

		sprintf_s(acNewFileName, (MAX_FILE_NAME_LEN - 1),"%s#%s", acSubDirName, pcFileName);
		sprintf_s(cmd,(MAX_CMD_LEN - 1), "copy %s ..\\%s\\%s", pcFileName, PAGE7_FOLDER_NAME, acNewFileName);
		system(cmd);
		puts(cmd); //输出目录路径
	}

	return ;
}

void merge(char * pcFileName)
{
	char *p;
	char *delim = "#";
	char acSubDir[_MAXPATH] = {0};
	char cmd[MAX_CMD_LEN] = {0};
	char acSrcFile[_MAXPATH] = {0};

	strcpy_s(acSrcFile, (_MAXPATH - 1), pcFileName);

	p = strtok(pcFileName,delim);
	if (NULL == p)
	{
		return ;
	}
	strcpy_s(acSubDir, (_MAXPATH - 1), p);

	p = strtok(NULL,delim);
	if (NULL == p)
	{
		return ;
	}

	sprintf_s(cmd, (MAX_CMD_LEN - 1), "move %s ../%s/%s", acSrcFile, acSubDir, p);

	system(cmd);
	puts(cmd); //输出目录路径

	return ;
}