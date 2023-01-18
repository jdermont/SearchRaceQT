#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

struct Point {
    int x,y;

    Point() : x(0), y(0) {

    }

    Point(int x, int y) : x(x), y(y) {

    }

    bool operator==(const Point& a) const {
        return x == a.x && y == a.y;
    }

    bool operator!=(const Point& a) const {
        return x != a.x || y != a.y;
    }
};

static const double PI = 3.141592653589793;

struct Car {
    int x = 0;
    int y = 0;
    int vx = 0;
    int vy = 0;
    int angle = 0;

    Car() {

    }

    Car(int x, int y, int vx, int vy, int angle) : x(x), y(y), vx(vx), vy(vy), angle(angle) {

    }

    int my_round(double x) {
        int s = x < 0 ? -1 : 1;
        return s * (int)round(s * x);
    }

    int truncate(double x) {
        double roundedX = my_round(x);
        if (abs(roundedX-x) < 0.00001) {
            return (int)roundedX;
        }

        return (int) (x < 0 ? ceil(x) : floor(x));
    }

    double getAngle(int x, int y, int tx, int ty){
        double dx = tx-x;
        double dy = ty-y;
        return atan2(dy, dx);
    }

    double shortAngleDist(double a0, double a1) {
        double max = PI * 2;
        double da = std::fmod(a1 - a0, max);
        return std::fmod(2 * da, max) - da;
    }

    double toRadians(double var0) {
        return var0 / 180.0 * PI;
    }

    double toDegrees(double var0) {
        return var0 * 180.0 / PI;
    }

    void apply(int degree, int thrust) {
        double newAngle = angle + degree;
        newAngle = toRadians(newAngle);

        double vx = cos(newAngle) * thrust;
        double vy = sin(newAngle) * thrust;

        double degrees = round(toDegrees(newAngle));
        newAngle = toRadians(degrees);
        while (newAngle > PI*2) newAngle -= PI*2;
        while (newAngle < 0) newAngle += PI*2;
        this->angle = (int)round(toDegrees(newAngle));

        this->x = truncate(this->x + this->vx + vx);
        this->y = truncate(this->y + this->vy + vy);

        this->vx = truncate(0.85 * (this->vx + vx));
        this->vy = truncate(0.85 * (this->vy + vy));
    }

    string toString() {
        return std::to_string(x)+" "+std::to_string(y)+" "+std::to_string(vx)+" "+std::to_string(vy)+" "+std::to_string(angle);
    }
};

struct Game {
    Car car;
    vector<Point> checkpoints;
    int current = 0;
    int rounds = 0;
    uint64_t seed;

    Game(uint64_t seed = 0xdeadbeefULL) {
        if (seed == 0) seed = 0xdeadbeefULL;
        this->seed = seed;
        checkpoints.clear();
        size_t r = 8 + nextInt(8);
        while (checkpoints.size() < r) {
            bool collision = true;
            while (collision) {
                collision = false;
                int x = 600 + nextInt(14800);
                int y = 600 + nextInt(7800);
                for (auto & checkpoint : checkpoints) {
                    float dist = sqrtf((x-checkpoint.x)*(x-checkpoint.x)+(y-checkpoint.y)*(y-checkpoint.y));
                    if (dist <= 1800) {
                        collision = true;
                        break;
                    }
                }
                if (checkpoints.size() > 0) {
                    auto & c = checkpoints.back();
                    float dist = sqrtf((x-c.x)*(x-c.x)+(y-c.y)*(y-c.y));
                    if (dist <= 1800) {
                        collision = true;
                        break;
                    }
                }
                if (collision) continue;
                checkpoints.push_back(Point(x,y));
            }
        }
        car.x = checkpoints[checkpoints.size()-1].x;
        car.y = checkpoints[checkpoints.size()-1].y;
        car.angle = car.toDegrees(car.getAngle(car.x,car.y,checkpoints[0].x,checkpoints[0].y));
    }

    int nextInt(int n) {
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;
        return (seed>>32) % n;
    }

    void apply(int degree, int thrust) {
        car.apply(degree, thrust);
        Point target = checkpoints[current];
        double d = distance(car.x,car.y,target.x,target.y);
        if (d <= 600) current++;
        rounds++;
    }

    Car savedCar;
    int savedCurrent;
    int savedRounds;

    void save() {
        savedCar.x = car.x;
        savedCar.y = car.y;
        savedCar.vx = car.vx;
        savedCar.vy = car.vy;
        savedCar.angle = car.angle;
        savedCurrent = current;
        savedRounds = rounds;
    }

    void reset() {
        car.x = savedCar.x;
        car.y = savedCar.y;
        car.vx = savedCar.vx;
        car.vy = savedCar.vy;
        car.angle = savedCar.angle;
        current = savedCurrent;
        rounds = savedRounds;
    }

    bool isOver() {
        return current == checkpoints.size() || rounds == 600;
    }

    double distance(double x, double y, double tx, double ty) {
        return sqrt((tx-x)*(tx-x) + (ty-y)*(ty-y));
    }

    double getAngle(int x, int y, int tx, int ty){
        double dx = tx-x;
        double dy = ty-y;
        return atan2(dy, dx);
    }

    double shortAngleDist(double a0, double a1) {
        double max = PI * 2;
        double da = std::fmod(a1 - a0, max);
        return std::fmod(2 * da, max) - da;
    }

    double toRadians(double var0) {
        return var0 / 180.0 * PI;
    }

    vector<float> getInputs() {
        auto & next1 = checkpoints[current];
        auto & next2 = checkpoints[(current+1)%checkpoints.size()];
        float angle = toRadians(car.angle);

        float a1 = sin(angle);
        float a2 = cos(angle);
        float b1 = car.x-next1.x;
        if (b1 > 6000) b1 = 6000;
        if (b1 < -6000) b1 = -6000;
        b1 = b1 / 6000.0;
        float b2 = car.y-next1.y;
        if (b2 > 6000) b2 = 6000;
        if (b2 < -6000) b2 = -6000;
        b2 = b2 / 6000.0;

        float c1 = next2.x-next1.x;
        if (c1 > 6000) c1 = 6000;
        if (c1 < -6000) c1 = -6000;
        c1 = c1 / 6000.0;
        float c2 = next2.y-next1.y;
        if (c2 > 6000) c2 = 6000;
        if (c2 < -6000) c2 = -6000;
        c2 = c2 / 6000.0;

        return { (float)(car.vx)/1000.0f, (float)(car.vy)/1000.0f, a1, a2, b1, b2, c1, c2 };
    }
};

#endif // GAME_H
