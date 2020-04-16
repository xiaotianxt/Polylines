#pragma once
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <stack>
#include <cstring>
#include <string>
#include <cstdio>
#include <time.h>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include <regex>
#include <math.h>
#include <direct.h>
#include <io.h>
#include <windows.h>
#include <tchar.h>

using namespace std;

bool equal(double a, double b); // 判断double类型是否相等

int compare(double a, double b); // double比大小

/*********************************
* 名称：		GetFiles
* 描述：		用于返回给定路径下的所有文件
* 
* 参数：		string cate_dir
* 返回值：	vector<string> 
*********************************/
vector<string> GetFiles(string cate_dir);

/*********************************
* 名称：		GetAllFormatFiles
* 描述：		给定路径, 返回所有当前路径下
			的同类型文件(不包含子文件
* 
* 参数：		path // 路径
			files // 文件
* 返回值：	
*********************************/
void GetAllFormatFiles(string path, vector<string>& files, string format);
