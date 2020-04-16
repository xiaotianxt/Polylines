#pragma once
#include "util.h"
#include "spatial_util.h"

using namespace std;

extern vector<PolyLine> polylines; // 全局变量用于保存所有折线
extern vector<Point> markers_out;

void Initialize();

vector<Point> Polychotomy(PolyLine);

Point Linechotomy(Point from, Point to, double len); // 给定两点，返回from到to方向上距离len的点坐标

vector<Point> EquiSpace(PolyLine polyline);

void SettleRect(); // 用于挪动所有的多边形

bool Overlap(Point, Point);

void RearrangeMarkers();
