/* Name: Omkar Pradeep Acharya
 * Program: Line Sweep Algorithm
 */

//Header files
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <set>
#include <cmath>

using namespace std;

//As two double values are too hard to be compared, epsilon represents the fraction by which if the those two values are different, then they are considered as equal
//e.g. 1.00000000000000001 and 1.000000000000000002 are considered as equal in this case
const double epsilon = 1e-12;   // 10^-12

//Data structure for representing a segment
struct Segment;

//Represents a container for the input segments
vector<Segment> segments;

//Current position of the sweep line (To be precise, only the x coordinate)
double sweep_line_x;

//Data structure for representing a point
struct Point {
    //X and Y coordinates of the point
    double x, y;
    //e represents the type of event at that point
    //e = 0 means e is the start point for a segment
    //e = 1 means e is the end point for a segment
    //e = 2 means e is the intersection point
    //s and st represents the two segments which are intersecting at the point (if any)
    int e, s, st;

    //Default constructor
    Point() {

    }

    //Parameterized Constructor
    Point(double a, double b, int c, int d) {
        x = a;
        y = b;
        e = c;
        s = d;
        st = -1;
    }
};

//Comparator function for std::multiset in C++ to sort the points in the following order:
//1. Sort the points from left to right. (X in ascending order)
//2. If they have same X coordinate, then sort them according to Y coordinates. (Y in ascending order)
struct myclass {
    bool operator()(const Point &p1, const Point &p2) const {
        if (p1.x < p2.x)
            return true;
        if (p1.x == p2.x) {
            if (p1.y < p2.y)
                return true;
        }
        return false;
   }
} myobject;

//Data structure for representing a segment
struct Segment {
    //Segment ID
    int id;
    //Starting point and ending point for the segment
    Point left_endpoint, right_endpoint;

    //Default constructor
    Segment() {

    }

    //Parameterized constructor
    Segment(Point p1, Point p2, int i) {
        left_endpoint = p1;
        right_endpoint = p2;
        id = i;
    }
};

//Calculate the intersection of a segment with the sweep line.
//Done by putting the value of current X in the equation of the segment: (y - y1) / (y2 - y1) = (x - x1) / (x2 - x1)
double get_y_for_sweep_line_x(Segment s) {
    return s.left_endpoint.y + ((s.right_endpoint.y - s.left_endpoint.y) * (sweep_line_x - s.left_endpoint.x) /
                                (s.right_endpoint.x - s.left_endpoint.x));
}

//Comparator function for std::set in C++ to sort the segments according to the Y values where they hit the sweep line
//1. Sort the points from top to bottom. (Hit point with the sweep line in descending order)
//2. If they have same hit point with the sweep line, then sort them according to their endpoint's Y coordinate. (Y in descending order)
struct myclass2 {
    bool operator()(const int &s1, const int &s2) const {
        if (s1 == s2)
            return false;
//  Get the Y coordinates of the points where these segments hit the sweep line.
        double y1 = get_y_for_sweep_line_x(segments[s1]);
        double y2 = get_y_for_sweep_line_x(segments[s2]);
//  If those two points are equal, then you've to order them according to their endpoint's Y coordinate.
//  epsilon does the equality function if the difference is too small.
        if (abs(y1 - y2) < epsilon) {
            if (segments[s1].right_endpoint.y > segments[s2].right_endpoint.y)
                return true;
            return false;
        }
//  The segment which has larger Y coordinate at the sweep line, it goes above the other segment
        if (y1 > y2)
            return true;
        return false;
    }
};

//Class for declaring all the necessary containers, iterators and std::set, std::multiset and all the operations
class LineSweep {
//  Represents the number of line segments given
    int number_of_segments;
//  Vector to store the start points and end points of all the segments
    vector<Point> points;
//  Set storing all the segments which are currenlty hitting the sweep line
    set<int, myclass2> sweep;
//  Set Iterators
    set<int, myclass2>::iterator it2;
    set<int, myclass2>::iterator it3;
//  Multiset of all the points
//  Now, I have already used the vector to store the points, but in the case of multiple intersections I am putting the new intersections points as the events. Thus, I need them to be sorted each time as per the defined comparator. It would be easier for set to maintain the sorted order within O(log n) insertion as compared to the vector
//  Why multiset? Multiple segments can have the same start points or same end points or consider the case where one segment has a start point which is the end point of the other.
    multiset<Point, myclass> pointss;
//  Set storing all the intersection points as a part of the output
    set<Point, myclass> intersection_points;
//  Multiset iterator
    multiset<Point, myclass>::iterator it;
    multiset<Point, myclass>::iterator it5;
//  Set Iterator
    set<Point, myclass>::iterator it6;

public:

//  Get the line segments from the user
    void get_segments();

//  Line sweep algorithm for finding the intersection points (if any)
    void line_sweep_algorithm();

//  Function to tell whether the two segments intersect or not
    bool segments_intersect(Point, Point, Point, Point);

//  Helper function for the above function
    double direction(Point, Point, Point);

//  Helper function for the above function
    bool on_segment(Point, Point, Point);

//  Function to find out the intesection point for the intersecting segments (if any)
    Point get_intersection_point(Segment, Segment);

//  Prints the segment (Just used for debugging)
    void print_seg(Segment);

//  Prints the point coordinates up to two decimal places
    void print_point(Point);
};

//Function to get the segments from the user
void LineSweep::get_segments() {
    cin >> number_of_segments;
    double a, b, c, d;
    for (int i = 0; i < number_of_segments; ++i) {
        cin >> a >> b >> c >> d;
//      If the start point is to the left of the end point, take it as it is.
        if (a < c) {
            Point p1(a, b, 0, i);
            Point p2(c, d, 1, i);
            Segment s(p1, p2, i);
            segments.push_back(s);
            points.push_back(p1);
            points.push_back(p2);
        } else {
//      If the end point is to the left of the start point, switch them.
            Point p1(a, b, 1, i);
            Point p2(c, d, 0, i);
            Segment s(p2, p1, i);
            segments.push_back(s);
            points.push_back(p1);
            points.push_back(p2);

        }
    }
}

//Function to get the intesection point for the two segments
//Here, solving the equations of the two segments

//Equation 1: (y - y1) / (y2 - y1) = (x - x1) / (x2 - x1)
//Equation 2: (y - y3) / (y4 - y3) = (x - x3) / (x4 - x3)
//Solve for X and Y as we have all the other values

Point LineSweep::get_intersection_point(Segment s1, Segment s2) {
    Point p;
    p.y = ((((s1.left_endpoint.x * s1.right_endpoint.y - s1.right_endpoint.x * s1.left_endpoint.y) *
             (s2.right_endpoint.y - s2.left_endpoint.y)) -
            ((s2.left_endpoint.x * s2.right_endpoint.y - s2.right_endpoint.x * s2.left_endpoint.y) *
             (s1.right_endpoint.y - s1.left_endpoint.y))) /
           ((s1.left_endpoint.x - s1.right_endpoint.x) * (s2.right_endpoint.y - s2.left_endpoint.y) -
            (s2.left_endpoint.x - s2.right_endpoint.x) * (s1.right_endpoint.y - s1.left_endpoint.y)));
    p.x = ((((s1.left_endpoint.x * s1.right_endpoint.y - s1.right_endpoint.x * s1.left_endpoint.y) *
             (s2.left_endpoint.x - s2.right_endpoint.x)) -
            ((s2.left_endpoint.x * s2.right_endpoint.y - s2.right_endpoint.x * s2.left_endpoint.y) *
             (s1.left_endpoint.x - s1.right_endpoint.x))) /
           ((s2.left_endpoint.x - s2.right_endpoint.x) * (s1.right_endpoint.y - s1.left_endpoint.y) -
            (s1.left_endpoint.x - s1.right_endpoint.x) * (s2.right_endpoint.y - s2.left_endpoint.y)));
    return p;
}

//Function telling whether given two segments intersect or not
//Using the rectangular box technique and the dot product technique given in the lecture slides
bool LineSweep::segments_intersect(Point p1, Point p2, Point p3, Point p4) {
    double d1, d2, d3, d4;
    d1 = direction(p3, p4, p1);
    d2 = direction(p3, p4, p2);
    d3 = direction(p1, p2, p3);
    d4 = direction(p1, p2, p4);
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
        return true;
    else if (d1 == 0 && on_segment(p3, p4, p1))
        return true;
    else if (d2 == 0 && on_segment(p3, p4, p2))
        return true;
    else if (d3 == 0 && on_segment(p1, p2, p3))
        return true;
    else if (d4 == 0 && on_segment(p1, p2, p4))
        return true;
    else
        return false;
}

double LineSweep::direction(Point pi, Point pj, Point pk) {
    return (pj.y - pi.y) * (pk.x - pj.x) - (pj.x - pi.x) * (pk.y - pj.y);
}

bool LineSweep::on_segment(Point pi, Point pj, Point pk) {
    return (min(pi.x, pj.x) <= pk.x && pk.x <= max(pi.x, pj.x)) && (min(pi.y, pj.y) <= pk.y && pk.y <= max(pi.y, pj.y));
}

void LineSweep::print_seg(Segment s) {
    cout << "(" << s.left_endpoint.x << ", " << s.left_endpoint.y << ")(" << s.right_endpoint.x << ", "
         << s.right_endpoint.y << ")";
}

void LineSweep::print_point(Point p) {
    cout << setprecision(2) << fixed << p.x << ' ' << p.y;
}

//The line sweep algorithm for finding the intersection points
void LineSweep::line_sweep_algorithm() {
    it2 = sweep.begin();
    it3 = sweep.begin();
    it = pointss.begin();
    it5 = pointss.begin();
    it6 = intersection_points.begin();

//  Sort the points according to the comparator written above.
    sort(points.begin(), points.end(), myobject);

//  Insert all the points from the vector into the set
    for (vector<Point>::iterator i = points.begin(); i != points.end(); ++i)
        pointss.insert(*i);

    Segment s, s1, s2;
//  Consider two segments intersecting in V shape or L shape. So either their starting point or their ending point is common.
//  So, those points who belong to the multiple segments have to be intersection points as they appear multiple times in the input.
//  Ignoring the case where a segment is kind of like a dot. (where the start point and end point is the same)
    for (multiset<Point>::iterator i = pointss.begin(); i != pointss.end(); ++i) {
        if (pointss.count(*i) > 1) {
            Point p = *i;
            intersection_points.insert(p);
        }
    }

    it = pointss.begin();

//  Iterate through all the points
    while (!pointss.empty()) {

//      Update the current sweep line location based on the event
        sweep_line_x = (*it).x;

//      CASE 1: THE POINT IS A START POINT
        if ((*it).e == 0) {
//          Insert it into the vertically sorted array of the segments by where they hit the line segment
            sweep.insert((*it).s);
            it2 = sweep.find((*it).s);

//          Find whether the current segment and the segment above that segment intersect
            if (sweep.size() > 1 && it2 != sweep.begin()) {
//              s - current segment
//              s1 - the segment above s
                s = segments[*it2];
                s1 = segments[*(--it2)];
                it2++;

//              Check for the intersection
                if (segments_intersect(s1.left_endpoint, s1.right_endpoint,
                                       s.left_endpoint,
                                       s.right_endpoint)) {
//                  If the intersection point is found, then insert it to the set of intersection points and the set of all the points
                    Point temp = get_intersection_point(s1, s);
                    temp.e = 2;
                    temp.st = s1.id;
                    temp.s = s.id;
                    it6 = intersection_points.find(temp);
                    if (it6 == intersection_points.end())
                        pointss.insert(temp);
                    it5 = pointss.find(temp);
                    intersection_points.insert(temp);
                }
            }

//          Find whether the current segment and the segment below that segment intersect
            if (sweep.size() > 1 && it2 != sweep.end()) {
//              s - current segment
//              s2 - the segment below s
                s = segments[*it2];
                if (++it2 != sweep.end()) {
                    s2 = segments[*it2];
                    it2--;
//                  Check for the intersection
                    if (segments_intersect(s.left_endpoint, s.right_endpoint,
                                           s2.left_endpoint,
                                           s2.right_endpoint)) {
//                      If the intersection point is found, then insert it to the set of intersection points and the set of all the points
                        Point temp = get_intersection_point(s, s2);
                        temp.e = 2;
                        temp.s = s2.id;
                        temp.st = s.id;
                        it6 = intersection_points.find(temp);
                        if (it6 == intersection_points.end())
                            pointss.insert(temp);
                        it5 = pointss.find(temp);
                        intersection_points.insert(temp);
                    }
                }
            }
        }

//      CASE 2: THE POINT IS A END POINT
        if ((*it).e == 1) {
            it2 = sweep.find((*it).s);
            if (sweep.size() > 2 && it2 != sweep.begin() && it2 != sweep.end()) {
//              s1 - the segment above the current segment
//              s2 - the segment below the current segment
                s1 = segments[*(--it2)];
                it2++;
                if (++it2 != sweep.end()) {
                    s2 = segments[*it2];
                    it2--;
//                  Check for the intersection
                    if (segments_intersect(s1.left_endpoint, s1.right_endpoint,
                                           s2.left_endpoint, s2.right_endpoint)) {
//                      If the intersection point is found, then insert it to the set of intersection points and the set of all the points
                        Point temp = get_intersection_point(s1, s2);
                        temp.e = 2;
                        temp.s = s2.id;
                        temp.st = s1.id;
                        it5 = pointss.find(temp);
                        it6 = intersection_points.find(temp);
                        if (it5 == pointss.end() && it6 == intersection_points.end())
                            pointss.insert(temp);
                        intersection_points.insert(temp);
                    }
                }
            }
//          Delete the current segment from the vertically sorted set as it doesn't exist any more after the end point
            sweep.erase((*it).s);
        }

//      CASE 3: THE POINT IS AN INTERSECTION POINT
        if ((*it).e == 2) {
            int temp1, temp2;
            temp1 = (*it).s;
            temp2 = (*it).st;

//          Swap the two segments as after the intersection point
//          So, the segment which was above the other segment now goes above it.
            sweep.erase(temp1);
            sweep.erase(temp2);
            sweep.insert(temp1);
            sweep.insert(temp2);

            it2 = sweep.find(temp1);
            it3 = sweep.find(temp2);

//          Same as the case 1, check the intersection of the segment and its above segment.
            if (sweep.size() > 1 && it2 != sweep.begin()) {
                s = segments[*it2];
                s1 = segments[*(--it2)];
                it2++;

                if (segments_intersect(s1.left_endpoint, s1.right_endpoint,
                                       s.left_endpoint,
                                       s.right_endpoint)) {
                    Point temp = get_intersection_point(s1, s);
                    temp.e = 2;
                    temp.st = s1.id;
                    temp.s = s.id;
                    it5 = pointss.find(temp);
                    it6 = intersection_points.find(temp);
                    if (it5 == pointss.end() && it6 == intersection_points.end())
                        pointss.insert(temp);
                    intersection_points.insert(temp);
                }
            }

//          Same as the case 1, check the intersection of the segment and its below segment.
            if (sweep.size() > 1 && it3 != sweep.end()) {
                s = segments[*it3];
                if (++it3 != sweep.end()) {
                    s2 = segments[*it3];
                    it3--;
                    if (segments_intersect(s.left_endpoint, s.right_endpoint,
                                           s2.left_endpoint,
                                           s2.right_endpoint)) {
                        Point temp = get_intersection_point(s, s2);
                        temp.e = 2;
                        temp.s = s2.id;
                        temp.st = s.id;
                        it5 = pointss.find(temp);
                        it6 = intersection_points.find(temp);

                        if (it5 == pointss.end() && it6 == intersection_points.end())
                            pointss.insert(temp);
                        intersection_points.insert(temp);
                    }
                }
            }
        }
//      Once all the cases for a particular point are considered, delete the point from the set.
        pointss.erase(it++);
    }

 // Print all the intersection points from the set
    for (set<Point>::iterator i = intersection_points.begin(); i != intersection_points.end(); ++i) {
        print_point(*i);
        cout << endl;
    }
}

//Main function
int main() {
    LineSweep ls;
    ls.get_segments();
    ls.line_sweep_algorithm();
    return 0;
}