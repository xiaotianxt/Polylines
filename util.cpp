#include "util.h"

const double eps = 1e-8; // double precision

bool equal(double a, double b) // 用于判断两个浮点数是否相等。精度为eps.
{
	if ((a - b) > -eps && (a - b) < eps) // 防止误差
		return true;
	else
		return false;
}

int compare(double a, double b)
{
	// a 比 b 大返回 -1
	if ((a - b) - eps > 0) // 如果减去eps还比0大那才是真的大
		return -1;
	// b 比 a 大返回 1
	if ((b - a) - eps > 0) // 同上
		return 1;
	return 0;
}

vector<string> GetFiles(string cate_dir)
{
	vector<string> files; //存放文件名
	_finddata_t file;
	long lf;
	//输入文件夹路径
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1)
	{
		cout << cate_dir << " not found!!!" << endl;
	}
	else //
	{
		while (_findnext(lf, &file) == 0)
		{
			//输出文件名
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(file.name);
		}
	}
	_findclose(lf);
	//排序，按从小到大排序
	sort(files.begin(), files.end());
	return files;
}

vector<string> FileName(string pattern)
{
	char current_address[100];
	memset(current_address, 0, 100);
	getcwd(current_address, 100); //获取当前路径
	cout << current_address << endl;
	strcat(current_address, "\\*");

	vector<string> files = GetFiles((string)current_address);
	vector<string> targetfiles;

	for (int i = 0; i < files.size(); i++)
	{
		cout << files[i] << endl;
	}

	cout << "end..." << endl;
	cin.get();
	return targetfiles;
}
