#ifndef BVH_PARSER_H
#define BVH_PARSER_H

#include "Skeleton.h"

#include <limits>


struct Frame {
    Frame(): translation(Vector3d::Zero()){}
    Vector3d translation;
    vector<Quaterniond> rotations;
    vector<double> eulers;
};

struct Motion {
    Motion() :
            maxP(-GltUtil::INF, -GltUtil::INF, -GltUtil::INF),
            minP(GltUtil::INF, GltUtil::INF, GltUtil::INF) {}
    int numFrames;
    int rotationsPerFrame;
    double frameTime;
    vector<string> channels;
    vector<Frame> frames;
    vector<Frame> interpolatedFrames;
    Vector3d maxP, minP;
    int fps;
    void interpolate(int fps);

    Frame interpolate(Frame const &frame1, Frame const &frame2, double lambda);
};

struct BvhData {
    Skeleton skeleton;
    Motion motion;

    Vector3d mvmntMax;
    Vector3d mvmntMin;

    void initFromBvhFile(char* fileName);

};

class BvhParser {

public:
    BvhParser(ifstream &myfile, BvhData &data) : myfile(myfile), data(data) {

    }

    void parse();

private:
    void verifyLine(ifstream &myfile, string &line, string text);

    void exitWithBadBvh();

    Vector3d getTranslation(int i, double n);

    AngleAxisd getQuaternion(int i, double n);

    void parseMotion();

    void parseHierarchy(Joint& joint, int &id, int &rotationIndex);

    void calculateSkelBox(Joint& joint);

    BvhData &data;
    ifstream &myfile;

    void initRoot(string line);
};


void parseBvhFile(char* filename, BvhData &data);

void saveAsBvhFile(BvhData& data);

#endif // BVH_PARSER_H