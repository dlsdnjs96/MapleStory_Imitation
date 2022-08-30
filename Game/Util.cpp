#include "stdafx.h"
#include "Util.h"

namespace Util {

    vector<2> calc_hit(Line l1, Line l2)
    {
        const float den = (l1.p1[0] - l1.p2[0]) * (l2.p1[1] - l2.p2[1]) - (l1.p1[1] - l1.p2[1]) * (l2.p1[0] - l2.p2[0]);

        if (den == 0) {
            return l1.p2;
        }

        const float t = ((l1.p1[0] - l2.p1[0]) * (l2.p1[1] - l2.p2[1]) - (l1.p1[1] - l2.p1[1]) * (l2.p1[0] - l2.p2[0])) / den;
        const float u = -((l1.p1[0] - l1.p2[0]) * (l1.p1[1] - l2.p1[1]) - (l1.p1[1] - l1.p2[1]) * (l1.p1[0] - l2.p1[0])) / den;

        if (t > 0 && t < 1 && u > 0 && u < 1)
        {
            l1.p2[0] = l1.p1[0] + t * (l1.p2[0] - l1.p1[0]);
            l1.p2[1] = l1.p1[1] + t * (l1.p2[1] - l1.p1[1]);
        }

        return l1.p2;
    }

    bool CounterClockWise(vector<2> const& A, vector<2> const& B, vector<2> const& C, vector<2> const& D)
    {
        float sign1 = (B[0] - A[0]) * (C[1] - A[1]) - (C[0] - A[0]) * (B[1] - A[1]);

        float sign2 = (B[0] - A[0]) * (D[1] - A[1]) - (D[0] - A[0]) * (B[1] - A[1]);

        float sign3 = (D[0] - C[0]) * (A[1] - C[1]) - (A[0] - C[0]) * (D[1] - C[1]);

        float sign4 = (D[0] - C[0]) * (B[1] - C[1]) - (B[0] - C[0]) * (D[1] - C[1]);



        return sign1 * sign2 < 0 && sign3 * sign4 < 0;
    }

    int ccw(vector<2> p1, vector<2> p2, vector<2> p3) {
        float cross_product = (p2[0] - p1[0]) * (p3[1] - p1[1]) - (p3[0] - p1[0]) * (p2[1] - p1[1]);

        if (cross_product > 0.f) {
            return 1;
        }
        else if (cross_product < 0.f) {
            return -1;
        }
        else {
            return 0;
        }
    }

    int comparator(vector<2> left, vector<2> right) {
        int ret;
        if (left[0] == right[0]) {
            ret = (left[1] <= right[1]);
        }
        else {
            ret = (left[0] <= right[0]);
        }
        return ret;
    }

    void swap(vector<2>* p1, vector<2>* p2) {
        vector<2> temp;
        temp = *p1;
        *p1 = *p2;
        *p2 = temp;
    }

    int LineIntersection(Line l1, Line l2) {
        int ret;

        int l1_l2 = ccw(l1.p1, l1.p2, l2.p1) * ccw(l1.p1, l1.p2, l2.p2);
        int l2_l1 = ccw(l2.p1, l2.p2, l1.p1) * ccw(l2.p1, l2.p2, l1.p2);


        if (l1_l2 == 0 && l2_l1 == 0) {
            if (comparator(l1.p2, l1.p1)) swap(&l1.p1, &l1.p2);
            if (comparator(l2.p2, l2.p1)) swap(&l2.p1, &l2.p2);

            ret = (comparator(l2.p1, l1.p2)) && (comparator(l1.p1, l2.p2));
        }
        else {
            ret = (l1_l2 <= 0) && (l2_l1 <= 0);
        }
        return ret;
    }

    bool GetCrossPoint(vector<2>& A, vector<2> B, vector<2> C, vector<2> D)
    {
        const float times = 100.f;
        vector<2> a = { A[0] * times, A[1] * times };
        vector<2> b = { B[0] * times, B[1] * times };
        vector<2> c = { C[0] * times, C[1] * times };
        vector<2> d = { D[0] * times, D[1] * times };
        //printf("GetCrossPoint %f %f %f %f %f %f %f %f\r\n", a[0], a[1], b[0], b[1], c[0], c[1], d[0], d[1]);

        if (LineIntersection(Line{ a, b }, Line{ c, d })) {
            a = calc_hit(Line{ a, b }, Line{ c, d }) / times;
            //freopen("output.txt", "w", stdout);
            //printf("GetCrossPoint %f %f %f %f %f %f %f %f\r\n", a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y);
            //printf("Cross Ture\r\n");
            return true;
        }
        //printf("Cross False\r\n");
        return false;
    }

    float GetHeightOnFoot(Line line, float x)
    {
        float temp = x - line.p1[0];
        temp /= line.p2[0] - line.p1[0];
        temp *= line.p2[1] - line.p1[1];
        return temp + line.p1[1];
    }

    bool OutOfLineX(Line line, float x)
    {
        if (max(line.p1[0], line.p2[0]) < x || min(line.p1[0], line.p2[0]) > x)
            return true;
        return false;
    }

    bool OutOfLineY(Line line, float y)
    {
        if (max(line.p1[1], line.p2[1]) < y || min(line.p1[1], line.p2[1]) > y)
            return true;
        return false;
    }


    float randTime(float low, float high)
    {
        return low + static_cast<float>(rand()) * static_cast<float>(high - low) / RAND_MAX;
    }

    vector<2> Rotate(vector<2> pos, float angle)
    {
        float const radian = 3.141592f / 180.0f * angle; // 각도 단위를 degree에서 radian으로 변환

        return vector<2>
        {
            (cos(radian)* pos[0]) - (sin(radian) * pos[1]),
                (sin(radian)* pos[0]) + (cos(radian) * pos[1])
        };

    }
    void PlayerSound(std::string content)
    {
        Sound::Sound sound;
        sound.Content = content;
        sound.volume = 0.5f;
        sound.Play();
    }

    vector<2> BezierCurve(vector<2> from, vector<2> to, float value)
    {
        return from + (to - from) * value;
    }

    vector<2> BezierCurve3(vector<2> p1, vector<2> p2, vector<2> p3, float value)
    {
        return BezierCurve(BezierCurve(p1, p2, value), BezierCurve(p2, p3, value), value);
    }

    vector<2> BezierCurve4(vector<2> p1, vector<2> p2, vector<2> p3, vector<2> p4, float value)
    {
        return BezierCurve3(BezierCurve(p1, p2, value), BezierCurve(p2, p3, value),
            BezierCurve(p3, p4, value), value);
    }
}