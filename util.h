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

using namespace std;

bool equal(double a, double b);

int compare(double a, double b);

/*********************************
* ���ƣ�		GetFiles
* ������		���ڷ��ظ���·���µ������ļ�
* 
* ������		string cate_dir
* ����ֵ��	vector<string> 
*********************************/
vector<string> GetFiles(string cate_dir);

vector<string> FileName(string pattern);
