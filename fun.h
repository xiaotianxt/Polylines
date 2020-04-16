#pragma once
#include "util.h"
#include "spatial_util.h"

using namespace std;

extern vector<PolyLine> polylines; // ȫ�ֱ������ڱ�����������
extern vector<Point> markers_out;

void Initialize();

vector<Point> Polychotomy(PolyLine);

Point Linechotomy(Point from, Point to, double len); // �������㣬����from��to�����Ͼ���len�ĵ�����

vector<Point> EquiSpace(PolyLine polyline);

void SettleRect(); // ����Ų�����еĶ����

bool Overlap(Point, Point);

void RearrangeMarkers();
