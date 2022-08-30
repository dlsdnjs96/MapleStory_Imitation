#pragma once
namespace Util
{

	vector<2> calc_hit(Line l1, Line l2);
	//vector<2> calc_hit(vector<2> from, vector<2> to, vector<2> p3, vector<2> p4);

	int comparator(vector<2> left, vector<2> right);
	void swap(vector<2>* p1, vector<2>* p2);
	int ccw(vector<2> p1, vector<2> p2, vector<2> p3);
	bool CounterClockWise(vector<2> const& A, vector<2> const& B, vector<2> const& C, vector<2> const& D);
	int LineIntersection(Line l1, Line l2);
	//vector<2> GetCrossPoint(vector<2> const& A, vector<2> const& B, vector<2> const& C, vector<2> const& D); 
	bool GetCrossPoint(vector<2>& A, vector<2> B, vector<2> C, vector<2> D);

	float GetHeightOnFoot(Line line, float x);
	bool OutOfLineX(Line line, float x);
	bool OutOfLineY(Line line, float y);
	//float Lerp(float p1, float p2, float d1);

	float randTime(float low, float high);

	vector<2> Rotate(vector<2> pos, float angle);

	void PlayerSound(std::string content);

	template <typename T>
	T Lerp(T p1, T p2, float value) {
		return (1 - value) * p1 + value * p2;
	}

	template <typename T>
	T Slerp(T p1, T p2, float value) {
		const float angle = 90.f * TORADIAN;
		return ((p1 * sin(angle) * (1.f - value)) + (p2 * sin(angle) * value)) / sin(angle);
	}

	template <typename T>
	T Rand(T low, T high)
	{
		return low + static_cast<T>(rand()) * static_cast<T>(high - low) / RAND_MAX;
	}



	vector<2> BezierCurve(vector<2> from, vector<2> to, float value);

	vector<2> BezierCurve3(vector<2> p1, vector<2> p2, vector<2> p3, float value);

	vector<2> BezierCurve4(vector<2> p1, vector<2> p2, vector<2> p3, vector<2> p4, float value);
};

